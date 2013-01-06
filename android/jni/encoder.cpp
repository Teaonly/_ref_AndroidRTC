#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <inttypes.h>
extern "C" {
#include <x264.h>
}

static x264_param_t opt;
static x264_t *hdl;
static x264_picture_t picin;
static x264_picture_t picout;
static int picture_seq;

int encoder_prepare(int wid, int hei) {

    // 0. building a default encoder parameters. 
    x264_param_default_preset(&opt, "ultrafast", "zerolatency");
    //x264_param_default(&opt);
    
    // 1. Setting the fields of parameter struct
    opt.i_width = wid;
    opt.i_height = hei;
    opt.i_slice_count = 3;
    //opt.b_intra_refresh = 1;
   
    // 3. Building the encoder handler
    x264_t *hdl = x264_encoder_open(&opt);
    x264_encoder_parameters(hdl, &opt);

    // 4. Prepare the output buffer and target file
    x264_picture_alloc(&picin, X264_CSP_I420, opt.i_width, opt.i_height);
    x264_picture_alloc(&picout, X264_CSP_I420, opt.i_width, opt.i_height);

    picture_seq = 0;

    return 0;
}

int encoder_encoding(unsigned char *plan) {
/*    
    // fill the YUV data
    picin.i_pts = i * opt.i_fps_den;
    if ( i % 10 == 0) {
        picin.i_type = X264_TYPE_I;
    } else {
        picin.i_type = X264_TYPE_AUTO;
    }
    picin.i_qpplus1 = 0; 

    if ( fread(picin.img.plane[0], opt.i_width*opt.i_height, 1, fpIn) <= 0)
        break;

    fread(picin.img.plane[1], opt.i_width*opt.i_height/4, 1, fpIn);
    fread(picin.img.plane[2], opt.i_width*opt.i_height/4, 1, fpIn);

    // encoding current picture 
    int nals;
    x264_nal_t *nal_pointer;
    x264_encoder_encode(hdl, &nal_pointer, &nals, &picin, &picout);    
    
    
    // fetching the current data
    for ( int i = 0; i < nals; i++) {
        std::cout << i << ": type = " << nal_pointer[i].i_type  << " payload size=" << nal_pointer[i].i_payload << std::endl;
        //x264_nal_encode(hdl, codedbuffer, &nal_pointer[i]);
        //fwrite(codedbuffer, nal_pointer[i].i_payload + 4, 1, fpOut); 
    
        fwrite(nal_pointer[i].p_payload, nal_pointer[i].i_payload, 1, fpOut); 
    }  
*/   
}

int encoder_release() {
    
    // 6. close the handle and free the picture
    x264_encoder_close(hdl);
    hdl = 0;
    //x264_picture_clean(&picin);
    //x264_picture_clean(&picout);

    return 0;
}
