#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T18:33:20
#
#-------------------------------------------------

#QT       += core quick qml quickcontrols2 positioning

TEMPLATE = app

TARGET = rc
target.path = rc
INSTALLS = target

CONFIG -= qt
CONFIG += c++11

INCLUDEPATH += =/usr/local/include
INCLUDEPATH += =/usr/include/freetype2
INCLUDEPATH += =/opt/vc/include
INCLUDEPATH += ../../src
INCLUDEPATH += ../../src/imgui
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
    LIBS += -L=/opt/vc/lib
    LIBS += -lts
    LIBS += -lmmal_core
    LIBS += -lmmal_util
    LIBS += -lmmal_vc_client
    LIBS += -lvcos
    LIBS += -lbcm_host
    LIBS += -lbrcmGLESv2
    LIBS += -lbrcmEGL
} else {
    QMAKE_MAKEFILE = "Makefile"
    CONFIG(debug, debug|release) {
        DEST_FOLDER = pc/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = pc/release
        DEFINES += NDEBUG
    }
    INCLUDEPATH += /usr/include/freetype2
    LIBS += -lGLESv2
    LIBS += -lEGL
    LIBS += -lglfw
}

LIBS += -lz -lpthread -lfreetype
LIBS += -L$${ROOT_LIBS_PATH}/qmath/lib/$${DEST_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/lib/$${DEST_FOLDER} -lqbase
LIBS += -L$${ROOT_LIBS_PATH}/def_lang/lib/$${DEST_FOLDER} -ldef_lang

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../bin

#RESOURCES += \
#    ../../src/res.qrc

HEADERS += \
    ../../src/Comms.h \
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
    ../../def/settings.def.h \
    ../../def/gen_support.h \
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
    ../../../libs/utils/hw/SPI_PIGPIO.h \
    ../../src/imgui/imconfig.h \
    ../../src/imgui/imgui.h \
    ../../src/imgui/imgui_internal.h \
    ../../src/imgui/misc/freetype/imgui_freetype.h \
    ../../src/GLFW_HAL.h \
    ../../src/IHAL.h \
    ../../src/PI_HAL.h \
    ../../../libs/utils/comms/esp32/Phy.h \
    ../../src/HUD.h

SOURCES += \
    ../../src/main.cpp \
    ../../src/Comms.cpp \
    ../../../libs/lz4/lz4.c \
    ../../../libs/utils/Coordinates.cpp \
    ../../src/Video_Decoder.cpp \
    ../../../libs/utils/hw/bcm2835.c \
    ../../../libs/utils/hw/command.c \
    ../../../libs/utils/hw/pigpio.c \
    ../../../libs/utils/comms/RCP.cpp \
    ../../../libs/utils/comms/UDP_Socket.cpp \
    ../../../libs/utils/hw/SPI_Dev.cpp \
    ../../../libs/utils/hw/I2C_Dev.cpp \
    ../../def/settings.def.cpp \
    ../../../libs/utils/hw/ADS1115.cpp \
    ../../../libs/utils/hw/I2C_BCM.cpp \
    ../../../libs/utils/hw/SPI_BCM.cpp \
    ../../../libs/utils/hw/UART_Dev.cpp \
    ../../../libs/utils/hw/UART_BB.cpp \
    ../../../libs/utils/hw/SPI_PIGPIO.cpp \
    ../../src/imgui/imgui.cpp \
    ../../src/imgui/misc/freetype/imgui_freetype.cpp \
    ../../src/imgui/imgui_demo.cpp \
    ../../src/imgui/imgui_draw.cpp \
    ../../src/imgui_impl_opengl3.cpp \
    ../../src/GLFW_HAL.cpp \
    ../../src/PI_HAL.cpp \
    ../../src/droin_sans_font.cpp \
    ../../../libs/utils/comms/esp32/Phy.cpp \
    ../../src/HUD.cpp

