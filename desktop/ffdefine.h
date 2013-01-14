#ifndef _BASE_H_
#define _BASE_H_

#include <stdlib.h>

#ifdef POSIX 
#include <stdint.h>
#include <sys/time.h>

typedef uint64_t MediaTime;
const MediaTime BAD_TIME = (uint64_t)(-1);
inline MediaTime CurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    MediaTime ret = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return ret;    
}
#elif WIN32
// TODO
#endif

typedef enum {
    TEACODEC_TYPE_UNKNOW,
    TEACODEC_TYPE_VIDEO,
    TEACODEC_TYPE_AUDIO
}TeaCodecType;

typedef enum {
    YUV_420_PLAN = 1,
    YUV_420_PACK = 2,
    RGBA_8_PACK  = 3
}VType;

class VideoPicture{
public:
    VideoPicture() {
        vplan[0] = vplan[1] = vplan[2] = NULL;
        width = 0;
        height = 0;
        vplan_length[0] = vplan_length[1] = vplan_length[2] = 0;
    }
    ~VideoPicture() {
        for(int i = 0; i < 3; i++) {
            if ( vplan[i] != NULL)
                delete vplan[i];
        }
    }
public:    
    VType video_type;
    unsigned int width;
    unsigned int height;
    unsigned char *vplan[3];
    unsigned int vplan_length[3];
    //MediaTime   mt;    
};

#endif
