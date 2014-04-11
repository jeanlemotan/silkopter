#pragma once

namespace math
{

	//////////////////////////////////////////////////////////////////////////
	// scalars

	//These are here to avoid passing basic types as references. 
	//The templated function captures complex types (vec2, vec3)
	//These should be branchless as much as possible
	template <typename T> inline T min(T a, T b)
	{
		return a <= b ? a : b;
	}
	template <typename T> inline T max(T a, T b)
	{
		return a >= b ? a : b;
	}
	template<typename T> inline vec2<T> min(vec2<T> const& a, vec2<T> const& b)
	{
		return vec2<T>(min(a.x, b.x), min(a.y, b.y));
	}
	template<typename T> inline vec3<T> min(vec3<T> const& a, vec3<T> const& b)
	{
		return vec3<T>(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
	}
	template<typename T> inline vec4<T> min(vec4<T> const& a, vec4<T> const& b)
	{
		return vec4<T>(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w));
	}
	template<typename T> inline vec2<T> max(vec2<T> const& a, vec2<T> const& b)
	{
		return vec2<T>(max(a.x, b.x), max(a.y, b.y));
	}
	template<typename T> inline vec3<T> max(vec3<T> const& a, vec3<T> const& b)
	{
		return vec3<T>(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
	}
	template<typename T> inline vec4<T> max(vec4<T> const& a, vec4<T> const& b)
	{
		return vec4<T>(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w));
	}

	//////////////////////////////////////////////////////////////////////////

	template<typename T> inline T clamp(T x, T _min, T _max)
	{
		return min(max(x, _min), _max);
	}
	template<typename T> inline vec2<T> clamp(vec2<T> const& x, vec2<T> const& _min, vec2<T> const& _max)
	{
		return vec2<T>(clamp(x.x, _min.x, _max.x), clamp(x.y, _min.y, _max.y));
	}
	template<typename T> inline vec3<T> clamp(vec3<T> const& x, vec3<T> const& _min, vec3<T> const& _max)
	{
		return vec3<T>(clamp(x.x, _min.x, _max.x), clamp(x.y, _min.y, _max.y), clamp(x.z, _min.z, _max.z));
	}
	template<typename T> inline vec4<T> clamp(vec4<T> const& x, vec4<T> const& _min, vec4<T> const& _max)
	{
		return vec4<T>(clamp(x.x, _min.x, _max.x), clamp(x.y, _min.y, _max.y), clamp(x.z, _min.z, _max.z), clamp(x.w, _min.w, _max.w));
	}

	//////////////////////////////////////////////////////////////////////////

