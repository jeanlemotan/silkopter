#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T18:33:20
#
#-------------------------------------------------

QT       += core gui opengl network widgets charts quick qml quickwidgets 3dcore 3drender 3dinput 3dextras

TARGET = gs
TEMPLATE = app

#TARGET = gs
#target.path = /home/jean/fpv/gs
#INSTALLS = target

CONFIG += c++11

INCLUDEPATH += ../../src
INCLUDEPATH += ../../src/qnodeseditor
INCLUDEPATH += ../../src/calibration
INCLUDEPATH += ../../src/stream_viewers
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qmath/include
INCLUDEPATH += ../../../../def_lang/include
INCLUDEPATH += ../../../libs
INCLUDEPATH += /usr/include/freetype2
INCLUDEPATH += /usr/include/ffmpeg
INCLUDEPATH += ../../../../autojsoncxx/include
INCLUDEPATH += ../../../../autojsoncxx/rapidjson/include
INCLUDEPATH += ../../../../eigen
INCLUDEPATH += ../../../brain/autogen
INCLUDEPATH += ../../../libs/common/comms/def

QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter -B$HOME/dev/bin/gold
QMAKE_CFLAGS += -Wno-unused-variable -Wno-unused-parameter -B$HOME/dev/bin/gold

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
} else {
    CONFIG(debug, debug|release) {
        DEST_FOLDER = pc/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = pc/release
        DEFINES += NDEBUG
    }
}

