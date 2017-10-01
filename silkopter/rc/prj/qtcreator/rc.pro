#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T18:33:20
#
#-------------------------------------------------

QT       += core quick qml quickcontrols2 positioning

TARGET = rc
TEMPLATE = app

TARGET = rc
target.path = rc
INSTALLS = target

#CONFIG -= qt
CONFIG += c++11

INCLUDEPATH += =/usr/local/include
INCLUDEPATH += ../../src
INCLUDEPATH += ../../def
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qmath/include
INCLUDEPATH += ../../../../def_lang/include
INCLUDEPATH += ../../../../asio/include
INCLUDEPATH += ../../../libs/utils/hw
INCLUDEPATH += ../../../libs/
INCLUDEPATH += ../../../libs/common/comms/def


QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-unused-variable -Wno-unused-parameter

PRECOMPILED_HEADER = ../../src/stdafx.h
CONFIG *= precompile_header

ROOT_LIBS_PATH = ../../../..

DEFINES += BOOST_ERROR_CODE_HEADER_ONLY
DEFINES += ASIO_STANDALONE
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
    LIBS += -lmmal_core
    LIBS += -lmmal_util
    LIBS += -lmmal_vc_client
    LIBS += -lvcos
    LIBS += -lbcm_host
    LIBS += -lGLESv2
    LIBS += -lEGL
    LIBS += -lpcap
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

