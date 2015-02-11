TEMPLATE = app
CONFIG += console
#CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

TARGET = brain
target.path = /home/pi
INSTALLS = target


#QMAKE_CXXFLAGS_RELEASE += -g
#QMAKE_CFLAGS_RELEASE += -g
#QMAKE_LFLAGS_RELEASE =

rpi {
    DEFINES+=RASPBERRY_PI
}

rpi {
    CONFIG(debug, debug|release) {
        DEST_FOLDER = rpi/debug
    } else {
        DEST_FOLDER = rpi/release
        DEFINES += NDEBUG
    }
} else {
    CONFIG(debug, debug|release) {
        DEST_FOLDER = pc/debug
    } else {
        DEST_FOLDER = pc/release
        DEFINES += NDEBUG
    }
}

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../bin/$${DEST_FOLDER}

QMAKE_CXXFLAGS += -isystem =/opt/vc/include -isystem =/opt/vc/include/interface/vcos/pthreads -isystem =/opt/vc/include/interface/vmcs_host/linux

INCLUDEPATH += ../../src
INCLUDEPATH += ../../autogen
INCLUDEPATH += ../../../libs
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qdata/include
INCLUDEPATH += ../../../../qmath/include
INCLUDEPATH += ../../../../autojsoncxx/include
INCLUDEPATH += ../../../../autojsoncxx/rapidjson/include

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
LIBS += -lz
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
    ../../src/device/MPU9250.cpp \
    ../../src/device/MS5611.cpp \
    ../../src/device/Raspicam.cpp \
    ../../src/device/RC5T619.cpp \
    ../../src/device/SRF02.cpp \
    ../../src/device/UBLOX.cpp \
    ../../src/storage/Storage_FS.cpp \
    ../../src/storage/Storage_MB85RC.cpp \
    ../../src/AHRS.cpp \
    ../../src/Battery.cpp \
    ../../src/Comms.cpp \
    ../../src/Gimbal_Servo_PiGPIO.cpp \
    ../../src/HAL_Motors_Sim.cpp \
    ../../src/HAL_Sensors_Sim.cpp \
    ../../src/main.cpp \
    ../../src/Motor_Mixer.cpp \
    ../../src/Motors_PiGPIO.cpp \
    ../../src/Sim_Comms.cpp \
    ../../src/UAV.cpp \
    ../../src/device/PIGPIO.cpp \
    ../../src/HAL.cpp \
    ../../src/HAL_old.cpp \
    ../../src/bus/UART_Linux.cpp \
    ../../src/bus/SPI_Linux.cpp \
    ../../src/bus/I2C_Linux.cpp

HEADERS += \
    ../../src/BrainStdAfx.h \
    ../../src/Toggle.h \
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
    ../../../libs/physics/constants.h \
    ../../../libs/common/Comm_Data.h \
    ../../../libs/common/Manual_Clock.h \
    ../../../libs/utils/RUDP.h \
    ../../../libs/common/sensors/Sensor_Samples.h \
    ../../src/device/MPU9250.h \
    ../../src/device/MS5611.h \
    ../../src/device/Raspicam.h \
    ../../src/device/RC5T619.h \
    ../../src/device/SRF02.h \
    ../../src/device/UBLOX.h \
    ../../src/interface/IAccelerometer.h \
    ../../src/interface/IADC.h \
    ../../src/interface/IAmmeter.h \
    ../../src/interface/IBarometer.h \
    ../../src/interface/ICamera.h \
    ../../src/interface/ICompass.h \
    ../../src/interface/IGPS.h \
    ../../src/interface/IGyroscope.h \
    ../../src/interface/II2C.h \
    ../../src/interface/IPWM.h \
    ../../src/interface/ISonar.h \
    ../../src/interface/ISPI.h \
    ../../src/interface/IThermometer.h \
    ../../src/interface/IUART.h \
    ../../src/interface/IVoltmeter.h \
    ../../src/storage/Storage_MB85RC.h \
    ../../src/Battery.h \
    ../../src/Gimbal_Servo_PiGPIO.h \
    ../../src/HAL_Motors_Sim.h \
    ../../src/HAL_Sensors.h \
    ../../src/HAL_Sensors_Sim.h \
    ../../src/Motors_PiGPIO.h \
    ../../src/Sim_Comms.h \
    ../../src/device/PIGPIO.h \
    ../../src/HAL.h \
    ../../src/bus/I2C_Linux.h \
    ../../src/bus/SPI_Linux.h \
    ../../src/bus/UART_Linux.h

