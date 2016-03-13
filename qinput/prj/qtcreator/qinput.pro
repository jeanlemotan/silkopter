TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../qbase/include
INCLUDEPATH += ../../../qmath/include

PRECOMPILED_HEADER = ../../include/QInputStdAfx.h
CONFIG *= precompile_header

QMAKE_CXXFLAGS += -Wno-unused-variable
QMAKE_CFLAGS += -Wno-unused-variable


rpi {
    DEFINES+=RASPBERRY_PI
    QMAKE_MAKEFILE = "Makefile.rpi"
    CONFIG(debug, debug|release) {
        DEST_FOLDER = rpi/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = rpi/release
        DEFINES += NDEBUG
    }
} else {
    QMAKE_MAKEFILE = "Makefile"
    CONFIG(debug, debug|release) {
        DEST_FOLDER = pc/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = pc/release
        DEFINES += NDEBUG
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

