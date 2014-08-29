TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
INCLUDEPATH += ../../../include
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qmath/include

OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc
RCC_DIR = ./.rcc
UI_DIR = ./.ui

SOURCES += \
    ../../../src/Gamepad.cpp \
    ../../../src/Input_Mgr.cpp \
    ../../../src/Keyboard.cpp \
    ../../../src/QInputStdAfx.cpp \
    ../../../src/Sensors.cpp \
    ../../../src/Touchscreen.cpp

HEADERS += \
    ../../../include/Gamepad.h \
    ../../../include/Input_Mgr.h \
    ../../../include/Keyboard.h \
    ../../../include/QInput.h \
    ../../../include/QInputStdAfx.h \
    ../../../include/Sensors.h \
    ../../../include/Touch.h \
    ../../../include/Touchscreen.h

