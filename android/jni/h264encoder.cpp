#include "helper.h"
#include "h264encoder.h"

enum {
    MSG_START_ENCODER,
};

H264Encoder::H264Encoder(talk_base::Thread* thread) {
    encoding_thread_ = thread;
    x264_hdl_ = NULL;
    ppIndex = -1;
}

H264Encoder::~H264Encoder() {
    if ( x264_hdl_ != NULL) {
        Release();    
    }
}

void H264Encoder::OnMessage(talk_base::Message *msg) {
    if ( msg->message_id == MSG_START_ENCODER) {
        doEncoding();        
    }
}

int H264Encoder::Prepare(const MediaDescription& desc) {
    if ( desc.isVideo == false)
        return -1;
    
    int wid = desc.width;
    int hei = desc.height;

    // 0. building a default encoder parameters. 
    x264_param_default_preset(&x264_opt_, "ultrafast", "zerolatency");
    //x264_param_default(&opt);
    
    // 1. Setting the fields of parameter struct
    x264_opt_.i_width = wid;
    x264_opt_.i_height = hei;
    //opt.i_slice_count = 5;
    //opt.b_intra_refresh = 1;
    
    // 3. Prepare the output buffer and target file
    x264_picture_alloc(&x264_picin_[0], X264_CSP_I420, x264_opt_.i_width, x264_opt_.i_height);
    x264_picture_alloc(&x264_picin_[1], X264_CSP_I420, x264_opt_.i_width, x264_opt_.i_height);
    x264_picture_alloc(&x264_picout_, X264_CSP_I420, x264_opt_.i_width, x264_opt_.i_height);
    ppIndex = -1;

    // 4. Building the encoder handler
    x264_hdl_ = x264_encoder_open(&x264_opt_);
    x264_encoder_parameters(x264_hdl_, &x264_opt_);

    return 0;
}

int H264Encoder::Release() {
    if (x264_hdl_ ) {
        encoding_thread_->Clear(this);
        //x264_picture_clean(&x264_picin_);
        //x264_picture_clean(&x264_picout_);
        x264_encoder_close(x264_hdl_);
        x264_hdl_ = NULL;
    }
    
    return 0;
}

int H264Encoder::EncodePicture(unsigned char *yuv) {
    if ( x264_hdl_ == NULL)
        return -1;

    int width = x264_opt_.i_width;
    int height = x264_opt_.i_height;
   
    if ( ppIndex == -1 ) {
        ppIndex = 0;
        memcpy(x264_picin_[ppIndex].img.plane[0], yuv, width*height);
        memcpy(x264_picin_[ppIndex].img.plane[1], yuv + width*height, width*height/4);
        memcpy(x264_picin_[ppIndex].img.plane[2], yuv + width*height + width*height/4, width*height/4);
    } else {
        int index = 1 - ppIndex;
        memcpy(x264_picin_[index].img.plane[0], yuv, width*height);
        memcpy(x264_picin_[index].img.plane[1], yuv + width*height, width*height/4);
        memcpy(x264_picin_[index].img.plane[2], yuv + width*height + width*height/4, width*height/4);
    }

    encoding_thread_->Clear(this);
    encoding_thread_->Post(this, MSG_START_ENCODER);
    
    return 0;
}

void H264Encoder::doEncoding() {
    // encoding current picture 
    int nals;
    x264_nal_t *nal_pointer;
    x264_encoder_encode(x264_hdl_, &nal_pointer, &nals, &x264_picin_[ppIndex], &x264_picout_);    
    LOGD("New frame is encoded!"); 
    // fetching the current data
    for ( int i = 0; i < nals; i++) {
        LOGD("\tpayload size = %d", nal_pointer[i].i_payload);
    }  
    
    ppIndex = 1 - ppIndex;
}


