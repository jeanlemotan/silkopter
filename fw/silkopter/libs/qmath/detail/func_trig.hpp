namespace math
{


//////////////////////////////////////////////////////////////////////////
// scalars

MATH_FORCE_INLINE float degrees(float radians)
{
	return radians * (180.0f / pi);
}
MATH_FORCE_INLINE float radians(float degrees)
{
	return degrees * pi / 180.f;
}
template<typename T>
MATH_FORCE_INLINE T degrees(T const& radians)
{
	return radians * 180.0f / pi;
}
template<typename T>
MATH_FORCE_INLINE T radians(T const& degrees)
{
	return degrees * pi / 180.f;
}

//////////////////////////////////////////////////////////////////////////

MATH_FORCE_INLINE float cos(float s)
{
	return ::cosf(s);
}
MATH_FORCE_INLINE float sin(float s)
{
	return ::sinf(s);
}
MATH_FORCE_INLINE float tan(float s)
{
	return ::tanf(s);
}
MATH_FORCE_INLINE float acos(float s)
{
	return ::acosf(s);
}
MATH_FORCE_INLINE float asin(float s)
{
	return ::asinf(s);
}
MATH_FORCE_INLINE float atan(float s)
{
	return ::atanf(s);
}
MATH_FORCE_INLINE float atan2(float y, float x)
{
	return ::atan2f(y, x);
}
MATH_FORCE_INLINE void sin_cos(float angle, float& s, float& c)
{
	s = sin(angle);
	c = cos(angle);
}

//////////////////////////////////////////////////////////////////////////
// vec2
template<typename T>
MATH_FORCE_INLINE vec2<T> degrees(vec2<T> const& v)
{
	return vec2<T>(degrees(v.x), degrees(v.y));
}
template<typename T>
MATH_FORCE_INLINE vec2<T> radians(vec2<T> const& v)
{
	return vec2<T>(radians(v.x), radians(v.y));
}

template<typename T>
MATH_FORCE_INLINE vec2<T> cos(vec2<T> const& v)
{
	return vec2<T>(cos(v.x), cos(v.y));
}
template<typename T>
MATH_FORCE_INLINE vec2<T> sin(vec2<T> const& v)
{
	return vec2<T>(sin(v.x), sin(v.y));
}
template<typename T>
MATH_FORCE_INLINE vec2<T> tan(vec2<T> const& v)
{
	return vec2<T>(tan(v.x), tan(v.y));
}
template<typename T>
MATH_FORCE_INLINE vec2<T> acos(vec2<T> const& v)
{
	return vec2<T>(acos(v.x), acos(v.y));
}
template<typename T>
MATH_FORCE_INLINE vec2<T> asin(vec2<T> const& v)
{
	return vec2<T>(asin(v.x), asin(v.y));
}
template<typename T>
MATH_FORCE_INLINE vec2<T> atan(vec2<T> const& v)
{
	return vec2<T>(atan(v.x), atan(v.y));
}
template<typename T>
MATH_FORCE_INLINE vec2<T> atan2(vec2<T> const& y, vec2<T> const& x)
{
	return vec2<T>(atan2(y.x, x.x), atan2(y.y, x.y));
}
template<typename T>
MATH_FORCE_INLINE void sin_cos(vec2<T> const& angle, vec2<T>& s, vec2<T>& c)
{
	s = sin(angle);
	c = cos(angle);
}

//////////////////////////////////////////////////////////////////////////
// vec3
template<typename T>
MATH_FORCE_INLINE vec3<T> degrees(vec3<T> const& v)
{
	return vec3<T>(degrees(v.x), degrees(v.y), degrees(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec3<T> radians(vec3<T> const& v)
{
	return vec3<T>(radians(v.x), radians(v.y), radians(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec3<T> cos(vec3<T> const& v)
{
	return vec3<T>(cos(v.x), cos(v.y), cos(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec3<T> sin(vec3<T> const& v)
{
	return vec3<T>(sin(v.x), sin(v.y), sin(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec3<T> tan(vec3<T> const& v)
{
	return vec3<T>(tan(v.x), tan(v.y), tan(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec3<T> acos(vec3<T> const& v)
{
	return vec3<T>(acos(v.x), acos(v.y), acos(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec3<T> asin(vec3<T> const& v)
{
	return vec3<T>(asin(v.x), asin(v.y), asin(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec3<T> atan(vec3<T> const& v)
{
	return vec3<T>(atan(v.x), atan(v.y), atan(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec3<T> atan2(vec3<T> const& y, vec3<T> const& x)
{
	return vec3<T>(atan2(y.x, x.x), atan2(y.y, x.y), atan2(y.z, x.z));
}
template<typename T>
MATH_FORCE_INLINE void sin_cos(vec3<T> const& angle, vec3<T>& s, vec3<T>& c)
{
	s = sin(angle);
	c = cos(angle);
}

//////////////////////////////////////////////////////////////////////////
// vec4
template<typename T>
MATH_FORCE_INLINE vec4<T> degrees(vec4<T> const& v)
{
	return vec4<T>(degrees(v.x), degrees(v.y), degrees(v.z), degrees(v.w));
}
template<typename T>
MATH_FORCE_INLINE vec4<T> radians(vec4<T> const& v)
{
	return vec4<T>(radians(v.x), radians(v.y), radians(v.z), radians(v.w));
}
template<typename T>
MATH_FORCE_INLINE vec4<T> cos(vec4<T> const& v)
{
	return vec4<T>(cos(v.x), cos(v.y), cos(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec4<T> sin(vec4<T> const& v)
{
	return vec4<T>(sin(v.x), sin(v.y), sin(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec4<T> tan(vec4<T> const& v)
{
	return vec4<T>(tan(v.x), tan(v.y), tan(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec4<T> acos(vec4<T> const& v)
{
	return vec4<T>(acos(v.x), acos(v.y), acos(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec4<T> asin(vec4<T> const& v)
{
	return vec4<T>(asin(v.x), asin(v.y), asin(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec4<T> atan(vec4<T> const& v)
{
	return vec4<T>(atan(v.x), atan(v.y), atan(v.z));
}
template<typename T>
MATH_FORCE_INLINE vec4<T> atan2(vec4<T> const& y, vec4<T> const& x)
{
	return vec4<T>(atan2(y.x, x.x), atan2(y.y, x.y), atan2(y.z, x.z), atan2(y.w, x.w));
}
template<typename T>
MATH_FORCE_INLINE void sin_cos(vec4<T> const& angle, vec4<T>& s, vec4<T>& c)
{
	s = sin(angle);
	c = cos(angle);
}

}