#pragma once

namespace math
{
	float degrees(float radians);
	float radians(float degrees);

	template<typename T> T degrees(T const& radians);
	template<typename T> T radians(T const& degrees);

	//////////////////////////////////////////////////////////////////////////

	float cos(float angle);
	float sin(float angle);
	float tan(float angle);
	float acos(float angle);
	float asin(float angle);
	float atan(float angle);
	float atan2(float y, float x);
	void sin_cos(float angle, float& sin, float& cos);

	//component wise trig for vectors.
	//vec3 val = sin(angles)  is equivalent to
	//	val.x = sin(angles.x)
	//	val.y = sin(angles.y)
	//	val.z = sin(angles.z)
	template<typename T> T cos(T const& angle);
	template<typename T> T sin(T const& angle);
	template<typename T> T tan(T const& angle);
	template<typename T> T acos(T const& angle);
	template<typename T> T asin(T const& angle);
	template<typename T> T atan(T const& angle);
	template<typename T> T atan2(T const& y, T const& x);
	template<typename T> void sin_cos(T const& angle, T& sin, T& cos);

}
