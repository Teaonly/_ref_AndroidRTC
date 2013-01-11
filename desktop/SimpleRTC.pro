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

INCLUDEPATH += $$ROOT_DIR
DEFINES = "HAMMER_TIME=1" "LOGGING=1" FEATURE_ENABLE_SSL "HASHNAMESPACE=__gnu_cxx" POSIX DISABLE_DYNAMIC_CAST "HAVE_OPENSSL_SSL_H=1" _REENTRANT OSX _DEBUG EXPAT_RELATIVE_PATH   
LIBS += libjingle.a

SOURCES += \ 
    main_gui.cpp \
    videowidget.cpp \
    videowidgetsurface.cpp \ 
    videodialog.cpp \
    media.cpp \
    datachannel.cpp \
    udpdatachannel.cpp \
    mediabuffer.cpp \
    rtpsinker.cpp

HEADERS += \
    videowidget.h \
    videowidgetsurface.h \
    videodialog.h

FORMS += \
    player.ui
    
