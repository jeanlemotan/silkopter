#pragma once

namespace math
{
	
	//////////////////////////////////////////////////////////////////////////
	//func test
	
	template <typename T> struct vec2;
	template <typename T> struct vec3;
	template <typename T> struct vec4;
	template <typename T> struct mat2;
	template <typename T> struct mat3;
	template <typename T> struct mat4;
	template <typename T> struct quat;

	//////////////////////////////////////////////////////////////////////////

	template<typename T> T epsilon();

	//////////////////////////////////////////////////////////////////////////
	// equality
	// All functions below use tolerance, even the ones without the tolerance parameter - they use std::limits::epsilon()
	// If you need exact comparison, use ==

	template<typename T> bool equals(T v1, T v2);
	template<typename T> bool equals(angle<T> const& v1, angle<T> const& v2);
	template<typename T> bool equals(vec2<T> const& v1, vec2<T> const& v2);
	template<typename T> bool equals(vec3<T> const& v1, vec3<T> const& v2);
	template<typename T> bool equals(vec4<T> const& v1, vec4<T> const& v2);
	template<typename T> bool equals(mat2<T> const& v1, mat2<T> const& v2);
	template<typename T> bool equals(mat3<T> const& v1, mat3<T> const& v2);
	template<typename T> bool equals(mat4<T> const& v1, mat4<T> const& v2);
	template<typename T> bool equals(trans3d<T> const& v1, trans3d<T> const& v2);
	template<typename T> bool equals(trans2d<T> const& v1, trans2d<T> const& v2);
	template<typename T> bool equals(quat<T> const& v1, quat<T> const& v2);

	template<typename T> bool equals(T v1, T v2, T tolerance);
	template<typename T> bool equals(angle<T> const& v1, angle<T> const& v2, T tolerance);
	template<typename T> bool equals(vec2<T> const& v1, vec2<T> const& v2, T tolerance);
	template<typename T> bool equals(vec3<T> const& v1, vec3<T> const& v2, T tolerance);
	template<typename T> bool equals(vec4<T> const& v1, vec4<T> const& v2, T tolerance);
	template<typename T> bool equals(mat2<T> const& v1, mat2<T> const& v2, T tolerance);
	template<typename T> bool equals(mat3<T> const& v1, mat3<T> const& v2, T tolerance);
	template<typename T> bool equals(mat4<T> const& v1, mat4<T> const& v2, T tolerance);
	template<typename T> bool equals(trans3d<T> const& v1, trans3d<T> const& v2, T tolerance);
	template<typename T> bool equals(trans2d<T> const& v1, trans2d<T> const& v2, T tolerance);
	template<typename T> bool equals(quat<T> const& v1, quat<T> const& v2, T tolerance);

	//////////////////////////////////////////////////////////////////////////
	// zero with tolerance

	template <typename T> bool is_zero(T v1);
	template <typename T> bool is_zero(angle<T> const& v1);
	template <typename T> bool is_zero(vec2<T> const& v1);
	template <typename T> bool is_zero(vec3<T> const& v1);
	template <typename T> bool is_zero(vec4<T> const& v1);
	template <typename T> bool is_zero(quat<T> const& v1);
	template <typename T> bool is_zero(mat2<T> const& v1);
	template <typename T> bool is_zero(mat3<T> const& v1);
	template <typename T> bool is_zero(mat4<T> const& v1);
	template <typename T> bool is_zero(trans3d<T> const& v1);
	template <typename T> bool is_zero(trans2d<T> const& v1);

	template <typename T> bool is_zero(T v1, T tolerance);
	template <typename T> bool is_zero(angle<T> const& v1, T tolerance);
	template <typename T> bool is_zero(vec2<T> const& v1, T tolerance);
	template <typename T> bool is_zero(vec3<T> const& v1, T tolerance);
	template <typename T> bool is_zero(vec4<T> const& v1, T tolerance);
	template <typename T> bool is_zero(quat<T> const& v1, T tolerance);
	template <typename T> bool is_zero(mat2<T> const& v1, T tolerance);
	template <typename T> bool is_zero(mat3<T> const& v1, T tolerance);
	template <typename T> bool is_zero(mat4<T> const& v1, T tolerance);
	template <typename T> bool is_zero(trans3d<T> const& v1, T tolerance);
	template <typename T> bool is_zero(trans2d<T> const& v1, T tolerance);

	
	//////////////////////////////////////////////////////////////////////////
	// one with tolerance

