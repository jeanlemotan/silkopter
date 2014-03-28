namespace math
{


//////////////////////////////////////////////////////////////////////////
// scalars

template<>
inline float degrees(float const& radians)
{
	return radians * (180.0f / anglef::pi.radians);
}
template<>
inline float radians(float const& degrees)
{
	return degrees * anglef::pi.radians / 180.f;
}
template<typename T>
inline T degrees(T const& radians)
{
	return radians * T(180) / angle<T>::pi.radians;
}
template<typename T>
inline T radians(T const& degrees)
{
	return degrees * angle<T>::pi.radians / T(180);
}

//////////////////////////////////////////////////////////////////////////

template<> inline float cos<float, standard>(float const& s) { return ::cosf(s); }
template<> inline float sin<float, standard>(float const& s) { return ::sinf(s); }
template<> inline float tan<float, standard>(float const& s) { return ::tanf(s); }
template<> inline float acos<float, standard>(float const& s) { return ::acosf(s); }
template<> inline float asin<float, standard>(float const& s) { return ::asinf(s); }
template<> inline float atan<float, standard>(float const& s) { return ::atanf(s); }
template<> inline float atan2<float, standard>(float const& y, float const& x) { return ::atan2f(y, x); }

template<> inline float cos<float, fast>(float const& s) { return ::cosf(s); }
template<> inline float sin<float, fast>(float const& s) { return ::sinf(s); }
template<> inline float tan<float, fast>(float const& s) { return ::tanf(s); }
template<> inline float acos<float, fast>(float const& s) { return ::acosf(s); }
template<> inline float asin<float, fast>(float const& s) { return ::asinf(s); }
template<> inline float atan<float, fast>(float const& s) { return ::atanf(s); }
template<> inline float atan2<float, fast>(float const& y, float const& x) { return ::atan2f(y, x); }

template<> inline float cos<float, safe>(float const& s) { return ::cosf(s); }
template<> inline float sin<float, safe>(float const& s) { return ::sinf(s); }
template<> inline float tan<float, safe>(float const& s) { return ::tanf(s); }
template<> inline float acos<float, safe>(float const& s) { return ::acosf(s); }
template<> inline float asin<float, safe>(float const& s) { return ::asinf(s); }
template<> inline float atan<float, safe>(float const& s) { return ::atanf(s); }
template<> inline float atan2<float, safe>(float const& y, float const& x) { return ::atan2f(y, x); }

template<> inline float cos<float, fast_safe>(float const& s) { return ::cosf(s); }
template<> inline float sin<float, fast_safe>(float const& s) { return ::sinf(s); }
template<> inline float tan<float, fast_safe>(float const& s) { return ::tanf(s); }
template<> inline float acos<float, fast_safe>(float const& s) { return ::acosf(s); }
template<> inline float asin<float, fast_safe>(float const& s) { return ::asinf(s); }
template<> inline float atan<float, fast_safe>(float const& s) { return ::atanf(s); }
template<> inline float atan2<float, fast_safe>(float const& y, float const& x) { return ::atan2f(y, x); }
	
	
template<typename T, class Policy = standard> inline void sin_cos(float angle, float& s, float& c)
{
	s = sin<T, Policy>(angle);
	c = cos<T, Policy>(angle);
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
inline vec2<T> acos(vec2<T> const& v)
{
	return vec2<T>(acos<T, Policy>(v.x), acos<T, Policy>(v.y));
}
template<typename T, class Policy = standard>
inline vec2<T> asin(vec2<T> const& v)
{
	return vec2<T>(asin<T, Policy>(v.x), asin<T, Policy>(v.y));
}
template<typename T, class Policy = standard>
inline vec2<T> atan(vec2<T> const& v)
{
	return vec2<T>(atan<T, Policy>(v.x), atan<T, Policy>(v.y));
}
template<typename T, class Policy = standard>
inline vec2<T> atan2(vec2<T> const& y, vec2<T> const& x)
{
	return vec2<T>(atan2<T, Policy>(y.x, x.x), atan2<T, Policy>(y.y, x.y));
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
inline vec3<T> acos(vec3<T> const& v)
{
	return vec3<T>(acos<T, Policy>(v.x), acos<T, Policy>(v.y), acos<T, Policy>(v.z));
}
template<typename T, class Policy = standard>
inline vec3<T> asin(vec3<T> const& v)
{
	return vec3<T>(asin<T, Policy>(v.x), asin<T, Policy>(v.y), asin<T, Policy>(v.z));
}
template<typename T, class Policy = standard>
inline vec3<T> atan(vec3<T> const& v)
{
	return vec3<T>(atan<T, Policy>(v.x), atan<T, Policy>(v.y), atan<T, Policy>(v.z));
}
template<typename T, class Policy = standard>
inline vec3<T> atan2(vec3<T> const& y, vec3<T> const& x)
{
	return vec3<T>(atan2<T, Policy>(y.x, x.x), atan2<T, Policy>(y.y, x.y), atan2<T, Policy>(y.z, x.z));
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
inline vec4<T> acos(vec4<T> const& v)
{
	return vec4<T>(acos<T, Policy>(v.x), acos<T, Policy>(v.y), acos<T, Policy>(v.z), acos<T, Policy>(v.w));
}
template<typename T, class Policy = standard>
inline vec4<T> asin(vec4<T> const& v)
{
	return vec4<T>(asin<T, Policy>(v.x), asin<T, Policy>(v.y), asin<T, Policy>(v.z), asin<T, Policy>(v.w));
}
template<typename T, class Policy = standard>
inline vec4<T> atan(vec4<T> const& v)
{
	return vec4<T>(atan<T, Policy>(v.x), atan<T, Policy>(v.y), atan<T, Policy>(v.z), atan<T, Policy>(v.w));
}
template<typename T, class Policy = standard>
inline vec4<T> atan2(vec4<T> const& y, vec4<T> const& x)
{
	return vec4<T>(atan2<T, Policy>(y.x, x.x), atan2<T, Policy>(y.y, x.y), atan2<T, Policy>(y.z, x.z), atan2<T, Policy>(y.w, x.w));
}
template<typename T, class Policy = standard>
inline void sin_cos(vec4<T> const& angle, vec4<T>& s, vec4<T>& c)
{
	s = sin<T, Policy>(angle);
	c = cos<T, Policy>(angle);
}

}