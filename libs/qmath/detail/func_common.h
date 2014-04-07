#pragma once

namespace math
{
	template<typename T, class Policy = standard> T sqrt(T const& v);
	template<typename T, class Policy = standard> T inv_sqrt(T const& v);

	template<typename T> T positive_zero(T const& v);

	//////////////////////////////////////////////////////////////////////////
	// absolute value

	template<typename T> vec2<T> abs(vec2<T> const& v);
	template<typename T> vec3<T> abs(vec3<T> const& v);
	template<typename T> vec4<T> abs(vec4<T> const& v);

	template<typename T> T abs(T v);

	//////////////////////////////////////////////////////////////////////////

	template<typename T> T sgn(T const& v);
	template<typename T> T sgn(angle<T> const& v);
	template<typename T, class Policy = standard> T pow(T const& a, T const& b);

	template<typename T, class Policy = standard> T inverse(T const& v);
	template<typename T> T transposed(T const& v);
	template<typename T, class Policy = standard> T normalized(T const& v);
	
	template<typename T> typename T::value_t dot(T const& v1, T const& v2);
	template<typename T> T cross(T const& v1, T const& v2);

	// special case for 2d cross
	template<typename T> T cross(vec2<T> const& v1, vec2<T> const& v2);

	// special case for 4d cross
	template<typename T> vec4<T> cross(vec4<T> const& x1, vec4<T> const& x2, vec4<T> const& x3);

	template<typename T> T square(T const& v);
	template<typename T, class Policy = standard> T distance(T const& v1, T const& v2);
	template<typename T> T distance_sq(T const& v1, T const& v2);
	template<typename T, class Policy = standard> T length(T const& v);
	template<typename T> T length_sq(T const& v);

	namespace batch
	{
		template<typename T> void dot(T* dst, size_t dstStride, vec3<T> const* src1, size_t src1Stride, vec3<T> const* src2, size_t src2Stride, size_t count);
		template<typename T> void dot(T* dst, size_t dstStride, vec4<T> const* src1, size_t src1Stride, vec4<T> const* src2, size_t src2Stride, size_t count);
	}

}

//#include "func_common.hpp"

