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


INCLUDEPATH += /usr/include/bullet
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
} else {
    LIBS += -lBulletCollision -lBulletDynamics -lLinearMath
}

SOURCES += \
    ../../src/BrainStdAfx.cpp \
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
    ../../src/processor/Comp_AHRS.cpp \
    ../../src/processor/Gravity_Filter.cpp \
    ../../src/source/EHealth.cpp \
    ../../src/controller/Rate_Controller.cpp \
    ../../src/controller/Stability_Controller.cpp \
    ../../src/controller/Velocity_Controller.cpp \
    ../../src/pilot/Multi_Pilot.cpp \
    ../../src/simulator/Multi_Simulation.cpp \
    ../../src/simulator/Multi_Simulator.cpp \
    ../../src/processor/Servo_Gimbal.cpp \
    ../../src/processor/Motor_Mixer.cpp \
    ../../src/source/ADS1115.cpp \
    ../../src/sink/PCA9685.cpp \
    ../../src/generator/Oscillator.cpp \
    ../../src/processor/Comp_ECEF_Position.cpp

HEADERS += \
    ../../src/BrainStdAfx.h \
    ../../src/Toggle.h \
    ../../../libs/utils/Channel.h \
    ../../../libs/utils/chrono.h \
    ../../../libs/utils/PID.h \
    ../../../libs/utils/Serial_Channel.h \
    ../../src/Comms.h \
    ../../../libs/utils/Json_Util.h \
    ../../../libs/utils/Timed_Scope.h \
    ../../../libs/physics/constants.h \
    ../../../libs/common/Comm_Data.h \
    ../../../libs/common/Manual_Clock.h \
    ../../../libs/utils/RUDP.h \
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
    ../../src/processor/LiPo_Battery.h \
    ../../src/processor/EKF_AHRS.h \
    ../../src/processor/Comp_AHRS.h \
    ../../../libs/utils/Butterworth.h \
    ../../../libs/common/node/INode.h \
    ../../../libs/common/node/ISink.h \
    ../../../libs/common/node/ISource.h \
    ../../src/processor/Gravity_Filter.h \
    ../../src/Globals.h \
    ../../src/processor/Throttle_PWM.h \
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
    ../../../libs/common/node/stream/IMagnetic_Field.h \
    ../../../libs/common/node/stream/IPressure.h \
    ../../../libs/common/node/stream/IPWM.h \
    ../../../libs/common/node/stream/ITemperature.h \
    ../../../libs/common/node/stream/ITorque.h \
    ../../../libs/common/node/stream/IVelocity.h \
    ../../../libs/common/node/stream/IVideo.h \
    ../../../libs/common/node/stream/IVoltage.h \
    ../../../libs/common/node/stream/IThrottle.h \
    ../../../libs/common/node/IProcessor.h \
    ../../src/generator/Vec3_Generator.h \
    ../../src/generator/Scalar_Generator.h \
    ../../src/source/EHealth.h \
    ../../../libs/common/node/IController.h \
    ../../../libs/common/node/IGenerator.h \
    ../../../libs/common/node/ILPF.h \
    ../../../libs/common/node/IPilot.h \
    ../../../libs/common/node/IResampler.h \
    ../../../libs/common/node/ITransformer.h \
    ../../src/controller/Rate_Controller.h \
    ../../src/controller/Stability_Controller.h \
    ../../src/controller/Velocity_Controller.h \
    ../../src/lpf/LPF.h \
    ../../src/resampler/Resampler.h \
    ../../src/transformer/Transformer.h \
    ../../src/transformer/Transformer_Inv.h \
    ../../src/pilot/Multi_Pilot.h \
    ../../src/simulator/Multi_Simulation.h \
    ../../src/simulator/Multi_Simulator.h \
    ../../../libs/common/node/IConfig.h \
    ../../../libs/common/node/IMulti_Simulator.h \
    ../../../libs/common/config/Multi.h \
    ../../src/processor/Servo_Gimbal.h \
    ../../src/processor/Motor_Mixer.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/allocators.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/document.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/encodedstream.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/encodings.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/filereadstream.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/filestream.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/filewritestream.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/memorybuffer.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/memorystream.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/prettywriter.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/rapidjson.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/reader.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/stringbuffer.h \
    ../../../../autojsoncxx/rapidjson/include/rapidjson/writer.h \
    ../../src/source/ADS1115.h \
    ../../src/sink/PCA9685.h \
    ../../../libs/common/node/stream/IFloat.h \
    ../../src/generator/Oscillator.h \
    ../../../libs/common/node/stream/IBool.h \
    ../../src/Sample_Accumulator.h \
    ../../src/MPL_Helper.h \
    ../../src/Basic_Output_Stream.h \
    ../../../libs/common/node/stream/IPosition.h \
    ../../src/processor/Comp_ECEF_Position.h \
    ../../../libs/common/node/stream/IGPS_Info.h

