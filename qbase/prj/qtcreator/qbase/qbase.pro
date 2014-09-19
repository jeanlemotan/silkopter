TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
INCLUDEPATH += ../../../include

OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc
RCC_DIR = ./.rcc
UI_DIR = ./.ui

rpi {
    DEFINES+=RASPBERRY_PI
}

rpi {
    CONFIG(debug, debug|release) {
        DEST_FOLDER = rpi/debug
    } else {
        DEST_FOLDER = rpi/release
    }
} else {
    CONFIG(debug, debug|release) {
        DEST_FOLDER = pc/debug
    } else {
        DEST_FOLDER = pc/release
    }
}

DESTDIR = ../../../lib/$${DEST_FOLDER}

SOURCES += \
    ../../../src/Clock.cpp \
    ../../../src/Console_Logger.cpp \
    ../../../src/Debug.cpp \
    ../../../src/Interned_String.cpp \
    ../../../src/Interned_String_DB.cpp \
    ../../../src/Log.cpp \
    ../../../src/Path.cpp \
    ../../../src/Profiler.cpp \
    ../../../src/QBaseStdAfx.cpp \
    ../../../src/thread/Scheduler.cpp \
    ../../../src/thread/Task.cpp \
    ../../../src/thread/Worker_Thread.cpp \
    ../../../src/util/Class_Factory.cpp \
    ../../../src/util/Encoder.cpp \
    ../../../src/util/File_System_Utils.cpp \
    ../../../src/util/Format.cpp \
    ../../../src/util/Rand.cpp \
    ../../../src/util/Util.cpp \
    ../../../include/util/Iterators.inl

HEADERS += \
    ../../../include/Clock.h \
    ../../../include/Console_Logger.h \
    ../../../include/Debug.h \
    ../../../include/FString.h \
    ../../../include/Interned_String.h \
    ../../../include/Interned_String_DB.h \
    ../../../include/Log.h \
    ../../../include/Logger.h \
    ../../../include/Path.h \
    ../../../include/Platform.h \
    ../../../include/Profiler.h \
    ../../../include/QBase.h \
    ../../../include/QBaseStdAfx.h \
    ../../../include/Types.h \
    ../../../include/mem/auto_buffer.h \
    ../../../include/mem/Embedded_Allocator.h \
    ../../../include/mem/FSBAllocator.hh \
    ../../../include/thread/Scheduler.h \
    ../../../include/thread/Task.h \
    ../../../include/thread/Worker_Thread.h \
    ../../../include/util/Circular_Buffer.h \
    ../../../include/util/Class_Factory.h \
    ../../../include/util/crc.h \
    ../../../include/util/Delayer.h \
    ../../../include/util/Encoder.h \
    ../../../include/util/File_System_Utils.h \
    ../../../include/util/Flag_Set.h \
    ../../../include/util/Format.h \
    ../../../include/util/Hash.h \
    ../../../include/util/Iterators.h \
    ../../../include/util/murmurhash.h \
    ../../../include/util/Noncopyable.h \
    ../../../include/util/qrtti.h \
    ../../../include/util/Rand.h \
    ../../../include/util/Signals.h \
    ../../../include/util/Singleton.h \
    ../../../include/util/Tween.h \
    ../../../include/util/Util.h

