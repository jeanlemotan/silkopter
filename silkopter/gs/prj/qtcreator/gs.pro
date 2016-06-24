#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T18:33:20
#
#-------------------------------------------------

QT       += core gui opengl network widgets

TARGET = gs
TEMPLATE = app

#TARGET = gs
#target.path = /home/jean/fpv/gs
#INSTALLS = target

CONFIG += c++11

INCLUDEPATH += ../../src
INCLUDEPATH += ../../src/qnodeseditor
INCLUDEPATH += ../../../../QMapControl/src
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qdata/include
INCLUDEPATH += ../../../../qmath/include
INCLUDEPATH += ../../../../def_lang/include
INCLUDEPATH += ../../../libs
INCLUDEPATH += /usr/include/freetype2
INCLUDEPATH += /usr/include/ffmpeg
INCLUDEPATH += ../../../../autojsoncxx/include
INCLUDEPATH += ../../../../autojsoncxx/rapidjson/include
INCLUDEPATH += ../../../../eigen
INCLUDEPATH += ../../../brain/autogen
INCLUDEPATH += ../../../libs/common/comms/def

QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter -B$HOME/dev/bin/gold
QMAKE_CFLAGS += -Wno-unused-variable -Wno-unused-parameter -B$HOME/dev/bin/gold

PRECOMPILED_HEADER = ../../src/stdafx.h
CONFIG *= precompile_header

ROOT_LIBS_PATH = ../../../..

rpi {
    DEFINES+=RASPBERRY_PI
    CONFIG(debug, debug|release) {
        DEST_FOLDER = rpi/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = rpi/release
        DEFINES += NDEBUG
    }
} else {
    CONFIG(debug, debug|release) {
        DEST_FOLDER = pc/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = pc/release
        DEFINES += NDEBUG
    }
}

LIBS += -L$${ROOT_LIBS_PATH}/def_lang/lib/$${DEST_FOLDER} -ldef_lang
LIBS += -L$${ROOT_LIBS_PATH}/qdata/lib/$${DEST_FOLDER} -lqdata
LIBS += -L$${ROOT_LIBS_PATH}/qmath/lib/$${DEST_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/lib/$${DEST_FOLDER} -lqbase
LIBS += -L$${ROOT_LIBS_PATH}/QMapControl/lib/$${DEST_FOLDER} -lqmapcontrol

LIBS += -lfreetype -lboost_system -lavutil -lavcodec -lavformat -lswscale -lfftw3 -lz
LIBS += -lboost_thread
LIBS += -lpcap

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../bin

RESOURCES += \
    ../../res/res.qrc

HEADERS += \
    ../../src/GS.h \
    ../../src/stdafx.h \
    ../../src/Nodes_Widget.h \
    ../../src/Properties_Widget.h \
    ../../src/Config_Widget.h \
    ../../src/Comms.h \
    ../../../libs/utils/RCP_UDP_Socket.h \
    ../../../libs/utils/RCP.h \
    ../../../libs/common/comms/def/gs_comms.def.h \
    ../../../libs/lz4/lz4.h

SOURCES += \
    ../../src/GS.cpp \
    ../../src/main.cpp \
    ../../src/Nodes_Widget.cpp \
    ../../src/Properties_Widget.cpp \
    ../../src/Config_Widget.cpp \
    ../../src/Comms.cpp \
    ../../../libs/utils/RCP_UDP_Socket.cpp \
    ../../../libs/utils/RCP.cpp \
    ../../../libs/common/comms/def/gs_comms.def.cpp \
    ../../../libs/lz4/lz4.c

FORMS += \
    ../../src/GS.ui

DISTFILES += \

