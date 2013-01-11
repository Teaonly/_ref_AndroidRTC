QT += network multimedia
CONFIG += console

INCLUDEPATH += $$ROOT_DIR
DEINFES = "HAMMER_TIME 1" "DLOGGING 1" FEATURE_ENABLE_SSL "HASHNAMESPACE __gnu_cxx" POSIX DISABLE_DYNAMIC_CAST "HAVE_OPENSSL_SSL_H 1" _REENTRANT OSX _DEBUG EXPAT_RELATIVE_PATH   
LIBS += $$ROOT_DIR/libjingle.a

SOURCES += \ 
    main_gui.cpp \
    videowidget.cpp \
    videowidgetsurface.cpp \
    media.cpp \
    datachannel.cpp \
    udpdatachannel.cpp \
    mediabuffer.cpp \
    rtpsinker.cpp

HEADER += \
    videowidget.h \
    videowidgetsurface.h     
