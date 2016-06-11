TEMPLATE = app
CONFIG += console

CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

PRECOMPILED_HEADER = ../../src/generator/stdafx.h
CONFIG *= precompile_header

rpi {
    DEFINES+=RASPBERRY_PI
    QMAKE_MAKEFILE = "Makefile.rpi"
    MAKEFILE = "Makefile.rpi"
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

OBJECTS_DIR = ./generator/.obj/$${DEST_FOLDER}
MOC_DIR = ./generator/.moc/$${DEST_FOLDER}
RCC_DIR = ./generator/.rcc/$${DEST_FOLDER}
UI_DIR = ./generator/.ui/$${DEST_FOLDER}
DESTDIR = ../../bin/$${DEST_FOLDER}

QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-unused-variable -Wno-unused-parameter
QMAKE_LFLAGS += -rdynamic


INCLUDEPATH += ../../include
INCLUDEPATH += ../../src


LIBS += -L../../lib/$${DEST_FOLDER} -ldef_lang
LIBS += -lboost_program_options

HEADERS += \
    ../../src/generator/MurmurHash2.h

SOURCES += \
    ../../src/generator/main.cpp \
    ../../src/generator/MurmurHash2.cpp

