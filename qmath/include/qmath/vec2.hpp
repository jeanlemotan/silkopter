namespace math
{
template<typename T> vec2<T> const vec2<T>::zero(0);																		   \
template<typename T> vec2<T> const vec2<T>::one(1);																		   \

template<typename T> inline constexpr vec2<T>::vec2() : x(T(0)), y(T(0)) {}
template<typename T> inline constexpr vec2<T>::vec2(math::ZUninitialized) {}
template<typename T> inline constexpr vec2<T>::vec2(T _x, T _y) : x(_x), y(_y) {}
template<typename T> inline constexpr vec2<T>::vec2(T v) : x(v), y(v) {}
template<typename T> inline constexpr vec2<T>::vec2(std::array<T, 2> const& v) : x(v[0]), y(v[1]) {}

template<typename T>
template<typename U>
inline constexpr vec2<T>::vec2(vec2<U> const& v) : x(T(v.x)), y(T(v.y)) {}

template<typename T>
inline void vec2<T>::set(T _x, T _y)
{
    x = _x;
    y = _y;
}
template<typename T> template<typename U> inline void vec2<T>::set(vec2<U> const& v)
{
    x = v.x;
    y = v.y;
}

template <typename T>
inline void vec2<T>::set_length(T len)
{
    T oldLen = length(*this);
    MATH_ASSERT(oldLen != 0);
    *this *= (len/oldLen);
}

template <typename T>
template<class Policy>
inline void vec2<T>::normalize()
{
    *this = normalized<T, Policy>(*this);
}

template<typename T>
inline T& vec2<T>::operator[](uint8_t i) 
{
    MATH_ASSERT(i < sizeof(*this) / sizeof(T));

    //if you hit this MATH_ASSERT, your compiler introduces padding. Check for #pragma packs without pop
    MATH_ASSERT(&((T*)this)[0] == &x && &((T*)this)[1] == &y);

    return ((T*)this)[i];
}

template<typename T>
inline T const& vec2<T>::operator[](uint8_t i) const
{
    MATH_ASSERT(i < sizeof(*this) / sizeof(T));

    //if you hit this MATH_ASSERT, your compiler introduces padding. Check for #pragma packs without pop
    MATH_ASSERT(&((T*)this)[0] == &x && &((T*)this)[1] == &y);

    return ((T*)this)[i];
}

template<typename T>
inline bool vec2<T>::operator==(vec2<T> const& v) const
{
    return x == v.x && y == v.y;
}

template<typename T>
inline bool vec2<T>::operator!=(vec2<T> const& v) const
{
    return !operator==(v);
}

template<typename T>
inline vec2<T> vec2<T>::operator-() const
{
    return vec2<T>(-x, -y);
}

template<typename T>
inline vec2<T> vec2<T>::operator+(T s) const
{
    return vec2<T>(x + s, y + s);
}
template<typename T>
inline vec2<T> vec2<T>::operator+(vec2<T> const& v) const
{
    return vec2<T>(x + v.x, y + v.y);
}

template<typename T>
inline vec2<T> vec2<T>::operator-(T s) const
{
    return vec2<T>(x - s, y - s);
}
template<typename T>
inline vec2<T> vec2<T>::operator-(vec2<T> const& v2) const
{
    return vec2<T>(x - v2.x, y - v2.y);
}

template<typename T>
inline vec2<T> vec2<T>::operator*(T s) const
{
    return vec2<T>(x * s, y * s);
}

template<typename T>
inline vec2<T> vec2<T>::operator/(T s) const
{
    MATH_ASSERT(s != T(0));
    return vec2<T>(x / s, y / s);
}
template<>
inline vec2<float> vec2<float>::operator/(float s) const
{
    MATH_ASSERT(s != 0);
    float is = float(1) / s;
    return vec2<float>(x * is, y * is);
}

template<typename T>
inline vec2<T>& vec2<T>::operator+=(T s)
{
    x += s;
    y += s;
    return *this;
}
template<typename T>
inline vec2<T>& vec2<T>::operator+=(vec2<T> const& v)
{
    x += v.x;
    y += v.y;
    return *this;
}

template<typename T>
inline vec2<T>& vec2<T>::operator-=(T s)
{
    x -= s;
    y -= s;
    return *this;
}
template<typename T>
inline vec2<T>& vec2<T>::operator-=(vec2<T> const& v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

template<typename T>
inline vec2<T>& vec2<T>::operator*=(T s)
{
    x *= s;
    y *= s;
    return *this;
}
template<typename T>
inline vec2<T>& vec2<T>::operator*=(vec2<T> const& v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

template<typename T>
inline vec2<T>& vec2<T>::operator/=(T s)
{
    MATH_ASSERT(s != T(0));
    x /= s;
    y /= s;
    return *this;
}
template<>
inline vec2<float>& vec2<float>::operator/=(float s)
{
    MATH_ASSERT(s != float(0));
    float ts = float(1) / s;
    x *= ts;
    y *= ts;
    return *this;
}

template<typename T>
inline vec2<T>& vec2<T>::operator/=(vec2<T> const& v)
{
    MATH_ASSERT(v.x != T(0) && v.y != T(0));
    x /= v.x;
    y /= v.y;
    return *this;
}

template <typename T> 
inline vec2<T> operator*(T s, vec2<T> const& v)
{
    return v*s;
}
template <typename T> 
inline vec2<T> operator/(T s, vec2<T> const& v)
{
    return vec2<T>(s) / v;
}

template<typename T>
inline vec2<T> operator*(vec2<T> const& v0, vec2<T> const& v1)
{
    return vec2<T>(v0.x * v1.x, v0.y * v1.y);
}

template<typename T>
inline vec2<T> operator/(vec2<T> const& u, vec2<T> const& v)
{
    MATH_ASSERT(v.x != T(0) && v.y != T(0));
    return vec2<T>(u.x / v.x, u.y / v.y);
}


} //math


