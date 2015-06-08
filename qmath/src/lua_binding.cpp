#include "_qmath.h"
#include "qmath/lua_binding.h"

//#include "luabind/luabind.hpp"

namespace math
{


void bind_to_lua(lua_State* L, const std::string& /*module_name*/)
{
    QASSERT(L);
    if (!L)
    {
        return;
    }

//    using namespace LuaIntf;

//    LuaBinding(L)
//        .beginModule(module_name.c_str())
//            .beginClass<vec2f>("vec2f")
//                .addConstructor(LUA_ARGS(_opt<float>, _opt<float>))
//                .addVariable("x", &vec2f::x, true)
//                .addVariable("y", &vec2f::y, true)
//                .addFunction("set", static_cast<void(vec2f::*)(float,float)>(&vec2f::set), LUA_ARGS(float, float))
//                .addFunction("set_length", &vec2f::set_length, LUA_ARGS(float))
//                .addFunction("normalize", &vec2f::normalize<standard>)
//            .endClass()
//            .beginClass<vec3f>("vec3f")
//                .addConstructor(LUA_ARGS(_opt<float>, _opt<float>, _opt<float>))
//                .addVariable("x", &vec3f::x, true)
//                .addVariable("y", &vec3f::y, true)
//                .addVariable("z", &vec3f::z, true)
//                .addFunction("set", static_cast<void(vec3f::*)(float,float,float)>(&vec3f::set), LUA_ARGS(float, float,float))
//                .addFunction("set_length", &vec3f::set_length, LUA_ARGS(float))
//                .addFunction("normalize", &vec3f::normalize<standard>)
//            .endClass()
//            .beginClass<vec4f>("vec4f")
//                .addConstructor(LUA_ARGS(_opt<float>, _opt<float>, _opt<float>, _opt<float>))
//                .addVariable("x", &vec4f::x, true)
//                .addVariable("y", &vec4f::y, true)
//                .addVariable("z", &vec4f::z, true)
//                .addFunction("set", static_cast<void(vec4f::*)(float,float,float,float)>(&vec4f::set), LUA_ARGS(float,float,float,float))
//                .addFunction("set_length", &vec4f::set_length, LUA_ARGS(float))
//                .addFunction("normalize", &vec4f::normalize<standard>)
//            .endClass()
//            .beginClass<quatf>("quatf")
//                .addConstructor(LUA_ARGS(_opt<float>, _opt<float>, _opt<float>, _opt<float>))
//                .addVariable("x", &quatf::x, true)
//                .addVariable("y", &quatf::y, true)
//                .addVariable("z", &quatf::z, true)
//                .addFunction("set", static_cast<void(quatf::*)(float,float,float,float)>(&quatf::set), LUA_ARGS(float,float,float,float))
//                .addFunction("normalize", &quatf::normalize<standard>)
//            .endClass()

//            .addFunction("epsilon", &math::epsilon<float>)
//            .addFunction("vec2f_equals", static_cast<bool(*)(vec2f const&,vec2f const&)>(&equals), LUA_ARGS(vec2f const&,vec2f const&))
//            .addFunction("vec3f_equals", static_cast<bool(*)(vec3f const&,vec3f const&)>(&equals), LUA_ARGS(vec3f const&,vec3f const&))
//            .addFunction("vec4f_equals", static_cast<bool(*)(vec4f const&,vec4f const&)>(&equals), LUA_ARGS(vec4f const&,vec4f const&))
//            .addFunction("quatf_equals", static_cast<bool(*)(quatf const&,quatf const&)>(&equals), LUA_ARGS(quatf const&,quatf const&))
//    .endModule();

//    luabind::open(L);
//    luabind::module(L, module_name.c_str())
//    [
//            luabind::class_<vec2f>("vec2f")
//                .def(luabind::constructor<>())
//                .def(luabind::constructor<float, float>())
//                  //.def("print", &NumberPrinter::print)
//    ];

}




}
