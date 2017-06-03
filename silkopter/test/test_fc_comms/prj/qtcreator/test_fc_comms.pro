#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T18:33:20
#
#-------------------------------------------------

TARGET = test_fc_comms
TEMPLATE = app

TARGET = test_fc_comms
target.path = test_fc_comms
INSTALLS = target

CONFIG -= qt
CONFIG += c++11

ROOT_LIBS_PATH = ../../../../..

INCLUDEPATH += ../../src
INCLUDEPATH += $${ROOT_LIBS_PATH}/qbase/include
INCLUDEPATH += $${ROOT_LIBS_PATH}/qmath/include
INCLUDEPATH += $${ROOT_LIBS_PATH}/def_lang/include
INCLUDEPATH += ../../../../libs
INCLUDEPATH += ../../../../libs/common/comms/def


QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-unused-variable -Wno-unused-parameter

PRECOMPILED_HEADER = ../../src/stdafx.h
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

LIBS += -lboost_system -lboost_thread
LIBS += -lz -lpthread
LIBS += -L$${ROOT_LIBS_PATH}/qmath/lib/$${DEST_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/lib/$${DEST_FOLDER} -lqbase

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../bin

RESOURCES +=

HEADERS += \
    ../../src/stdafx.h \
    ../../../../libs/utils/hw/RF4463F30.h \
    ../../../../libs/utils/hw/Si4463.h \
    ../../../../libs/utils/hw/SPI_Dev.h \
    ../../../../libs/utils/hw/RF4463F30_Config.h \
    ../../../../libs/utils/hw/si446x_patch.h \
    ../../../../libs/utils/comms/fec.h \
    ../../../../libs/utils/comms/RC_Phy.h \
    ../../../../libs/utils/comms/RC_Protocol.h \
    ../../../../libs/utils/hw/ISPI.h \
    ../../../../libs/utils/Queue.h \
    ../../../../libs/utils/hw/command.h \
    ../../../../libs/utils/hw/pigpio.h

SOURCES += \
    ../../src/main.cpp \
    ../../../../libs/utils/hw/RF4463F30.cpp \
    ../../../../libs/utils/hw/Si4463.cpp \
    ../../../../libs/utils/hw/SPI_Dev.cpp \
    ../../../../libs/utils/comms/fec.cpp \
    ../../../../libs/utils/comms/RC_Phy.cpp \
    ../../../../libs/utils/comms/RC_Protocol.cpp \
    ../../../../libs/utils/Queue.cpp \
    ../../../../libs/utils/hw/command.c \
    ../../../../libs/utils/hw/pigpio.c

