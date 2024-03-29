TEMPLATE = lib
CONFIG += staticlib

CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

PRECOMPILED_HEADER = ../../include/QBaseStdAfx.h
CONFIG *= precompile_header

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

OBJECTS_DIR = ./qbase/.obj/$${DEST_FOLDER}
MOC_DIR = ./qbase/.moc/$${DEST_FOLDER}
RCC_DIR = ./qbase/.rcc/$${DEST_FOLDER}
UI_DIR = ./qbase/.ui/$${DEST_FOLDER}
DESTDIR = ../../lib/$${DEST_FOLDER}

QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter -ffunction-sections -fdata-sections
QMAKE_CFLAGS += -Wno-unused-variable -Wno-unused-parameter -ffunction-sections -fdata-sections
QMAKE_LFLAGS += -rdynamic -lpthread -Wl,--no-whole-archive -Wl,--gc-sections

INCLUDEPATH += =/usr/local/include
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../asio/include

SOURCES += \
    ../../src/Console_Logger.cpp \
    ../../src/Debug.cpp \
    ../../src/Log.cpp \
    ../../src/Path.cpp \
    ../../src/Profiler.cpp \
    ../../src/util/Encoder.cpp \
    ../../src/util/File_System_Utils.cpp \
    ../../src/util/Format.cpp \
    ../../src/util/Util.cpp \
    ../../include/util/Iterators.inl

HEADERS += \
    ../../include/Console_Logger.h \
    ../../include/Debug.h \
    ../../include/FString.h \
    ../../include/Log.h \
    ../../include/Logger.h \
    ../../include/Path.h \
    ../../include/Platform.h \
    ../../include/Profiler.h \
    ../../include/QBase.h \
    ../../include/QBaseStdAfx.h \
    ../../include/Types.h \
    ../../include/mem/auto_buffer.h \
    ../../include/mem/Embedded_Allocator.h \
    ../../include/mem/FSBAllocator.hh \
    ../../include/util/Circular_Buffer.h \
    ../../include/util/crc.h \
    ../../include/util/Delayer.h \
    ../../include/util/Encoder.h \
    ../../include/util/File_System_Utils.h \
    ../../include/util/Flag_Set.h \
    ../../include/util/Format.h \
    ../../include/util/Hash.h \
    ../../include/util/Iterators.h \
    ../../include/util/murmurhash.h \
    ../../include/util/Noncopyable.h \
    ../../include/util/Signals.h \
    ../../include/util/Tween.h \
    ../../include/util/Util.h