LIBS += -L$${ROOT_LIBS_PATH}/def_lang/lib/$${DEST_FOLDER} -ldef_lang
LIBS += -L$${ROOT_LIBS_PATH}/qmath/lib/$${DEST_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/lib/$${DEST_FOLDER} -lqbase

LIBS += -lboost_system -lavutil -lavcodec -lavformat -lswscale -lfftw3f -lz
LIBS += -lboost_thread
#LIBS += -lpcap

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../bin

RESOURCES += \
    ../../res/res.qrc

HEADERS += \
    ../../src/GS.h \
    ../../src/stdafx.h \
    ../../src/Nodes_Widget.h \
    ../../src/Comms.h \
    ../../../libs/common/comms/def/gs_comms.def.h \
    ../../../libs/lz4/lz4.h \
    ../../src/Properties_Delegate.h \
    ../../src/Properties_Model.h \
    ../../src/Properties_Browser.h \
    ../../src/value_editors/Value_Editor_Factory.h \
    ../../src/value_editors/Double_Value_Editor.h \
    ../../src/value_editors/Empty_Value_Editor.h \
    ../../src/value_editors/Enum_Value_Editor.h \
    ../../src/value_editors/Float_Value_Editor.h \
    ../../src/value_editors/IValue_Editor.h \
    ../../src/value_editors/Optional_Value_Editor.h \
    ../../src/value_editors/Qualified_Value.h \
    ../../src/value_editors/String_Value_Editor.h \
    ../../src/value_editors/Variant_Value_Editor.h \
    ../../src/value_editors/Vec2f_Value_Editor.h \
    ../../src/value_editors/Vec2i_Value_Editor.h \
    ../../src/value_editors/Vec3f_Value_Editor.h \
    ../../src/value_editors/Vec3i_Value_Editor.h \
    ../../src/value_editors/Vec4f_Value_Editor.h \
    ../../src/value_editors/Vec4i_Value_Editor.h \
    ../../src/value_editors/details/Numeric_Value_Editor_Helper.h \
    ../../src/value_editors/details/Qualified_Value.inl \
    ../../src/value_editors/details/VecX_Value_Editor_Helper.h \
    ../../src/value_editors/Bool_Value_Editor.h \
    ../../src/value_editors/Int_Value_Editor.h \
    ../../src/Multirotor_Render_Widget.h \
    ../../src/ITab_Widget.h \
    ../../src/UAV_Descriptor_Widget.h \
    ../../src/qnodeseditor/qneblock.h \
    ../../src/qnodeseditor/qneconnection.h \
    ../../src/qnodeseditor/qneport.h \
    ../../src/qnodeseditor/qnodeseditor.h \
    ../../src/calibration/Acceleration_Calibration_Wizard.h \
    ../../src/calibration/Angular_Velocity_Calibration_Wizard.h \
    ../../src/calibration/Magnetic_Field_Calibration_Wizard.h \
    ../../src/Stream_Viewer_Window.h \
    ../../src/stream_viewers/Acceleration_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/IStream_Viewer_Widget.h \
    ../../src/stream_viewers/Numeric_Viewer_Widget.h \
    ../../src/stream_viewers/ADC_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Angular_Velocity_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Battery_State_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Bool_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Current_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Distance_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Float_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Force_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Frame_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/GPS_Info_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Linear_Acceleration_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Magnetic_Field_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Multirotor_Commands_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Multirotor_State_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Position_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Pressure_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Proximity_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/PWM_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Temperature_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Throttle_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Torque_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Velocity_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Video_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/Voltage_Stream_Viewer_Widget.h \
    ../../src/stream_viewers/video/Video_Decoder.h \
    ../../../libs/utils/comms/Channel.h \
    ../../../libs/utils/comms/RCP.h \
    ../../../libs/utils/comms/UDP_Socket.h \
    ../../src/QHexSpinBox.h \
    ../../src/Internal_Telemetry_Widget.h \
    ../../../libs/utils/Coordinates.h \
    ../../src/simulator/Simulator.h \
    ../../src/simulator/OrbitCameraController.h \
    ../../../libs/common/comms/def/messages.def.h

SOURCES += \
    ../../src/GS.cpp \
    ../../src/main.cpp \
    ../../src/Nodes_Widget.cpp \
    ../../src/Comms.cpp \
    ../../../libs/common/comms/def/gs_comms.def.cpp \
    ../../../libs/lz4/lz4.c \
    ../../src/Properties_Delegate.cpp \
    ../../src/Properties_Model.cpp \
    ../../src/Properties_Browser.cpp \
    ../../src/value_editors/Value_Editor_Factory.cpp \
    ../../src/value_editors/Double_Value_Editor.cpp \
    ../../src/value_editors/Empty_Value_Editor.cpp \
    ../../src/value_editors/Enum_Value_Editor.cpp \
    ../../src/value_editors/Float_Value_Editor.cpp \
    ../../src/value_editors/Numeric_Value_Editor_Helper.cpp \
    ../../src/value_editors/Optional_Value_Editor.cpp \
    ../../src/value_editors/String_Value_Editor.cpp \
    ../../src/value_editors/Variant_Value_Editor.cpp \
    ../../src/value_editors/Vec2f_Value_Editor.cpp \
    ../../src/value_editors/Vec2i_Value_Editor.cpp \
    ../../src/value_editors/Vec3f_Value_Editor.cpp \
    ../../src/value_editors/Vec3i_Value_Editor.cpp \
    ../../src/value_editors/Vec4f_Value_Editor.cpp \
    ../../src/value_editors/Vec4i_Value_Editor.cpp \
    ../../src/value_editors/VecX_Value_Editor_Helper.cpp \
    ../../src/value_editors/Bool_Value_Editor.cpp \
    ../../src/value_editors/Int_Value_Editor.cpp \
    ../../src/Multirotor_Render_Widget.cpp \
    ../../src/UAV_Descriptor_Widget.cpp \
    ../../src/qnodeseditor/qneblock.cpp \
    ../../src/qnodeseditor/qneconnection.cpp \
    ../../src/qnodeseditor/qneport.cpp \
    ../../src/qnodeseditor/qnodeseditor.cpp \
    ../../src/calibration/Acceleration_Calibration_Wizard.cpp \
    ../../src/calibration/Angular_Velocity_Calibration_Wizard.cpp \
    ../../src/calibration/Magnetic_Field_Calibration_Wizard.cpp \
    ../../src/Stream_Viewer_Window.cpp \
    ../../src/stream_viewers/Acceleration_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Numeric_Viewer_Widget.cpp \
    ../../src/stream_viewers/ADC_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Angular_Velocity_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Battery_State_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Bool_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Current_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Distance_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Float_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Force_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Frame_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/GPS_Info_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Linear_Acceleration_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Magnetic_Field_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Multirotor_Commands_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Multirotor_State_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Position_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Pressure_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Proximity_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/PWM_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Temperature_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Throttle_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Torque_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Velocity_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Video_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/Voltage_Stream_Viewer_Widget.cpp \
    ../../src/stream_viewers/video/Video_Decoder.cpp \
    ../../../libs/utils/comms/RCP.cpp \
    ../../../libs/utils/comms/UDP_Socket.cpp \
    ../../src/Internal_Telemetry_Widget.cpp \
    ../../../libs/utils/Coordinates.cpp \
    ../../src/simulator/Simulator.cpp \
    ../../src/simulator/OrbitCameraController.cpp \
    ../../../libs/common/comms/def/messages.def.cpp

FORMS += \
    ../../src/GS.ui \
    ../../src/UAV_Descriptor_Widget.ui \
    ../../src/New_Node_Dialog.ui \
    ../../src/calibration/Acceleration_Calibration_Wizard_Collect.ui \
    ../../src/calibration/Acceleration_Calibration_Wizard_Done.ui \
    ../../src/calibration/Acceleration_Calibration_Wizard_Instructions.ui \
    ../../src/calibration/Acceleration_Calibration_Wizard_Intro.ui \
    ../../src/calibration/Acceleration_Calibration_Wizard_Reset.ui \
    ../../src/calibration/Angular_Velocity_Calibration_Wizard_Collect.ui \
    ../../src/calibration/Angular_Velocity_Calibration_Wizard_Done.ui \
    ../../src/calibration/Angular_Velocity_Calibration_Wizard_Instructions.ui \
    ../../src/calibration/Angular_Velocity_Calibration_Wizard_Reset.ui \
    ../../src/calibration/Magnetic_Field_Calibration_Wizard_Collect.ui \
    ../../src/calibration/Magnetic_Field_Calibration_Wizard_Done.ui \
    ../../src/calibration/Magnetic_Field_Calibration_Wizard_Instructions.ui \
    ../../src/calibration/Magnetic_Field_Calibration_Wizard_Reset.ui \
    ../../src/Stream_Viewer_Window.ui \
    ../../src/stream_viewers/Numeric_Viewer_Widget.ui \
    ../../src/simulator/Simulator.ui

DISTFILES += \

