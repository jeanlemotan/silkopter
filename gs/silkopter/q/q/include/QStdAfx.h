#ifndef _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _SCL_SECURE_NO_WARNINGS
#	define _SCL_SECURE_NO_WARNINGS
#endif

//android ndk needed this
#define _GLIBCXX_USE_C99_MATH 1

#include "Config.h"

//#include <string>
// #include <vector>
// #include <list>
// #include <map>
#include <unordered_map>
// #include <limits>
#include <deque>
#include <set>
#include <array>
// #include <algorithm>
// #include <utility>
// #include <memory>
// #include <functional>

//android ndk needed this
//#define BOOST_NO_INTRINSIC_WCHAR_T
//#define BOOST_SP_DISABLE_THREADS //was ok, but not anymore. use atomic increment ios patch instead
//#define BOOST_NO_TYPEID

//#include <boost/config/no_tr1/cmath.hpp>
//#include <boost/shared_array.hpp>
//#include <boost/bind.hpp>
//#include <boost/ptr_container/ptr_vector.hpp>
//#include <boost/xpressive/xpressive.hpp>

#include <boost/optional.hpp>

//#include "../../pugixml/src/pugixml.hpp"

#include "QBase.h"
#include "QData.h"
#include "_qmath.h"

////////////////////////////////////////////////////////////////////////////
// Internal header files

#include "Types.h"
#include "Ptr_Fw_Declaration.h"

#include "mem/Memory.h"

#include "util/Util.h"

#include "res/Loader.h"
#include "res/Streamer.h"
#include "res/Factory.h"

#include "system/System.h"

#include "util/Bounds.h"
#include "util/Fps.h"

#include "res/Resource.h"

#include "video/Shader_Source.h"
#include "video/Sampler_Def.h"
#include "video/Uniform_Def.h"
#include "video/Vertex_Declaration.h"
#include "video/Vertex_Buffer.h"
#include "video/Index_Buffer.h"
#include "video/Uniform_Buffer.h"
#include "video/Attribute_Def.h"
#include "video/Viewport.h"
#include "video/Uniform.h"
#include "video/Shader.h"
#include "video/Capabilities.h"
#include "video/Texture.h"
#include "video/Uber_Shader.h"
#include "video/Sampler.h"
#include "video/Render_State.h"
#include "video/Render_State_Def.h"
#include "video/Pass.h"
#include "video/Technique.h"
#include "video/Material.h"
#include "video/Post_FX.h"
#include "video/Renderer.h"
#include "video/Buffer_Pool.h"
#include "video/Render_Target.h"
#include "video/Color.h"
#include "video/Image.h"
#include "video/Dynamic_Image.h"
#include "video/Atlaser.h"
#include "video/Render_Job.h"

#include "res/Simple_Share_Streamer.h"

#include "anim/Pose.h"
#include "anim/Animation_Node.h"
#include "anim/Animation.h"
#include "anim/Controller.h"

#include "scene/Frustum.h"
#include "scene/Skeleton.h"
#include "scene/Camera.h"
#include "scene/Scene_Node.h"
#include "scene/Model_Base.h"
#include "scene/Model.h"
#include "scene/Scene.h"

#include "draw/Painter.h"
#include "draw/Sprite_Base.h"
#include "draw/Sprite.h"

#include "text/utf8/utf8.h"
#include "text/Font.h"
#include "text/Texter.h"

#include "video/GLES/GLES_Renderer.h"

