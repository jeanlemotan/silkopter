#pragma once

namespace math
{

template<typename T>
struct angle
{
	typedef T value_t;
	T radians;

	static angle<T> const zero;
	static T const pi;
	static T const pi2;
	static T const _2pi;
	static T const _3pi2;

	angle();
	explicit angle(T radians);
	angle(angle<T> const& a);

	// Conversion constructors
	template<typename U> explicit angle(angle<U> const& a);

	//! normalizes an angle to 0 .. 2PI
	angle<T>& normalize();

	bool operator==(angle<T> const& v) const;
	bool operator!=(angle<T> const& v) const;

	angle<T> operator-() const;

	angle<T> operator+(angle<T> const& v) const;
	angle<T> operator-(angle<T> const& v) const;
	angle<T> operator*(angle<T> const& v) const;
	angle<T> operator/(angle<T> const& v) const;
	angle<T>& operator=(angle<T> const& v);
	angle<T>& operator+=(angle<T> const& v);
	angle<T>& operator-=(angle<T> const& v);
	angle<T>& operator*=(angle<T> const& v);
	angle<T>& operator/=(angle<T> const& v);

	angle<T> operator+(T v) const;
	angle<T> operator-(T v) const;
	angle<T> operator*(T v) const;
	angle<T> operator/(T v) const;
	angle<T>& operator=(T v);
	angle<T>& operator+=(T v);
	angle<T>& operator-=(T v);
	angle<T>& operator*=(T v);
	angle<T>& operator/=(T v);
};

}
