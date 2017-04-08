TEMPLATE = app
CONFIG += console
#CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

TARGET = fc
target.path = /root
INSTALLS = target

PRECOMPILED_HEADER = ../../src/FCStdAfx.h
CONFIG *= precompile_header



#QMAKE_CXXFLAGS_RELEASE += -g
#QMAKE_CFLAGS_RELEASE += -g
#QMAKE_LFLAGS_RELEASE =

rpi {
    DEFINES+=RASPBERRY_PI
    QMAKE_MAKEFILE = "Makefile.rpi"
    MAKEFILE = "Makefile.rpi"
    CONFIG(debug, debug|release) {
        DEST_FOLDER = rpi/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = rpi/release
        DEFINES += NDEBUG
    }
} else {
    QMAKE_MAKEFILE = "Makefile"
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

QMAKE_CXXFLAGS += -isystem =/opt/vc/include -isystem =/opt/vc/include/interface/vcos/pthreads -isystem =/opt/vc/include/interface/vmcs_host/linux -isystem =/usr/include/bullet
QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-unused-variable -Wno-unused-parameter
QMAKE_LFLAGS += -rdynamic -Wl,--whole-archive -lpthread -Wl,--no-whole-archive


INCLUDEPATH += /usr/include/bullet
INCLUDEPATH += ../../src
INCLUDEPATH += ../../def
INCLUDEPATH += ../../autogen
INCLUDEPATH += ../../../libs
INCLUDEPATH += ../../../libs/common/comms/def
INCLUDEPATH += ../../../../def_lang/include
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qmath/include
INCLUDEPATH += ../../../../ReactPhysics3D/src
INCLUDEPATH += ../../../../eigen

LIBS += -L=/opt/vc/lib/

ROOT_LIBS_PATH = ../../../..

LIBS += -L$${ROOT_LIBS_PATH}/def_lang/lib/$${DEST_FOLDER} -ldef_lang
LIBS += -L$${ROOT_LIBS_PATH}/qmath/lib/$${DEST_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/lib/$${DEST_FOLDER} -lqbase

LIBS += -lpthread
LIBS += -lboost_system
LIBS += -lboost_thread
LIBS += -lrt
LIBS += -lz
LIBS += -lpcap
LIBS += -lBulletCollision
LIBS += -lBulletDynamics
LIBS += -lLinearMath
rpi {
    LIBS += -lmmal_core
    LIBS += -lmmal_util
    LIBS += -lmmal_vc_client
    LIBS += -lvcos
    LIBS += -lbcm_host
#    LIBS += -lGLESv2
#    LIBS += -lEGL
} else {
    LIBS += -lopencv_core
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_highgui
    LIBS += -lopencv_ml
    #LIBS += -lopencv_videoio
}

SOURCES += \
    ../../src/main.cpp \
    ../../src/sink/PIGPIO.cpp \
    ../../src/source/MPU9250.cpp \
    ../../src/source/MS5611.cpp \
    ../../src/source/Raspicam.cpp \
    ../../src/source/RC5T619.cpp \
    ../../src/source/SRF02.cpp \
    ../../src/source/UBLOX.cpp \
    ../../src/processor/ADC_Ammeter.cpp \
    ../../src/processor/ADC_Voltmeter.cpp \
    ../../src/processor/Comp_AHRS.cpp \
    ../../src/processor/Gravity_Filter.cpp \
    ../../src/source/EHealth.cpp \
    ../../src/controller/Rate_Controller.cpp \
    ../../src/simulator/Multirotor_Simulation.cpp \
    ../../src/simulator/Multirotor_Simulator.cpp \
    ../../src/processor/Servo_Gimbal.cpp \
    ../../src/processor/Motor_Mixer.cpp \
    ../../src/sink/PCA9685.cpp \
    ../../src/generator/Oscillator.cpp \
    ../../../libs/lz4/lz4.c \
    ../../src/source/OpenCV_Capture.cpp \
    ../../src/source/SRF01.cpp \
    ../../src/brain/Multirotor_Brain.cpp \
    ../../src/source/MaxSonar.cpp \
    ../../src/processor/Throttle_To_PWM.cpp \
    ../../../libs/utils/Coordinates.cpp \
    ../../src/processor/Comp_ECEF.cpp \
    ../../src/processor/KF_ECEF.cpp \
    ../../src/processor/Proximity.cpp \
    ../../src/source/AVRADC.cpp \
    ../../src/processor/Pressure_Velocity.cpp \
    ../../src/processor/ENU_Frame_System.cpp \
    ../../src/source/RaspiCamControl.cpp \
    ../../src/brain/LiPo_Battery.cpp \
    ../../src/pilot/Multirotor_Pilot.cpp \
    ../../def/hal.def.cpp \
    ../../../libs/common/comms/def/gs_comms.def.cpp \
    ../../src/uav_properties/IMultirotor_Properties.cpp \
    ../../src/HAL.cpp \
    ../../src/RC_Comms.cpp \
    ../../src/GS_Comms.cpp \
    ../../src/uav_properties/Hexa_Multirotor_Properties.cpp \
    ../../src/uav_properties/Hexatri_Multirotor_Properties.cpp \
    ../../src/uav_properties/Octo_Multirotor_Properties.cpp \
    ../../src/uav_properties/Octoquad_Multirotor_Properties.cpp \
    ../../src/uav_properties/Quad_Multirotor_Properties.cpp \
    ../../src/uav_properties/Tri_Multirotor_Properties.cpp \
    ../../src/source/CPPM_Receiver.cpp \
    ../../../libs/utils/hw/bcm2835.c \
    ../../../libs/utils/hw/command.c \
    ../../../libs/utils/hw/pigpio.c \
    ../../../libs/utils/comms/RCP.cpp \
    ../../../libs/utils/comms/RF4463F30_Socket.cpp \
    ../../../libs/utils/comms/RFMON_Socket.cpp \
    ../../../libs/utils/comms/UDP_Socket.cpp \
    ../../../libs/utils/hw/radiotap/radiotap.cpp \
    ../../../libs/utils/hw/RF4463F30.cpp \
    ../../../libs/utils/hw/Si4463.cpp \
    ../../../libs/utils/hw/SPI_Dev.cpp \
    ../../../libs/utils/comms/Video_Streamer.cpp \
    ../../../libs/utils/comms/fec.cpp \
    ../../../libs/utils/hw/RFM22B.cpp \
    ../../../libs/utils/comms/RC_Phy.cpp \
    ../../../libs/utils/comms/RC_Protocol.cpp \
    ../../../libs/common/comms/def/messages.def.cpp \
    ../../src/FCStdAfx.cpp \
    ../../../libs/utils/hw/ADS1115.cpp \
    ../../../libs/utils/hw/I2C_BCM.cpp \
    ../../../libs/utils/hw/I2C_Dev.cpp \
    ../../../libs/utils/hw/SPI_BCM.cpp \
    ../../../libs/utils/hw/UART_BB.cpp \
    ../../../libs/utils/hw/UART_Dev.cpp \
    ../../src/source/ADS1115_Source.cpp \
    ../../src/bus/I2C_BCM_Bus.cpp \
    ../../src/bus/I2C_Linux_Bus.cpp \
    ../../src/bus/SPI_BCM_Bus.cpp \
    ../../src/bus/SPI_Linux_Bus.cpp \
    ../../src/bus/UART_Linux_Bus.cpp \
    ../../src/bus/UART_BB_Bus.cpp \
    ../../src/processor/Quad_Multirotor_Motor_Mixer.cpp

HEADERS += \
    ../../../libs/utils/chrono.h \
    ../../../libs/utils/PID.h \
    ../../../libs/utils/Timed_Scope.h \
    ../../../libs/physics/constants.h \
    ../../../libs/common/Comm_Data.h \
    ../../../libs/common/Manual_Clock.h \
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
    ../../src/lpf/LPF.h \
    ../../src/resampler/Resampler.h \
    ../../src/transformer/Transformer.h \
    ../../src/transformer/Transformer_Inv.h \
    ../../src/simulator/Multirotor_Simulation.h \
    ../../src/simulator/Multirotor_Simulator.h \
    ../../../libs/common/node/IConfig.h \
    ../../src/processor/Servo_Gimbal.h \
    ../../src/processor/Motor_Mixer.h \
    ../../src/sink/PCA9685.h \
    ../../src/generator/Oscillator.h \
    ../../src/Sample_Accumulator.h \
    ../../src/MPL_Helper.h \
    ../../src/Basic_Output_Stream.h \
    ../../../libs/lz4/lz4.h \
    ../../src/source/OpenCV_Capture.h \
    ../../../libs/utils/Serialization.h \
    ../../src/source/SRF01.h \
    ../../src/brain/Multirotor_Brain.h \
    ../../../libs/common/node/IBrain.h \
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
    ../../../libs/common/stream/IMultirotor_State.h \
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
    ../../../libs/common/bus/IUART.h \
    ../../../libs/kalman/ekfilter.hpp \
    ../../../libs/kalman/ekfilter_impl.hpp \
    ../../../libs/kalman/kfilter.hpp \
    ../../../libs/kalman/kfilter_impl.hpp \
    ../../../libs/kalman/kmatrix.hpp \
    ../../../libs/kalman/kmatrix_impl.hpp \
    ../../../libs/kalman/ktypes.hpp \
    ../../../libs/kalman/kvector.hpp \
    ../../../libs/kalman/kvector_impl.hpp \
    ../../src/processor/Comp_ECEF.h \
    ../../src/processor/KF_ECEF.h \
    ../../src/processor/Proximity.h \
    ../../../libs/common/stream/IMultirotor_Commands.h \
    ../../src/source/AVRADC.h \
    ../../../libs/common/node/ICombiner.h \
    ../../src/combiner/Combiner.h \
    ../../src/processor/Pressure_Velocity.h \
    ../../src/processor/ENU_Frame_System.h \
    ../../src/source/RaspiCamControl.h \
    ../../src/brain/LiPo_Battery.h \
    ../../src/pilot/Multirotor_Pilot.h \
    ../../../libs/common/node/IMultirotor_Simulator.h \
    ../../def/gen_support.h \
    ../../def/hal.def.h \
    ../../../libs/common/comms/def/gs_comms.def.h \
    ../../src/uav_properties/IMultirotor_Properties.h \
    ../../src/uav_properties/IUAV_Properties.h \
    ../../src/HAL.h \
    ../../src/RC_Comms.h \
    ../../src/GS_Comms.h \
    ../../src/uav_properties/Hexa_Multirotor_Properties.h \
    ../../src/uav_properties/Hexatri_Multirotor_Properties.h \
    ../../src/uav_properties/Octo_Multirotor_Properties.h \
    ../../src/uav_properties/Octoquad_Multirotor_Properties.h \
    ../../src/uav_properties/Quad_Multirotor_Properties.h \
    ../../src/uav_properties/Tri_Multirotor_Properties.h \
    ../../src/source/CPPM_Receiver.h \
    ../../../libs/utils/comms/Channel.h \
    ../../../libs/utils/comms/ISocket.h \
    ../../../libs/utils/comms/RCP.h \
    ../../../libs/utils/comms/RF4463F30_Socket.h \
    ../../../libs/utils/comms/RFMON_Socket.h \
    ../../../libs/utils/comms/UDP_Socket.h \
    ../../../libs/utils/hw/radiotap/ieee80211_radiotap.h \
    ../../../libs/utils/hw/radiotap/radiotap.h \
    ../../../libs/utils/hw/bcm2835.h \
    ../../../libs/utils/hw/command.h \
    ../../../libs/utils/hw/pigpio.h \
    ../../../libs/utils/hw/RF4463F30_Config_old.h \
    ../../../libs/utils/hw/RF4463F30_Config.h \
    ../../../libs/utils/hw/RF4463F30.h \
    ../../../libs/utils/hw/si446x_patch.h \
    ../../../libs/utils/hw/Si4463.h \
    ../../../libs/utils/hw/SPI_Dev.h \
    ../../../libs/utils/comms/Video_Streamer.h \
    ../../../libs/utils/Pool.h \
    ../../../libs/utils/comms/fec.h \
    ../../../libs/utils/hw/RFM22B.h \
    ../../../libs/utils/comms/RC_Phy.h \
    ../../../libs/utils/comms/RC_Protocol.h \
    ../../../libs/common/comms/def/messages.def.h \
    ../../../libs/common/stream/IMultirotor_Simulator_State.h \
    ../../src/FCStdAfx.h \
    ../../../libs/utils/hw/ADS1115.h \
    ../../../libs/utils/hw/I2C_BCM.h \
    ../../../libs/utils/hw/I2C_Dev.h \
    ../../../libs/utils/hw/II2C.h \
    ../../../libs/utils/hw/ISPI.h \
    ../../../libs/utils/hw/IUART.h \
    ../../../libs/utils/hw/SPI_BCM.h \
    ../../../libs/utils/hw/UART_BB.h \
    ../../../libs/utils/hw/UART_Dev.h \
    ../../src/source/ADS1115_Source.h \
    ../../src/bus/I2C_BCM_Bus.h \
    ../../src/bus/I2C_Linux_Bus.h \
    ../../src/bus/SPI_BCM_Bus.h \
    ../../src/bus/SPI_Linux_Bus.h \
    ../../src/bus/UART_Linux_Bus.h \
    ../../src/bus/UART_BB_Bus.h \
    ../../src/processor/Quad_Multirotor_Motor_Mixer.h

DISTFILES +=

