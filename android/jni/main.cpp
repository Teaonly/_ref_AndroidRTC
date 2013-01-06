#include <string>
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "TEAONLY"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  JNIDEFINE(fname) Java_teaonly_rtc_MainActivity_##fname

extern "C" {
    JNIEXPORT jint JNICALL JNIDEFINE(nativeStart)(JNIEnv* env, jclass clz, jstring remote, jint port, jint wid, jint hei);
    JNIEXPORT jint JNICALL JNIDEFINE(nativeStop)(JNIEnv* env, jclass clz);

    JNIEXPORT jint JNICALL JNIDEFINE(nativePushFrame)(JNIEnv* env, jclass clz, jint wid, jint hei, jbyteArray yuvData);
}

static std::string convert_jstring(JNIEnv *env, const jstring &js) {
    static char outbuf[1024];
    std::string str;
 
    int len = env->GetStringLength(js);
    env->GetStringUTFRegion(js, 0, len, outbuf);

    str = outbuf;
    return str;
}

JNIEXPORT jint JNICALL JNIDEFINE(nativeMain)(JNIEnv* env, jclass clz, jstring jremote, jint port, jint wid, jint hei) {
    std::string remote = convert_jstring(env, jremote); 
        
    return 0; 
}

JNIEXPORT jint JNICALL JNIDEFINE(nativeStop)(JNIEnv* env, jclass clz) {

    return 0;
}

JNIEXPORT jint JNICALL JNIDEFINE(nativePushFrame)(JNIEnv* env, jclass clz, jint wid, jint hei, jbyteArray yuvData) {

}
