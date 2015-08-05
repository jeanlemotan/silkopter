TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../qbase/include

PRECOMPILED_HEADER = ../../include/QDataStdAfx.h
CONFIG *= precompile_header


#QMAKE_CXXFLAGS_RELEASE += -g
#QMAKE_CFLAGS_RELEASE += -g
#QMAKE_LFLAGS_RELEASE =

QMAKE_CXXFLAGS += -Wno-unused-variable
QMAKE_CFLAGS += -Wno-unused-variable


rpi {
    DEFINES+=RASPBERRY_PI
    QMAKE_MAKEFILE = "Makefile.rpi"
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
DESTDIR = ../../lib/$${DEST_FOLDER}



SOURCES += \
    ../../src/data/File_Sink.cpp \
    ../../src/data/File_Source.cpp \
    ../../src/data/File_System.cpp \
    ../../src/data/Folder_Data_Pack.cpp \
    ../../src/data/Memory_Sink.cpp \
    ../../src/data/Memory_Source.cpp \
    ../../src/data/Sub_Source.cpp \
    ../../src/data/Zip_Data_Pack.cpp \
    ../../src/util/Folder_Walker.cpp \
    ../../src/util/Preprocessor.cpp \
    ../../src/util/Stream_Encoder.cpp

HEADERS += \
    ../../include/QData.h \
    ../../include/QDataStdAfx.h \
    ../../include/data/Data_Pack.h \
    ../../include/data/File_Sink.h \
    ../../include/data/File_Source.h \
    ../../include/data/File_System.h \
    ../../include/data/Folder_Data_Pack.h \
    ../../include/data/Memory_Sink.h \
    ../../include/data/Memory_Source.h \
    ../../include/data/Sink.h \
    ../../include/data/Source.h \
    ../../include/data/Sub_Source.h \
    ../../include/data/Zip_Data_Pack.h \
    ../../include/util/Folder_Walker.h \
    ../../include/util/Preprocessor.h \
    ../../include/util/Stream_Encoder.h

