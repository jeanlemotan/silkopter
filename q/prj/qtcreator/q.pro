TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
INCLUDEPATH += ../../include
INCLUDEPATH += ../../../qbase/include
INCLUDEPATH += ../../../qdata/include
INCLUDEPATH += ../../../qmath/include
INCLUDEPATH += /usr/include/freetype2

QMAKE_CXXFLAGS += -isystem =/opt/vc/include -isystem =/opt/vc/include/interface/vcos/pthreads -isystem =/opt/vc/include/interface/vmcs_host/linux


PRECOMPILED_HEADER = ../../include/QStdAfx.h
CONFIG *= precompile_header

QMAKE_CXXFLAGS += -Wno-unused-variable
QMAKE_CFLAGS += -Wno-unused-variable

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


OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../lib/$${DEST_FOLDER}


SOURCES += \
    ../../src/anim/Animation.cpp \
    ../../src/draw/Painter.cpp \
    ../../src/draw/Sprite.cpp \
    ../../src/draw/Sprite_Base.cpp \
    ../../src/mem/Memory.cpp \
    ../../src/res/Factory.cpp \
    ../../src/res/Resource.cpp \
    ../../src/res/Simple_Share_Streamer.cpp \
    ../../src/res/Impl/Default_Technique_Loader.cpp \
    ../../src/res/Impl/Free_Type_Font_Loader.cpp \
    ../../src/res/Impl/PVR_Loader.cpp \
    ../../src/res/Impl/QAnimation_Loader.cpp \
    ../../src/res/Impl/QModel_Base_Loader.cpp \
    ../../src/res/Impl/Soil_Loader.cpp \
    ../../src/res/Impl/Zup_Sprite_Loader.cpp \
    ../../src/res/Impl/pvrtc/PVRTDecompress.cpp \
    ../../src/res/Impl/soil/image_DXT.cpp \
    ../../src/res/Impl/soil/image_helper.cpp \
    ../../src/res/Impl/soil/SOIL.cpp \
    ../../src/res/Impl/soil/stb_image_aug.cpp \
    ../../src/scene/Camera.cpp \
    ../../src/scene/Model.cpp \
    ../../src/scene/Model_Base.cpp \
    ../../src/scene/Scene.cpp \
    ../../src/scene/Skeleton.cpp \
    ../../src/system/System.cpp \
    ../../src/text/edtaa3func.cpp \
    ../../src/text/Font.cpp \
    ../../src/text/Texter.cpp \
    ../../src/video/Material.cpp \
    ../../src/video/GLES/GLES30_Interface_Ext.cpp \
    ../../src/video/GLES/GLES_Buffer_Impl.cpp \
    ../../src/video/GLES/GLES_Command_Stream.cpp \
    ../../src/video/GLES/GLES_Fbo_Wrapper.cpp \
    ../../src/video/GLES/GLES_Index_Buffer.cpp \
    ../../src/video/GLES/GLES_Interface.cpp \
    ../../src/video/GLES/GLES_Renderer.cpp \
    ../../src/video/GLES/GLES_Render_Target.cpp \
    ../../src/video/GLES/GLES_Shader.cpp \
    ../../src/video/GLES/GLES_Texture.cpp \
    ../../src/video/GLES/GLES_Uniform_Buffer.cpp \
    ../../src/video/GLES/GLES_Vertex_Buffer.cpp \
    ../../src/video/Atlaser.cpp \
    ../../src/video/Dynamic_Image.cpp \
    ../../src/video/Image.cpp \
    ../../src/video/Index_Buffer.cpp \
    ../../src/video/Pass.cpp \
    ../../src/video/Render_Job.cpp \
    ../../src/video/Render_Target.cpp \
    ../../src/video/Shader.cpp \
    ../../src/video/Shader_Source.cpp \
    ../../src/video/Shader_Tokenizer.cpp \
    ../../src/video/Technique.cpp \
    ../../src/video/Texture.cpp \
    ../../src/video/Uber_Shader.cpp \
    ../../src/video/Uniform_Buffer.cpp \
    ../../src/video/Vertex_Buffer.cpp \
    ../../src/video/Vertex_Declaration.cpp

