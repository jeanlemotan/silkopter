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
    ../../src/main.cpp \
    ../../src/UAV.cpp \
    ../../src/Comms.cpp \
    ../../src/AHRS.cpp \
    ../../src/Motor_Mixer.cpp \
    ../../src/HAL_Sensors_Sim.cpp \
    ../../src/HAL.cpp \
    ../../src/Sim_Comms.cpp \
    ../../src/HAL_Motors_Sim.cpp \
    ../../src/Battery.cpp \
    ../../src/sensors/GPS_UBLOX.cpp \
    ../../src/sensors/i2c.cpp \
    ../../src/sensors/MPU9250.cpp \
    ../../src/sensors/MS5611.cpp \
    ../../src/sensors/OdroidW_ADC.cpp \
    ../../src/sensors/SRF02.cpp \
    ../../src/sensors/GPS_Detector.cpp \
    ../../src/sensors/GPS_Detector_UART.cpp \
    ../../src/sensors/Raspicam.cpp \
    ../../src/actuators/Gimbal_Servo_PiGPIO.cpp \
    ../../src/actuators/Motors_PiGPIO.cpp \
    ../../src/storage/Storage_FS.cpp \
    ../../src/storage/Storage_MB85RC.cpp \
    ../../src/HAL_PI.cpp \
    ../../src/buses/Bus_I2C_Pi.cpp \
    ../../src/buses/Bus_SPI_Pi.cpp \
    ../../src/buses/Bus_UART_Pi.cpp

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
    ../../src/HAL_Motors_Sim.h \
    ../../src/HAL_Sensors.h \
    ../../src/HAL_Sensors_Sim.h \
    ../../src/Sim_Comms.h \
    ../../../libs/common/Comm_Data.h \
    ../../../libs/common/Manual_Clock.h \
    ../../../libs/utils/RUDP.h \
    ../../../libs/common/sensors/Sensor_Samples.h \
    ../../src/Battery.h \
    ../../src/sensors/GPS_UBLOX.h \
    ../../src/sensors/i2c.h \
    ../../src/sensors/MPU9250.h \
    ../../src/sensors/MS5611.h \
    ../../src/sensors/OdroidW_ADC.h \
    ../../src/sensors/SRF02.h \
    ../../src/sensors/GPS_Protocol.h \
    ../../src/sensors/GPS_Detector.h \
    ../../src/sensors/spi.h \
    ../../src/sensors/GPS_UBLOX_SPI.h \
    ../../src/sensors/MPU9250_I2C.h \
    ../../src/sensors/MPU9250_SPI.h \
    ../../src/sensors/GPS_UBLOX_UART.h \
    ../../src/sensors/GPS_Detector_I2C.h \
    ../../src/sensors/GPS_Detector_SPI.h \
    ../../src/sensors/GPS_Detector_UART.h \
    ../../src/sensors/GPS_UBLOX_I2C.h \
    ../../src/sensors/IAccelerometer.h \
    ../../src/sensors/IBarometer.h \
    ../../src/sensors/ICompass.h \
    ../../src/sensors/IGPS.h \
    ../../src/sensors/IGyroscope.h \
    ../../src/sensors/IThermometer.h \
    ../../src/sensors/ISonar.h \
    ../../src/sensors/Bus_Config.h \
    ../../src/sensors/ICamera.h \
    ../../src/actuators/IGimbal.h \
    ../../src/actuators/IMotors.h \
    ../../src/sensors/Raspicam.h \
    ../../src/actuators/Motors_PiGPIO.h \
    ../../src/actuators/Gimbal_Servo_PiGPIO.h \
    ../../src/IHAL.h \
    ../../src/sensors/IVoltmeter.h \
    ../../src/sensors/IAmmeter.h \
    ../../src/storage/Storage_MB85RC.h \
    ../../src/HAL_PI.h \
    ../../src/buses/Bus_I2C_Pi.h \
    ../../src/buses/Bus_SPI_Pi.h \
    ../../src/buses/Bus_UART_Pi.h \
    ../../src/buses/IBus_I2C.h \
    ../../src/buses/IBus_SPI.h \
    ../../src/buses/IBus_UART.h