	template <typename T> bool is_one(T v1);
	template <typename T> bool is_one(vec2<T> const& v1);
	template <typename T> bool is_one(vec3<T> const& v1);
	template <typename T> bool is_one(vec4<T> const& v1);
	template <typename T> bool is_one(quat<T> const& v1);
	template <typename T> bool is_one(mat2<T> const& v1);
	template <typename T> bool is_one(mat3<T> const& v1);
	template <typename T> bool is_one(mat4<T> const& v1);
	template <typename T> bool is_one(trans3d<T> const& v1);
	template <typename T> bool is_one(trans2d<T> const& v1);

	template <typename T> bool is_one(T v1, T tolerance);
	template <typename T> bool is_one(vec2<T> const& v1, T tolerance);
	template <typename T> bool is_one(vec3<T> const& v1, T tolerance);
	template <typename T> bool is_one(vec4<T> const& v1, T tolerance);
	template <typename T> bool is_one(quat<T> const& v1, T tolerance);
	template <typename T> bool is_one(mat2<T> const& v1, T tolerance);
	template <typename T> bool is_one(mat3<T> const& v1, T tolerance);
	template <typename T> bool is_one(mat4<T> const& v1, T tolerance);
	template <typename T> bool is_one(trans3d<T> const& v1, T tolerance);
	template <typename T> bool is_one(trans2d<T> const& v1, T tolerance);

	//////////////////////////////////////////////////////////////////////////
	// identity with tolerance

	template <typename T> bool is_identity(mat2<T> const& v1);
	template <typename T> bool is_identity(mat3<T> const& v1);
	template <typename T> bool is_identity(mat4<T> const& v1);
	template <typename T> bool is_identity(trans3d<T> const& v1);
	template <typename T> bool is_identity(trans2d<T> const& v1);
	template <typename T> bool is_identity(quat<T> const& v1);

	template <typename T> bool is_identity(mat2<T> const& v1, T tolerance);
	template <typename T> bool is_identity(mat3<T> const& v1, T tolerance);
	template <typename T> bool is_identity(mat4<T> const& v1, T tolerance);
	template <typename T> bool is_identity(trans3d<T> const& v1, T tolerance);
	template <typename T> bool is_identity(trans2d<T> const& v1, T tolerance);
	template <typename T> bool is_identity(quat<T> const& v1, T tolerance);

	//////////////////////////////////////////////////////////////////////////
	// positive

	template <typename T> bool is_positive(T v1);
	template <typename T> bool is_positive(angle<T> const& v1);
	template <typename T> bool is_positive(vec2<T> const& v1);
	template <typename T> bool is_positive(vec3<T> const& v1);
	template <typename T> bool is_positive(vec4<T> const& v1);
	template <typename T> bool is_positive(quat<T> const& v1);
	template <typename T> bool is_positive(mat2<T> const& v1);
	template <typename T> bool is_positive(mat3<T> const& v1);
	template <typename T> bool is_positive(mat4<T> const& v1);
	template <typename T> bool is_positive(trans3d<T> const& v1);
	template <typename T> bool is_positive(trans2d<T> const& v1);

	//////////////////////////////////////////////////////////////////////////
	// negative

	template <typename T> bool is_negative(T v1);
	template <typename T> bool is_negative(angle<T> const& v1);
	template <typename T> bool is_negative(vec2<T> const& v1);
	template <typename T> bool is_negative(vec3<T> const& v1);
	template <typename T> bool is_negative(vec4<T> const& v1);
	template <typename T> bool is_negative(quat<T> const& v1);
	template <typename T> bool is_negative(mat2<T> const& v1);
	template <typename T> bool is_negative(mat3<T> const& v1);
	template <typename T> bool is_negative(mat4<T> const& v1);
	template <typename T> bool is_negative(trans3d<T> const& v1);
	template <typename T> bool is_negative(trans2d<T> const& v1);


    //////////////////////////////////////////////////////////////////////////
    // nan

    template <typename T> bool is_nan(T v1);
    template <typename T> bool is_nan(angle<T> const& v1);

	//////////////////////////////////////////////////////////////////////////
	// component wise

	namespace cwise
	{
		template<typename T> vec2<bool> equals(vec2<T> const& v1, vec2<T> const& v2);
		template<typename T> vec3<bool> equals(vec3<T> const& v1, vec3<T> const& v2);
		template<typename T> vec4<bool> equals(vec4<T> const& v1, vec4<T> const& v2);
		template<typename T> vec4<bool> equals(quat<T> const& v1, quat<T> const& v2);

