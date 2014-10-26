TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../qbase/include
INCLUDEPATH += ../../../qmath/include

PRECOMPILED_HEADER = ../../include/QInputStdAfx.h


rpi {
    DEFINES+=RASPBERRY_PI
}

rpi {
    CONFIG(debug, debug|release) {
        DEST_FOLDER = rpi/debug
    } else {
        DEST_FOLDER = rpi/release
    }
} else {
    CONFIG(debug, debug|release) {
        DEST_FOLDER = pc/debug
    } else {
        DEST_FOLDER = pc/release
    }
}

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../lib/$${DEST_FOLDER}


SOURCES += \
    ../../src/Gamepad.cpp \
    ../../src/Input_Mgr.cpp \
    ../../src/Keyboard.cpp \
    ../../src/Sensors.cpp \
    ../../src/Touchscreen.cpp

HEADERS += \
    ../../include/Gamepad.h \
    ../../include/Input_Mgr.h \
    ../../include/Keyboard.h \
    ../../include/QInput.h \
    ../../include/QInputStdAfx.h \
    ../../include/Sensors.h \
    ../../include/Touch.h \
    ../../include/Touchscreen.h

