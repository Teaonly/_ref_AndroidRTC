TARGET  = SimpleRTC
QT += network multimedia
CONFIG += console
CONFIG(debug, debug|release) {
    DESTDIR = debug
	OBJECTS_DIR = debug/obj
    MOC_DIR = debug/moc
} else {
    DESTDIR = release
    OBJECTS_DIR = release/obj
    MOC_DIR = release/moc
}

mac {
    FFMPEG_INC_DIR = /Users/teaonly/proj/media/ffmpeg-1.0.1
    FFMPEG_AVCODEC_LIBRARY = /Users/teaonly/proj/media/ffmpeg-1.0.1/libavcodec/libavcodec.a
    FFMPEG_AVUTIL_LIBRARY = /Users/teaonly/proj/media/ffmpeg-1.0.1/libavutil/libavutil.a
    FFMPEG_AVFORMAT_LIBRARY = /Users/teaonly/proj/media/ffmpeg-1.0.1/libavformat/libavformat.a
}

win32 {
    FFMPEG_INC_DIR = "C:\\workspace\\projects\\media\\ffmpeg-dev\\include"
    FFMPEG_AVCODEC_LIBRARY = "C:\\workspace\\projects\\media\\ffmpeg-dev\\lib\\avcodec.lib"
    FFMPEG_AVUTIL_LIBRARY = "C:\\workspace\\projects\\media\\ffmpeg-dev\\lib\\avutil.lib"
    FFMPEG_AVFORMAT_LIBRARY = "C:\\workspace\\projects\\media\\ffmpeg-dev\\lib\\avformat.lib"    
}

INCLUDEPATH += $$ROOT_DIR $$FFMPEG_INC_DIR
DEFINES = "HAMMER_TIME=1" "LOGGING=1" FEATURE_ENABLE_SSL "HASHNAMESPACE=__gnu_cxx" DISABLE_DYNAMIC_CAST "HAVE_OPENSSL_SSL_H=1" _REENTRANT _DEBUG EXPAT_RELATIVE_PATH

mac {   
    LIBS += libjingle.a $$FFMPEG_AVCODEC_LIBRARY $$FFMPEG_AVUTIL_LIBRARY $$FFMPEG_AVFORMAT_LIBRARY -lz -framework VideoDecodeAcceleration -framework CoreVideo
    DEFINES += POSIX OSX
}

win32 {
    INCLUDEPATH += "C:\\workspace\\projects\\AndroidRTC\\desktop\\win" 
    LIBS += $$quote(C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/shell32.lib)
    LIBS += $$quote(C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/ws2_32.lib)
    LIBS += $$quote(C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/crypt32.lib)
    LIBS += $$quote(C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/iphlpapi.lib)
    LIBS += $$quote(C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/secur32.lib)
    LIBS += $$quote(C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/wininet.lib)
    LIBS += $$quote(C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/advapi32.lib)
    LIBS += "C:\\workspace\\projects\\AndroidRTC\\desktop\\win\\Debug\\libjingle.lib" 
    LIBS += $$FFMPEG_AVCODEC_LIBRARY  $$FFMPEG_AVUTIL_LIBRARY
    DEFINES += WIN32 _WINDOWS 
}

SOURCES += \ 
    main_gui.cpp \
    videowidget.cpp \
    videowidgetsurface.cpp \ 
    videodialog.cpp \
    media.cpp \
    datachannel.cpp \
    udpdatachannel.cpp \
    mediabuffer.cpp \
    rtpsinker.cpp \
    ffdecoder.cpp \
    ezplayer.cpp \
    ezrender.cpp

HEADERS += \
    videowidget.h \
    videowidgetsurface.h \
    videodialog.h

FORMS += \
    player.ui
    
