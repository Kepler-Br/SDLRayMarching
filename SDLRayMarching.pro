TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    mainloop.cpp \
    customtime.cpp \
    camera.cpp \
    window.cpp \
    raymarcher.cpp \
    raymarchworker.cpp


LIBS += -lSDL2

HEADERS += \
    mainloop.h \
    customtime.h \
    camera.h \
    window.h \
    raymarcher.h \
    raymarchworker.h

