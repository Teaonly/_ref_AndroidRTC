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
    FFMPEG_INC_DIR = "C:\workspace\Media\ffmpeg-dev\include"
    FFMPEG_AVCODEC_LIBRARY = "C:\workspace\Media\ffmpeg-dev\lib\libavcodec.lib"
    FFMPEG_AVUTIL_LIBRARY = "C:\workspace\Media\ffmpeg-dev\lib\libavutil.lib"
    FFMPEG_AVFORMAT_LIBRARY = "C:\workspace\Media\ffmpeg-dev\lib\libavformat.lib"    
}


INCLUDEPATH += $$ROOT_DIR $$FFMPEG_INC_DIR
DEFINES = "HAMMER_TIME=1" "LOGGING=1" FEATURE_ENABLE_SSL "HASHNAMESPACE=__gnu_cxx" POSIX DISABLE_DYNAMIC_CAST "HAVE_OPENSSL_SSL_H=1" _REENTRANT OSX _DEBUG EXPAT_RELATIVE_PATH
mac:!unix {   
    LIBS += libjingle.a $$FFMPEG_AVCODEC_LIBRARY $$FFMPEG_AVUTIL_LIBRARY $$FFMPEG_AVFORMAT_LIBRARY -lz -framework VideoDecodeAcceleration -framework CoreVideo
}

unix {
    LIBS += libjingle.a $$FFMPEG_AVCODEC_LIBRARY $$FFMPEG_AVUTIL_LIBRARY $$FFMPEG_AVFORMAT_LIBRARY -lz
}

win32 {
    LIBS += ".\libjingle.lib"
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
    
