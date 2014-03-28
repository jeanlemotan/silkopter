namespace math
{

template<typename T> angle<T> const angle<T>::zero(0);
template<typename T> angle<T> const angle<T>::pi(T(3.1415926535897932384626433832795028841971));
template<typename T> angle<T> const angle<T>::pi2(T(3.1415926535897932384626433832795028841971) / T(2));
template<typename T> angle<T> const angle<T>::_2pi(T(3.1415926535897932384626433832795028841971) * T(2));
template<typename T> angle<T> const angle<T>::_3pi2(T(3.1415926535897932384626433832795028841971) * T(3) / T(2));

template<typename T> inline angle<T>::angle() : radians(T(0)) {}
template<typename T> inline angle<T>::angle(T radians) : radians(radians) {}
template<typename T> inline angle<T>::angle(angle<T> const& a) : radians(a.radians) {}
template<typename T> template<typename U> inline angle<T>::angle(angle<U> const& v) : radians(T(v.radians)) {}

template<typename T> inline angle<T>& angle<T>::normalize()
{
	T a = radians;
	while (a >= _2pi)
	{
		a -= _2pi;
	}

	while (a < (T)0)
	{
		a += _2pi;
	}

	radians = a;
	return *this;
}

template<typename T> inline bool angle<T>::operator==(angle<T> const& v) const
{
	if (radians == v.radians)
	{
		return true;
	}
	auto v1 = normalized(*this);
	auto v2 = normalized(v);
	return v1.radians == v2.radians;
}
template<typename T> inline bool angle<T>::operator!=(angle<T> const& v) const
{
	return !operator==(v);
}
template<typename T> inline angle<T> angle<T>::operator-() const
{
	return angle<T>(-radians);
}
template<typename T> inline angle<T> angle<T>::operator+(angle<T> const& v) const
{
	return angle<T>(radians + v.radians);
}
template<typename T> inline angle<T> angle<T>::operator-(angle<T> const& v) const
{
	return angle<T>(radians - v.radians);
}
template<typename T> inline angle<T> angle<T>::operator*(angle<T> const& v) const
{
	return angle<T>(radians * v.radians);
}
template<typename T> inline angle<T> angle<T>::operator/(angle<T> const& v) const
{
	QASSERT(v.radians != 0);
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
	return *this;
}
template<typename T> inline angle<T>& angle<T>::operator-=(angle<T> const& v)
{
	radians -= v.radians;
	return *this;
}
template<typename T> inline angle<T>& angle<T>::operator*=(angle<T> const& v)
{
	radians *= v.radians;
	return *this;
}
template<typename T> inline angle<T>& angle<T>::operator/=(angle<T> const& v)
{
	QASSERT(v.radians != 0);
	radians /= v.radians;
	return *this;
}

}
