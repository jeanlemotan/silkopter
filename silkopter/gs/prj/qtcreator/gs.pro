#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T18:33:20
#
#-------------------------------------------------

QT       += core gui opengl network widgets

TARGET = gs
TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += ../../src
INCLUDEPATH += ../../src/qnodeseditor
INCLUDEPATH += ../../../../QMapControl/src
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qdata/include
INCLUDEPATH += ../../../../qmath/include
INCLUDEPATH += ../../../../qinput/include
INCLUDEPATH += ../../../../q/include
INCLUDEPATH += ../../../libs
INCLUDEPATH += /usr/include/freetype2
INCLUDEPATH += /usr/include/ffmpeg
INCLUDEPATH += ../../../../autojsoncxx/include
INCLUDEPATH += ../../../../autojsoncxx/rapidjson/include
INCLUDEPATH += ../../../../eigen
INCLUDEPATH += ../../../brain/autogen

QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter -B$HOME/dev/bin/gold
QMAKE_CFLAGS += -Wno-unused-variable -Wno-unused-parameter -B$HOME/dev/bin/gold

PRECOMPILED_HEADER = ../../src/stdafx.h
CONFIG *= precompile_header

ROOT_LIBS_PATH = ../../../..
CONFIG(debug, debug|release) {
    DEST_FOLDER = pc/debug
} else {
    DEST_FOLDER = pc/release
}

