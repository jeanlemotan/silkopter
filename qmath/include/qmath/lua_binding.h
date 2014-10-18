#pragma once

//#include <lua5.2/lua.hpp>
extern "C"
{
struct lua_State;
}

namespace math
{

extern void bind_to_lua(lua_State* L, std::string const& module_name = "qmath");

}
