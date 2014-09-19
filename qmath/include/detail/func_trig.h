#pragma once

namespace math
{
    template<typename T> angle<T> radians(T const& degrees);
    template<typename T> T degrees(angle<T> const& angle);

	//////////////////////////////////////////////////////////////////////////

	//component wise trig for vectors.
	//vec3 val = sin(angles)  is equivalent to
	//	val.x = sin(angles.x)
	//	val.y = sin(angles.y)
	//	val.z = sin(angles.z)
	template<typename T, class Policy = standard> T cos(T const& angle);
	template<typename T, class Policy = standard> T sin(T const& angle);
	template<typename T, class Policy = standard> T tan(T const& angle);
	template<typename T, class Policy = standard> void sin_cos(T const& angle, T& sin, T& cos);

    template<typename T, class Policy = standard> T cos(angle<T> const& angle);
    template<typename T, class Policy = standard> T sin(angle<T> const& angle);
    template<typename T, class Policy = standard> T tan(angle<T> const& angle);
    template<typename T, class Policy = standard> void sin_cos(angle<T> const& angle, T& sin, T& cos);

    template<typename T, class Policy = standard> angle<T> acos(T const& v);
    template<typename T, class Policy = standard> angle<T> asin(T const& v);
    template<typename T, class Policy = standard> angle<T> atan(T const& v);
    template<typename T, class Policy = standard> angle<T> atan2(T const& y, T const& x);
}
