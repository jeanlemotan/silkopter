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

INCLUDEPATH += ../../../src
INCLUDEPATH += ../../../../../qbase/include
INCLUDEPATH += ../../../../../qdata/include
INCLUDEPATH += ../../../../../qmath/include
INCLUDEPATH += ../../../../../qinput/include
INCLUDEPATH += ../../../../../q/include
INCLUDEPATH += ../../../../libs
INCLUDEPATH += /usr/include/freetype2

ROOT_LIBS_PATH = $${PWD}/../../../../..
debug {
    LIB_FOLDER = lib/pc/debug
} else {
    LIB_FOLDER = lib/pc/release
}

LIBS += -L$${ROOT_LIBS_PATH}/q/$${LIB_FOLDER} -lq
LIBS += -L$${ROOT_LIBS_PATH}/qinput/$${LIB_FOLDER} -lqinput
LIBS += -L$${ROOT_LIBS_PATH}/qdata/$${LIB_FOLDER} -lqdata
LIBS += -L$${ROOT_LIBS_PATH}/qmath/$${LIB_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/$${LIB_FOLDER} -lqbase

LIBS += -lfreetype -lboost_system -lavcodec -lavformat -lswscale -lOIS


DESTDIR = ../../../bin

SOURCES += \
    ../../../src/Attitude.cpp \
    ../../../src/Axis.cpp \
    ../../../src/Camera_Controller3D.cpp \
    ../../../src/GL_Widget.cpp \
    ../../../src/main.cpp \
    ../../../src/qcustomplot.cpp \
    ../../../src/Render_Widget.cpp \
    ../../../src/Sensors.cpp \
    ../../../src/Silkopter.cpp \
    ../../../src/stdafx.cpp \
    ../../../src/UAV_Inertial.cpp \
    ../../../src/Video_Client.cpp \
    ../../../src/Video_Stream.cpp \
    ../../../src/Comms.cpp \
    ../../../src/Input_Widget.cpp \
    ../../../src/PID_Widget.cpp

HEADERS  += \
    ../../../src/Attitude.h \
    ../../../src/Axis.h \
    ../../../src/Camera_Controller3D.h \
    ../../../src/Config.h \
    ../../../src/GL_Widget.h \
    ../../../src/qcustomplot.h \
    ../../../src/Render_Widget.h \
    ../../../src/Sensors.h \
    ../../../src/Silkopter.h \
    ../../../src/stdafx.h \
    ../../../src/UAV_Inertial.h \
    ../../../src/Video_Client.h \
    ../../../src/Video_Stream.h \
    ../../../../libs/utils/chrono.h \
    ../../../../libs/utils/PID.h \
    ../../../../libs/utils/Serial_Channel.h \
    ../../../../libs/common/input/Camera_Input.h \
    ../../../../libs/common/input/UAV_Input.h \
    ../../../src/Comms.h \
    ../../../src/Input_Widget.h \
    ../../../src/PID_Widget.h

FORMS    += \
    ../../../src/Sensors.ui \
    ../../../src/Silkopter.ui \
    ../../../src/UAV_Inertial.ui \
    ../../../src/Video_Stream.ui \
    ../../../src/Input_Widget.ui \
    ../../../src/PID_Widget.ui
