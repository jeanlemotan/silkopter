#pragma once

#include <string>

///////////////////////////////////////////////////////////////////////////////
// Defines RayIntersectPrimitive functions
///////////////////////////////////////////////////////////////////////////////

namespace math
{
#ifndef __AVR__
	std::string to_string(uint8_t v);
	std::string to_string(uint16_t v);
	std::string to_string(uint32_t v);
	std::string to_string(int8_t v);
	std::string to_string(int16_t v);
	std::string to_string(int32_t v);
	std::string to_string(float v);
	std::string to_string(double v);
	template<class T> std::string to_string(vec2<T> const& v);
	template<class T> std::string to_string(vec3<T> const& v);
	template<class T> std::string to_string(vec4<T> const& v);
	template<class T> std::string to_string(mat2<T> const& v);
	template<class T> std::string to_string(mat3<T> const& v);
	template<class T> std::string to_string(mat4<T> const& v);
	template<class T> std::string to_string(quat<T> const& v);

	bool from_string(uint8_t& v, std::string const& s);
	bool from_string(uint16_t& v, std::string const& s);
	bool from_string(uint32_t& v, std::string const& s);
	bool from_string(int8_t& v, std::string const& s);
	bool from_string(int16_t& v, std::string const& s);
	bool from_string(int32_t& v, std::string const& s);
	bool from_string(float& v, std::string const& s);
	bool from_string(double& v, std::string const& s);

	template <class T> bool from_string(vec2<T>& v, std::string const& s);
	template <class T> bool from_string(vec3<T>& v, std::string const& s);
	template <class T> bool from_string(vec4<T>& v, std::string const& s);
	template <class T> bool from_string(mat2<T>& v, std::string const& s);
	template <class T> bool from_string(mat3<T>& v, std::string const& s);
	template <class T> bool from_string(mat4<T>& v, std::string const& s);
	template <class T> bool from_string(quat<T>& v, std::string const& s);
#endif
}


//util format adapters
namespace q
{
namespace util
{
namespace formatting
{

	template<class Dst_Adapter, class Placeholder, class T>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, math::vec2<T> const& p);

	template<class Dst_Adapter, class Placeholder, class T>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, math::vec3<T> const& p);

	template<class Dst_Adapter, class Placeholder, class T>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, math::vec4<T> const& p);

	template<class Dst_Adapter, class Placeholder, class T>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, math::quat<T> const& p);

}
}
}