		template<typename T> vec2<bool> equals(vec2<T> const& v1, vec2<T> const& v2, T tolerance);
		template<typename T> vec3<bool> equals(vec3<T> const& v1, vec3<T> const& v2, T tolerance);
		template<typename T> vec4<bool> equals(vec4<T> const& v1, vec4<T> const& v2, T tolerance);
		template<typename T> vec4<bool> equals(quat<T> const& v1, quat<T> const& v2, T tolerance);

		//////////////////////////////////////////////////////////////////////////

		template <typename T> vec2<bool> is_zero(vec2<T> const& v);
		template <typename T> vec3<bool> is_zero(vec3<T> const& v);
		template <typename T> vec4<bool> is_zero(vec4<T> const& v);
		template <typename T> vec4<bool> is_zero(quat<T> const& v);

		template<typename T> vec2<bool> is_zero(vec2<T> const& v, T tolerance);
		template<typename T> vec3<bool> is_zero(vec3<T> const& v, T tolerance);
		template<typename T> vec4<bool> is_zero(vec4<T> const& v, T tolerance);
		template<typename T> vec4<bool> is_zero(quat<T> const& v, T tolerance);

		//////////////////////////////////////////////////////////////////////////
		
		template <typename T> vec2<bool> is_negative(vec2<T> const& v);
		template <typename T> vec3<bool> is_negative(vec3<T> const& v);
		template <typename T> vec4<bool> is_negative(vec4<T> const& v);
		template <typename T> vec4<bool> is_negative(quat<T> const& v);

		//////////////////////////////////////////////////////////////////////////
		
		template <typename T> vec2<bool> is_positive(vec2<T> const& v);
		template <typename T> vec3<bool> is_positive(vec3<T> const& v);
		template <typename T> vec4<bool> is_positive(vec4<T> const& v);
		template <typename T> vec4<bool> is_positive(quat<T> const& v);

		//////////////////////////////////////////////////////////////////////////

		template <typename T> vec2<bool> is_one(vec2<T> const& v);
		template <typename T> vec3<bool> is_one(vec3<T> const& v);
		template <typename T> vec4<bool> is_one(vec4<T> const& v);
		template <typename T> vec4<bool> is_one(quat<T> const& v);

		template<typename T> vec2<bool> is_one(vec2<T> const& v, T tolerance);
		template<typename T> vec3<bool> is_one(vec3<T> const& v, T tolerance);
		template<typename T> vec4<bool> is_one(vec4<T> const& v, T tolerance);
		template<typename T> vec4<bool> is_one(quat<T> const& v, T tolerance);

        //////////////////////////////////////////////////////////////////////////

        template <typename T> vec2<bool> is_nan(vec2<T> const& v);
        template <typename T> vec3<bool> is_nan(vec3<T> const& v);
        template <typename T> vec4<bool> is_nan(vec4<T> const& v);
        template <typename T> vec4<bool> is_nan(quat<T> const& v);

		//////////////////////////////////////////////////////////////////////////

		bool all(vec2<bool> const& v);
		bool all(vec3<bool> const& v);
		bool all(vec4<bool> const& v);
		
		bool any(vec2<bool> const& v);
		bool any(vec3<bool> const& v);
		bool any(vec4<bool> const& v);

		bool none(vec2<bool> const& v);
		bool none(vec3<bool> const& v);
		bool none(vec4<bool> const& v);
	}
	
	
	//////////////////////////////////////////////////////////////////////////
	
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
    template<typename T, class Policy = standard> T length(vec2<T> const& v);
    template<typename T, class Policy = standard> T length(vec3<T> const& v);
    template<typename T, class Policy = standard> T length(vec4<T> const& v);
    template<typename T, class Policy = standard> T length(quat<T> const& v);

	template<typename T> T length_sq(T const& v);
    template<typename T> T length_sq(vec2<T> const& v);
    template<typename T> T length_sq(vec3<T> const& v);
    template<typename T> T length_sq(vec4<T> const& v);
    template<typename T> T length_sq(quat<T> const& v);

	namespace batch
	{
		template<typename T> void dot(T* dst, size_t dstStride, vec3<T> const* src1, size_t src1Stride, vec3<T> const* src2, size_t src2Stride, size_t count);
		template<typename T> void dot(T* dst, size_t dstStride, vec4<T> const* src1, size_t src1Stride, vec4<T> const* src2, size_t src2Stride, size_t count);
	}

}

//#include "func_common.hpp"