HEADERS += \
    ../../src/res/Impl/Default_Technique_Loader.h \
    ../../src/res/Impl/Free_Type_Font_Loader.h \
    ../../src/res/Impl/PVR_Loader.h \
    ../../src/res/Impl/QAnimation_Loader.h \
    ../../src/res/Impl/QModel_Base_Loader.h \
    ../../src/res/Impl/Soil_Loader.h \
    ../../src/res/Impl/Zup_Sprite_Loader.h \
    ../../src/res/Impl/pvrtc/PVRTDecompress.h \
    ../../src/res/Impl/pvrtc/PVRTGlobal.h \
    ../../src/res/Impl/pvrtc/PVRTTexture.h \
    ../../src/res/Impl/soil/image_DXT.h \
    ../../src/res/Impl/soil/image_helper.h \
    ../../src/res/Impl/soil/SOIL.h \
    ../../src/res/Impl/soil/stbi_DDS_aug.h \
    ../../src/res/Impl/soil/stbi_DDS_aug_c.h \
    ../../src/res/Impl/soil/stb_image_aug.h \
    ../../src/text/edtaa3func.h \
    ../../src/video/GLES/GL_Ext_Functions.h \
    ../../include/Config.h \
    ../../include/Ptr_Fw_Declaration.h \
    ../../include/q.h \
    ../../include/QStdAfx.h \
    ../../include/anim/Animation.h \
    ../../include/anim/Animation_Node.h \
    ../../include/anim/Controller.h \
    ../../include/anim/Pose.h \
    ../../include/draw/Painter.h \
    ../../include/draw/Sprite.h \
    ../../include/draw/Sprite_Base.h \
    ../../include/mem/Memory.h \
    ../../include/res/Factory.h \
    ../../include/res/Loader.h \
    ../../include/res/Resource.h \
    ../../include/res/Simple_Share_Streamer.h \
    ../../include/res/Streamer.h \
    ../../include/scene/Camera.h \
    ../../include/scene/Dummy.h \
    ../../include/scene/Frustum.h \
    ../../include/scene/Model.h \
    ../../include/scene/Model_Base.h \
    ../../include/scene/Scene.h \
    ../../include/scene/Scene_Node.h \
    ../../include/scene/Skeleton.h \
    ../../include/system/System.h \
    ../../include/text/Font.h \
    ../../include/text/Texter.h \
    ../../include/text/utf8/utf8.h \
    ../../include/text/utf8/utf8/core.h \
    ../../include/text/utf8/utf8/unchecked.h \
    ../../include/util/Bounds.h \
    ../../include/util/Fps.h \
    ../../include/video/Atlaser.h \
    ../../include/video/Attribute_Def.h \
    ../../include/video/Auto_Uniform_Def.h \
    ../../include/video/Buffer.h \
    ../../include/video/Buffer_Pool.h \
    ../../include/video/Capabilities.h \
    ../../include/video/Color.h \
    ../../include/video/Dynamic_Image.h \
    ../../include/video/Image.h \
    ../../include/video/Index_Buffer.h \
    ../../include/video/Material.h \
    ../../include/video/Pass.h \
    ../../include/video/Post_FX.h \
    ../../include/video/Renderer.h \
    ../../include/video/Render_Job.h \
    ../../include/video/Render_State.h \
    ../../include/video/Render_State_Def.h \
    ../../include/video/Render_Target.h \
    ../../include/video/Sampler.h \
    ../../include/video/Sampler_Def.h \
    ../../include/video/Shader.h \
    ../../include/video/Shader_Source.h \
    ../../include/video/Shader_Tokenizer.h \
    ../../include/video/Technique.h \
    ../../include/video/Texture.h \
    ../../include/video/Uber_Shader.h \
    ../../include/video/Uniform.h \
    ../../include/video/Uniform_Buffer.h \
    ../../include/video/Uniform_Def.h \
    ../../include/video/Vertex_Buffer.h \
    ../../include/video/Vertex_Declaration.h \
    ../../include/video/Viewport.h \
    ../../include/video/GLES/GLES_Buffer_Impl.h \
    ../../include/video/GLES/GLES_Command_Stream.h \
    ../../include/video/GLES/GLES_Fbo_Wrapper.h \
    ../../include/video/GLES/GLES_Headers.h \
    ../../include/video/GLES/GLES_Index_Buffer.h \
    ../../include/video/GLES/GLES_Interface.h \
    ../../include/video/GLES/GLES_Renderer.h \
    ../../include/video/GLES/GLES_Render_Target.h \
    ../../include/video/GLES/GLES_Shader.h \
    ../../include/video/GLES/GLES_Texture.h \
    ../../include/video/GLES/GLES_Uniform_Buffer.h \
    ../../include/video/GLES/GLES_Vertex_Buffer.h \
    ../../include/video/GLES/std_glext.h

