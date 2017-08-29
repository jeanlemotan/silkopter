TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../qbase/include
INCLUDEPATH += ../../../asio/include
INCLUDEPATH += /usr/include/lua5.1
INCLUDEPATH += =/usr/local/include


#QMAKE_CXXFLAGS_RELEASE += -g
#QMAKE_CFLAGS_RELEASE += -g
#QMAKE_LFLAGS_RELEASE =

QMAKE_CXXFLAGS += -Wno-unused-variable
QMAKE_CFLAGS += -Wno-unused-variable

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
} android {
    DEFINES+=ANDROID
    QMAKE_MAKEFILE = "Makefile.android"
    MAKEFILE = "Makefile.android"
    CONFIG(debug, debug|release) {
        DEST_FOLDER = android/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = android/release
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
    ../../src/data.cpp \
    ../../src/lua_binding.cpp

HEADERS += \
    ../../include/qmath.h \
    ../../include/_qmath.h \
    ../../include/qmath/aabb2.h \
    ../../include/qmath/aabb2.hpp \
    ../../include/qmath/aabb3.h \
    ../../include/qmath/aabb3.hpp \
    ../../include/qmath/angle.h \
    ../../include/qmath/angle.hpp \
    ../../include/qmath/cast.h \
    ../../include/qmath/cast.hpp \
    ../../include/qmath/color.h \
    ../../include/qmath/color.hpp \
    ../../include/qmath/constants.h \
    ../../include/qmath/func_binary.h \
    ../../include/qmath/func_binary.hpp \
    ../../include/qmath/func_common.h \
    ../../include/qmath/func_common.hpp \
    ../../include/qmath/func_interp.h \
    ../../include/qmath/func_interp.hpp \
    ../../include/qmath/func_projection.h \
    ../../include/qmath/func_projection.hpp \
    ../../include/qmath/func_range.h \
    ../../include/qmath/func_range.hpp \
    ../../include/qmath/func_rayintersect.h \
    ../../include/qmath/func_rayintersect.hpp \
    ../../include/qmath/func_static_constructors.h \
    ../../include/qmath/func_static_constructors.hpp \
    ../../include/qmath/func_string.h \
    ../../include/qmath/func_string.hpp \
    ../../include/qmath/func_test.h \
    ../../include/qmath/func_test.hpp \
    ../../include/qmath/func_transform.h \
    ../../include/qmath/func_transform.hpp \
    ../../include/qmath/func_trig.h \
    ../../include/qmath/func_trig.hpp \
    ../../include/qmath/line2.h \
    ../../include/qmath/line2.hpp \
    ../../include/qmath/line3.h \
    ../../include/qmath/line3.hpp \
    ../../include/qmath/lua_binding.h \
    ../../include/qmath/mat2.h \
    ../../include/qmath/mat2.hpp \
    ../../include/qmath/mat3.h \
    ../../include/qmath/mat3.hpp \
    ../../include/qmath/mat4.h \
    ../../include/qmath/mat4.hpp \
    ../../include/qmath/packet.h \
    ../../include/qmath/packet.hpp \
    ../../include/qmath/plane.h \
    ../../include/qmath/plane.hpp \
    ../../include/qmath/quat.h \
    ../../include/qmath/quat.hpp \
    ../../include/qmath/rect.h \
    ../../include/qmath/rect.hpp \
    ../../include/qmath/standard_types.h \
    ../../include/qmath/trans2d.h \
    ../../include/qmath/trans2d.hpp \
    ../../include/qmath/trans3d.h \
    ../../include/qmath/trans3d.hpp \
    ../../include/qmath/triangle3.h \
    ../../include/qmath/triangle3.hpp \
    ../../include/qmath/vec2.h \
    ../../include/qmath/vec2.hpp \
    ../../include/qmath/vec3.h \
    ../../include/qmath/vec3.hpp \
    ../../include/qmath/vec4.h \
    ../../include/qmath/vec4.hpp

