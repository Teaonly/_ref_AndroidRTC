#ifndef _HELPER_H_
#define _HELPER_H_

#include <vector>
#include <string>

#ifdef ANDROID
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "TEAONLY"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define  LOGD(...) 
#endif

/*
tcp://10.10.10.10:342
proto     tcp
location  10.10.10.10
resource  342

p2p://xxx@xxx.com/pc
proto     p2p
location  xxx@xxx.com
resource  pc 

*/
class MediaURL {
public:
    bool Parse(const std::string &url);
    std::string proto;
    std::string location;
    std::string resource;
    std::string target;
};


/*
video:h264:wid:hei

audio:g711:8000
*/
class MediaDescription {
public:
    bool Parse(const std::string& description);
    bool isVideo;
    bool isAudio;
    std::string codec;
    unsigned int width;
    unsigned int height;
    unsigned int sample;
};


#endif
