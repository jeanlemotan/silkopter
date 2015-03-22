TEMPLATE = app
CONFIG += console
#CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

TARGET = brain
target.path = /home/pi
INSTALLS = target

PRECOMPILED_HEADER = ../../src/BrainStdAfx.h
CONFIG *= precompile_header



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
QMAKE_CXXFLAGS += -Wno-unused-variable
QMAKE_CFLAGS += -Wno-unused-variable


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
    ../../src/storage/Storage_FS.cpp \
    ../../src/storage/Storage_MB85RC.cpp \
    ../../src/Comms.cpp \
    ../../src/main.cpp \
    ../../src/HAL.cpp \
    ../../src/bus/UART_Linux.cpp \
    ../../src/bus/SPI_Linux.cpp \
    ../../src/bus/I2C_Linux.cpp \ 
    ../../src/sink/PIGPIO.cpp \
    ../../src/source/MPU9250.cpp \
    ../../src/source/MS5611.cpp \
    ../../src/source/Raspicam.cpp \
    ../../src/source/RC5T619.cpp \
    ../../src/source/SRF02.cpp \
    ../../src/source/UBLOX.cpp \
    ../../src/processor/ADC_Ammeter.cpp \
    ../../src/processor/ADC_Voltmeter.cpp \
    ../../src/processor/LiPo_Battery.cpp \
    ../../src/processor/Dead_Reckoning.cpp \
    ../../src/processor/EKF_AHRS.cpp \
    ../../src/processor/Comp_AHRS.cpp \
    ../../src/processor/Gravity_Filter.cpp \
    ../../src/processor/Pilot.cpp \
    ../../src/processor/Comp_ECEF_Location.cpp \
    ../../src/processor/Motor_Mixer.cpp \
    ../../src/processor/Rate_Controller.cpp \
    ../../src/processor/Stability_Controller.cpp \
    ../../src/processor/Velocity_Controller.cpp \
    ../../src/generator/Factor_Generator.cpp

HEADERS += \
    ../../src/BrainStdAfx.h \
    ../../src/Toggle.h \
    ../../../libs/utils/Channel.h \
    ../../../libs/utils/chrono.h \
    ../../../libs/utils/PID.h \
    ../../../libs/utils/Serial_Channel.h \
    ../../src/Comms.h \
    ../../../libs/utils/Json_Helpers.h \
    ../../src/Motor_Mixer.h \
    ../../../libs/utils/Timed_Scope.h \
    ../../../libs/physics/constants.h \
    ../../../libs/common/Comm_Data.h \
    ../../../libs/common/Manual_Clock.h \
    ../../../libs/utils/RUDP.h \
    ../../src/storage/Storage_MB85RC.h \
    ../../src/HAL.h \
    ../../src/bus/I2C_Linux.h \
    ../../src/bus/SPI_Linux.h \
    ../../src/bus/UART_Linux.h \ 
    ../../../libs/common/node/bus/II2C.h \
    ../../../libs/common/node/bus/ISPI.h \
    ../../../libs/common/node/bus/IUART.h \
    ../../../libs/common/node/source/IInertial.h \
    ../../src/sink/PIGPIO.h \
    ../../src/source/MPU9250.h \
    ../../src/source/MS5611.h \
    ../../src/source/Raspicam.h \
    ../../src/source/RC5T619.h \
    ../../src/source/SRF02.h \
    ../../src/source/UBLOX.h \
    ../../../libs/common/node/bus/IBus.h \
    ../../../libs/common/node/stream/IStream.h \
    ../../src/processor/ADC_Voltmeter.h \
    ../../src/processor/ADC_Ammeter.h \
    ../../src/processor/LPF.h \
    ../../src/processor/Resampler.h \
    ../../src/processor/LiPo_Battery.h \
    ../../src/processor/EKF_AHRS.h \
    ../../src/processor/Comp_AHRS.h \
    ../../../libs/utils/Butterworth.h \
    ../../../libs/common/node/INode.h \
    ../../../libs/common/node/ISink.h \
    ../../../libs/common/node/ISource.h \
    ../../../libs/common/node/processor/ILPF.h \
    ../../../libs/common/node/processor/IResampler.h \
    ../../src/processor/Gravity_Filter.h \
    ../../src/Globals.h \
    ../../src/Ctor_Helper.h \
    ../../src/processor/Motor_Mixer.h \
    ../../src/processor/Pilot.h \
    ../../../libs/common/node/processor/IPilot.h \
    ../../src/processor/Throttle_PWM.h \
    ../../src/processor/Comp_ECEF_Location.h \
    ../../../libs/common/node/processor/ITransformer.h \
    ../../src/processor/Transformer.h \
    ../../src/processor/Transformer_Inv.h \
    ../../../libs/common/node/data/IData.h \
    ../../../libs/common/node/stream/IAcceleration.h \
    ../../../libs/common/node/stream/IADC.h \
    ../../../libs/common/node/stream/IAngular_Velocity.h \
    ../../../libs/common/node/stream/IBattery_State.h \
    ../../../libs/common/node/stream/ICommands.h \
    ../../../libs/common/node/stream/ICurrent.h \
    ../../../libs/common/node/stream/IDistance.h \
    ../../../libs/common/node/stream/IForce.h \
    ../../../libs/common/node/stream/IFrame.h \
    ../../../libs/common/node/stream/ILinear_Acceleration.h \
    ../../../libs/common/node/stream/ILocation.h \
    ../../../libs/common/node/stream/IMagnetic_Field.h \
    ../../../libs/common/node/stream/IPressure.h \
    ../../../libs/common/node/stream/IPWM.h \
    ../../../libs/common/node/stream/ITemperature.h \
    ../../../libs/common/node/stream/ITorque.h \
    ../../../libs/common/node/stream/IVelocity.h \
    ../../../libs/common/node/stream/IVideo.h \
    ../../../libs/common/node/stream/IVoltage.h \
    ../../src/processor/Rate_Controller.h \
    ../../src/processor/Velocity_Controller.h \
    ../../src/processor/Stability_Controller.h \
    ../../../libs/common/node/processor/IController.h \
    ../../../libs/common/node/stream/IThrottle.h \
    ../../../libs/common/node/IProcessor.h \
    ../../../libs/common/node/stream/IFactor.h \
    ../../../libs/common/node/processor/IGenerator.h \
    ../../src/generator/Factor_Generator.h \
    ../../src/generator/Vec3_Generator.h \
    ../../src/generator/Scalar_Generator.h

