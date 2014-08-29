#pragma once

///////////////////////////////////////////////////////////////////////////////
// Defines transform functions
///////////////////////////////////////////////////////////////////////////////

namespace math
{
	//////////////////////////////////////////////////////////////////////////
	// arithmetic
	//////////////////////////////////////////////////////////////////////////

	template<typename T> trans3d<T>& multiply(trans3d<T>& result, trans3d<T> const& a, trans3d<T> const& b);
	template<typename T> trans2d<T>& multiply(trans2d<T>& result, trans2d<T> const& a, trans2d<T> const& b);
	template<typename T> mat4<T>& multiply(mat4<T>& result, mat4<T> const& a, mat4<T> const& b);
	template<typename T> mat3<T>& multiply(mat3<T>& result, mat3<T> const& a, mat3<T> const& b);
	template<typename T> mat2<T>& multiply(mat2<T>& result, mat2<T> const& a, mat2<T> const& b);

	namespace cwise
	{
		//mat * mat
		template<typename T> mat4<T>& multiply(mat4<T>& result, mat4<T> const& a, mat4<T> const& b);
		template<typename T> mat4<T>& add(mat4<T>& result, mat4<T> const& a, mat4<T> const& b);
		template<typename T> mat4<T>& substract(mat4<T>& result, mat4<T> const& a, mat4<T> const& b);

		template<typename T> mat3<T>& multiply(mat3<T>& result, mat3<T> const& a, mat3<T> const& b);
		template<typename T> mat3<T>& add(mat3<T>& result, mat3<T> const& a, mat3<T> const& b);
		template<typename T> mat3<T>& substract(mat3<T>& result, mat3<T> const& a, mat3<T> const& b);

		template<typename T> mat2<T>& multiply(mat2<T>& result, mat2<T> const& a, mat2<T> const& b);
		template<typename T> mat2<T>& add(mat2<T>& result, mat2<T> const& a, mat2<T> const& b);
		template<typename T> mat2<T>& substract(mat2<T>& result, mat2<T> const& a, mat2<T> const& b);

		//mat * scalar
		template<typename T> mat4<T>& multiply(mat4<T>& result, mat4<T> const& a, T b);
		template<typename T> mat4<T>& add(mat4<T>& result, mat4<T> const& a, T b);
		template<typename T> mat4<T>& substract(mat4<T>& result, mat4<T> const& a, T b);

		template<typename T> mat3<T>& multiply(mat3<T>& result, mat3<T> const& a, T  b);
		template<typename T> mat3<T>& add(mat3<T>& result, mat3<T> const& a, T b);
		template<typename T> mat3<T>& substract(mat3<T>& result, mat3<T> const& a, T b);

		template<typename T> mat2<T>& multiply(mat2<T>& result, mat2<T> const& a, T  b);
		template<typename T> mat2<T>& add(mat2<T>& result, mat2<T> const& a, T b);
		template<typename T> mat2<T>& substract(mat2<T>& result, mat2<T> const& a, T b);
	}

	//////////////////////////////////////////////////////////////////////////
	// transforms
	//////////////////////////////////////////////////////////////////////////

	//Multiplies all vector components with the matrix.
	template<typename T> vec2<T> transform(mat2<T> const& m, vec2<T> const& v);

	//Multiplies all vector components with the matrix.
	template<typename T> vec3<T> transform(mat3<T> const& m, vec3<T> const& v);

	//Multiplies all vector components with the matrix.
	template<typename T> vec4<T> transform(mat4<T> const& m, vec4<T> const& v);

	//Equivalent to transform(mat4, vec4(v, 1))
	template<typename T> vec3<T> transform(mat4<T> const& m, vec3<T> const& v);
	template<typename T> vec3<T> transform(trans3d<T> const& m, vec3<T> const& v);

	//Equivalent to transform(mat3, vec3(v, 1))
	template<typename T> vec2<T> transform(mat3<T> const& m, vec2<T> const& v);
	template<typename T> vec2<T> transform(trans2d<T> const& m, vec2<T> const& v);

	template<typename T> aabb3<T> transform(mat4<T> const& m, aabb3<T> const& v);
	template<typename T> aabb3<T> transform(trans3d<T> const& m, aabb3<T> const& v);
	template<typename T> aabb2<T> transform(trans2d<T> const& m, aabb2<T> const& v);
	template<typename T> aabb2<T> transform(mat3<T> const& m, aabb2<T> const& v);

	//////////////////////////////////////////////////////////////////////////
	// Rotations
	//////////////////////////////////////////////////////////////////////////

	//Equivalent to transform(mat3, vec3(v, 0))
	//Transforms but without applying translation
	template<typename T> vec2<T> rotate(mat3<T> const& m, vec2<T> const& v);
	template<typename T> vec2<T> rotate(trans2d<T> const& m, vec2<T> const& v);

	//Equivalent to transform(mat4, vec4(v, 0))
	//Transforms but without applying translation
	template<typename T> vec3<T> rotate(mat4<T> const& m, vec3<T> const& v);
	template<typename T> vec3<T> rotate(quat<T> const& q, vec3<T> const& v);
	template<typename T> vec3<T> rotate(trans3d<T> const& m, vec3<T> const& v);
	template<typename T> vec2<T> rotate(trans2d<T> const& m, vec2<T> const& v);

	//////////////////////////////////////////////////////////////////////////
	// Project
	//////////////////////////////////////////////////////////////////////////

	//Transforms the vector and divides by the W
	//The W is returned undivided.
	template<typename T> vec4<T> project(mat4<T> const& m, vec4<T> const& v);

	//Equivalent to project(mat4, vec4(v, 1))
	template<typename T> vec3<T> project(mat4<T> const& m, vec3<T> const& v);

	//////////////////////////////////////////////////////////////////////////
	//
	namespace batch
	{
		template<typename T> void multiply(mat4<T> const& m, mat4<T>* dst, size_t dstStride, mat4<T> const* src, size_t srcStride, size_t count);
		template<typename T> void multiply(trans3d<T> const& m, trans3d<T>* dst, size_t dstStride, trans3d<T> const* src, size_t srcStride, size_t count);

		template<typename T> void transform(mat4<T> const& m, vec3<T>* dst, size_t dstStride, vec3<T> const* src, size_t srcStride, size_t count);
		template<typename T> void transform(trans3d<T> const& m, vec3<T>* dst, size_t dstStride, vec3<T> const* src, size_t srcStride, size_t count);
		template<typename T> void transform(mat4<T> const& m, vec4<T>* dst, size_t dstStride, vec4<T> const* src, size_t srcStride, size_t count);
	}

}//namespace math
