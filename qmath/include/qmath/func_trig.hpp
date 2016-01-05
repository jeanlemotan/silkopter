namespace math
{


//////////////////////////////////////////////////////////////////////////
// scalars

template<typename T>
inline T radians(T const& degrees)
{
    QASSERT(!is_nan(degrees));
    return degrees * angle<T>::pi / T(180);
}

template<typename T>
inline T degrees(T const& a)
{
    QASSERT(!is_nan(a));
    return a * T(180) / angle<T>::pi;
}

//////////////////////////////////////////////////////////////////////////

namespace detail
{
	//one version of fast_sin - around 1100 cycles on atmega2560
	//max_error = 0.001091, avg_error = 0.000505
// 	inline float _fast_sin(float x)
// 	{
// 		float res = x * (1.27323954f - 0.405284735f * math::abs(x));
// 		return res * (.225f * (math::abs(res) - 1.f) + 1.f);
// 	}
// 
// 	inline float fast_sin(float x)
// 	{
// 		//always wrap input angle to -PI..PI
// 		if (is_negative(x) && x < -3.14159266f)
// 		{
// 			do { x += 6.28318531f; } while (x < -3.14159266f);
// 		}
// 		else if (x > 3.14159266f)
// 		{
// 			do { x -= 6.28318531f; } while (x > 3.14159266f);
// 		}
// 		return _fast_sin(x);
// 	}
// 	inline float fast_cos(float x)
// 	{
// 		return fast_sin(x + 1.5707963268f);
// 	}
// 	inline void fast_sin_cos(float x, float& s, float& c)
// 	{
// 		//always wrap input angle to -PI..PI
// 		if (is_negative(x) && x < -3.14159266f)
// 		{
// 			do { x += 6.28318531f; } while (x < -3.14159266f);
// 
// 			s = _fast_sin(x);
// 			c = _fast_sin(x + 1.5707963268f); //no need to check for overflow
// 		}
// 		else if (x > 3.14159266f)
// 		{
// 			do { x -= 6.28318531f; } while (x > 3.14159266f);
// 
// 			s = _fast_sin(x);
// 			c = _fast_sin(x + 1.5707963268f); //no need to check for overflow
// 		}
// 		else
// 		{
// 			s = _fast_sin(x);
// 			x += 1.5707963268f;
// 			if (is_positive(x) && x > 3.14159266f)
// 			{
// 				x -= 6.28318531f;
// 			}
// 			c = _fast_sin(x);
// 		}
// 	}

//////////////////////////////////////////////////////////////////////////
// lookup table version: around 600 cycles
	//max_error = 0.000538  avg_error = 0.000347
	
	//mantissa of (sin(x = 0 - pi/2) + 1) + 1
	//the last element is to allow interpolating without checking for out of bounds
	extern const uint16_t k_sin16[806];

	inline float fast_sin_pi2(float x) //x = 64 .. 64 + pi/2
	{
		//0	1234567 0 1234567 01234567 01234567
		//s eeeeeee e ------t tttttttt iiiiiiii

		uint8_t* __restrict x1_ptr = reinterpret_cast<uint8_t*>(&x);
		uint8_t it = *x1_ptr; //low 8 bits are for interpolating between table entries
		uint16_t fx = *reinterpret_cast<uint16_t*>(x1_ptr + 1)&0x7FFF; //high 9 bits are for table lookup
	
		//now fx should be between 0 and 804

		//compose the float from a precomputed exponent (1.0) by appending the decimals
		uint16_t t1 = k_sin16[fx];
		uint16_t t2 = k_sin16[fx + 1];
		//v = a + (b - a)*t;
		uint32_t v = (uint32_t(t1) << 7) + ((uint32_t(t2 - t1) * it) >> 1);

		uint32_t f = 0x3F800000 | v;
		float* __restrict ret = reinterpret_cast<float*>(&f); //1 .. 2
		return *ret;
	}
	inline float fast_sin(float x)
	{
		while (is_positive(x) && x >= anglef::_2pi) x -= anglef::_2pi;
		while (is_negative(x) && x < 0) x += anglef::_2pi;

		if (x >= anglef::pi)
		{
			if (x >= anglef::_3pi2)
			{
				//64 + anglef::_2pi
				return 1.f - fast_sin_pi2(64.f + anglef::_2pi - x);
			}
			else
			{
				//64 + x - anglef::pi
				return 1.f - fast_sin_pi2(64.f - anglef::pi + x);
			}
		}
		else
		{
			if (x >= anglef::pi2)
			{
				//64 + anglef::pi - x
				return fast_sin_pi2(64.f + anglef::pi - x) - 1.f;
			}
			else
			{
				return fast_sin_pi2(64 + x) - 1.f;
			}
		}
	}

	inline float fast_cos(float x)
	{
		return fast_sin(x + anglef::pi2);
	}
	inline void fast_sin_cos(float x, float& s, float& c)
	{
		while (is_positive(x) && x >= anglef::_2pi) x -= anglef::_2pi;
		while (is_negative(x) && x < 0) x += anglef::_2pi;
		
		if (x >= anglef::pi)
		{
			if (x >= anglef::_3pi2)
			{
				//64 + anglef::_2pi
				s = 1.f - fast_sin_pi2(64.f + anglef::_2pi - x);
				c = fast_sin_pi2(64.f - anglef::_3pi2 + x) - 1.f;
			}
			else
			{
				//64 + x - anglef::pi
				s = 1.f - fast_sin_pi2(64.f - anglef::pi + x);
				c = 1.f - fast_sin_pi2(64.f + anglef::_3pi2 - x);
			}
		}
		else
		{
			if (x >= anglef::pi2)
			{
				//64 + anglef::pi - x
				s = fast_sin_pi2(64.f + anglef::pi - x) - 1.f;
				c = 1.f - fast_sin_pi2(64.f - anglef::pi2 + x);
			}
			else
			{
				s = fast_sin_pi2(64.f + x) - 1.f;
				c = fast_sin_pi2(64.f + anglef::pi2 - x) - 1.f;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//standard

template<> inline float cos<float, standard>(float const& s)
{
    QASSERT(!is_nan(s));
    return ::cosf(s);
}
template<> inline float sin<float, standard>(float const& s)
{
    QASSERT(!is_nan(s));
    return ::sinf(s);
}
template<> inline float tan<float, standard>(float const& s)
{
    QASSERT(!is_nan(s));
    return ::tanf(s);
}
template<> inline void sin_cos<float, standard>(float const& angle, float& s, float& c)
{
    QASSERT(!is_nan(angle));
    s = ::sinf(angle);
	c = ::cosf(angle);
}

template<> inline double cos<double, standard>(double const& s)
{
    QASSERT(!is_nan(s));
    return ::cos(s);
}
template<> inline double sin<double, standard>(double const& s)
{
    QASSERT(!is_nan(s));
    return ::sin(s);
}
template<> inline double tan<double, standard>(double const& s)
{
    QASSERT(!is_nan(s));
    return ::tan(s);
}
template<> inline void sin_cos<double, standard>(double const& angle, double& s, double& c)
{
    QASSERT(!is_nan(angle));
    s = ::sin(angle);
	c = ::cos(angle);
}

//////////////////////////////////////////////////////////////////////////
//fast

template<> inline float cos<float, fast>(float const& s)
{
    QASSERT(!is_nan(s));
    return detail::fast_cos(s);
}
template<> inline float sin<float, fast>(float const& s)
{
    QASSERT(!is_nan(s));
    return detail::fast_sin(s);
}
template<> inline float tan<float, fast>(float const& s)
{
    QASSERT(!is_nan(s));
    return ::tanf(s);
}
template<> inline void sin_cos<float, fast>(float const& angle, float& s, float& c)
{
    QASSERT(!is_nan(angle));
    detail::fast_sin_cos(angle, s, c);
}

template<> inline double cos<double, fast>(double const& s)
{
    QASSERT(!is_nan(s));
    return ::cos(s);
}
template<> inline double sin<double, fast>(double const& s)
{
    QASSERT(!is_nan(s));
    return ::sin(s);
}
template<> inline double tan<double, fast>(double const& s)
{
    QASSERT(!is_nan(s));
    return ::tan(s);
}
template<> inline void sin_cos<double, fast>(double const& angle, double& s, double& c)
{
    QASSERT(!is_nan(angle));
    s = ::sin(angle);
	c = ::cos(angle);
}

//////////////////////////////////////////////////////////////////////////
//safe

template<> inline float cos<float, safe>(float const& s)
{
    QASSERT(!is_nan(s));
    return is_nan(s) ? 1 : ::cosf(s);
}
template<> inline float sin<float, safe>(float const& s)
{
    QASSERT(!is_nan(s));
    return is_nan(s) ? 0 : ::sinf(s);
}
template<> inline float tan<float, safe>(float const& s)
{
    QASSERT(!is_nan(s));
    return is_nan(s) ? 0 : ::tanf(s);
}
template<> inline void sin_cos<float, safe>(float const& angle, float& s, float& c)
{
    QASSERT(!is_nan(angle));
    if (is_nan(angle))
    {
        s = 0;
        c = 1;
    }
    else
    {
        s = ::sinf(angle);
        c = ::cosf(angle);
    }
}

template<> inline double cos<double, safe>(double const& s)
{
    QASSERT(!is_nan(s));
    return is_nan(s) ? 1 : ::cos(s);
}
template<> inline double sin<double, safe>(double const& s)
{
    QASSERT(!is_nan(s));
    return is_nan(s) ? 0 : ::sin(s);
}
template<> inline double tan<double, safe>(double const& s)
{
    QASSERT(!is_nan(s));
    return is_nan(s) ? 0 : ::tan(s);
}
template<> inline void sin_cos<double, safe>(double const& angle, double& s, double& c)
{
    QASSERT(!is_nan(angle));
    if (is_nan(angle))
    {
        s = 0;
        c = 1;
    }
    else
    {
        s = ::sin(angle);
        c = ::cos(angle);
    }
}

template<typename T, class Policy> inline void sin_cos(T const& angle, T& s, T& c)
{
	s = sin<T, Policy>(angle);
	c = cos<T, Policy>(angle);
}

//////////////////////////////////////////////////////////////////////////

template<typename T, class Policy>
inline T cos(T const& angle)
{
    QASSERT(!is_nan(angle));
    return std::cos(angle);
}
template<typename T, class Policy>
inline T sin(T const& angle)
{
    QASSERT(!is_nan(angle));
    return std::sin(angle);
}
template<typename T, class Policy>
inline T tan(T const& angle)
{
    QASSERT(!is_nan(angle));
    return std::tan(angle);
}

//////////////////////////////////////////////////////////////////////////

template<> inline float acos<float, standard>(float const& s)
{
    QASSERT(!is_nan(s));
    return float(::acosf(s));
}
template<> inline float asin<float, standard>(float const& s)
{
    QASSERT(!is_nan(s));
    return float(::asinf(s));
}
template<> inline float atan<float, standard>(float const& s)
{
    QASSERT(!is_nan(s));
    return float(::atanf(s));
}
template<> inline float atan2<float, standard>(float const& y, float const& x)
{
    QASSERT(!is_nan(x) && !is_nan(y));
    return float(::atan2f(y, x));
}

template<> inline double acos<double, standard>(double const& s)
{
    QASSERT(!is_nan(s));
    return double(::acos(s));
}
template<> inline double asin<double, standard>(double const& s)
{
    QASSERT(!is_nan(s));
    return double(::asin(s));
}
template<> inline double atan<double, standard>(double const& s)
{
    QASSERT(!is_nan(s));
    return double(::atan(s));
}
template<> inline double atan2<double, standard>(double const& y, double const& x)
{
    QASSERT(!is_nan(x) && !is_nan(y));
    return double(::atan2(y, x));
}

template<> inline float acos<float, fast>(float const& s)
{
    QASSERT(!is_nan(s));
    return float(::acosf(s));
}
template<> inline float asin<float, fast>(float const& s)
{
    QASSERT(!is_nan(s));
    return float(::asinf(s));
}
template<> inline float atan<float, fast>(float const& s)
{
    QASSERT(!is_nan(s));
    return float(::atanf(s));
}
template<> inline float atan2<float, fast>(float const& y, float const& x)
{
    QASSERT(!is_nan(x) && !is_nan(y));
    return float(::atan2f(y, x));
}

template<> inline double acos<double, fast>(double const& s)
{
    QASSERT(!is_nan(s));
    return double(::acos(s));
}
template<> inline double asin<double, fast>(double const& s)
{
    QASSERT(!is_nan(s));
    return double(::asin(s));
}
template<> inline double atan<double, fast>(double const& s)
{
    QASSERT(!is_nan(s));
    return double(::atan(s));
}
template<> inline double atan2<double, fast>(double const& y, double const& x)
{
    QASSERT(!is_nan(x) && !is_nan(y));
    return double(::atan2(y, x));
}

template<> inline float acos<float, safe>(float const& s)
{
    QASSERT(!is_nan(s));
    return is_nan(s) ? angle<float>::pi2 : float(::acosf(clamp(s, 0.f, 1.f)));
}
template<> inline float asin<float, safe>(float const& s)
{
    QASSERT(!is_nan(s));
    return is_nan(s) ? 0 : float(::asinf(s));
}
template<> inline float atan<float, safe>(float const& s)
{
    QASSERT(!is_nan(s));
    return is_nan(s) ? 0 : float(::atanf(s));
}
template<> inline float atan2<float, safe>(float const& y, float const& x)
{
    QASSERT(!is_nan(x) && !is_nan(y) && !is_zero(y));
    return is_nan(x) || is_nan(y) || is_zero(y) ? 0 : float(::atan2f(y, x));
}

template<> inline double acos<double, safe>(double const& s)
{
    QASSERT(!is_nan(s));
    return is_nan(s) ? angle<double>::pi2 : double(::acos(s));
}
template<> inline double asin<double, safe>(double const& s)
{
    QASSERT(!is_nan(s));
    return is_nan(s) ? 0 : double(::asin(s));
}
template<> inline double atan<double, safe>(double const& s)
{
    QASSERT(!is_nan(s));
    return is_nan(s) ? 0 : double(::atan(s));
}
template<> inline double atan2<double, safe>(double const& y, double const& x)
{
    QASSERT(!is_nan(x) && !is_nan(y) && !is_zero(y));
    return is_nan(x) || is_nan(y) || is_zero(y) ? 0 : double(::atan2(y, x));
}

//////////////////////////////////////////////////////////////////////////
// vec2
template<typename T>
inline vec2<T> degrees(vec2<T> const& v)
{
	return vec2<T>(degrees(v.x), degrees(v.y));
}
template<typename T>
inline vec2<T> radians(vec2<T> const& v)
{
    return vec2<T>(radians(v.x), radians(v.y));
}

template<typename T, class Policy = standard>
inline vec2<T> cos(vec2<T> const& v)
{
	return vec2<T>(cos<T, Policy>(v.x), cos<T, Policy>(v.y));
}
template<typename T, class Policy = standard>
inline vec2<T> sin(vec2<T> const& v)
{
	return vec2<T>(sin<T, Policy>(v.x), sin<T, Policy>(v.y));
}
template<typename T, class Policy = standard>
inline vec2<T> tan(vec2<T> const& v)
{
	return vec2<T>(tan<T, Policy>(v.x), tan<T, Policy>(v.y));
}
template<typename T, class Policy = standard>
inline void sin_cos(vec2<T> const& angle, vec2<T>& s, vec2<T>& c)
{
	s = sin<T, Policy>(angle);
	c = cos<T, Policy>(angle);
}

//////////////////////////////////////////////////////////////////////////
// vec3
template<typename T>
inline vec3<T> degrees(vec3<T> const& v)
{
	return vec3<T>(degrees(v.x), degrees(v.y), degrees(v.z));
}
template<typename T>
inline vec3<T> radians(vec3<T> const& v)
{
    return vec3<T>(radians(v.x), radians(v.y), radians(v.z));
}
template<typename T, class Policy = standard>
inline vec3<T> cos(vec3<T> const& v)
{
	return vec3<T>(cos<T, Policy>(v.x), cos<T, Policy>(v.y), cos<T, Policy>(v.z));
}
template<typename T, class Policy = standard>
inline vec3<T> sin(vec3<T> const& v)
{
	return vec3<T>(sin<T, Policy>(v.x), sin<T, Policy>(v.y), sin<T, Policy>(v.z));
}
template<typename T, class Policy = standard>
inline vec3<T> tan(vec3<T> const& v)
{
	return vec3<T>(tan<T, Policy>(v.x), tan<T, Policy>(v.y), tan<T, Policy>(v.z));
}
template<typename T, class Policy = standard>
inline void sin_cos(vec3<T> const& angle, vec3<T>& s, vec3<T>& c)
{
	s = sin<T, Policy>(angle);
	c = cos<T, Policy>(angle);
}

//////////////////////////////////////////////////////////////////////////
// vec4
template<typename T>
inline vec4<T> degrees(vec4<T> const& v)
{
	return vec4<T>(degrees(v.x), degrees(v.y), degrees(v.z), degrees(v.w));
}
template<typename T>
inline vec4<T> radians(vec4<T> const& v)
{
    return vec4<T>(radians(v.x), radians(v.y), radians(v.z), radians(v.w));
}
template<typename T, class Policy = standard>
inline vec4<T> cos(vec4<T> const& v)
{
	return vec4<T>(cos<T, Policy>(v.x), cos<T, Policy>(v.y), cos<T, Policy>(v.z), cos<T, Policy>(v.w));
}
template<typename T, class Policy = standard>
inline vec4<T> sin(vec4<T> const& v)
{
	return vec4<T>(sin<T, Policy>(v.x), sin<T, Policy>(v.y), sin<T, Policy>(v.z), sin<T, Policy>(v.w));
}
template<typename T, class Policy = standard>
inline vec4<T> tan(vec4<T> const& v)
{
	return vec4<T>(tan<T, Policy>(v.x), tan<T, Policy>(v.y), tan<T, Policy>(v.z), tan<T, Policy>(v.w));
}
template<typename T, class Policy = standard>
inline void sin_cos(vec4<T> const& angle, vec4<T>& s, vec4<T>& c)
{
	s = sin<T, Policy>(angle);
	c = cos<T, Policy>(angle);
}

}
