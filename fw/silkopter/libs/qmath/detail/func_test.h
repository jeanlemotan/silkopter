#pragma once

//#include <limits>

namespace math
{
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

	bool equals(float v1, float v2, float tolerance);

	bool equals(angle<float> const& v1, angle<float> const& v2, float tolerance);
	bool equals(vec2<float> const& v1, vec2<float> const& v2, float tolerance);
	bool equals(vec3<float> const& v1, vec3<float> const& v2, float tolerance);
	bool equals(vec4<float> const& v1, vec4<float> const& v2, float tolerance);
	bool equals(mat2<float> const& v1, mat2<float> const& v2, float tolerance);
	bool equals(mat3<float> const& v1, mat3<float> const& v2, float tolerance);
	bool equals(mat4<float> const& v1, mat4<float> const& v2, float tolerance);
	bool equals(trans3d<float> const& v1, trans3d<float> const& v2, float tolerance);
	bool equals(trans2d<float> const& v1, trans2d<float> const& v2, float tolerance);
	bool equals(quat<float> const& v1, quat<float> const& v2, float tolerance);

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

	bool is_zero(float v1, float tolerance);

	bool is_zero(angle<float> const& v1, float tolerance);
	bool is_zero(vec2<float> const& v1, float tolerance);
	bool is_zero(vec3<float> const& v1, float tolerance);
	bool is_zero(vec4<float> const& v1, float tolerance);
	bool is_zero(quat<float> const& v1, float tolerance);
	bool is_zero(mat2<float> const& v1, float tolerance);
	bool is_zero(mat3<float> const& v1, float tolerance);
	bool is_zero(mat4<float> const& v1, float tolerance);
	bool is_zero(trans3d<float> const& v1, float tolerance);
	bool is_zero(trans2d<float> const& v1, float tolerance);

	
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

	bool is_one(float v1, float tolerance);

	bool is_one(vec2<float> const& v1, float tolerance);
	bool is_one(vec3<float> const& v1, float tolerance);
	bool is_one(vec4<float> const& v1, float tolerance);
	bool is_one(quat<float> const& v1, float tolerance);
	bool is_one(mat2<float> const& v1, float tolerance);
	bool is_one(mat3<float> const& v1, float tolerance);
	bool is_one(mat4<float> const& v1, float tolerance);
	bool is_one(trans3d<float> const& v1, float tolerance);
	bool is_one(trans2d<float> const& v1, float tolerance);

	//////////////////////////////////////////////////////////////////////////
	// identity with tolerance

	template <typename T> bool is_identity(mat2<T> const& v1);
	template <typename T> bool is_identity(mat3<T> const& v1);
	template <typename T> bool is_identity(mat4<T> const& v1);
	template <typename T> bool is_identity(trans3d<T> const& v1);
	template <typename T> bool is_identity(trans2d<T> const& v1);
	template <typename T> bool is_identity(quat<T> const& v1);

	bool is_identity(mat2<float> const& v1, float tolerance);
	bool is_identity(mat3<float> const& v1, float tolerance);
	bool is_identity(mat4<float> const& v1, float tolerance);
	bool is_identity(trans3d<float> const& v1, float tolerance);
	bool is_identity(trans2d<float> const& v1, float tolerance);
	bool is_identity(quat<float> const& v1, float tolerance);

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
	// component wise

	namespace cwise
	{
		template<typename T> vec2<bool> equals(vec2<T> const& v1, vec2<T> const& v2);
		template<typename T> vec3<bool> equals(vec3<T> const& v1, vec3<T> const& v2);
		template<typename T> vec4<bool> equals(vec4<T> const& v1, vec4<T> const& v2);
		template<typename T> vec4<bool> equals(quat<T> const& v1, quat<T> const& v2);

		vec2<bool> equals(vec2<float> const& v1, vec2<float> const& v2, float tolerance);
		vec3<bool> equals(vec3<float> const& v1, vec3<float> const& v2, float tolerance);
		vec4<bool> equals(vec4<float> const& v1, vec4<float> const& v2, float tolerance);
		vec4<bool> equals(quat<float> const& v1, quat<float> const& v2, float tolerance);

		//////////////////////////////////////////////////////////////////////////

		template <typename T> vec2<bool> is_zero(vec2<T> const& v);
		template <typename T> vec3<bool> is_zero(vec3<T> const& v);
		template <typename T> vec4<bool> is_zero(vec4<T> const& v);
		template <typename T> vec4<bool> is_zero(quat<T> const& v);

		vec2<bool> is_zero(vec2<float> const& v, float tolerance);
		vec3<bool> is_zero(vec3<float> const& v, float tolerance);
		vec4<bool> is_zero(vec4<float> const& v, float tolerance);
		vec4<bool> is_zero(quat<float> const& v, float tolerance);

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

		vec2<bool> is_one(vec2<float> const& v, float tolerance);
		vec3<bool> is_one(vec3<float> const& v, float tolerance);
		vec4<bool> is_one(vec4<float> const& v, float tolerance);
		vec4<bool> is_one(quat<float> const& v, float tolerance);

		//////////////////////////////////////////////////////////////////////////

		bool all(vec2<bool> const& v);
		bool all(vec3<bool> const& v);
		bool all(vec4<bool> const& v);
		bool any(vec2<bool> const& v);
		bool any(vec3<bool> const& v);
		bool any(vec4<bool> const& v);
	}

}
