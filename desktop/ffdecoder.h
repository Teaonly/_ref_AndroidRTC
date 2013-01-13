#include <map>
#include <iostream>
#include <assert.h>

#include "mediabuffer.h"
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

extern "C" {
    struct AVCodec;
    struct AVCodecContext;
    struct AVFrame;
};

#include "ffdefine.h"

#define OLD_FFMPEG 1

class FFDecoder {
public:
    FFDecoder(AVCodecContext *pCC, AVCodec *pC);
    virtual ~FFDecoder();
    virtual int DecodeVideoPacket(MediaPackage *, VideoPicture*);
    
    TeaCodecType type;
private:
    AVCodecContext *pCodecCtx;
    AVCodec        *pCodec;
    AVFrame        *pFrame;
};

FFDecoder* CreateH264Decoder();

