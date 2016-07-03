#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T18:33:20
#
#-------------------------------------------------

QT       += core gui opengl network widgets

TARGET = gs
TEMPLATE = app

#TARGET = gs
#target.path = /home/jean/fpv/gs
#INSTALLS = target

CONFIG += c++11

INCLUDEPATH += ../../src
INCLUDEPATH += ../../src/qnodeseditor
INCLUDEPATH += ../../src/calibration
INCLUDEPATH += ../../../../QMapControl/src
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qdata/include
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
LIBS += -L$${ROOT_LIBS_PATH}/qdata/lib/$${DEST_FOLDER} -lqdata
LIBS += -L$${ROOT_LIBS_PATH}/qmath/lib/$${DEST_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/lib/$${DEST_FOLDER} -lqbase
LIBS += -L$${ROOT_LIBS_PATH}/QMapControl/lib/$${DEST_FOLDER} -lqmapcontrol

LIBS += -lfreetype -lboost_system -lavutil -lavcodec -lavformat -lswscale -lfftw3 -lz
LIBS += -lboost_thread
LIBS += -lpcap

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
    ../../../libs/utils/RCP_UDP_Socket.h \
    ../../../libs/utils/RCP.h \
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
    ../../src/calibration/Magnetic_Field_Calibration_Wizard.h

SOURCES += \
    ../../src/GS.cpp \
    ../../src/main.cpp \
    ../../src/Nodes_Widget.cpp \
    ../../src/Comms.cpp \
    ../../../libs/utils/RCP_UDP_Socket.cpp \
    ../../../libs/utils/RCP.cpp \
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
    ../../src/calibration/Magnetic_Field_Calibration_Wizard.cpp

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
    ../../src/calibration/Magnetic_Field_Calibration_Wizard_Reset.ui

DISTFILES += \

