#pragma once

#include <math.h>

namespace math
{

	//////////////////////////////////////////////////////////////////////////

	template<typename T> inline T abs(T v)
	{
		return v < 0 ? -v : v;
	}
	template<> inline float abs(float v)
	{
		auto* __restrict v2 = reinterpret_cast<uint32_t*>(&v);
		uint32_t a = (*v2) & 0x7FFFFFFF;
		auto* __restrict a2 = reinterpret_cast<float*>(&a);
		return *a2;
	}
	template<> inline int64_t abs(int64_t a)
	{
		return a < 0 ? -a : a;
	}
	template<typename T> inline angle<T> abs(angle<T> const& v)
	{
		return angle<T>(abs(v.radians));
	}
	template<typename T> inline vec2<T> abs(vec2<T> const& v)
	{
		return vec2<T>(abs(v.x), abs(v.y));
	}
	template<typename T> inline vec2<T> sgn(vec2<T> const& v)
	{
		return vec2<T>(sgn(v.x), sgn(v.y));
	}
	template<typename T> inline vec3<T> abs(vec3<T> const& v)
	{
		return vec3<T>(abs(v.x), abs(v.y), abs(v.z));
	}
	template<typename T> inline vec3<T> sgn(vec3<T> const& v)
	{
		return vec3<T>(sgn(v.x), sgn(v.y), sgn(v.z));
	}
	template<typename T> inline vec4<T> abs(vec4<T> const& v)
	{
		return vec4<T>(abs(v.x), abs(v.y), abs(v.z), abs(v.w));
	}

	//////////////////////////////////////////////////////////////////////////

	template<typename T> inline T sgn(T const& v)
	{
		return v < T(0) ? T(-1) : v == T(0) ? T(0) : T(1);
	}
	template<typename T> inline T sgn(angle<T> const& v)
	{
		return sgn(v.radians);
	}
	template<typename T> inline vec4<T> sgn(vec4<T> const& v)
	{
		return vec4<T>(sgn(v.x), sgn(v.y), sgn(v.z), sgn(v.w));
	}


	template<typename T> inline T epsilon()
	{
		return std::numeric_limits<T>::epsilon();
	}

	//////////////////////////////////////////////////////////////////////////

