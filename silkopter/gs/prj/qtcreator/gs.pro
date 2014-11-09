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

LIBS += -lfreetype -lboost_system -lavcodec -lavformat -lswscale -lfftw3

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../bin

SOURCES += \
    ../../src/Attitude.cpp \
    ../../src/Axis.cpp \
    ../../src/Camera_Controller3D.cpp \
    ../../src/GL_Widget.cpp \
    ../../src/main.cpp \
    ../../src/qcustomplot.cpp \
    ../../src/Render_Widget.cpp \
    ../../src/Sensors.cpp \
    ../../src/GS.cpp \
    ../../src/UAV_Inertial.cpp \
    ../../src/Video_Client.cpp \
    ../../src/Video_Stream.cpp \
    ../../src/Comms.cpp \
    ../../src/Input_Widget.cpp \
    ../../src/PID_Widget.cpp

HEADERS  += \
    ../../src/Attitude.h \
    ../../src/Axis.h \
    ../../src/Camera_Controller3D.h \
    ../../src/Config.h \
    ../../src/GL_Widget.h \
    ../../src/qcustomplot.h \
    ../../src/Render_Widget.h \
    ../../src/Sensors.h \
    ../../src/GS.h \
    ../../src/stdafx.h \
    ../../src/UAV_Inertial.h \
    ../../src/Video_Client.h \
    ../../src/Video_Stream.h \
    ../../../libs/utils/chrono.h \
    ../../../libs/utils/PID.h \
    ../../../libs/utils/Serial_Channel.h \
    ../../../libs/common/input/Camera_Input.h \
    ../../../libs/common/input/UAV_Input.h \
    ../../src/Comms.h \
    ../../src/Input_Widget.h \
    ../../src/PID_Widget.h \
    ../../../libs/common/sensors/Sensor_Samples.h \
    ../../../libs/common/Manual_Clock.h \
    ../../../libs/utils/Channel.h \
    ../../../libs/common/Comm_Data.h

FORMS    += \
    ../../src/Sensors.ui \
    ../../src/GS.ui \
    ../../src/UAV_Inertial.ui \
    ../../src/Video_Stream.ui \
    ../../src/Input_Widget.ui \
    ../../src/PID_Widget.ui

RESOURCES += \
    ../../src/res.qrc
