#pragma once

namespace math
{
	template<typename T> struct vec3;
	template<typename T> struct vec4;
	template<typename T> struct packet2;

template<typename T>
struct vec2
{
	typedef T value_t;
	T x, y;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static struct ZUninitialized {} uninitialized;
	static vec2<T> const zero;
	static vec2<T> const one;

	//////////////////////////////////////
	// Basic constructors
	vec2();

	//constructs the vector uninitialized
	vec2(ZUninitialized);

	//constructs from direct x, y values
	vec2(T x, T y);

	//construct from a single value - x == y == s
	//this has to be explicit to avoid undesirable casts
	explicit vec2(T s);

	//copy constructor
	vec2(vec2<T> const& v);
	
	// Conversion vector constructors
	template<typename U> explicit vec2(vec2<U> const& v);

	//casting
	//implementation is in cast.inl
	explicit vec2(vec3<T> const& v);
	explicit vec2(vec4<T> const& v);

	explicit vec2(packet2<T> const& v);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	vec2<T>& set(T x, T y);
	void set_length(T len);
	vec2<T>& normalize();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// operators
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	T& operator[](uint8_t i);
	T const& operator[](uint8_t i) const;

	bool operator==(vec2<T> const& v) const;
	bool operator!=(vec2<T> const& v) const;

	vec2<T> operator-() const;

	vec2<T> operator+(T s) const;
	vec2<T> operator+(vec2<T> const& v2) const;

	vec2<T> operator-(T s) const;
	vec2<T> operator-(vec2<T> const& v2) const;

	vec2<T> operator*(T s) const;
	vec2<T> operator/(T s) const;
	vec2<T>& operator=(vec2<T> const& v);

	//casting
	//implementation is in cast.inl
	vec2<T>& operator=(vec3<T> const& v);
	vec2<T>& operator=(vec4<T> const& v);

	vec2<T>& operator+=(T s);
	vec2<T>& operator+=(vec2<T> const& v);

	vec2<T>& operator-=(T s);
	vec2<T>& operator-=(vec2<T> const& v);

	vec2<T>& operator*=(T s);
	vec2<T>& operator*=(vec2<T> const& v);

	vec2<T>& operator/=(T s);
	vec2<T>& operator/=(vec2<T> const& v);
};

template <typename T> vec2<T> operator*(T s, vec2<T> const&);
template <typename T> vec2<T> operator/(T s, vec2<T> const&);
    
template<typename T> vec2<T> operator*(vec2<T> const& v0, vec2<T> const& v1);
template<typename T> vec2<T> operator/(vec2<T> const& v0, vec2<T> const& v2);

//////////////////////////////////////////////////////////////////////////

template <typename T>
typename vec2<T>::ZUninitialized vec2<T>::uninitialized;

} //math

#if !defined ARDUINO
template<typename T> std::ostream& operator<<(std::ostream& stream, math::vec2<T> const& v);
template<typename T> std::istream& operator>>(std::istream& stream, math::vec2<T>& v);
#endif

