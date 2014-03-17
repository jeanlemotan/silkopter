namespace math
{


template<typename T>
vec2<T> const vec2<T>::zero(0);

template<typename T>
vec2<T> const vec2<T>::one(1);

template<typename T>
MATH_FORCE_INLINE vec2<T>::vec2() : x(T(0)), y(T(0)) {}

template<typename T>
MATH_FORCE_INLINE vec2<T>::vec2(ZUninitialized) {}

template<typename T>
MATH_FORCE_INLINE vec2<T>::vec2(T _x, T _y) : x(_x), y(_y) {}

template<typename T>
MATH_FORCE_INLINE vec2<T>::vec2(T v) : x(v), y(v) {}

template<typename T>
MATH_FORCE_INLINE vec2<T>::vec2(vec2<T> const& v) : x(v.x), y(v.y) {}

template<typename T>
MATH_FORCE_INLINE vec2<T>::vec2(packet2<T> const& v) : x(v.x), y(v.y) {}

template<typename T>
template<typename U>
MATH_FORCE_INLINE vec2<T>::vec2(vec2<U> const& v) : x(T(v.x)), y(T(v.y)) {}

template<typename T>
MATH_FORCE_INLINE vec2<T>& vec2<T>::set(T _x, T _y)
{
	x = _x;
	y = _y;
	return *this;
}
template<typename T> template<typename U> MATH_FORCE_INLINE vec2<T>& vec2<T>::set(vec2<U> const& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

template <typename T>
MATH_FORCE_INLINE void vec2<T>::set_length(T len)
{
	T oldLen = length(*this);
	assert(oldLen != 0);
	*this *= (len/oldLen);
}

template <typename T>
MATH_FORCE_INLINE vec2<T>& vec2<T>::normalize()
{
	*this = normalized(*this);
	return *this;
}

template<typename T>
MATH_FORCE_INLINE T& vec2<T>::operator[](uint8_t i) 
{
	assert(i < sizeof(*this) / sizeof(T));

	//if you hit this assert, your compiler introduces padding. Check for #pragma packs without pop
	assert(&((T*)this)[0] == &x && &((T*)this)[1] == &y); 

	return ((T*)this)[i];
}

template<typename T>
MATH_FORCE_INLINE T const& vec2<T>::operator[](uint8_t i) const
{
	assert(i < sizeof(*this) / sizeof(T));

	//if you hit this assert, your compiler introduces padding. Check for #pragma packs without pop
	assert(&((T*)this)[0] == &x && &((T*)this)[1] == &y); 

	return ((T*)this)[i];
}

template<typename T>
MATH_FORCE_INLINE bool vec2<T>::operator==(vec2<T> const& v) const
{
	return x == v.x && y == v.y;
}

template<typename T>
MATH_FORCE_INLINE bool vec2<T>::operator!=(vec2<T> const& v) const
{
	return !operator==(v);
}

template<typename T>
MATH_FORCE_INLINE vec2<T> vec2<T>::operator-() const
{
	return vec2<T>(-x, -y);
}

template<typename T>
MATH_FORCE_INLINE vec2<T> vec2<T>::operator+(T s) const
{
	return vec2<T>(x + s, y + s);
}
template<typename T>
MATH_FORCE_INLINE vec2<T> vec2<T>::operator+(vec2<T> const& v) const
{
	return vec2<T>(x + v.x, y + v.y);
}

template<typename T>
MATH_FORCE_INLINE vec2<T> vec2<T>::operator-(T s) const
{
	return vec2<T>(x - s, y - s);
}
template<typename T>
MATH_FORCE_INLINE vec2<T> vec2<T>::operator-(vec2<T> const& v2) const
{
	return vec2<T>(x - v2.x, y - v2.y);
}

template<typename T>
MATH_FORCE_INLINE vec2<T> vec2<T>::operator*(T s) const
{
	return vec2<T>(x * s, y * s);
}

template<typename T>
MATH_FORCE_INLINE vec2<T> vec2<T>::operator/(T s) const
{
	assert(s != T(0));
	return vec2<T>(x / s, y / s);
}
template<>
MATH_FORCE_INLINE vec2<float> vec2<float>::operator/(float s) const
{
	assert(s != 0);
	float is = float(1) / s;
	return vec2<float>(x * is, y * is);
}

template<typename T>
MATH_FORCE_INLINE vec2<T>& vec2<T>::operator=(vec2<T> const& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

template<typename T>
MATH_FORCE_INLINE vec2<T>& vec2<T>::operator+=(T s)
{
	x += s;
	y += s;
	return *this;
}
template<typename T>
MATH_FORCE_INLINE vec2<T>& vec2<T>::operator+=(vec2<T> const& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

template<typename T>
MATH_FORCE_INLINE vec2<T>& vec2<T>::operator-=(T s)
{
	x -= s;
	y -= s;
	return *this;
}
template<typename T>
MATH_FORCE_INLINE vec2<T>& vec2<T>::operator-=(vec2<T> const& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

template<typename T>
MATH_FORCE_INLINE vec2<T>& vec2<T>::operator*=(T s)
{
	x *= s;
	y *= s;
	return *this;
}
template<typename T>
MATH_FORCE_INLINE vec2<T>& vec2<T>::operator*=(vec2<T> const& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

template<typename T>
MATH_FORCE_INLINE vec2<T>& vec2<T>::operator/=(T s)
{
	assert(s != T(0));
	x /= s;
	y /= s;
	return *this;
}
template<>
MATH_FORCE_INLINE vec2<float>& vec2<float>::operator/=(float s)
{
	assert(s != float(0));
	float ts = float(1) / s;
	x *= ts;
	y *= ts;
	return *this;
}

template<typename T>
MATH_FORCE_INLINE vec2<T>& vec2<T>::operator/=(vec2<T> const& v)
{
	assert(v.x != T(0) && v.y != T(0));
	x /= v.x;
	y /= v.y;
	return *this;
}

template <typename T> 
MATH_FORCE_INLINE vec2<T> operator*(T s, vec2<T> const& v)
{
	return v*s;
}
template <typename T> 
MATH_FORCE_INLINE vec2<T> operator/(T s, vec2<T> const& v)
{
	return vec2<T>(s) / v;
}

template<typename T>
MATH_FORCE_INLINE vec2<T> operator*(vec2<T> const& v0, vec2<T> const& v1)
{
	return vec2<T>(v0.x * v1.x, v0.y * v1.y);
}

template<typename T>
MATH_FORCE_INLINE vec2<T> operator/(vec2<T> const& u, vec2<T> const& v)
{
	assert(v.x != T(0) && v.y != T(0));
	return vec2<T>(u.x / v.x, u.y / v.y);
}


} //math


#if !defined ARDUINO
template<typename T> 
inline std::ostream& operator<<(std::ostream& stream, math::vec2<T> const& v)
{
	stream << v.x << ',' << v.y;
	return stream;
}

template<typename T> 
inline std::istream& operator>>(std::istream& stream, math::vec2<T>& v)
{
	char comma;
	stream >> v.x >> comma >> v.y;
	return stream;
}

#endif
