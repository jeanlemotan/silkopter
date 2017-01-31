namespace math
{
template<typename T> vec4<T> const vec4<T>::zero(0);																		   \
template<typename T> vec4<T> const vec4<T>::one(1);																		   \

template<typename T> inline constexpr vec4<T>::vec4() : x(T(0)), y(T(0)), z(T(0)), w(T(0)) {}
template<typename T> inline constexpr vec4<T>::vec4(math::ZUninitialized) {}
template<typename T> inline constexpr vec4<T>::vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
template<typename T> inline constexpr vec4<T>::vec4(T v) : x(v), y(v), z(v), w(v) {}
template<typename T> inline constexpr vec4<T>::vec4(std::array<T, 4> const& v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}
template<typename T> inline constexpr vec4<T>::vec4(vec2<T> const& v) : x(v.x), y(v.y), z(0), w(0) {}
template<typename T> inline constexpr vec4<T>::vec4(vec2<T> const& v, T z, T w) : x(v.x), y(v.y), z(z), w(w) {}
template<typename T> inline constexpr vec4<T>::vec4(vec3<T> const& v) : x(v.x), y(v.y), z(v.z), w(0) {}
template<typename T> inline constexpr vec4<T>::vec4(vec3<T> const& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}

template<typename T> template<typename U> inline constexpr vec4<T>::vec4(vec4<U> const& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w)) {}
template<typename T> inline void vec4<T>::set(T _x, T _y, T _z, T _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}
template<typename T> template<typename U> inline void vec4<T>::set(vec4<U> const& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

template <typename T> inline void vec4<T>::set_length(T len)
{
    T oldLen = length(*this);
    MATH_ASSERT(oldLen != 0);
    *this *= (len/oldLen);
}

template <typename T> template<class Policy> inline void vec4<T>::normalize()
{
    *this = normalized<T, Policy>(*this);
}
template<typename T> inline bool vec4<T>::operator==(vec4<T> const& v) const
{
    return equals(*this, v);
}

template<typename T> inline bool vec4<T>::operator!=(vec4<T> const& v) const
{
    return !equals(*this, v);
}

template<typename T> inline vec4<T> vec4<T>::operator-() const
{
    return vec4<T>(-x, -y, -z, -w);
}

template<typename T> inline T& vec4<T>::operator[](uint8_t i)
{
    MATH_ASSERT(i < sizeof(*this) / sizeof(T));

    //if you hit this MATH_ASSERT, your compiler introduces padding. Check for #pragma packs without pop
    MATH_ASSERT(&((T*)this)[0] == &x && &((T*)this)[1] == &y);

    return ((T*)this)[i];
}

template<typename T> inline T const& vec4<T>::operator[](uint8_t i) const
{
    MATH_ASSERT(i < sizeof(*this) / sizeof(T));

    //if you hit this MATH_ASSERT, your compiler introduces padding. Check for #pragma packs without pop
    MATH_ASSERT(&((T*)this)[0] == &x && &((T*)this)[1] == &y);

    return ((T*)this)[i];
}

template<typename T> inline vec4<T> vec4<T>::operator+(T s) const
{
    return vec4<T>(x + s, y + s, z + s, w + s);
}
template<typename T> inline vec4<T> vec4<T>::operator+(vec4<T> const& v) const
{
    return vec4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
}

template<typename T> inline vec4<T> vec4<T>::operator-(T s) const
{
    return vec4<T>(x - s, y - s, z - s, w - s);
}
template<typename T> inline vec4<T> vec4<T>::operator-(vec4<T> const& v) const
{
    return vec4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
}

template<typename T> inline vec4<T> vec4<T>::operator*(T s) const
{
    return vec4<T>(x * s, y * s, z * s, w * s);
}

template<typename T> inline vec4<T> vec4<T>::operator/(T s) const
{
    MATH_ASSERT(s != T(0));
    return vec4<T>(x / s, y / s, z / s, w / s);
}

template<> inline vec4<float> vec4<float>::operator/(float s) const
{
    MATH_ASSERT(s != float(0));
    float ts = float(1) / s;
    return vec4<float>(x * ts, y * ts, z * ts, w * ts);
}

template<typename T> inline vec4<T>& vec4<T>::operator=(vec2<T> const& v)
{
    x = v.x;
    y = v.y;
    z = 0;
    w = 0;
    return *this;
}

template<typename T> inline vec4<T>& vec4<T>::operator=(vec3<T> const& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = 0;
    return *this;
}

template<typename T> inline vec4<T>& vec4<T>::operator+=(T s)
{
    x += s;
    y += s;
    z += s;
    w += s;
    return *this;
}
template<typename T> inline vec4<T>& vec4<T>::operator+=(vec4<T> const& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

template<typename T> inline vec4<T>& vec4<T>::operator-=(T s)
{
    x -= s;
    y -= s;
    z -= s;
    w -= s;
    return *this;
}
template<typename T> inline vec4<T>& vec4<T>::operator-=(vec4<T> const& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

template<typename T> inline vec4<T>& vec4<T>::operator*=(T s)
{
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
}
template<typename T> inline vec4<T>& vec4<T>::operator*=(vec4<T> const& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
}

template<typename T> inline vec4<T>& vec4<T>::operator/=(T s)
{
    MATH_ASSERT(s != T(0));
    x /= s;
    y /= s;
    z /= s;
    w /= s;
    return *this;
}

template<> inline vec4<float>& vec4<float>::operator/=(float s)
{
    MATH_ASSERT(s != float(0));
    float ts = float(1) / s;
    x *= ts;
    y *= ts;
    z *= ts;
    w *= ts;
    return *this;
}

template<typename T> inline vec4<T>& vec4<T>::operator/=(vec4<T> const& v)
{
    MATH_ASSERT(v.x != T(0) && v.y != T(0) && v.z != T(0) && v.w != T(0));
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
}

template<typename T> inline vec4<T> operator*(T s, vec4<T> const& v)
{
    return v*s;
}
template<typename T> inline vec4<T> operator/(T s, vec4<T> const& v)
{
    return vec4<T>(s) / v;
}



template<typename T> inline vec4<T> operator*(vec4<T> const& v0, vec4<T> const& v1)
{
    return vec4<T>(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z, v0.w * v1.w);
}

template<typename T> inline vec4<T> operator/(vec4<T> const& u, vec4<T> const& v)
{
    MATH_ASSERT(v.x != T(0) && v.y != T(0) && v.z != T(0) && v.w != T(0));
    return vec4<T>(u.x / v.x, u.y / v.y, u.z / v.z, u.w / v.w);
}


} //math


