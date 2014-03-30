namespace math
{

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

	inline bool equals(float v1, float v2, float tolerance)
	{
		assert(tolerance >= 0);
		// http://realtimecollisiondetection.net/pubs/Tolerances/
		// Abs(x - y) <= Max(absTol, relTol * Max(Abs(x), Abs(y)))
		// we assume absTol=relTol, this leaves
		// Abs(x - y) <= absTol * Max(1.0f, Abs(x), Abs(y))
		return abs(v1 - v2) <= tolerance * max(max(1.0f, abs(v1)), abs(v2));
	}
	inline bool equals(angle<float> const& v1, angle<float> const& v2, float tolerance)
	{
		return equals(v1.radians, v2.radians, tolerance);
	}
	inline bool equals(vec2<float> const& v1, vec2<float> const& v2, float tolerance)
	{
		return cwise::all(cwise::equals(v1, v2, tolerance));
	}

	
	inline bool equals(vec3<float> const& v1, vec3<float> const& v2, float tolerance)
	{
		return cwise::all(cwise::equals(v1, v2, tolerance));
	}
	inline bool equals(vec4<float> const& v1, vec4<float> const& v2, float tolerance)
	{
		return cwise::all(cwise::equals(v1, v2, tolerance));
	}

	inline bool equals(quat<float> const& v1, quat<float> const& v2, float tolerance)
	{
		return cwise::all(cwise::equals(v1, v2, tolerance));
	}
	inline bool equals(mat2<float> const& v1, mat2<float> const& v2, float tolerance)
	{
		float const* m1 = v1.data();
		float const* m2 = v2.data();
		for (uint8_t i = 0; i < mat2<float>::elementCount; i++)
		{
			if (!equals(m1[i], m2[i], tolerance))
			{
				return false;
			}
		}
		return true;
	}
	inline bool equals(mat3<float> const& v1, mat3<float> const& v2, float tolerance)
	{
		float const* m1 = v1.data();
		float const* m2 = v2.data();
		for (uint8_t i = 0; i < mat3<float>::elementCount; i++)
		{
			if (!equals(m1[i], m2[i], tolerance))
			{
				return false;
			}
		}
		return true;
	}
	inline bool equals(mat4<float> const& v1, mat4<float> const& v2, float tolerance)
	{
		return cwise::all(cwise::equals(v1.get_column(0), v2.get_column(0), tolerance)) &&
			cwise::all(cwise::equals(v1.get_column(1), v2.get_column(1), tolerance)) &&
			cwise::all(cwise::equals(v1.get_column(2), v2.get_column(2), tolerance)) &&
			cwise::all(cwise::equals(v1.get_column(3), v2.get_column(3), tolerance));
	}
	inline bool equals(trans3d<float> const& v1, trans3d<float> const& v2, float tolerance)
	{
		return equals(v1.mat, v2.mat, tolerance);
	}
	inline bool equals(trans2d<float> const& v1, trans2d<float> const& v2, float tolerance)
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

	inline bool is_zero(float v, float tolerance)
	{
		assert(tolerance >= 0);
		return abs(v) <= tolerance;
	}
	inline bool is_zero(angle<float> const& v, float tolerance)
	{
		return is_zero(v.radians, tolerance);
	}
	inline bool is_zero(vec2<float> const& v, float tolerance)
	{
		return cwise::all(cwise::is_zero(v, tolerance));
	}
	inline bool is_zero(vec3<float> const& v, float tolerance)
	{
		return cwise::all(cwise::is_zero(v, tolerance));
	}
	inline bool is_zero(vec4<float> const& v, float tolerance)
	{
		return cwise::all(cwise::is_zero(v, tolerance));
	}
	inline bool is_zero(quat<float> const& v, float tolerance)
	{
		return cwise::all(cwise::is_zero(v, tolerance));
	}
	inline bool is_zero(mat2<float> const& v, float tolerance)
	{
		float const* m = v.data();
		for (uint8_t i = 0; i < mat2<float>::elementCount; i++)
		{
			if (!is_zero(m[i], tolerance))
			{
				return false;
			}
		}
		return true;
	}
	inline bool is_zero(mat3<float> const& v, float tolerance)
	{
		float const* m = v.data();
		for (uint8_t i = 0; i < mat3<float>::elementCount; i++)
		{
			if (!is_zero(m[i], tolerance))
			{
				return false;
			}
		}
		return true;
	}
	inline bool is_zero(mat4<float> const& v, float tolerance)
	{
		return cwise::all(cwise::is_zero(v.get_column(0), tolerance)) &&
			cwise::all(cwise::is_zero(v.get_column(1), tolerance)) &&
			cwise::all(cwise::is_zero(v.get_column(2), tolerance)) &&
			cwise::all(cwise::is_zero(v.get_column(3), tolerance));
	}
	inline bool is_zero(trans3d<float> const& v, float tolerance)
	{
		return is_zero(v.mat, tolerance);
	}
	inline bool is_zero(trans2d<float> const& v, float tolerance)
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

	inline bool is_one(float v, float tolerance)
	{
		assert(tolerance >= 0);
		return abs(v - 1.f) <= tolerance;
	}
	inline bool is_one(vec2<float> const& v, float tolerance)
	{
		return cwise::all(cwise::is_one(v, tolerance));
	}
	inline bool is_one(vec3<float> const& v, float tolerance)
	{
		return cwise::all(cwise::is_one(v, tolerance));
	}
	inline bool is_one(vec4<float> const& v, float tolerance)
	{
		return cwise::all(cwise::is_one(v, tolerance));
	}
	inline bool is_one(quat<float> const& v, float tolerance)
	{
		return cwise::all(cwise::is_one(v, tolerance));
	}
	inline bool is_one(mat2<float> const& v, float tolerance)
	{
		float const* m = v.data();
		for (uint8_t i = 0; i < mat2<float>::elementCount; i++)
		{
			if (!is_one(m[i], tolerance))
			{
				return false;
			}
		}
		return true;
	}
	inline bool is_one(mat3<float> const& v, float tolerance)
	{
		float const* m = v.data();
		for (uint8_t i = 0; i < mat3<float>::elementCount; i++)
		{
			if (!is_one(m[i], tolerance))
			{
				return false;
			}
		}
		return true;
	}
	inline bool is_one(mat4<float> const& v, float tolerance)
	{
		return cwise::all(cwise::is_one(v.get_column(0), tolerance)) &&
			cwise::all(cwise::is_one(v.get_column(1), tolerance)) &&
			cwise::all(cwise::is_one(v.get_column(2), tolerance)) &&
			cwise::all(cwise::is_one(v.get_column(3), tolerance));
	}
	inline bool is_one(trans3d<float> const& v, float tolerance)
	{
		return is_one(v.mat, tolerance);
	}
	inline bool is_one(trans2d<float> const& v, float tolerance)
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

	inline bool is_identity(mat2<float> const& v, float tolerance)
	{
		return equals(v, mat2<float>::identity, tolerance);
	}
	inline bool is_identity(mat3<float> const& v, float tolerance)
	{
		return equals(v, mat3<float>::identity, tolerance);
	}
	inline bool is_identity(mat4<float> const& v, float tolerance)
	{
		return equals(v, mat4<float>::identity, tolerance);
	}
	inline bool is_identity(trans3d<float> const& v, float tolerance)
	{
		return equals(v, trans3d<float>::identity, tolerance);
	}
	inline bool is_identity(trans2d<float> const& v, float tolerance)
	{
		return equals(v, trans2d<float>::identity, tolerance);
	}
	inline bool is_identity(quat<float> const& v, float tolerance)
	{
		return equals(v, quat<float>::identity, tolerance);
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

		// http://realtimecollisiondetection.net/pubs/Tolerances/
		// Abs(x - y) <= Max(absTol, relTol * Max(Abs(x), Abs(y)))
		// we assume absTol=relTol, this leaves
		// Abs(x - y) <= absTol * Max(1.0f, Abs(x), Abs(y))

		inline vec2<bool> equals(vec2<float> const& v1, vec2<float> const& v2, float tolerance)
		{
			vec2<float> dif(vec2<float>::uninitialized);
			dif = abs(v1 - v2);
			vec2<float> e(vec2<float>::uninitialized);
			e = vec2<float>(tolerance) * max(max(vec2<float>(1.0f), abs(v1)), abs(v2));
			return vec2<bool>(dif.x <= e.x, dif.y <= e.y);
		}
		inline vec3<bool> equals(vec3<float> const& v1, vec3<float> const& v2, float tolerance)
		{
			vec3<float> dif(vec3<float>::uninitialized);
			dif = abs(v1 - v2);
			vec3<float> e(vec3<float>::uninitialized);
			e = vec3<float>(tolerance) * max(max(vec3<float>(1.0f), abs(v1)), abs(v2));
			return vec3<bool>(dif.x <= e.x, dif.y <= e.y, dif.z <= e.z);
		}
		inline vec4<bool> equals(vec4<float> const& v1, vec4<float> const& v2, float tolerance)
		{
			vec4<float> dif(vec4<float>::uninitialized);
			dif = abs(v1 - v2);
			vec4<float> e(vec4<float>::uninitialized);
			e = vec4<float>(tolerance) * max(max(vec4<float>(1.0f), abs(v1)), abs(v2));
			return vec4<bool>(dif.x <= e.x, dif.y <= e.y, dif.z <= e.z, dif.w <= e.w);
		}
		inline vec4<bool> equals(quat<float> const& v1, quat<float> const& v2, float tolerance)
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

		inline vec2<bool> is_zero(vec2<float> const& v, float tolerance)
		{
			return cwise::equals(v, vec2<float>::zero, tolerance);
		}
		inline vec3<bool> is_zero(vec3<float> const& v, float tolerance)
		{
			return cwise::equals(v, vec3<float>::zero, tolerance);
		}
		inline vec4<bool> is_zero(vec4<float> const& v, float tolerance)
		{
			return cwise::equals(v, vec4<float>::zero, tolerance);
		}
		inline vec4<bool> is_zero(quat<float> const& v, float tolerance)
		{
			return cwise::equals(v, quat<float>::zero, tolerance);
		}

		//////////////////////////////////////////////////////////////////////////

		template <typename T> inline vec2<bool> is_one(vec2<T> const& v)
		{
			return vec2<bool>(v.x == 0, v.y == 0);
		}
		template <typename T> inline vec3<bool> is_one(vec3<T> const& v)
		{
			return vec3<bool>(v.x == 0, v.y == 0, v.z == 0);
		}
		template <typename T> inline vec4<bool> is_one(vec4<T> const& v)
		{
			return vec4<bool>(v.x == 0, v.y == 0, v.z == 0, v.w == 0);
		}
		template <typename T> inline vec4<bool> is_one(quat<T> const& v)
		{
			return vec4<bool>(v.x == 0, v.y == 0, v.z == 0, v.w == 0);
		}

		inline vec2<bool> is_one(vec2<float> const& v, float tolerance)
		{
			return cwise::equals(v, vec2<float>::one, tolerance);
		}
		inline vec3<bool> is_one(vec3<float> const& v, float tolerance)
		{
			return cwise::equals(v, vec3<float>::one, tolerance);
		}
		inline vec4<bool> is_one(vec4<float> const& v, float tolerance)
		{
			return cwise::equals(v, vec4<float>::one, tolerance);
		}
		inline vec4<bool> is_one(quat<float> const& v, float tolerance)
		{
			return cwise::equals(v, quat<float>::one, tolerance);
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

}