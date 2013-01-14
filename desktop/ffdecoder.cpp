extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

#include "ffdecoder.h"

FFDecoder::FFDecoder(AVCodecContext *pCC, AVCodec *pC) {
    pCodecCtx = pCC;
    pCodec = pC;
    pFrame = NULL;

    if ( pCodecCtx->codec_type == AVMEDIA_TYPE_VIDEO) {
        type = TEACODEC_TYPE_VIDEO;
        pFrame = avcodec_alloc_frame();
    }else if ( pCodecCtx->codec_type == AVMEDIA_TYPE_AUDIO)
        type = TEACODEC_TYPE_AUDIO;
    else
        type = TEACODEC_TYPE_UNKNOW;
}

FFDecoder::~FFDecoder(){
    avcodec_close(pCodecCtx);
    if ( pFrame != NULL)
        av_free(pFrame);
}

int FFDecoder::DecodeVideoPacket(MediaPackage *pkt, VideoPicture* vp) {
    if ( type != TEACODEC_TYPE_VIDEO)
        return 0;
    
    
    //convert MediaPakage to FFMPEG's AVPacket
    AVPacket ffpkt;
    ffpkt.data = pkt->data;
    ffpkt.size = pkt->length;
    //ffpkt.pts = pkt->pts;
    //ffpkt.dts = pkt->dts;
    
    int isFinished;
    avcodec_decode_video2(pCodecCtx, pFrame, &isFinished, &ffpkt);

    if ( !isFinished ) 
        return 0;
    vp->video_type = YUV_420_PLAN;
    vp->width = pCodecCtx->width;
    vp->height = pCodecCtx->height;
   
    for(int i = 0; i < 3; i++) {
        if( (vp->vplan_length[i] != (unsigned int)pFrame->linesize[i]) && (pFrame->linesize[i] > 0) ) {
            vp->vplan_length[i] = pFrame->linesize[i];
            if ( vp->vplan[i] != NULL ) {
                free(vp->vplan[i]);           
            }
            vp->vplan[i] = (unsigned char *)malloc( vp->height * vp->vplan_length[i]);            
        }
        if ( vp->vplan_length[i] > 0) {
            assert(vp->vplan[i] != NULL);       
            if ( i >= 1)
                memcpy( vp->vplan[i], pFrame->data[i], vp->height * pFrame->linesize[i] / 2);
            else
                memcpy( vp->vplan[i], pFrame->data[i], vp->height * pFrame->linesize[i]);
        }
    }

    return 1;
}


FFDecoder* CreateH264Decoder() {
    avcodec_register_all();
    AVCodec *pC = avcodec_find_decoder(AV_CODEC_ID_H264);
    AVCodecContext *pCC = avcodec_alloc_context3(pC);
    int ret = avcodec_open2(pCC, pC, NULL);
    if ( ret >= 0) {
        return new FFDecoder(pCC, pC);
    }   

    return NULL;
}


