#include "helper.h"
#include "h264encoder.h"

enum {
    MSG_START_ENCODER,
};


H264Encoder::H264Encoder(talk_base::Thread* thread) {
    encoding_thread_ = thread;
    x264_hdl_ = NULL;
}

H264Encoder::~H264Encoder() {
    if ( x264_hdl_ != NULL) {
        Release();    
    }
}

void H264Encoder::OnMessage(talk_base::Message *msg) {

}

int H264Encoder::Prepare(int wid, int hei) {
    // 0. building a default encoder parameters. 
    x264_param_default_preset(&x264_opt_, "ultrafast", "zerolatency");
    //x264_param_default(&opt);
    
    // 1. Setting the fields of parameter struct
    x264_opt_.i_width = wid;
    x264_opt_.i_height = hei;

    //opt.i_slice_count = 5;
    //opt.b_intra_refresh = 1;
 
    // 3. Prepare the output buffer and target file
    x264_picture_alloc(&x264_picin_, X264_CSP_I420, x264_opt_.i_width, x264_opt_.i_height);
    x264_picture_alloc(&x264_picout_, X264_CSP_I420, x264_opt_.i_width, x264_opt_.i_height);
   
    // 4. Building the encoder handler
    x264_t *x264_hdl_ = x264_encoder_open(&x264_opt_);
    x264_encoder_parameters(x264_hdl_, &x264_opt_);


    return 0;
}

int H264Encoder::Release() {
    x264_picture_clean(&x264_picin_);
    x264_picture_clean(&x264_picout_);
    x264_encoder_close(x264_hdl_);
    x264_hdl_ = NULL;
    return 0;
}
