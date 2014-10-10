TEMPLATE = app
CONFIG += console
#CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

TARGET = brain
target.path = /home/pi
INSTALLS = target

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
DESTDIR = ../../bin/$${DEST_FOLDER}

QMAKE_CXXFLAGS += -isystem =/opt/vc/include -isystem =/opt/vc/include/interface/vcos/pthreads -isystem =/opt/vc/include/interface/vmcs_host/linux

INCLUDEPATH += ../../src
INCLUDEPATH += ../../../libs
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qdata/include
INCLUDEPATH += ../../../../qmath/include
INCLUDEPATH += ../../../../rapidjson/include

LIBS += -L=/opt/vc/lib/

ROOT_LIBS_PATH = ../../../..

LIBS += -L$${ROOT_LIBS_PATH}/qdata/lib/$${DEST_FOLDER} -lqdata
LIBS += -L$${ROOT_LIBS_PATH}/qmath/lib/$${DEST_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/lib/$${DEST_FOLDER} -lqbase

LIBS += -lpthread
LIBS += -lboost_system
LIBS += -lboost_thread
LIBS += -lboost_program_options
LIBS += -lrt
rpi {
    LIBS += -lpigpio
    LIBS += -lmmal_core
    LIBS += -lmmal_util
    LIBS += -lmmal_vc_client
    LIBS += -lvcos
    LIBS += -lbcm_host
    LIBS += -lGLESv2
    LIBS += -lEGL
}

SOURCES += \
    ../../src/BrainStdAfx.cpp \
    ../../src/main.cpp \
    ../../src/Video_Server.cpp \
    ../../src/UAV.cpp \
    ../../src/Comms.cpp \
    ../../src/AHRS.cpp \
    ../../src/Motor_Mixer.cpp \
    ../../src/chips/MPU9250.cpp \
    ../../src/chips/MS5611.cpp \
    ../../src/chips/i2c.cpp \
    ../../src/HAL_Raspicam.cpp \
    ../../src/HAL_Sensors_Pi.cpp \
    ../../src/HAL_Sensors_Sim.cpp \
    ../../src/HAL.cpp \
    ../../src/HAL_Motors_PiGPIO.cpp \
    ../../src/PiGPIO.cpp \
    ../../src/Sim_Comms.cpp

HEADERS += \
    ../../src/BrainStdAfx.h \
    ../../src/Video_Server.h \
    ../../src/Toggle.h \
    ../../../libs/common/input/Camera_Input.h \
    ../../../libs/common/input/UAV_Input.h \
    ../../../libs/utils/Channel.h \
    ../../../libs/utils/chrono.h \
    ../../../libs/utils/PID.h \
    ../../../libs/utils/Serial_Channel.h \
    ../../src/UAV.h \
    ../../src/Comms.h \
    ../../../libs/utils/Json_Helpers.h \
    ../../src/AHRS.h \
    ../../src/Motor_Mixer.h \
    ../../../libs/utils/Timed_Scope.h \
    ../../src/chips/MPU9250.h \
    ../../../libs/physics/constants.h \
    ../../src/chips/MS5611.h \
    ../../src/chips/i2c.h \
    ../../src/HAL_Camera.h \
    ../../src/HAL_Raspicam.h \
    ../../src/HAL_Camera_Mount.h \
    ../../src/HAL_Motors.h \
    ../../src/HAL_Motors_PiGPIO.h \
    ../../src/HAL_Motors_Sim.h \
    ../../src/HAL_Sensors.h \
    ../../src/HAL_Sensors_Pi.h \
    ../../src/HAL_Sensors_Sim.h \
    ../../src/HAL.h \
    ../../src/HAL_Camera_Mount_PiGPIO.h \
    ../../src/PiGPIO.h \
    ../../src/Sim_Comms.h