	template <typename T> inline bool equals(T v1, T v2)
	{
		return v1 == v2;
	}
	template <typename T> inline bool equals(angle<T> const& v1, angle<T> const& v2)
	{
		return equals(v1.radians, v2.radians);
	}
	template <typename T> inline bool equals(vec2<T> const& v1, vec2<T> const& v2)
	{
		return cwise::all(cwise::equals(v1, v2));
	}
	template <typename T> inline bool equals(vec3<T> const& v1, vec3<T> const& v2)
	{
		return cwise::all(cwise::equals(v1, v2));
	}
	template <typename T> inline bool equals(vec4<T> const& v1, vec4<T> const& v2)
	{
		return cwise::all(cwise::equals(v1, v2));
	}
	template <typename T> inline bool equals(quat<T> const& v1, quat<T> const& v2)
	{
		return cwise::all(cwise::equals(v1, v2));
	}
	template <typename T> inline bool equals(mat2<T> const& v1, mat2<T> const& v2)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (!equals(v1.m[i], v2.m[i]))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool equals(mat3<T> const& v1, mat3<T> const& v2)
	{
		for (size_t i = 0; i < 9; i++)
		{
			if (!equals(v1.m[i], v2.m[i]))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool equals(mat4<T> const& v1, mat4<T> const& v2)
	{
		return cwise::all(cwise::equals(v1.get_column(0), v2.get_column(0))) &&
		cwise::all(cwise::equals(v1.get_column(1), v2.get_column(1))) &&
		cwise::all(cwise::equals(v1.get_column(2), v2.get_column(2))) &&
		cwise::all(cwise::equals(v1.get_column(3), v2.get_column(3)));
	}
	template <typename T> inline bool equals(trans3d<T> const& v1, trans3d<T> const& v2)
	{
		return equals(v1.mat, v2.mat);
	}
	template <typename T> inline bool equals(trans2d<T> const& v1, trans2d<T> const& v2)
	{
		return equals(v1.mat, v2.mat);
	}


	//////////////////////////////////////////////////////////////////////////
	template <typename T> inline bool equals(T v1, T v2, T tolerance)
	{
		assert(tolerance >= T(0));
		return abs(v1 - v2) <= tolerance;
	}

	inline bool equals(float v1, float v2, float tolerance)
	{
		assert(tolerance >= 0);
		// http://realtimecollisiondetection.net/pubs/Tolerances/
		// Abs(x - y) <= Max(absTol, relTol * Max(Abs(x), Abs(y)))
		// we assume absTol=relTol, this leaves
		// Abs(x - y) <= absTol * Max(1.0f, Abs(x), Abs(y))
		return abs(v1 - v2) <= tolerance * max(max(1.0f, abs(v1)), abs(v2));
	}
	template <typename T> inline bool equals(angle<T> const& v1, angle<T> const& v2, T tolerance)
	{
		return equals(v1.radians, v2.radians, tolerance);
	}
	template <typename T> inline bool equals(vec2<T> const& v1, vec2<T> const& v2, T tolerance)
	{
		return cwise::all(cwise::equals(v1, v2, tolerance));
	}
	template <typename T> inline bool equals(vec3<T> const& v1, vec3<T> const& v2, T tolerance)
	{
		return cwise::all(cwise::equals(v1, v2, tolerance));
	}
	template <typename T> inline bool equals(vec4<T> const& v1, vec4<T> const& v2, T tolerance)
	{
		return cwise::all(cwise::equals(v1, v2, tolerance));
	}
	template <typename T> inline bool equals(quat<T> const& v1, quat<T> const& v2, T tolerance)
	{
		return cwise::all(cwise::equals(v1, v2, tolerance));
	}
	template <typename T> inline bool equals(mat2<T> const& v1, mat2<T> const& v2, T tolerance)
	{
		T const* m1 = v1.data();
		T const* m2 = v2.data();
		for (uint8_t i = 0; i < mat2<T>::element_count; i++)
		{
			if (!equals(m1[i], m2[i], tolerance))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool equals(mat3<T> const& v1, mat3<T> const& v2, T tolerance)
	{
		T const* m1 = v1.data();
		T const* m2 = v2.data();
		for (uint8_t i = 0; i < mat3<T>::element_count; i++)
		{
			if (!equals(m1[i], m2[i], tolerance))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool equals(mat4<T> const& v1, mat4<T> const& v2, T tolerance)
	{
		return cwise::all(cwise::equals(v1.get_column(0), v2.get_column(0), tolerance)) &&
		cwise::all(cwise::equals(v1.get_column(1), v2.get_column(1), tolerance)) &&
		cwise::all(cwise::equals(v1.get_column(2), v2.get_column(2), tolerance)) &&
		cwise::all(cwise::equals(v1.get_column(3), v2.get_column(3), tolerance));
	}
	template <typename T> inline bool equals(trans3d<T> const& v1, trans3d<T> const& v2, T tolerance)
	{
		return equals(v1.mat, v2.mat, tolerance);
	}
	template <typename T> inline bool equals(trans2d<T> const& v1, trans2d<T> const& v2, T tolerance)
	{
		return equals(v1.mat, v2.mat, tolerance);
	}
	//////////////////////////////////////////////////////////////////////////
	// zero with tolerance

	template <typename T> inline bool is_zero(T v)
	{
		return v == T(0);
	}
	template <typename T> inline bool is_zero(angle<T> const& v)
	{
		return is_zero(v.radians);
	}
	template <typename T> inline bool is_zero(vec2<T> const& v)
	{
		return cwise::all(cwise::is_zero(v));
	}
	template <typename T> inline bool is_zero(vec3<T> const& v)
	{
		return cwise::all(cwise::is_zero(v));
	}
	template <typename T> inline bool is_zero(vec4<T> const& v)
	{
		return cwise::all(cwise::is_zero(v));
	}
	template <typename T> inline bool is_zero(quat<T> const& v)
	{
		return cwise::all(cwise::is_zero(v));
	}
	template <typename T> inline bool is_zero(mat2<T> const& v)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (!is_zero(v.m[i]))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool is_zero(mat3<T> const& v)
	{
		for (size_t i = 0; i < 9; i++)
		{
			if (!is_zero(v.m[i]))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool is_zero(mat4<T> const& v)
	{
		return cwise::all(cwise::is_zero(v.get_column(0))) &&
		cwise::all(cwise::is_zero(v.get_column(1))) &&
		cwise::all(cwise::is_zero(v.get_column(2))) &&
		cwise::all(cwise::is_zero(v.get_column(3)));
	}
	template <typename T> inline bool is_zero(trans3d<T> const& v)
	{
		return is_zero(v.mat);
	}
	template <typename T> inline bool is_zero(trans2d<T> const& v)
	{
		return is_zero(v.mat);
	}

	//////////////////////////////////////////////////////////////////////////

	template <typename T> inline bool is_zero(T v, T tolerance)
	{
		assert(tolerance >= 0);
		return abs(v) <= tolerance;
	}
	template <typename T> inline bool is_zero(angle<T> const& v, T tolerance)
	{
		return is_zero(v.radians, tolerance);
	}
	template <typename T> inline bool is_zero(vec2<T> const& v, T tolerance)
	{
		return cwise::all(cwise::is_zero(v, tolerance));
	}
	template <typename T> inline bool is_zero(vec3<T> const& v, T tolerance)
	{
		return cwise::all(cwise::is_zero(v, tolerance));
	}
	template <typename T> inline bool is_zero(vec4<T> const& v, T tolerance)
	{
		return cwise::all(cwise::is_zero(v, tolerance));
	}
	template <typename T> inline bool is_zero(quat<T> const& v, T tolerance)
	{
		return cwise::all(cwise::is_zero(v, tolerance));
	}
	template <typename T> inline bool is_zero(mat2<T> const& v, T tolerance)
	{
		T const* m = v.data();
		for (uint8_t i = 0; i < mat2<T>::element_count; i++)
		{
			if (!is_zero(m[i], tolerance))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool is_zero(mat3<T> const& v, T tolerance)
	{
		T const* m = v.data();
		for (uint8_t i = 0; i < mat3<T>::element_count; i++)
		{
			if (!is_zero(m[i], tolerance))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool is_zero(mat4<T> const& v, T tolerance)
	{
		return cwise::all(cwise::is_zero(v.get_column(0), tolerance)) &&
		cwise::all(cwise::is_zero(v.get_column(1), tolerance)) &&
		cwise::all(cwise::is_zero(v.get_column(2), tolerance)) &&
		cwise::all(cwise::is_zero(v.get_column(3), tolerance));
	}
	template <typename T> inline bool is_zero(trans3d<T> const& v, T tolerance)
	{
		return is_zero(v.mat, tolerance);
	}
	template <typename T> inline bool is_zero(trans2d<T> const& v, T tolerance)
	{
		return is_zero(v.mat, tolerance);
	}

	//////////////////////////////////////////////////////////////////////////
	// one with tolerance

	template <typename T> inline bool is_one(T v)
	{
		return v == T(1);
	}
	template <typename T> inline bool is_one(vec2<T> const& v)
	{
		return cwise::all(cwise::is_one(v));
	}
	template <typename T> inline bool is_one(vec3<T> const& v)
	{
		return cwise::all(cwise::is_one(v));
	}
	template <typename T> inline bool is_one(vec4<T> const& v)
	{
		return cwise::all(cwise::is_one(v));
	}
	template <typename T> inline bool is_one(quat<T> const& v)
	{
		return cwise::all(cwise::is_one(v));
	}
	template <typename T> inline bool is_one(mat2<T> const& v)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (!is_one(v.m[i]))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool is_one(mat3<T> const& v)
	{
		for (size_t i = 0; i < 9; i++)
		{
			if (!is_one(v.m[i]))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool is_one(mat4<T> const& v)
	{
		return cwise::all(cwise::is_one(v.get_column(0))) &&
		cwise::all(cwise::is_one(v.get_column(1))) &&
		cwise::all(cwise::is_one(v.get_column(2))) &&
		cwise::all(cwise::is_one(v.get_column(3)));
	}
	template <typename T> inline bool is_one(trans3d<T> const& v)
	{
		return is_one(v.mat);
	}
	template <typename T> inline bool is_one(trans2d<T> const& v)
	{
		return is_one(v.mat);
	}

	//////////////////////////////////////////////////////////////////////////

	template <typename T> inline bool is_one(T v, T tolerance)
	{
		assert(tolerance >= 0);
		return abs(v - T(1)) <= tolerance;
	}
	template <typename T> inline bool is_one(vec2<T> const& v, T tolerance)
	{
		return cwise::all(cwise::is_one(v, tolerance));
	}
	template <typename T> inline bool is_one(vec3<T> const& v, T tolerance)
	{
		return cwise::all(cwise::is_one(v, tolerance));
	}
	template <typename T> inline bool is_one(vec4<T> const& v, T tolerance)
	{
		return cwise::all(cwise::is_one(v, tolerance));
	}
	template <typename T> inline bool is_one(quat<T> const& v, T tolerance)
	{
		return cwise::all(cwise::is_one(v, tolerance));
	}
	template <typename T> inline bool is_one(mat2<T> const& v, T tolerance)
	{
		T const* m = v.data();
		for (uint8_t i = 0; i < mat2<T>::element_count; i++)
		{
			if (!is_one(m[i], tolerance))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool is_one(mat3<T> const& v, T tolerance)
	{
		T const* m = v.data();
		for (uint8_t i = 0; i < mat3<T>::element_count; i++)
		{
			if (!is_one(m[i], tolerance))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool is_one(mat4<T> const& v, T tolerance)
	{
		return cwise::all(cwise::is_one(v.get_column(0), tolerance)) &&
		cwise::all(cwise::is_one(v.get_column(1), tolerance)) &&
		cwise::all(cwise::is_one(v.get_column(2), tolerance)) &&
		cwise::all(cwise::is_one(v.get_column(3), tolerance));
	}
	template <typename T> inline bool is_one(trans3d<T> const& v, T tolerance)
	{
		return is_one(v.mat, tolerance);
	}
	template <typename T> inline bool is_one(trans2d<T> const& v, T tolerance)
	{
		return is_one(v.mat, tolerance);
	}

	//////////////////////////////////////////////////////////////////////////
	// positive

	template <typename T> inline bool is_positive(T v)
	{
		return v > T(0);
	}
	template <> inline bool is_positive(float v)
	{
		uint8_t const* __restrict lb = (reinterpret_cast<uint8_t const*>(&v) + 3); //msb in little endian
		return (*lb & 128) == 0;
	}
	template <typename T> inline bool is_positive(angle<T> const& v)
	{
		return is_positive(v.radians);
	}
	template <typename T> inline bool is_positive(vec2<T> const& v)
	{
		return cwise::all(cwise::is_positive(v));
	}
	template <typename T> inline bool is_positive(vec3<T> const& v)
	{
		return cwise::all(cwise::is_positive(v));
	}
	template <typename T> inline bool is_positive(vec4<T> const& v)
	{
		return cwise::all(cwise::is_positive(v));
	}
	template <typename T> inline bool is_positive(quat<T> const& v)
	{
		return cwise::all(cwise::is_positive(v));
	}
	template <typename T> inline bool is_positive(mat2<T> const& v)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (!is_positive(v.m[i]))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool is_positive(mat3<T> const& v)
	{
		for (size_t i = 0; i < 9; i++)
		{
			if (!is_positive(v.m[i]))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool is_positive(mat4<T> const& v)
	{
		return cwise::all(cwise::is_positive(v.get_column(0))) &&
		cwise::all(cwise::is_positive(v.get_column(1))) &&
		cwise::all(cwise::is_positive(v.get_column(2))) &&
		cwise::all(cwise::is_positive(v.get_column(3)));
	}

	//////////////////////////////////////////////////////////////////////////
	// negative

	template <typename T> inline bool is_negative(T v)
	{
		return v > T(0);
	}
	template <> inline bool is_negative(float v)
	{
		uint8_t const* __restrict lb = (reinterpret_cast<uint8_t const*>(&v) + 3); //msb in little endian
		return (*lb & 128) != 0;
	}
	template <typename T> inline bool is_negative(angle<T> const& v)
	{
		return is_negative(v.radians);
	}
	template <typename T> inline bool is_negative(vec2<T> const& v)
	{
		return cwise::all(cwise::is_negative(v));
	}
	template <typename T> inline bool is_negative(vec3<T> const& v)
	{
		return cwise::all(cwise::is_negative(v));
	}
	template <typename T> inline bool is_negative(vec4<T> const& v)
	{
		return cwise::all(cwise::is_negative(v));
	}
	template <typename T> inline bool is_negative(quat<T> const& v)
	{
		return cwise::all(cwise::is_negative(v));
	}
	template <typename T> inline bool is_negative(mat2<T> const& v)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (!is_negative(v.m[i]))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool is_negative(mat3<T> const& v)
	{
		for (size_t i = 0; i < 9; i++)
		{
			if (!is_negative(v.m[i]))
			{
				return false;
			}
		}
		return true;
	}
	template <typename T> inline bool is_negative(mat4<T> const& v)
	{
		return cwise::all(cwise::is_negative(v.get_column(0))) &&
		cwise::all(cwise::is_negative(v.get_column(1))) &&
		cwise::all(cwise::is_negative(v.get_column(2))) &&
		cwise::all(cwise::is_negative(v.get_column(3)));
	}
	

	//////////////////////////////////////////////////////////////////////////
	// identity with tolerance

	template <typename T> inline bool is_identity(mat2<T> const& v)
	{
		return equals(v, mat2<T>::identity);
	}
	template <typename T> inline bool is_identity(mat3<T> const& v)
	{
		return equals(v, mat3<T>::identity);
	}
	template <typename T> inline bool is_identity(mat4<T> const& v)
	{
		return equals(v, mat4<T>::identity);
	}
	template <typename T> inline bool is_identity(trans3d<T> const& v)
	{
		return equals(v, trans3d<T>::identity);
	}
	template <typename T> inline bool is_identity(trans2d<T> const& v)
	{
		return equals(v, trans2d<T>::identity);
	}
	template <typename T> inline bool is_identity(quat<T> const& v)
	{
		return equals(v, quat<T>::identity);
	}

	template <typename T> inline bool is_identity(mat2<T> const& v, T tolerance)
	{
		return equals(v, mat2<T>::identity, tolerance);
	}
	template <typename T> inline bool is_identity(mat3<T> const& v, T tolerance)
	{
		return equals(v, mat3<T>::identity, tolerance);
	}
	template <typename T> inline bool is_identity(mat4<T> const& v, T tolerance)
	{
		return equals(v, mat4<T>::identity, tolerance);
	}
	template <typename T> inline bool is_identity(trans3d<T> const& v, T tolerance)
	{
		return equals(v, trans3d<T>::identity, tolerance);
	}
	template <typename T> inline bool is_identity(trans2d<T> const& v, T tolerance)
	{
		return equals(v, trans2d<T>::identity, tolerance);
	}
	template <typename T> inline bool is_identity(quat<T> const& v, T tolerance)
	{
		return equals(v, quat<T>::identity, tolerance);
	}
	//////////////////////////////////////////////////////////////////////////
	// bool tests

	namespace cwise
	{
		template<typename T> inline vec2<bool> equals(vec2<T> const& v1, vec2<T> const& v2)
		{
			return vec2<bool>(v1.x == v2.x, v1.y == v2.y);
		}
		template<typename T> inline vec3<bool> equals(vec3<T> const& v1, vec3<T> const& v2)
		{
			return vec3<bool>(v1.x == v2.x, v1.y == v2.y, v1.z == v2.z);
		}
		template<typename T> inline vec4<bool> equals(vec4<T> const& v1, vec4<T> const& v2)
		{
			return vec4<bool>(v1.x == v2.x, v1.y == v2.y, v1.z == v2.z, v1.w == v2.w);
		}
		template<typename T> inline vec4<bool> equals(quat<T> const& v1, quat<T> const& v2)
		{
			return vec4<bool>(v1.x == v2.x, v1.y == v2.y, v1.z == v2.z, v1.w == v2.w);
		}

		template<typename T> inline vec2<bool> equals(vec2<T> const& v1, vec2<T> const& v2, T tolerance)
		{
			return vec2<bool>(math::equals(v1.x, v2.x, tolerance), math::equals(v1.y, v2.y, tolerance));
		}
		template<typename T> inline vec3<bool> equals(vec3<T> const& v1, vec3<T> const& v2, T tolerance)
		{
			return vec3<bool>(math::equals(v1.x, v2.x, tolerance), math::equals(v1.y, v2.y, tolerance), math::equals(v1.z, v2.z, tolerance));
		}
		template<typename T> inline vec4<bool> equals(vec4<T> const& v1, vec4<T> const& v2, T tolerance)
		{
			return vec4<bool>(math::equals(v1.x, v2.x, tolerance), math::equals(v1.y, v2.y, tolerance), math::equals(v1.z, v2.z, tolerance), math::equals(v1.w, v2.w, tolerance));
		}
		template<typename T> inline vec4<bool> equals(quat<T> const& v1, quat<T> const& v2, T tolerance)
		{
			return cwise::equals(reinterpret_cast<vec4<T> const&>(v1), reinterpret_cast<vec4<T> const&>(v2), tolerance);
		}


		// http://realtimecollisiondetection.net/pubs/Tolerances/
		// Abs(x - y) <= Max(absTol, relTol * Max(Abs(x), Abs(y)))
		// we assume absTol=relTol, this leaves
		// Abs(x - y) <= absTol * Max(1.0f, Abs(x), Abs(y))

		template<> inline vec2<bool> equals(vec2<float> const& v1, vec2<float> const& v2, float tolerance)
		{
			vec2<float> dif(vec2<float>::uninitialized);
			dif = abs(v1 - v2);
			vec2<float> e(vec2<float>::uninitialized);
			e = vec2<float>(tolerance) * max(max(vec2<float>(1.0f), abs(v1)), abs(v2));
			return vec2<bool>(dif.x <= e.x, dif.y <= e.y);
		}
		template<> inline vec3<bool> equals(vec3<float> const& v1, vec3<float> const& v2, float tolerance)
		{
			vec3<float> dif(vec3<float>::uninitialized);
			dif = abs(v1 - v2);
			vec3<float> e(vec3<float>::uninitialized);
			e = vec3<float>(tolerance) * max(max(vec3<float>(1.0f), abs(v1)), abs(v2));
			return vec3<bool>(dif.x <= e.x, dif.y <= e.y, dif.z <= e.z);
		}
		template<> inline vec4<bool> equals(vec4<float> const& v1, vec4<float> const& v2, float tolerance)
		{
			vec4<float> dif(vec4<float>::uninitialized);
			dif = abs(v1 - v2);
			vec4<float> e(vec4<float>::uninitialized);
			e = vec4<float>(tolerance) * max(max(vec4<float>(1.0f), abs(v1)), abs(v2));
			return vec4<bool>(dif.x <= e.x, dif.y <= e.y, dif.z <= e.z, dif.w <= e.w);
		}
		template<> inline vec4<bool> equals(quat<float> const& v1, quat<float> const& v2, float tolerance)
		{
			return cwise::equals(reinterpret_cast<vec4<float> const&>(v1), reinterpret_cast<vec4<float> const&>(v2), tolerance);
		}
		//////////////////////////////////////////////////////////////////////////

		template <typename T> inline vec2<bool> is_zero(vec2<T> const& v)
		{
			return vec2<bool>(v.x == 0, v.y == 0);
		}
		template <typename T> inline vec3<bool> is_zero(vec3<T> const& v)
		{
			return vec3<bool>(v.x == 0, v.y == 0, v.z == 0);
		}
		template <typename T> inline vec4<bool> is_zero(vec4<T> const& v)
		{
			return vec4<bool>(v.x == 0, v.y == 0, v.z == 0, v.w == 0);
		}
		template <typename T> inline vec4<bool> is_zero(quat<T> const& v)
		{
			return vec4<bool>(v.x == 0, v.y == 0, v.z == 0, v.w == 0);
		}

		template <typename T> inline vec2<bool> is_zero(vec2<T> const& v, T tolerance)
		{
			return cwise::equals(v, vec2<T>::zero, tolerance);
		}
		template <typename T> inline vec3<bool> is_zero(vec3<T> const& v, T tolerance)
		{
			return cwise::equals(v, vec3<T>::zero, tolerance);
		}
		template <typename T> inline vec4<bool> is_zero(vec4<T> const& v, T tolerance)
		{
			return cwise::equals(v, vec4<T>::zero, tolerance);
		}
		template <typename T> inline vec4<bool> is_zero(quat<T> const& v, T tolerance)
		{
			return cwise::equals(v, quat<T>::zero, tolerance);
		}

		//////////////////////////////////////////////////////////////////////////

		template <typename T> inline vec2<bool> is_one(vec2<T> const& v)
		{
            return vec2<bool>(v.x == 1, v.y == 1);
		}
		template <typename T> inline vec3<bool> is_one(vec3<T> const& v)
		{
            return vec3<bool>(v.x == 1, v.y == 1, v.z == 1);
		}
		template <typename T> inline vec4<bool> is_one(vec4<T> const& v)
		{
            return vec4<bool>(v.x == 1, v.y == 1, v.z == 1, v.w == 1);
		}
		template <typename T> inline vec4<bool> is_one(quat<T> const& v)
		{
            return vec4<bool>(v.x == 1, v.y == 1, v.z == 1, v.w == 1);
		}

		template <typename T> inline vec2<bool> is_one(vec2<T> const& v, T tolerance)
		{
			return cwise::equals(v, vec2<T>::one, tolerance);
		}
		template <typename T> inline vec3<bool> is_one(vec3<T> const& v, T tolerance)
		{
			return cwise::equals(v, vec3<T>::one, tolerance);
		}
		template <typename T> inline vec4<bool> is_one(vec4<T> const& v, T tolerance)
		{
			return cwise::equals(v, vec4<T>::one, tolerance);
		}
		template <typename T> inline vec4<bool> is_one(quat<T> const& v, T tolerance)
		{
			return cwise::equals(v, quat<T>::one, tolerance);
		}

		//////////////////////////////////////////////////////////////////////////

		template <typename T> inline vec2<bool> is_positive(vec2<T> const& v)
		{
			return vec2<bool>(is_positive(v.x), is_positive(v.y));
		}
		template <typename T> inline vec3<bool> is_positive(vec3<T> const& v)
		{
			return vec3<bool>(is_positive(v.x), is_positive(v.y), is_positive(v.z));
		}
		template <typename T> inline vec4<bool> is_positive(vec4<T> const& v)
		{
			return vec4<bool>(is_positive(v.x), is_positive(v.y), is_positive(v.z), is_positive(v.w));
		}
		template <typename T> inline vec4<bool> is_positive(quat<T> const& v)
		{
			return vec4<bool>(is_positive(v.x), is_positive(v.y), is_positive(v.z), is_positive(v.w));
		}

		//////////////////////////////////////////////////////////////////////////

		template <typename T> inline vec2<bool> is_negative(vec2<T> const& v)
		{
			return vec2<bool>(is_negative(v.x), is_negative(v.y));
		}
		template <typename T> inline vec3<bool> is_negative(vec3<T> const& v)
		{
			return vec3<bool>(is_negative(v.x), is_negative(v.y), is_negative(v.z));
		}
		template <typename T> inline vec4<bool> is_negative(vec4<T> const& v)
		{
			return vec4<bool>(is_negative(v.x), is_negative(v.y), is_negative(v.z), is_negative(v.w));
		}
		template <typename T> inline vec4<bool> is_negative(quat<T> const& v)
		{
			return vec4<bool>(is_negative(v.x), is_negative(v.y), is_negative(v.z), is_negative(v.w));
		}

		//////////////////////////////////////////////////////////////////////////


		inline bool all(vec2<bool> const& v)
		{
			return v.x && v.y;
		}
		inline bool all(vec3<bool> const& v)
		{
			return v.x && v.y && v.z;
		}
		inline bool all(vec4<bool> const& v)
		{
			return v.x && v.y && v.z && v.w;
		}
		inline bool any(vec2<bool> const& v)
		{
			return v.x || v.y;
		}
		inline bool any(vec3<bool> const& v)
		{
			return v.x || v.y || v.z;
		}
		inline bool any(vec4<bool> const& v)
		{
			return v.x || v.y || v.z || v.w;
		}

	}

	//////////////////////////////////////////////////////////////////////////

	template<typename T, class Policy> inline T sqrt(T const& v); //not defined
	
	template<> inline float sqrt<float, standard>(float const& v) { return ::sqrtf(v); }
	template<> inline float sqrt<float, safe>(float const& v) { return ::sqrtf(is_negative(v) ? 0.f : v); }
	template<> inline float sqrt<float, fast>(float const& v) 
	{ 
#ifdef __AVR__
		return ::sqrtf(v); 
#else
		float const x = v;
		float const xhalf = 0.5f*x;
		union // get bits for floating value
		{
			float x;
			int i;
		} u;
		u.x = x;
		u.i = 0x5f3759df - (u.i >> 1);  // gives initial guess y0
		return x*u.x*(1.5f - xhalf*u.x*u.x);// Newton step, repeating increases accuracy 	
#endif
	}

	template<> inline double sqrt<double, standard>(double const& v) { return ::sqrt(v); }
	template<> inline double sqrt<double, fast>(double const& v) { return ::sqrt(v); }
	template<> inline double sqrt<double, safe>(double const& v) { return ::sqrt(is_negative(v) ? 0 : v); }

	template<typename T, class Policy = standard> inline vec2<T> sqrt(vec2<T> const& v)
	{
		return vec2<T>(sqrt<T, Policy>(v.x), sqrt<T, Policy>(v.y));
	}
	template<typename T, class Policy = standard> inline vec3<T> sqrt(vec3<T> const& v)
	{
		return vec3<T>(sqrt<T, Policy>(v.x), sqrt<T, Policy>(v.y), sqrt<T, Policy>(v.z));
	}
	template<typename T, class Policy = standard> inline vec4<T> sqrt(vec4<T> const& v)
	{
		return vec4<T>(sqrt<T, Policy>(v.x), sqrt<T, Policy>(v.y), sqrt<T, Policy>(v.z), sqrt<T, Policy>(v.w));
	}
	//////////////////////////////////////////////////////////////////////////

	template<typename T, class Policy> inline T inverse(T const& v)
	{
		assert(v != 0);
		return 1 / v;
	}
	template<> inline float inverse<float, safe>(float const& v)
	{
		return is_zero(v) ? std::numeric_limits<float>::max() : (1.f / v);
	}
	template<> inline double inverse<double, safe>(double const& v)
	{
		return is_zero(v) ? std::numeric_limits<double>::max() : (1.f / v);
	}
	template<typename T, class Policy = standard> inline mat2<T> inverse(mat2<T> const& v)
	{
		mat2<T> tmp(v);
		bool res = tmp.template invert<Policy>();
		assert(res);
		return tmp;
	}
	template<typename T, class Policy = standard> inline mat3<T> inverse(mat3<T> const& v)
	{
		mat3<T> tmp(v);
		bool res = tmp.template invert<Policy>();
		assert(res);
		return tmp;
	}
	template<typename T, class Policy = standard> inline trans2d<T> inverse(const trans2d<T>& v)
	{
		trans2d<T> tmp(v);
		bool res = tmp.template invert<Policy>();
		assert(res);
		return tmp;
	}
	template<typename T, class Policy = standard> inline mat4<T> inverse(const mat4<T>& v)
	{
		mat4<T> tmp(v);
		bool res = tmp.template invert<Policy>();
		assert(res);
		return tmp;
	}
	template<typename T, class Policy = standard> inline trans3d<T> inverse(trans3d<T> const& v)
	{
		trans3d<T> tmp(v);
		bool res = tmp.template invert<Policy>();
		assert(res);
		return tmp;
	}
	template<typename T, class Policy = standard> inline quat<T> inverse(quat<T> const& v)
	{
		quat<T> tmp(v);
		tmp.template invert<Policy>();
		return tmp;
	}

	template<typename T, class Policy> inline T inv_sqrt(T const& v)
	{
		assert(!is_zero(v));
		return inverse<T, Policy>(sqrt<T, Policy>(v));
	}
	template<> inline float inv_sqrt<float, fast>(float const& v)
	{
		assert(!is_zero(v));
#ifdef __AVR__
        return inverse<float, fast>(sqrt<float, fast>(v));
#else
		float xhalf = 0.5f*v;
		int i = *(int*)&v;
		i = 0x5f3759df - (i >> 1);
        float* __restrict rp = reinterpret_cast<float*>(&i);
        float r = *rp;
		r = r*(1.5f - xhalf*r*r);
		return r;
#endif
	}
	template<> inline float inv_sqrt<float, safe>(float const& v)
	{
		return is_zero(v) ? std::numeric_limits<float>::max() : inverse<float, safe>(sqrt<float, safe>(v));
	}
	template<> inline double inv_sqrt<double, safe>(double const& v)
	{
		return is_zero(v) ? std::numeric_limits<double>::max() : inverse<double, safe>(sqrt<double, safe>(v));
	}
	template<typename T, class Policy = standard> inline vec2<T> inv_sqrt(vec2<T> const& v)
	{
		return vec2<T>(inv_sqrt<T, Policy>(v.x), inv_sqrt<T, Policy>(v.y));
	}
	template<typename T, class Policy = standard> inline vec3<T> inv_sqrt(vec3<T> const& v)
	{
		return vec3<T>(inv_sqrt<T, Policy>(v.x), inv_sqrt<T, Policy>(v.y), inv_sqrt<T, Policy>(v.z));
	}
	template<typename T, class Policy = standard> inline vec4<T> inv_sqrt(vec4<T> const& v)
	{
		return vec4<T>(inv_sqrt<T, Policy>(v.x), inv_sqrt<T, Policy>(v.y), inv_sqrt<T, Policy>(v.z), inv_sqrt<T, Policy>(v.w));
	}

	//////////////////////////////////////////////////////////////////////////

	template<typename T, class Policy> inline T pow(T const& a, T const& b);
	template<> inline float pow<float, standard>(float const& a, float const& b) { return ::powf(a, b); }
	template<> inline float pow<float, fast>(float const& a, float const& b) { return ::powf(a, b); }
	template<> inline float pow<float, safe>(float const& a, float const& b) { return ::powf(a, b); }

	template<> inline double pow<double, standard>(double const& a, double const& b) { return ::pow(a, b); }
	template<> inline double pow<double, fast>(double const& a, double const& b) { return ::pow(a, b); }
	template<> inline double pow<double, safe>(double const& a, double const& b) { return ::pow(a, b); }
		
	template<typename T, class Policy = standard> inline vec2<T> pow(vec2<T> const& a, vec2<T> const& b)
	{
		return vec2<T>(pow<T, Policy>(a.x, b.x), pow<T, Policy>(a.y, b.y));
	}
	template<typename T, class Policy = standard> inline vec3<T> pow(vec3<T> const& a, vec3<T> const& b)
	{
		return vec3<T>(pow<T, Policy>(a.x, b.x), pow<T, Policy>(a.y, b.y), pow<T, Policy>(a.z, b.z));
	}
	template<typename T, class Policy = standard> inline vec4<T> pow(vec4<T> const& a, vec4<T> const& b)
	{
		return vec4<T>(pow<T, Policy>(a.x, b.x), pow<T, Policy>(a.y, b.y), pow<T, Policy>(a.z, b.z), pow<T, Policy>(a.w, b.w));
	}

	//////////////////////////////////////////////////////////////////////////

	template<> inline float positive_zero(float const& v)
	{
		return v == -0.f ? 0.f : v;
	}
	template<typename T> inline T positive_zero(T const& v)
	{
		return v;
	}
	template<typename T> inline angle<T> positive_zero(angle<T> const& v)
	{
		return angle<T>(positive_zero(v.x));
	}
	template<typename T> inline vec2<T> positive_zero(vec2<T> const& v)
	{
		return vec2<T>(positive_zero(v.x), positive_zero(v.y));
	}
	template<typename T> inline vec3<T> positive_zero(vec3<T> const& v)
	{
		return vec3<T>(positive_zero(v.x), positive_zero(v.y), positive_zero(v.z));
	}
	template<typename T> inline vec4<T> positive_zero(vec4<T> const& v)
	{
		return vec4<T>(positive_zero(v.x), positive_zero(v.y), positive_zero(v.z), positive_zero(v.w));
	}
	template<typename T> inline quat<T> positive_zero(quat<T> const& v)
	{
		return quat<T>(positive_zero(v.x), positive_zero(v.y), positive_zero(v.z), positive_zero(v.w));
	}
	template<typename T> inline mat2<T> transposed(mat2<T> const& v)
	{
		mat2<T> tmp(v);
		tmp.transpose();
		return tmp;
	}
	template<typename T> inline mat3<T> transposed(mat3<T> const& v)
	{
		mat3<T> tmp(v);
		tmp.transpose();
		return tmp;
	}
	template<typename T> inline mat4<T> transposed(mat4<T> const& v)
	{
		mat4<T> tmp(v);
		tmp.transpose();
		return tmp;
	}
	template<typename T> inline T length_sq(vec2<T> const& v)
	{
		return dot(v, v);
	}
	template<typename T> inline T length_sq(vec3<T> const& v)
	{
		return dot(v, v);
	}
	template<typename T> inline T length_sq(vec4<T> const& v)
	{
		return dot(v, v);
	}
	template<typename T> inline T length_sq(quat<T> const& v)
	{
		return dot(v, v);
	}
    template<typename T, class Policy> inline T length(vec2<T> const& v)
	{
		return sqrt<T, Policy>(length_sq(v));
	}
    template<typename T, class Policy> inline T length(vec3<T> const& v)
	{
		return sqrt<T, Policy>(length_sq(v));
	}
    template<typename T, class Policy> inline T length(vec4<T> const& v)
	{
		return sqrt<T, Policy>(length_sq(v));
	}

    template<typename T, class Policy> inline T length(quat<T> const& v)
	{
		return sqrt<T, Policy>(length_sq(v));
	}
	template <typename T> inline T dot(vec2<T> const& v1, vec2<T> const& v2)
	{
		return v1.x*v2.x + v1.y*v2.y;
	}
	template <typename T> inline T dot(vec3<T> const& v1, vec3<T> const& v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	}
	template <typename T> inline T dot(vec4<T> const& v1, vec4<T> const& v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
	}

    template<typename T, class Policy> inline angle<T> normalized(angle<T> const& v)
	{
		angle<T> x(v);
		x.normalize();
		return x;
	}
	template<typename T, class Policy = standard> inline vec2<T> normalized(vec2<T> const& v)
	{
		T len_sq = length_sq(v);
		T len_inv = inv_sqrt<T, Policy>(len_sq);
		return v * len_inv;
	}
	template<typename T, class Policy = standard> inline vec3<T> normalized(vec3<T> const& v)
	{
		T len_sq = length_sq(v);
		T len_inv = inv_sqrt<T, Policy>(len_sq);
		return v * len_inv;
	}
	template<typename T, class Policy = standard> inline vec4<T> normalized(vec4<T> const& v)
	{
		T len_sq = length_sq(v);
		T len_inv = inv_sqrt<T, Policy>(len_sq);
		return v * len_inv;
	}
	template<typename T, class Policy = standard> inline quat<T> normalized(quat<T> const& v)
	{
		quat<T> tmp(v);
		tmp.template normalize<Policy>();
		return tmp;
	}

	template<typename T> inline T cross(vec2<T> const& v1, vec2<T> const& v2)
	{
		return (v1.x*v2.y - v1.y*v2.x);
	}
	template<typename T> inline vec3<T> cross(vec3<T> const& v1, vec3<T> const& v2)
	{
		return vec3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}
	template<typename T> vec4<T> cross(vec4<T> const& x1, vec4<T> const& x2, vec4<T> const& x3)
	{
		// 4d Cross product (GPG6, section 2.2)
#define DOT(AX,AY,AZ,BX,BY,BZ) AX*BX+AY*BY+AZ*BZ
#define CROSS(AX,AY,AZ,BX,BY,BZ,CX,CY,CZ) CX = AY*BZ - AZ*BY; CY = AZ*BX - AX*BZ; CZ = AX*BY - AY*BX;
		vec4<T> c1(vec4<T>::uninitialized);
		vec4<T> c2(vec4<T>::uninitialized);
		vec4<T> c3(vec4<T>::uninitialized);
		vec4<T> c4(vec4<T>::uninitialized);
		CROSS(x2.y,x2.z,x2.w,x3.y,x3.z,x3.w,c1.x,c1.y,c1.z);
		CROSS(x2.x,x2.z,x2.w,x3.x,x3.z,x3.w,c2.x,c2.y,c2.z);
		CROSS(x2.x,x2.y,x2.w,x3.x,x3.y,x3.w,c3.x,c3.y,c3.z);
		CROSS(x2.x,x2.y,x2.z,x3.x,x3.y,x3.z,c4.x,c4.y,c4.z);

		return vec4<T>(
			DOT(x1.y,x1.z,x1.w,c1.x,c1.y,c1.z),
			-DOT(x1.x,x1.z,x1.w,c2.x,c2.y,c2.z),
			DOT(x1.x,x1.y,x1.w,c3.x,c3.y,c3.z),
			-DOT(x1.x,x1.y,x1.z,c4.x,c4.y,c4.z)
			);

#undef DOT
#undef CROSS
	}
	template<typename T> inline T square(T const& v)
	{
		return v*v;
	}
	template<typename T, class Policy> inline T distance(T const& v1, T const& v2)
	{
		return abs(v2 - v1);
	}
	template<typename T, class Policy = standard> inline angle<T> distance(angle<T> const& v1, angle<T> const& v2)
	{
		auto start = v1.radians;
		auto end = v2.radians;
		auto difference = abs(end - start);
		if (difference > angle<T>::pi.radians)
		{
			// We need to add on to one of the values.
			if (end > start)
			{
				// We'll add it on to start...
				start += angle<T>::_2pi.radians;
			}
			else
			{
				// Add it on to end.
				end += angle<T>::_2pi.radians;
			}
		}
		return angle<T>(start - end);
	}
	template<typename T, class Policy = standard> inline T distance(vec2<T> const& v1, vec2<T> const& v2)
	{
		return sqrt<T, Policy>(distance_sq(v1, v2));
	}
	template<typename T, class Policy = standard> inline T distance(vec3<T> const& v1, vec3<T> const& v2)
	{
		return sqrt<T, Policy>(distance_sq(v1, v2));
	}
	template<typename T, class Policy = standard> inline T distance(vec4<T> const& v1, vec4<T> const& v2)
	{
		return sqrt<T, Policy>(distance_sq(v1, v2));
	}

	template<typename T, class Policy = standard> inline T distance(line3<T> const& l1, line3<T> const& l2)
	{
		return sqrt<T, Policy>(distance_sq(l1, l2));
	}
	template<typename T> inline T distance_sq(vec2<T> const& v1, vec2<T> const& v2)
	{
		auto v = v2 - v1;
		return dot(v, v);
	}
	template<typename T> inline T distance_sq(vec3<T> const& v1, vec3<T> const& v2)
	{
		auto v = v2 - v1;
		return dot(v, v);
	}
	template<typename T> inline T distance_sq(vec4<T> const& v1, vec4<T> const& v2)
	{
		auto v = v2 - v1;
		return dot(v, v);
	}
	template<typename T> T distance_sq(line3<T> const& l1, line3<T> const& l2)
	{
		vec3<T> u(l1.end - l1.start);
		vec3<T> v(l2.end - l2.start);
		vec3<T> w(l1.start - l2.start);
		T    a = dot(u, u);         // always >= 0
		T    b = dot(u, v);
		T    c = dot(v, v);         // always >= 0
		T    d = dot(u, w);
		T    e = dot(v, w);
		T    D = a*c - b*b;        // always >= 0
		T    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
		T    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

		// compute the line parameters of the two closest points
		if (D < std::numeric_limits<T>::epsilon()) 
		{ // the lines are almost parallel
			sN = T(0);         // force using point P0 on segment S1
			sD = T(1);         // to prevent possible division by 0.0 later
			tN = e;
			tD = c;
		}
		else 
		{                 // get the closest points on the infinite lines
			sN = (b*e - c*d);
			tN = (a*e - b*d);
			if (sN < T(0)) 
			{        // sc < 0 => the s=0 edge is visible
				sN = T(0);
				tN = e;
				tD = c;
			}
			else if (sN > sD) 
			{  // sc > 1  => the s=1 edge is visible
				sN = sD;
				tN = e + b;
				tD = c;
			}
		}

		if (tN < T(0)) 
		{            // tc < 0 => the t=0 edge is visible
			tN = T(0);
			// recompute sc for this edge
			if (-d < T(0))
			{
				sN = T(0);
			}
			else if (-d > a)
			{
				sN = sD;
			}
			else 
			{
				sN = -d;
				sD = a;
			}
		}
		else if (tN > tD) 
		{      // tc > 1  => the t=1 edge is visible
			tN = tD;
			// recompute sc for this edge
			if ((-d + b) < T(0))
			{
				sN = 0;

			}
			else if ((-d + b) > a)
			{
				sN = sD;
			}
			else 
			{
				sN = (-d +  b);
				sD = a;
			}
		}
		// finally do the division to get sc and tc
		sc = (abs(sN) < std::numeric_limits<T>::epsilon() ? T(0) : sN / sD);
		tc = (abs(tN) < std::numeric_limits<T>::epsilon() ? T(0) : tN / tD);

		// get the difference of the two closest points
		vec3<T> dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)
		return length_sq(dP);   // return the closest distance
	}

	namespace batch
	{
		template<typename T> void dot(T* dst, size_t dstStride, vec3<T> const* src1, size_t src1Stride, vec3<T> const* src2, size_t src2Stride, size_t count)
		{
			auto* __restrict d = dst;
			auto const* __restrict s1 = src1;
			auto const* __restrict s2 = src2;

			assert(src1 && src2 && dst && count);
			for (size_t i = 0; i < count; ++i)
			{
				*d = dot(*s1, *s2);
				s1 = (vec3<T> const* __restrict)(((char const* __restrict)s1) + src1Stride);
				s2 = (vec3<T> const* __restrict)(((char const* __restrict)s2) + src2Stride);
				d = (T* __restrict)(((char* __restrict)d) + dstStride);
			}
		}
		template<typename T> void dot(T* dst, size_t dstStride, vec4<T> const* src1, size_t src1Stride, vec4<T> const* src2, size_t src2Stride, size_t count)
		{
			auto* __restrict d = dst;
			auto const* __restrict s1 = src1;
			auto const* __restrict s2 = src2;

			assert(src1 && src2 && dst && count);
			for (size_t i = 0; i < count; ++i)
			{
				*d = dot(*s1, *s2);
				s1 = (vec4<T> const* __restrict)(((char const* __restrict)s1) + src1Stride);
				s2 = (vec4<T> const* __restrict)(((char const* __restrict)s2) + src2Stride);
				d = (T* __restrict)(((char* __restrict)d) + dstStride);
			}
		}
	}

}
