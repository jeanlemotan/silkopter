#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T18:33:20
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sim
TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += ../../src
INCLUDEPATH += ../../../libs
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qdata/include
INCLUDEPATH += ../../../../qmath/include
INCLUDEPATH += ../../../../q/include
INCLUDEPATH += ../../../libs
INCLUDEPATH += /usr/include/freetype2
INCLUDEPATH += /usr/include/bullet

ROOT_LIBS_PATH = $${PWD}/../../../..
CONFIG(debug, debug|release) {
    DEST_FOLDER = pc/debug
} else {
    DEST_FOLDER = pc/release
}

LIBS += -L$${ROOT_LIBS_PATH}/q/lib/$${DEST_FOLDER} -lq
LIBS += -L$${ROOT_LIBS_PATH}/qdata/lib/$${DEST_FOLDER} -lqdata
LIBS += -L$${ROOT_LIBS_PATH}/qmath/lib/$${DEST_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/lib/$${DEST_FOLDER} -lqbase

LIBS += -lfreetype -lboost_system -lavcodec -lavformat -lswscale -lBulletCollision -lBulletDynamics -lLinearMath

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../bin

FORMS += \
    ../../src/Simulator.ui \
    ../../src/Status_Widget.ui \
    ../../src/Sensors_Widget.ui \
    ../../src/Environment_Widget.ui \
    ../../src/Test_Ahrs.ui

HEADERS += \
    ../../src/Axis.h \
    ../../src/Camera_Controller3D.h \
    ../../src/Curve.h \
    ../../src/GL_Widget.h \
    ../../src/Motor.h \
    ../../src/PWM_Out_Widget.h \
    ../../src/qcustomplot.h \
    ../../src/Render_Widget.h \
    ../../src/Simulator.h \
    ../../src/Status_Widget.h \
    ../../src/stdafx.h \
    ../../src/World.h \
    ../../src/UAV.h \
    ../../src/Config.h \
    ../../src/Sensors_Widget.h \
    ../../src/Environment_Widget.h \
    ../../src/Move_Gizmo.h \
    ../../src/Rotate_Gizmo.h \
    ../../src/Select_Gizmo.h

SOURCES += \
    ../../src/Axis.cpp \
    ../../src/Camera_Controller3D.cpp \
    ../../src/GL_Widget.cpp \
    ../../src/main.cpp \
    ../../src/Motor.cpp \
    ../../src/PWM_Out_Widget.cpp \
    ../../src/qcustomplot.cpp \
    ../../src/Render_Widget.cpp \
    ../../src/Simulator.cpp \
    ../../src/Status_Widget.cpp \
    ../../src/stdafx.cpp \
    ../../src/World.cpp \
    ../../src/UAV.cpp \
    ../../src/Sensors_Widget.cpp \
    ../../src/Environment_Widget.cpp \
    ../../src/Move_Gizmo.cpp \
    ../../src/Rotate_Gizmo.cpp \
    ../../src/Select_Gizmo.cpp

RESOURCES += \
    ../../src/res.qrc