LIBS += -lz -lpthread
LIBS += -L$${ROOT_LIBS_PATH}/qmath/lib/$${DEST_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/lib/$${DEST_FOLDER} -lqbase
LIBS += -L$${ROOT_LIBS_PATH}/def_lang/lib/$${DEST_FOLDER} -ldef_lang

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../bin

RESOURCES += \
    ../../src/res.qrc

HEADERS += \
    ../../src/Comms.h \
    ../../src/HAL.h \
    ../../src/stdafx.h \
    ../../../libs/common/Comm_Data.h \
    ../../../libs/utils/Butterworth.h \
    ../../../libs/utils/PID.h \
    ../../../libs/utils/Timed_Scope.h \
    ../../../libs/common/bus/IBus.h \
    ../../../libs/common/bus/II2C.h \
    ../../../libs/common/bus/ISPI.h \
    ../../../libs/common/bus/IUART.h \
    ../../../libs/common/stream/IAcceleration.h \
    ../../../libs/common/stream/IADC.h \
    ../../../libs/common/stream/IAngular_Velocity.h \
    ../../../libs/common/stream/IBattery_State.h \
    ../../../libs/common/stream/ICurrent.h \
    ../../../libs/common/stream/IDistance.h \
    ../../../libs/common/stream/IForce.h \
    ../../../libs/common/stream/IFrame.h \
    ../../../libs/common/stream/ILinear_Acceleration.h \
    ../../../libs/common/stream/IMagnetic_Field.h \
    ../../../libs/common/stream/IPressure.h \
    ../../../libs/common/stream/IPWM.h \
    ../../../libs/common/stream/IStream.h \
    ../../../libs/common/stream/ITemperature.h \
    ../../../libs/common/stream/IThrottle.h \
    ../../../libs/common/stream/ITorque.h \
    ../../../libs/common/stream/IVelocity.h \
    ../../../libs/common/stream/IVideo.h \
    ../../../libs/common/stream/IVoltage.h \
    ../../../libs/common/node/IController.h \
    ../../../libs/common/node/IGenerator.h \
    ../../../libs/common/node/ILPF.h \
    ../../../libs/common/node/IMultirotor_Simulator.h \
    ../../../libs/common/node/INode.h \
    ../../../libs/common/node/IPilot.h \
    ../../../libs/common/node/IProcessor.h \
    ../../../libs/common/node/IResampler.h \
    ../../../libs/common/node/ISink.h \
    ../../../libs/common/node/ISource.h \
    ../../../libs/common/node/ITransformer.h \
    ../../../libs/common/stream/IBool.h \
    ../../../libs/common/stream/IPosition.h \
    ../../../libs/common/stream/IGPS_Info.h \
    ../../../libs/lz4/lz4.h \
    ../../../libs/common/stream/IFloat.h \
    ../../../libs/common/stream/IMultirotor_Commands.h \
    ../../../libs/common/stream/IMultirotor_State.h \
    ../../../libs/common/stream/IProximity.h \
    ../../../libs/common/stream/Stream_Base.h \
    ../../../libs/utils/Coordinates.h \
    ../../../libs/common/node/ICombiner.h \
    ../../../libs/common/stream/IMultirotor_Commands.h \
    ../../../libs/common/stream/IMultirotor_State.h \
    ../../src/Video_Decoder.h \
    ../../../libs/utils/hw/bcm2835.h \
    ../../../libs/utils/hw/command.h \
    ../../../libs/utils/hw/pigpio.h \
    ../../../libs/utils/comms/Channel.h \
    ../../../libs/utils/comms/RCP.h \
    ../../../libs/utils/comms/ISocket.h \
    ../../../libs/utils/comms/UDP_Socket.h \
    ../../../libs/utils/hw/SPI_Dev.h \
    ../../src/ISticks.h \
    ../../../libs/utils/hw/I2C_Dev.h \
    ../../src/Sticks_ADS1115.h \
    ../../src/IStick_Actuators.h \
    ../../src/Stick_Actuators_Throttle_DRV883x.h \
    ../../src/IRotary_Encoder.h \
    ../../src/Rotary_Encoder_PIGPIO.h \
    ../../src/IInput_Device.h \
    ../../src/IButton_Matrix.h \
    ../../src/Button_Matrix_PIGPIO.h \
    ../../src/IButton.h \
    ../../src/Button_PIGPIO.h \
    ../../def/settings.def.h \
    ../../def/gen_support.h \
    ../../src/IHaptic.h \
    ../../../libs/utils/comms/ASIO_Socket_Adapter.h \
    ../../../libs/utils/comms/RCP_Channel.h \
    ../../../libs/utils/hw/ADS1115.h \
    ../../../libs/utils/hw/II2C.h \
    ../../../libs/utils/hw/ISPI.h \
    ../../../libs/utils/hw/I2C_BCM.h \
    ../../../libs/utils/hw/IUART.h \
    ../../../libs/utils/hw/SPI_BCM.h \
    ../../../libs/utils/hw/UART_BB.h \
    ../../../libs/utils/hw/UART_Dev.h \
    ../../src/Battery_Info_ADS1115.h \
    ../../src/Gimbal_Control_ADS1115.h \
    ../../src/IBattery_Info.h \
    ../../src/IGimbal_Control.h \
    ../../../libs/utils/hw/SPI_PIGPIO.h \
    ../../src/QMLMenus.h \
    ../../src/QMLVideoSurface.h \
    ../../src/QMLTelemetry.h \
    ../../src/QMLHUD.h \
    ../../../libs/utils/comms/esp8266/Fec_Encoder.h \
    ../../../libs/utils/comms/esp8266/Phy.h \
    ../../../libs/utils/comms/esp8266/Pool.h \
    ../../../libs/utils/comms/esp8266/utils/fec.h \
    ../../../libs/utils/comms/esp8266/Queue.h

SOURCES += \
    ../../src/main.cpp \
    ../../src/Comms.cpp \
    ../../src/HAL.cpp \
    ../../../libs/lz4/lz4.c \
    ../../../libs/utils/Coordinates.cpp \
    ../../src/Video_Decoder.cpp \
    ../../../libs/utils/hw/bcm2835.c \
    ../../../libs/utils/hw/command.c \
    ../../../libs/utils/hw/pigpio.c \
    ../../../libs/utils/comms/RCP.cpp \
    ../../../libs/utils/comms/UDP_Socket.cpp \
    ../../../libs/utils/hw/SPI_Dev.cpp \
    ../../src/Sticks_ADS1115.cpp \
    ../../../libs/utils/hw/I2C_Dev.cpp \
    ../../src/Stick_Actuators_Throttle_DRV883x.cpp \
    ../../src/Rotary_Encoder_PIGPIO.cpp \
    ../../src/Button_Matrix_PIGPIO.cpp \
    ../../src/Button_PIGPIO.cpp \
    ../../def/settings.def.cpp \
    ../../../libs/utils/hw/ADS1115.cpp \
    ../../../libs/utils/hw/I2C_BCM.cpp \
    ../../../libs/utils/hw/SPI_BCM.cpp \
    ../../../libs/utils/hw/UART_Dev.cpp \
    ../../../libs/utils/hw/UART_BB.cpp \
    ../../src/Battery_Info_ADS1115.cpp \
    ../../src/Gimbal_Control_ADS1115.cpp \
    ../../../libs/utils/hw/SPI_PIGPIO.cpp \
    ../../src/QMLMenus.cpp \
    ../../src/QMLVideoSurface.cpp \
    ../../src/QMLTelemetry.cpp \
    ../../src/QMLHUD.cpp \
    ../../../libs/utils/comms/esp8266/Fec_Encoder.cpp \
    ../../../libs/utils/comms/esp8266/Phy.cpp \
    ../../../libs/utils/comms/esp8266/utils/fec.cpp

DISTFILES += \
    ../../src/node.png \
    ../../src/pilot.png \
    ../../src/processor.png \
    ../../src/resampler.png \
    ../../src/sink.png \
    ../../src/source.png \
    ../../src/lpf.png
