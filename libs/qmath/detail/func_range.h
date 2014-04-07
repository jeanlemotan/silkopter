#pragma once

namespace math
{
	//min
	template<typename T> vec2<T> min(vec2<T> const& a, vec2<T> const& b);
	template<typename T> vec3<T> min(vec3<T> const& a, vec3<T> const& b);
	template<typename T> vec4<T> min(vec4<T> const& a, vec4<T> const& b);
	template<typename T> T min(T a, T b);

	//max
	template<typename T> vec2<T> max(vec2<T> const& a, vec2<T> const& b);
	template<typename T> vec3<T> max(vec3<T> const& a, vec3<T> const& b);
	template<typename T> vec4<T> max(vec4<T> const& a, vec4<T> const& b);
	template<typename T> T max(T a, T b);

	//clamping
	template<typename T> vec2<T> clamp(vec2<T> const& x, vec2<T> const& min, vec2<T> const& max);
	template<typename T> vec3<T> clamp(vec3<T> const& x, vec3<T> const& min, vec3<T> const& max);
	template<typename T> vec4<T> clamp(vec4<T> const& x, vec4<T> const& min, vec4<T> const& max);
	template<typename T> T clamp(T x, T min, T max);

	//component wise min
	namespace cwise
	{
		template<typename T> T min(vec2<T> const& a);
		template<typename T> T max(vec2<T> const& a);
		template<typename T> T min(vec3<T> const& a);
		template<typename T> T max(vec3<T> const& a);
		template<typename T> T min(vec4<T> const& a);
		template<typename T> T max(vec4<T> const& a);
		template<typename T> T min(quat<T> const& a);
		template<typename T> T max(quat<T> const& a);
		template<typename T> T min(mat2<T> const& a);
		template<typename T> T max(mat2<T> const& a);
		template<typename T> T min(mat3<T> const& a);
		template<typename T> T max(mat3<T> const& a);
		template<typename T> T min(mat4<T> const& a);
		template<typename T> T max(mat4<T> const& a);
	}

	//rounding
	template<typename T> T floor(T const& x);
	template<typename T> T ceil(T const& x);
	template<typename T> T fract(T const& x);

	//rounds to the closest integer
	template<typename T> T round(T const& x);
	float round(float x);

	//round to decimal
	template<typename T> T round(T const& x, uint8_t decimals);
	float round(float x, uint8_t decimals);
}

//#include "func_range.hpp"
