#include <jni.h>
#include <android/log.h>

#include "talk/base/basicdefs.h"
#include "talk/base/common.h"
#include "talk/base/helpers.h"
#include "talk/base/logging.h"
#include "talk/base/thread.h"
#include "talk/base/messagequeue.h"

#define  LOG_TAG    "TEAONLY"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#define  JNIDEFINE(fname) Java_teaonly_rtc_MainActivity_##fname
extern "C" {
    JNIEXPORT jint JNICALL JNIDEFINE(nativeStart)(JNIEnv* env, jclass clz, jobject obj, jstring remote, jint port);
}

static std::string convert_jstring(JNIEnv *env, const jstring &js) {
    static char outbuf[1024];
    std::string str;
 
    int len = env->GetStringLength(js);
    env->GetStringUTFRegion(js, 0, len, outbuf);

    str = outbuf;
    return str;
}

static jint get_native_fd(JNIEnv* env, jclass clz, jobject fdesc) {
  jclass clazz;
  jfieldID fid;

  /* get the fd from the FileDescriptor */
  if (!(clazz = env->GetObjectClass(fdesc)) ||
    !(fid = env->GetFieldID(clazz,"descriptor","I"))) return -1;

  /* return the descriptor */
  return env->GetIntField(fdesc,fid);
}

static int event_fd;

JNIEXPORT jint JNICALL JNIDEFINE(nativeMain)(JNIEnv* env, jclass clz, jobject obj, jstring jremote, jint port) {
    std::string remote = convert_jstring(env, jremote); 
    
    //event_fd = get_native_fd(env, clz, obj);
    
    return 0; 
}


