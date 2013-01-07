#include <string>
#include "talk/base/thread.h"
#include "helper.h"
#include "rtpstreamer.h"


#define  JNIDEFINE(fname) Java_teaonly_rtc_MainActivity_##fname

extern "C" {
    JNIEXPORT jint JNICALL JNIDEFINE(nativeStart)(JNIEnv* env, jclass clz, jstring remote, jstring description);
    JNIEXPORT jint JNICALL JNIDEFINE(nativeStop)(JNIEnv* env, jclass clz);

    JNIEXPORT jint JNICALL JNIDEFINE(nativePushFrame)(JNIEnv* env, jclass clz, jbyteArray yuvData);
}

static std::string convert_jstring(JNIEnv *env, const jstring &js) {
    static char outbuf[1024];
    std::string str;
 
    int len = env->GetStringLength(js);
    env->GetStringUTFRegion(js, 0, len, outbuf);

    str = outbuf;
    return str;
}


//*************************************
// Gloal definement
//*************************************

RtpStreamer *streamer = NULL;
talk_base::Thread *streaming_thread = NULL;
talk_base::Thread *encoding_thread = NULL;

JNIEXPORT jint JNICALL JNIDEFINE(nativeStart)(JNIEnv* env, jclass clz, jstring jremote, jstring jdescription) {
    std::string remote = convert_jstring(env, jremote); 
    std::string description = convert_jstring(env, jdescription);

    if ( streaming_thread == NULL) {
        streaming_thread = new talk_base::Thread();
        streaming_thread->Start();
    } 

    if ( encoding_thread == NULL) {
        encoding_thread = new talk_base::Thread();
        encoding_thread->Start();
    }

    streamer = new RtpStreamer(streaming_thread, encoding_thread);
    int ret = streamer->StartStreaming(remote, description);
    
    if ( ret < 0) {
        delete streamer;
        streamer = NULL;
    }    

    return ret; 
}

JNIEXPORT jint JNICALL JNIDEFINE(nativeStop)(JNIEnv* env, jclass clz) {

    streamer->StopStreaming();
    delete streamer;
    streamer = NULL;

    return 0;
}

JNIEXPORT jint JNICALL JNIDEFINE(nativePushFrame)(JNIEnv* env, jclass clz, jbyteArray yuvData) {
    if ( streamer != NULL) {
        jbyte* cameraFrame= env->GetByteArrayElements(yuvData, NULL);
        int ret = streamer->ProvideCameraFrame((unsigned char *)cameraFrame);
        env->ReleaseByteArrayElements(yuvData, cameraFrame, JNI_ABORT);
        return ret;
    } 
    return -1;
}
