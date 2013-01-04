LOCAL_PATH:= $(call my-dir)

###########################################################
# building libx264 
#
include $(CLEAR_VARS)
LOCAL_CFLAGS := -O3 -Wall -std=gnu99 -fno-fast-math -fPIC -fomit-frame-pointer -fno-tree-vectorize
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/x264
LOCAL_MODULE    := libx264

ifeq ($(TARGET_ARCH_ABI),armeabi)
    LOCAL_CFLAGS += -DNDK_ARM    
endif
ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS += -DNDK_X86
endif 

include $(LOCAL_PATH)/libx264_build.mk

include $(BUILD_STATIC_LIBRARY)

###########################################################
# building libapp
#
include $(CLEAR_VARS)
LOCAL_MODULE := libapp
LOCAL_CPPFLAGS := -O2 -Wall  
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/x264

ifeq ($(TARGET_ARCH_ABI),armeabi)
    LOCAL_CFLAGS += -DNDK_ARM    
endif
ifeq ($(TARGET_ARCH_ABI),x86)
    LOCAL_CFLAGS += -DNDK_X86
endif 


include $(LOCAL_PATH)/libapp_build.mk

LOCAL_WHOLE_STATIC_LIBRARIES := libx264
LOCAL_SHARED_LIBRARIES := libcutils\
                          libstlport\
                          libdl 
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)