LIBS += -L$${ROOT_LIBS_PATH}/q/lib/$${DEST_FOLDER} -lq
LIBS += -L$${ROOT_LIBS_PATH}/qinput/lib/$${DEST_FOLDER} -lqinput
LIBS += -L$${ROOT_LIBS_PATH}/qdata/lib/$${DEST_FOLDER} -lqdata
LIBS += -L$${ROOT_LIBS_PATH}/qmath/lib/$${DEST_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/lib/$${DEST_FOLDER} -lqbase
LIBS += -L$${ROOT_LIBS_PATH}/QMapControl/lib/$${DEST_FOLDER} -lqmapcontrol

LIBS += -lfreetype -lboost_system -lavcodec -lavformat -lswscale -lfftw3 -lz
LIBS += -lboost_thread
LIBS += -lpcap

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../bin

RESOURCES += \
    ../../src/res.qrc

HEADERS += \
    ../../src/GS.h \
    ../../src/qcustomplot.h \
    ../../src/Comms.h \
    ../../src/HAL.h \
    ../../src/stdafx.h \
    ../../../libs/common/Comm_Data.h \
    ../../../libs/utils/Butterworth.h \
    ../../../libs/utils/Channel.h \
    ../../../libs/utils/PID.h \
    ../../../libs/utils/Serial_Channel.h \
    ../../../libs/utils/Timed_Scope.h \
    ../../../libs/common/node/bus/IBus.h \
    ../../../libs/common/node/bus/II2C.h \
    ../../../libs/common/node/bus/ISPI.h \
    ../../../libs/common/node/bus/IUART.h \
    ../../src/qnodeseditor/qneblock.h \
    ../../src/qnodeseditor/qneconnection.h \
    ../../src/qnodeseditor/qneport.h \
    ../../src/qnodeseditor/qnodeseditor.h \
    ../../src/HAL_Window.h \
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
    ../../src/Stream_Viewer_Widget.h \
    ../../src/Numeric_Viewer.h \
    ../../src/Map_Viewer.h \
    ../../src/Sim_Window.h \
    ../../src/GL_Widget.h \
    ../../src/Render_Widget.h \
    ../../src/Camera_Controller_3D.h \
    ../../../libs/utils/Json_Util.h \
    ../../../libs/common/config/Multi.h \
    ../../../libs/common/node/stream/IAcceleration.h \
    ../../../libs/common/node/stream/IADC.h \
    ../../../libs/common/node/stream/IAngular_Velocity.h \
    ../../../libs/common/node/stream/IBattery_State.h \
    ../../../libs/common/node/stream/ICurrent.h \
    ../../../libs/common/node/stream/IDistance.h \
    ../../../libs/common/node/stream/IForce.h \
    ../../../libs/common/node/stream/IFrame.h \
    ../../../libs/common/node/stream/ILinear_Acceleration.h \
    ../../../libs/common/node/stream/IMagnetic_Field.h \
    ../../../libs/common/node/stream/IPhysical_State.h \
    ../../../libs/common/node/stream/IPressure.h \
    ../../../libs/common/node/stream/IPWM.h \
    ../../../libs/common/node/stream/IStream.h \
    ../../../libs/common/node/stream/ITemperature.h \
    ../../../libs/common/node/stream/IThrottle.h \
    ../../../libs/common/node/stream/ITorque.h \
    ../../../libs/common/node/stream/IVelocity.h \
    ../../../libs/common/node/stream/IVideo.h \
    ../../../libs/common/node/stream/IVoltage.h \
    ../../../libs/common/node/IController.h \
    ../../../libs/common/node/IGenerator.h \
    ../../../libs/common/node/ILPF.h \
    ../../../libs/common/node/IMulti_Simulator.h \
    ../../../libs/common/node/INode.h \
    ../../../libs/common/node/IPilot.h \
    ../../../libs/common/node/IProcessor.h \
    ../../../libs/common/node/IResampler.h \
    ../../../libs/common/node/ISink.h \
    ../../../libs/common/node/ISource.h \
    ../../../libs/common/node/ITransformer.h \
    ../../src/Multi_Config_Window.h \
    ../../src/Axis.h \
    ../../src/Video_Viewer.h \
    ../../../libs/common/node/stream/IBool.h \
    ../../../libs/common/node/stream/IPosition.h \
    ../../../libs/common/node/stream/IGPS_Info.h \
    ../../src/Custom_Item_Delegate.h \
    ../../../libs/utils/RCP_RFMON_Socket.h \
    ../../../libs/utils/RCP_UDP_Socket.h \
    ../../../libs/utils/RCP.h \
    ../../../libs/lz4/lz4.h \
    ../../src/calibration/Acceleration_Calibration_Wizard.h \
    ../../src/calibration/Angular_Velocity_Calibration_Wizard.h \
    ../../src/calibration/Magnetic_Field_Calibration_Wizard.h \
    ../../../libs/common/node/stream/IFloat.h \
    ../../../libs/common/node/stream/IMulti_Input.h \
    ../../../libs/common/node/stream/IMulti_State.h \
    ../../../libs/common/node/stream/IProximity.h \
    ../../../libs/common/node/stream/Stream_Base.h \
    ../../src/input/Gamepad_Multi.h \
    ../../src/input/Multi.h \
    ../../src/hud/Multi_HUD.h

SOURCES += \
    ../../src/GS.cpp \
    ../../src/main.cpp \
    ../../src/qcustomplot.cpp \
    ../../src/Comms.cpp \
    ../../src/HAL.cpp \
    ../../src/qnodeseditor/qneblock.cpp \
    ../../src/qnodeseditor/qneconnection.cpp \
    ../../src/qnodeseditor/qneport.cpp \
    ../../src/qnodeseditor/qnodeseditor.cpp \
    ../../src/HAL_Window.cpp \
    ../../../libs/json_editor/JSON_Model.cpp \
    ../../src/Stream_Viewer_Widget.cpp \
    ../../src/Numeric_Viewer.cpp \
    ../../src/Map_Viewer.cpp \
    ../../src/Sim_Window.cpp \
    ../../src/GL_Widget.cpp \
    ../../src/Render_Widget.cpp \
    ../../src/Camera_Controller_3D.cpp \
    ../../src/Multi_Config_Window.cpp \
    ../../src/Axis.cpp \
    ../../src/Video_Viewer.cpp \
    ../../../libs/utils/RCP_UDP_Socket.cpp \
    ../../../libs/utils/RCP.inl \
    ../../../libs/lz4/lz4.c \
    ../../src/calibration/Acceleration_Calibration_Wizard.cpp \
    ../../src/calibration/Angular_Velocity_Calibration_Wizard.cpp \
    ../../src/calibration/Magnetic_Field_Calibration_Wizard.cpp \
    ../../src/input/Gamepad_Multi.cpp \
    ../../src/hud/Multi_HUD.cpp

FORMS += \
    ../../src/New_Node.ui \
    ../../src/Numeric_Viewer.ui \
    ../../src/GS.ui \
    ../../src/Sim_Window.ui \
    ../../src/Multi_Config_Window.ui \
    ../../src/calibration/Acceleration_Calibration_Wizard_Intro.ui \
    ../../src/calibration/Acceleration_Calibration_Wizard_Reset.ui \
    ../../src/calibration/Acceleration_Calibration_Wizard_Instructions.ui \
    ../../src/calibration/Acceleration_Calibration_Wizard_Collect.ui \
    ../../src/calibration/Acceleration_Calibration_Wizard_Done.ui \
    ../../src/calibration/Angular_Velocity_Calibration_Wizard_Collect.ui \
    ../../src/calibration/Angular_Velocity_Calibration_Wizard_Done.ui \
    ../../src/calibration/Angular_Velocity_Calibration_Wizard_Instructions.ui \
    ../../src/calibration/Angular_Velocity_Calibration_Wizard_Reset.ui \
    ../../src/calibration/Magnetic_Field_Calibration_Wizard_Collect.ui \
    ../../src/calibration/Magnetic_Field_Calibration_Wizard_Done.ui \
    ../../src/calibration/Magnetic_Field_Calibration_Wizard_Instructions.ui \
    ../../src/calibration/Magnetic_Field_Calibration_Wizard_Reset.ui

DISTFILES += \
    ../../src/node.png \
    ../../src/pilot.png \
    ../../src/processor.png \
    ../../src/resampler.png \
    ../../src/sink.png \
    ../../src/source.png \
    ../../src/lpf.png
