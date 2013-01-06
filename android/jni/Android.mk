LOCAL_PATH:= $(call my-dir)

###########################################################
# building libexpat 
#
include $(CLEAR_VARS)
LOCAL_CFLAGS := -DHAVE_MEMMOVE=1 -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1
LOCAL_MODULE    := libexpat
LOCAL_SRC_FILES := expat/xmlparse.c \
                   expat/xmlrole.c \
                   expat/xmltok.c
include $(BUILD_STATIC_LIBRARY)

###########################################################
# building openssl 
#
include $(LOCAL_PATH)/openssl/Crypto.mk
include $(LOCAL_PATH)/openssl/Ssl.mk


###########################################################
# building libjingle 
#
include $(CLEAR_VARS)
LOCAL_MODULE := libjingle
LOCAL_CPP_EXTENSION := .cc .cpp
LOCAL_CPPFLAGS := -O2 -g3 -Werror -Wall -Wno-switch -Wno-non-virtual-dtor -Wno-ctor-dtor-privacy -fno-rtti -fpic -fno-exceptions 
LOCAL_CPPFLAGS += -DLOGGING=1 -DFEATURE_ENABLE_SSL -DHAVE_OPENSSL_SSL_H=1 -DEXPAT_RELATIVE_PATH -DHASHNAMESPACE=__gnu_cxx  -DHASH_NAMESPACE=__gnu_cxx -DPOSIX -DDISABLE_DYNAMIC_CAST -D_REENTRANT -DLINUX -DDISABLE_EFFECTS=1 -DANDROID -DARCH_CPU_LITTLE_ENDIAN 
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/expat $(LOCAL_PATH)/openssl/include
include $(LOCAL_PATH)/libjingle_build.mk

LOCAL_WHOLE_STATIC_LIBRARIES := libexpat\
                                libcrypto\
                                libssl

LOCAL_SHARED_LIBRARIES := libcutils\
                          libstlport\
                          libdl 

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

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

LOCAL_CPPFLAGS += -DLOGGING=1 -DFEATURE_ENABLE_SSL -DHAVE_OPENSSL_SSL_H=1 -DEXPAT_RELATIVE_PATH -DHASHNAMESPACE=__gnu_cxx  -DHASH_NAMESPACE=__gnu_cxx -DPOSIX -DDISABLE_DYNAMIC_CAST -D_REENTRANT -DLINUX -DDISABLE_EFFECTS=1 -DANDROID -DARCH_CPU_LITTLE_ENDIAN 
include $(LOCAL_PATH)/libapp_build.mk

LOCAL_WHOLE_STATIC_LIBRARIES := libx264
LOCAL_SHARED_LIBRARIES := libjingle\
                          libcutils\
                          libstlport\
                          libdl 
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)
