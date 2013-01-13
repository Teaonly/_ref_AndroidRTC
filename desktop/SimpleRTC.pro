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

FFMPEG_INC_DIR = /Users/teaonly/proj/media/ffmpeg-1.0.1
FFMPEG_AVCODEC_LIBRARY = /Users/teaonly/proj/media/ffmpeg-1.0.1/libavcodec/libavcodec.a
FFMPEG_AVUTIL_LIBRARY = /Users/teaonly/proj/media/ffmpeg-1.0.1/libavutil/libavutil.a

INCLUDEPATH += $$ROOT_DIR $$FFMPEG_INC_DIR
DEFINES = "HAMMER_TIME=1" "LOGGING=1" FEATURE_ENABLE_SSL "HASHNAMESPACE=__gnu_cxx" POSIX DISABLE_DYNAMIC_CAST "HAVE_OPENSSL_SSL_H=1" _REENTRANT OSX _DEBUG EXPAT_RELATIVE_PATH   
LIBS += libjingle.a $$FFMPEG_AVCODEC_LIBRARY $$FFMPEG_AVUTIL_LIBRARY

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
    
