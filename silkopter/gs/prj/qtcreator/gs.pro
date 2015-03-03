#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T18:33:20
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gs
TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += ../../src
INCLUDEPATH += ../../src/qnodeseditor
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qdata/include
INCLUDEPATH += ../../../../qmath/include
INCLUDEPATH += ../../../../qinput/include
INCLUDEPATH += ../../../../q/include
INCLUDEPATH += ../../../libs
INCLUDEPATH += /usr/include/freetype2
INCLUDEPATH += ../../../../autojsoncxx/include
INCLUDEPATH += ../../../../autojsoncxx/rapidjson/include


PRECOMPILED_HEADER = ../../include/stdafx.h

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

LIBS += -lfreetype -lboost_system -lavcodec -lavformat -lswscale -lfftw3 -lz

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
    ../../../libs/common/node/processor/IAHRS.h \
    ../../../libs/common/node/processor/IBattery.h \
    ../../../libs/common/node/processor/ICompass.h \
    ../../../libs/common/node/processor/IFilter.h \
    ../../../libs/common/node/processor/IMultirotor.h \
    ../../../libs/common/node/processor/IProcessor.h \
    ../../../libs/common/node/processor/ITransform.h \
    ../../../libs/common/node/sink/IPWM.h \
    ../../../libs/common/node/sink/ISink.h \
    ../../../libs/common/node/source/ISource.h \
    ../../../libs/common/node/stream/IAcceleration.h \
    ../../../libs/common/node/stream/IADC_Value.h \
    ../../../libs/common/node/stream/IAngular_Velocity.h \
    ../../../libs/common/node/stream/IBattery_State.h \
    ../../../libs/common/node/stream/ICardinal_Points.h \
    ../../../libs/common/node/stream/ICurrent.h \
    ../../../libs/common/node/stream/IDistance.h \
    ../../../libs/common/node/stream/ILinear_Acceleration.h \
    ../../../libs/common/node/stream/ILocation.h \
    ../../../libs/common/node/stream/IMagnetic_Field.h \
    ../../../libs/common/node/stream/IPressure.h \
    ../../../libs/common/node/stream/IPWM_Value.h \
    ../../../libs/common/node/stream/IReference_Frame.h \
    ../../../libs/common/node/stream/IStream.h \
    ../../../libs/common/node/stream/ITemperature.h \
    ../../../libs/common/node/stream/IVideo.h \
    ../../../libs/common/node/stream/IVoltage.h \
    ../../src/stdafx.h \
    ../../../libs/common/Comm_Data.h \
    ../../../libs/utils/Butterworth.h \
    ../../../libs/utils/Channel.h \
    ../../../libs/utils/Json_Helpers.h \
    ../../../libs/utils/PID.h \
    ../../../libs/utils/RUDP.h \
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
    ../../src/qnodeseditor/ui_qnemainwindow.h \
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
    ../../../../autojsoncxx/rapidjson/include/rapidjson/writer.h

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
    ../../src/HAL_Window.cpp
