#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T18:33:20
#
#-------------------------------------------------

QT       += core quick qml

TARGET = gs
TEMPLATE = app

TARGET = gs
target.path = ~/gs
INSTALLS = target

CONFIG += c++11

INCLUDEPATH += ../../src
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qdata/include
INCLUDEPATH += ../../../../qmath/include
INCLUDEPATH += ../../../../qinput/include
INCLUDEPATH += ../../../libs
INCLUDEPATH += ../../../../autojsoncxx/include
INCLUDEPATH += ../../../../autojsoncxx/rapidjson/include
INCLUDEPATH += ../../../../eigen
INCLUDEPATH += ../../../brain/autogen

QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-unused-variable -Wno-unused-parameter

PRECOMPILED_HEADER = ../../src/stdafx.h
CONFIG *= precompile_header

ROOT_LIBS_PATH = ../../../..

rpi {
    DEFINES+=RASPBERRY_PI
    CONFIG(debug, debug|release) {
        DEST_FOLDER = rpi/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = rpi/release
        DEFINES += NDEBUG
    }
    LIBS += -lfreetype
    LIBS += -lmmal_core
    LIBS += -lmmal_util
    LIBS += -lmmal_vc_client
    LIBS += -lvcos
    LIBS += -lbcm_host
    LIBS += -lGLESv2
    LIBS += -lEGL
    LIBS += -lboost_system
} else {
    CONFIG(debug, debug|release) {
        DEST_FOLDER = pc/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = pc/release
        DEFINES += NDEBUG
    }
    LIBS += -lfreetype -lavutil -lavcodec -lavformat -lswscale -lboost_system
}

LIBS += -L$${ROOT_LIBS_PATH}/qinput/lib/$${DEST_FOLDER} -lqinput
LIBS += -L$${ROOT_LIBS_PATH}/qdata/lib/$${DEST_FOLDER} -lqdata
LIBS += -L$${ROOT_LIBS_PATH}/qmath/lib/$${DEST_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/lib/$${DEST_FOLDER} -lqbase

LIBS += -lfftw3 -lpcap -lz

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
    ../../../libs/utils/Channel.h \
    ../../../libs/utils/PID.h \
    ../../../libs/utils/Serial_Channel.h \
    ../../../libs/utils/Timed_Scope.h \
    ../../../libs/common/bus/IBus.h \
    ../../../libs/common/bus/II2C.h \
    ../../../libs/common/bus/ISPI.h \
    ../../../libs/common/bus/IUART.h \
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
    ../../../libs/json_editor/JSON_Model.h \
    ../../../libs/utils/Json_Util.h \
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
    ../../../libs/utils/RCP_RFMON_Socket.h \
    ../../../libs/utils/RCP_UDP_Socket.h \
    ../../../libs/utils/RCP.h \
    ../../../libs/lz4/lz4.h \
    ../../../libs/common/stream/IFloat.h \
    ../../../libs/common/stream/IMultirotor_Commands.h \
    ../../../libs/common/stream/IMultirotor_State.h \
    ../../../libs/common/stream/IProximity.h \
    ../../../libs/common/stream/Stream_Base.h \
    ../../../libs/utils/Coordinates.h \
    ../../../libs/utils/radiotap/ieee80211_radiotap.h \
    ../../../libs/utils/radiotap/radiotap.h \
    ../../../libs/common/node/ICombiner.h \
    ../../src/Nodes.h \
    ../../../libs/common/config/Multirotor_Config.h \
    ../../../libs/common/config/UAV_Config.h \
    ../../../libs/common/stream/IMultirotor_Commands.h \
    ../../../libs/common/stream/IMultirotor_State.h \
    ../../src/Comms_QMLProxy.h \
    ../../src/Menus_QMLProxy.h \
    ../../src/OS_QMLProxy.h \
    ../../src/Comms_Slots.h \
    ../../src/NodeEditor/UAVNodeEditor.h \
    ../../src/NodeEditor/NodeDefModel.h

SOURCES += \
    ../../src/main.cpp \
    ../../src/Comms.cpp \
    ../../src/HAL.cpp \
    ../../../libs/json_editor/JSON_Model.cpp \
    ../../../libs/utils/RCP_UDP_Socket.cpp \
    ../../../libs/lz4/lz4.c \
    ../../../libs/utils/Coordinates.cpp \
    ../../../libs/utils/RCP.cpp \
    ../../../libs/utils/RCP_RFMON_Socket.cpp \
    ../../../libs/utils/radiotap/radiotap.cpp \
    ../../src/Comms_QMLProxy.cpp \
    ../../src/Menus_QMLProxy.cpp \
    ../../src/OS_QMLProxy.cpp \
    ../../src/Comms_Slots.cpp \
    ../../src/NodeEditor/UAVNodeEditor.cpp \
    ../../src/NodeEditor/NodeDefModel.cpp

DISTFILES += \
    ../../src/node.png \
    ../../src/pilot.png \
    ../../src/processor.png \
    ../../src/resampler.png \
    ../../src/sink.png \
    ../../src/source.png \
    ../../src/lpf.png
