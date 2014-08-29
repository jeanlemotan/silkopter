TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../qbase/include

OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc
RCC_DIR = ./.rcc
UI_DIR = ./.ui

SOURCES += \
    ../../../src/QDataStdAfx.cpp \
    ../../../src/data/File_Sink.cpp \
    ../../../src/data/File_Source.cpp \
    ../../../src/data/File_System.cpp \
    ../../../src/data/Folder_Data_Pack.cpp \
    ../../../src/data/Memory_Sink.cpp \
    ../../../src/data/Memory_Source.cpp \
    ../../../src/data/Sub_Source.cpp \
    ../../../src/data/Zip_Data_Pack.cpp \
    ../../../src/util/Folder_Walker.cpp \
    ../../../src/util/Preprocessor.cpp \
    ../../../src/util/Stream_Encoder.cpp

HEADERS += \
    ../../../include/QData.h \
    ../../../include/QDataStdAfx.h \
    ../../../include/data/Data_Pack.h \
    ../../../include/data/File_Sink.h \
    ../../../include/data/File_Source.h \
    ../../../include/data/File_System.h \
    ../../../include/data/Folder_Data_Pack.h \
    ../../../include/data/Memory_Sink.h \
    ../../../include/data/Memory_Source.h \
    ../../../include/data/Sink.h \
    ../../../include/data/Source.h \
    ../../../include/data/Sub_Source.h \
    ../../../include/data/Zip_Data_Pack.h \
    ../../../include/util/Folder_Walker.h \
    ../../../include/util/Preprocessor.h \
    ../../../include/util/Stream_Encoder.h

