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
    ../../../src/data.cpp

HEADERS += \
    ../../../include/qmath.h \
    ../../../include/_qmath.h \
    ../../../include/detail/aabb2.h \
    ../../../include/detail/aabb2.hpp \
    ../../../include/detail/aabb3.h \
    ../../../include/detail/aabb3.hpp \
    ../../../include/detail/angle.h \
    ../../../include/detail/angle.hpp \
    ../../../include/detail/cast.h \
    ../../../include/detail/cast.hpp \
    ../../../include/detail/color.h \
    ../../../include/detail/color.hpp \
    ../../../include/detail/constants.h \
    ../../../include/detail/func_binary.h \
    ../../../include/detail/func_binary.hpp \
    ../../../include/detail/func_common.h \
    ../../../include/detail/func_common.hpp \
    ../../../include/detail/func_interp.h \
    ../../../include/detail/func_interp.hpp \
    ../../../include/detail/func_projection.h \
    ../../../include/detail/func_projection.hpp \
    ../../../include/detail/func_range.h \
    ../../../include/detail/func_range.hpp \
    ../../../include/detail/func_rayintersect.h \
    ../../../include/detail/func_rayintersect.hpp \
    ../../../include/detail/func_static_constructors.h \
    ../../../include/detail/func_static_constructors.hpp \
    ../../../include/detail/func_string.h \
    ../../../include/detail/func_string.hpp \
    ../../../include/detail/func_test.h \
    ../../../include/detail/func_test.hpp \
    ../../../include/detail/func_transform.h \
    ../../../include/detail/func_transform.hpp \
    ../../../include/detail/func_trig.h \
    ../../../include/detail/func_trig.hpp \
    ../../../include/detail/line2.h \
    ../../../include/detail/line2.hpp \
    ../../../include/detail/line3.h \
    ../../../include/detail/line3.hpp \
    ../../../include/detail/mat2.h \
    ../../../include/detail/mat2.hpp \
    ../../../include/detail/mat3.h \
    ../../../include/detail/mat3.hpp \
    ../../../include/detail/mat4.h \
    ../../../include/detail/mat4.hpp \
    ../../../include/detail/packet.h \
    ../../../include/detail/packet.hpp \
    ../../../include/detail/plane.h \
    ../../../include/detail/plane.hpp \
    ../../../include/detail/quat.h \
    ../../../include/detail/quat.hpp \
    ../../../include/detail/rect.h \
    ../../../include/detail/rect.hpp \
    ../../../include/detail/standard_types.h \
    ../../../include/detail/trans2d.h \
    ../../../include/detail/trans2d.hpp \
    ../../../include/detail/trans3d.h \
    ../../../include/detail/trans3d.hpp \
    ../../../include/detail/triangle3.h \
    ../../../include/detail/triangle3.hpp \
    ../../../include/detail/vec2.h \
    ../../../include/detail/vec2.hpp \
    ../../../include/detail/vec3.h \
    ../../../include/detail/vec3.hpp \
    ../../../include/detail/vec4.h \
    ../../../include/detail/vec4.hpp

