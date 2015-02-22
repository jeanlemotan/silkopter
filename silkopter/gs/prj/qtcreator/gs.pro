#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T18:33:20
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gs
TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += ../../src
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qdata/include
INCLUDEPATH += ../../../../qmath/include
INCLUDEPATH += ../../../../qinput/include
INCLUDEPATH += ../../../../q/include
INCLUDEPATH += ../../../libs
INCLUDEPATH += /usr/include/freetype2

PRECOMPILED_HEADER = ../../include/stdafx.h

ROOT_LIBS_PATH = ../../../..
CONFIG(debug, debug|release) {
    DEST_FOLDER = pc/debug
} else {
    DEST_FOLDER = pc/release
}

LIBS += -L$${ROOT_LIBS_PATH}/q/lib/$${DEST_FOLDER} -lq
LIBS += -L$${ROOT_LIBS_PATH}/qinput/lib/$${DEST_FOLDER} -lqinput
LIBS += -L$${ROOT_LIBS_PATH}/qdata/lib/$${DEST_FOLDER} -lqdata
LIBS += -L$${ROOT_LIBS_PATH}/qmath/lib/$${DEST_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/lib/$${DEST_FOLDER} -lqbase

LIBS += -lfreetype -lboost_system -lavcodec -lavformat -lswscale -lfftw3 -lz

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../bin

RESOURCES += \
    ../../src/res.qrc
