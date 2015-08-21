TEMPLATE = app
CONFIG += console
#CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

TARGET = brain
target.path = /root
INSTALLS = target

PRECOMPILED_HEADER = ../../src/BrainStdAfx.h
CONFIG *= precompile_header



#QMAKE_CXXFLAGS_RELEASE += -g
#QMAKE_CFLAGS_RELEASE += -g
#QMAKE_LFLAGS_RELEASE =

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

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../bin/$${DEST_FOLDER}

QMAKE_CXXFLAGS += -isystem =/opt/vc/include -isystem =/opt/vc/include/interface/vcos/pthreads -isystem =/opt/vc/include/interface/vmcs_host/linux
QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-unused-variable -Wno-unused-parameter
QMAKE_LFLAGS += -rdynamic


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
LIBS += -lpcap
rpi {
    LIBS += -lnl-3
    LIBS += -lnl-genl-3
    LIBS += -lpigpio
    LIBS += -lmmal_core
    LIBS += -lmmal_util
    LIBS += -lmmal_vc_client
    LIBS += -lvcos
    LIBS += -lbcm_host
    LIBS += -lGLESv2
    LIBS += -lEGL
} else {
    LIBS += -lBulletCollision -lBulletDynamics -lLinearMath -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video
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
    ../../src/simulator/Multi_Simulation.cpp \
    ../../src/simulator/Multi_Simulator.cpp \
    ../../src/processor/Servo_Gimbal.cpp \
    ../../src/processor/Motor_Mixer.cpp \
    ../../src/source/ADS1115.cpp \
    ../../src/sink/PCA9685.cpp \
    ../../src/generator/Oscillator.cpp \
    ../../src/processor/Comp_ECEF_Position.cpp \
    ../../../libs/utils/RCP_UDP_Socket.cpp \
    ../../../libs/utils/RCP_RFMON_Socket.cpp \
    ../../../libs/utils/radiotap/radiotap.cpp \
    ../../../libs/utils/RCP.inl \
    ../../../libs/lz4/lz4.c \
    ../../src/source/OpenCV_Capture.cpp \
    ../../src/source/SRF01.cpp \
    ../../src/brain/Multi_Brain.cpp \
    ../../src/pilot/Multi_Pilot.cpp \
    ../../src/source/MaxSonar.cpp \
    ../../src/processor/Throttle_To_PWM.cpp \
    ../../../libs/utils/Coordinates.cpp

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
    ../../src/HAL.h \
    ../../src/bus/I2C_Linux.h \
    ../../src/bus/SPI_Linux.h \
    ../../src/bus/UART_Linux.h \ 
    ../../../libs/common/node/source/IInertial.h \
    ../../src/sink/PIGPIO.h \
    ../../src/source/MPU9250.h \
    ../../src/source/MS5611.h \
    ../../src/source/Raspicam.h \
    ../../src/source/RC5T619.h \
    ../../src/source/SRF02.h \
    ../../src/source/UBLOX.h \
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
    ../../../libs/common/node/data/IData.h \
    ../../../libs/common/node/IProcessor.h \
    ../../src/generator/Vec3_Generator.h \
    ../../src/generator/Scalar_Generator.h \
    ../../src/source/EHealth.h \
    ../../../libs/common/node/IController.h \
    ../../../libs/common/node/IGenerator.h \
    ../../../libs/common/node/ILPF.h \
    ../../../libs/common/node/IResampler.h \
    ../../../libs/common/node/ITransformer.h \
    ../../src/controller/Rate_Controller.h \
    ../../src/controller/Stability_Controller.h \
    ../../src/controller/Velocity_Controller.h \
    ../../src/lpf/LPF.h \
    ../../src/resampler/Resampler.h \
    ../../src/transformer/Transformer.h \
    ../../src/transformer/Transformer_Inv.h \
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
    ../../src/generator/Oscillator.h \
    ../../src/Sample_Accumulator.h \
    ../../src/MPL_Helper.h \
    ../../src/Basic_Output_Stream.h \
    ../../src/processor/Comp_ECEF_Position.h \
    ../../../libs/utils/RCP.h \
    ../../../libs/utils/RCP_UDP_Socket.h \
    ../../../libs/utils/radiotap/ieee80211_radiotap.h \
    ../../../libs/utils/radiotap/radiotap.h \
    ../../../libs/utils/RCP_RFMON_Socket.h \
    ../../../libs/lz4/lz4.h \
    ../../src/source/OpenCV_Capture.h \
    ../../../libs/utils/Serialization.h \
    ../../src/source/SRF01.h \
    ../../src/brain/Multi_Brain.h \
    ../../../libs/common/node/IBrain.h \
    ../../src/pilot/Multi_Pilot.h \
    ../../../libs/common/node/IPilot.h \
    ../../src/source/MaxSonar.h \
    ../../src/processor/Throttle_To_PWM.h \
    ../../../libs/utils/Coordinates.h \
    ../../../libs/common/stream/IAcceleration.h \
    ../../../libs/common/stream/IADC.h \
    ../../../libs/common/stream/IAngular_Velocity.h \
    ../../../libs/common/stream/IBattery_State.h \
    ../../../libs/common/stream/IBool.h \
    ../../../libs/common/stream/ICurrent.h \
    ../../../libs/common/stream/IDistance.h \
    ../../../libs/common/stream/IFloat.h \
    ../../../libs/common/stream/IForce.h \
    ../../../libs/common/stream/IFrame.h \
    ../../../libs/common/stream/IGPS_Info.h \
    ../../../libs/common/stream/ILinear_Acceleration.h \
    ../../../libs/common/stream/IMagnetic_Field.h \
    ../../../libs/common/stream/IMulti_Input.h \
    ../../../libs/common/stream/IMulti_State.h \
    ../../../libs/common/stream/IPosition.h \
    ../../../libs/common/stream/IPressure.h \
    ../../../libs/common/stream/IProximity.h \
    ../../../libs/common/stream/IPWM.h \
    ../../../libs/common/stream/IStream.h \
    ../../../libs/common/stream/ITemperature.h \
    ../../../libs/common/stream/IThrottle.h \
    ../../../libs/common/stream/ITorque.h \
    ../../../libs/common/stream/IVelocity.h \
    ../../../libs/common/stream/IVideo.h \
    ../../../libs/common/stream/IVoltage.h \
    ../../../libs/common/stream/Stream_Base.h \
    ../../../libs/common/bus/IBus.h \
    ../../../libs/common/bus/II2C.h \
    ../../../libs/common/bus/ISPI.h \
    ../../../libs/common/bus/IUART.h

DISTFILES +=