	namespace cwise
	{
		template<typename T> inline T min(vec2<T> const& a)
		{
			return math::min(a.x, a.y);
		}
		template<typename T> inline T max(vec2<T> const& a)
		{
			return math::max(a.x, a.y);
		}
		template<typename T> inline T min(vec3<T> const& a)
		{
			return math::min(math::min(a.x, a.y), a.z);
		}
		template<typename T> inline T max(vec3<T> const& a)
		{
			return math::max(math::max(a.x, a.y), a.z);
		}
		template<typename T> inline T min(vec4<T> const& a)
		{
			return math::min(math::min(math::min(a.x, a.y), a.z), a.w);
		}
		template<typename T> inline T max(vec4<T> const& a)
		{
			return math::max(math::max(math::max(a.x, a.y), a.z), a.w);
		}
		template<typename T> inline T min(quat<T> const& a)
		{
			return math::min(math::min(math::min(a.x, a.y), a.z), a.w);
		}
		template<typename T> inline T max(quat<T> const& a)
		{
			return math::max(math::max(math::max(a.x, a.y), a.z), a.w);
		}
		template<typename T> inline T min(mat2<T> const& a)
		{
			T x = math::min(a.m[0], a.m[1]);
			x = math::min(x, a.m[2]);
			x = math::min(x, a.m[3]);
			return x;
		}
		template<typename T> inline T max(mat2<T> const& a)
		{
			T x = math::max(a.m[0], a.m[1]);
			x = math::max(x, a.m[2]);
			x = math::max(x, a.m[3]);
			return x;
		}
		template<typename T> inline T min(mat3<T> const& a)
		{
			T x = math::min(a.m[0], a.m[1]); x = math::min(x, a.m[2]); 
			x = math::min(x, a.m[3]); x = math::min(x, a.m[4]); 
			x = math::min(x, a.m[5]); x = math::min(x, a.m[6]); 
			x = math::min(x, a.m[7]); x = math::min(x, a.m[8]);
			return x;
		}
		template<typename T> inline T max(mat3<T> const& a)
		{
			T x = math::max(a.m[0], a.m[1]); x = math::max(x, a.m[2]);
			x = math::max(x, a.m[3]); x = math::max(x, a.m[4]);
			x = math::max(x, a.m[5]); x = math::max(x, a.m[6]);
			x = math::max(x, a.m[7]); x = math::max(x, a.m[8]);
			return x;
		}
		template<typename T> inline T min(mat4<T> const& a)
		{
			T x = math::min(a.m[0], a.m[1]); x = math::min(x, a.m[2]); 
			x = math::min(x, a.m[3]); x = math::min(x, a.m[4]); 
			x = math::min(x, a.m[5]); x = math::min(x, a.m[6]); 
			x = math::min(x, a.m[7]); x = math::min(x, a.m[8]);
			x = math::min(x, a.m[9]); x = math::min(x, a.m[10]);
			x = math::min(x, a.m[11]); x = math::min(x, a.m[12]);
			x = math::min(x, a.m[13]); x = math::min(x, a.m[14]);
			x = math::min(x, a.m[15]);
			return x;
		}
		template<typename T> inline T max(mat4<T> const& a)
		{
			T x = math::max(a.m[0], a.m[1]); x = math::max(x, a.m[2]);
			x = math::max(x, a.m[3]); x = math::max(x, a.m[4]);
			x = math::max(x, a.m[5]); x = math::max(x, a.m[6]);
			x = math::max(x, a.m[7]); x = math::max(x, a.m[8]);
			x = math::max(x, a.m[9]); x = math::max(x, a.m[10]);
			x = math::max(x, a.m[11]); x = math::max(x, a.m[12]);
			x = math::max(x, a.m[13]); x = math::max(x, a.m[14]);
			x = math::max(x, a.m[15]);
			return x;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	template<typename T> inline T floor(T const& x);
	template<> inline float floor(float const& x)
	{
		return floorf(x);
	}

	template<typename T>
	inline vec2<T> floor(vec2<T> const& x)
	{
		return vec2<T>(floor(x.x), floor(x.y));
	}

	template<typename T>
	inline vec3<T> floor(vec3<T> const& x)
	{
		return vec3<T>(floor(x.x), floor(x.y), floor(x.z));
	}

	template<typename T>
	inline vec4<T> floor(vec4<T> const& x)
	{
		return vec4<T>(floor(x.x), floor(x.y), floor(x.z), floor(x.w));
	}


	template<typename T> inline T ceil(T const& x);
	template<> inline float ceil(float const& x)
	{
		return ceilf(x);
	}

	template<typename T>
	inline vec2<T> ceil(vec2<T> const& x)
	{
		return vec2<T>(ceil(x.x), ceil(x.y));
	}

	template<typename T>
	inline vec3<T> ceil(vec3<T> const& x)
	{
		return vec3<T>(ceil(x.x), ceil(x.y), ceil(x.z));
	}

	template<typename T>
	inline vec4<T> ceil(vec4<T> const& x)
	{
		return vec4<T>(ceil(x.x), ceil(x.y), ceil(x.z), ceil(x.w));
	}


	template<typename T>
	inline T fract(T const& x)
	{
		return x - floor(x);
	}

	template<typename T>
	inline vec2<T> fract(vec2<T> const& x)
	{
		return vec2<T>(fract(x.x), fract(x.y));
	}

	template<typename T>
	inline vec3<T> fract(vec3<T> const& x)
	{
		return vec3<T>(fract(x.x), fract(x.y), fract(x.z));
	}

	template<typename T>
	inline vec4<T> fract(vec4<T> const& x)
	{
		return vec4<T>(fract(x.x), fract(x.y), fract(x.z), fract(x.w));
	}

	//rounds to the closest integer
	inline float round(float x)
	{
		return floor(x + 0.5f);
	}

	template<typename T>
	inline vec2<T> round(vec2<T> const& x)
	{
		return vec2<T>(round(x.x), round(x.y));
	}

	template<typename T>
	inline vec3<T> round(vec3<T> const& x)
	{
		return vec3<T>(round(x.x), round(x.y), round(x.z));
	}

	template<typename T>
	inline vec4<T> round(vec4<T> const& x)
	{
		return vec4<T>(round(x.x), round(x.y), round(x.z), round(x.w));
	}

	//rounds to the closest integer
	template<typename T>
	inline T round(T const& x)
	{
		return x; //for non-float types, nothing
	}


	//rounds to the closest integer
	inline float round(float x, uint8_t decimals)
	{
		float ix = floor(x);
		float frac = x - ix;
		float p = ::powf(10.f, (float)decimals);
		float f = floor(round(frac * p));
		f /= p;
		return ix + f;
	}

	template<typename T>
	inline vec2<T> round(vec2<T> const& x, uint8_t decimals)
	{
		return vec2<T>(round(x.x, decimals), round(x.y, decimals));
	}

	template<typename T>
	inline vec3<T> round(vec3<T> const& x, uint8_t decimals)
	{
		return vec3<T>(round(x.x, decimals), round(x.y, decimals), round(x.z, decimals));
	}

	template<typename T>
	inline vec4<T> round(vec4<T> const& x, uint8_t decimals)
	{
		return vec4<T>(round(x.x, decimals), round(x.y, decimals), round(x.z, decimals), round(x.w, decimals));
	}

	//rounds to the closest integer
	template<typename T>
	inline T round(T const& x, uint8_t decimals)
	{
		return x; //for non-float types, nothing
	}

}