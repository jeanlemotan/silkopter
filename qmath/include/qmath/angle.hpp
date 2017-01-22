namespace math
{
template<typename T> angle<T> const angle<T>::zero(0);																	   \
template<typename T> T const angle<T>::pi(T(3.1415926535897932384626433832795028841971));							   \
template<typename T> T const angle<T>::pi2(T(3.1415926535897932384626433832795028841971) / T(2));					   \
template<typename T> T const angle<T>::_2pi(T(3.1415926535897932384626433832795028841971) * T(2));					   \
template<typename T> T const angle<T>::_3pi2(T(3.1415926535897932384626433832795028841971) * T(3) / T(2));			   \

template<typename T> inline angle<T>::angle() : radians(T(0)) {}
template<typename T> inline angle<T>::angle(T radians) : radians(radians) { normalize(); }
template<typename T> inline angle<T>::angle(angle<T> const& a) : radians(a.radians) {}
template<typename T> template<typename U> inline angle<T>::angle(angle<U> const& v) : radians(T(v.radians)) {}

template<typename T> inline angle<T>& angle<T>::normalize()
{
	T a = radians;
	while (a >= _2pi)
	{
		a -= _2pi;
	}

	while (math::is_negative(a))
	{
		a += _2pi;
	}

	radians = a;
	return *this;
}

template<typename T> inline bool angle<T>::operator==(angle<T> const& v) const
{
	return (radians == v.radians);
}
template<typename T> inline bool angle<T>::operator!=(angle<T> const& v) const
{
	return !operator==(v);
}
template<typename T> inline angle<T> angle<T>::operator-() const
{
	angle<T> x(*this); 
	x.radians = -x.radians; //no need to normalize
	return x;
}
template<typename T> inline angle<T> angle<T>::operator+(angle<T> const& v) const
{
	return angle<T>(radians + v.radians);
}
template<typename T> inline angle<T> angle<T>::operator-(angle<T> const& v) const
{
	T difference = radians - v.radians;
	while (difference < -pi) difference += _2pi;
	while (difference > pi) difference -= _2pi;
	angle<T> x;
	x.radians = difference;
	return x;
}
template<typename T> inline angle<T> angle<T>::operator*(angle<T> const& v) const
{
	return angle<T>(radians * v.radians);
}
template<typename T> inline angle<T> angle<T>::operator/(angle<T> const& v) const
{
	MATH_ASSERT(v.radians != 0);
	return angle<T>(radians / v.radians);
}
template<typename T> inline angle<T>& angle<T>::operator=(angle<T> const& v)
{
	radians = v.radians;
	return *this;
}
template<typename T> inline angle<T>& angle<T>::operator+=(angle<T> const& v)
{
	radians += v.radians;
	normalize();
	return *this;
}
template<typename T> inline angle<T>& angle<T>::operator-=(angle<T> const& v)
{
	radians -= v.radians;
	normalize();
	return *this;
}
template<typename T> inline angle<T>& angle<T>::operator*=(angle<T> const& v)
{
	radians *= v.radians;
	normalize();
	return *this;
}
template<typename T> inline angle<T>& angle<T>::operator/=(angle<T> const& v)
{
	MATH_ASSERT(v.radians != 0);
	radians /= v.radians;
	normalize();
	return *this;
}

//////////////////////////////////////////////////////////////////////////

template<typename T> inline angle<T> angle<T>::operator-(T v) const
{
	T difference = radians - v;
	while (difference < -pi) difference += _2pi;
	while (difference > pi) difference -= _2pi;
	angle<T> x;
	x.radians = difference;
	return x;
}
template<typename T> inline angle<T> angle<T>::operator*(T v) const
{
	return angle<T>(radians * v);
}
template<typename T> inline angle<T> angle<T>::operator/(T v) const
{
	MATH_ASSERT(v != 0);
	return angle<T>(radians / v);
}
template<typename T> inline angle<T>& angle<T>::operator=(T v)
{
	radians = v;
	return *this;
}
template<typename T> inline angle<T>& angle<T>::operator+=(T v)
{
	radians += v;
	normalize();
	return *this;
}
template<typename T> inline angle<T>& angle<T>::operator-=(T v)
{
	radians -= v;
	normalize();
	return *this;
}
template<typename T> inline angle<T>& angle<T>::operator*=(T v)
{
	radians *= v;
	normalize();
	return *this;
}
template<typename T> inline angle<T>& angle<T>::operator/=(T v)
{
	MATH_ASSERT(v != 0);
	radians /= v;
	normalize();
	return *this;
}


}
