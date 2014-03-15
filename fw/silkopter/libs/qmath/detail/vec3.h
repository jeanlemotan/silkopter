#pragma once

namespace math
{
	template<typename T> struct vec2;
	template<typename T> struct vec4;
	template<typename T> struct packet3;

template<typename T>
struct vec3
{
	typedef T value_t;

	T x, y, z;

	static struct ZUninitialized {} uninitialized;
	static vec3<T> const zero;
	static vec3<T> const one;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////
	// Basic constructors
	vec3();

	//constructs the vector uninitialized
	vec3(ZUninitialized);

	//constructs from direct x, y, z values
	vec3(T x, T y, T z);

	//construct from a single value - x == y == z = s
	//this has to be explicit to avoid undesirable casts
	explicit vec3(T s);

	//copy constructor
	vec3(vec3<T> const& v);
	
	// Conversion vector constructors
	template<typename U> explicit vec3(vec3<U> const& v);

	//cast
	//implementation is in cast.inl
	explicit vec3(vec2<T> const& v);
	explicit vec3(vec2<T> const& v, T z);
	explicit vec3(vec4<T> const& v);

	explicit vec3(packet3<T> const& v);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Methods
	// Use only non-mutable methods here pls

	vec3<T>& set(T x, T y, T z);
	void set_length(T len);
	vec3<T>& normalize();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// operators
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	T& operator[](uint8_t i);
	T const& operator[](uint8_t i) const;

	bool operator==(vec3<T> const& v) const;
	bool operator!=(vec3<T> const& v) const;

	vec3<T> operator-() const;

	vec3<T> operator+(T s) const;
	vec3<T> operator+(vec3<T> const& v) const;

	vec3<T> operator-(T s) const;
	vec3<T> operator-(vec3<T> const& v) const;

	vec3<T> operator*(T s) const;
	vec3<T> operator/(T s) const;

	vec3<T>& operator=(vec3<T> const& v);

	//cast
	//implementation is in cast.inl
	vec3<T>& operator=(vec2<T> const& v);
	vec3<T>& operator=(vec4<T> const& v);

	vec3<T>& operator+=(T s);
	vec3<T>& operator+=(vec3<T> const& v);

	vec3<T>& operator-=(T s);
	vec3<T>& operator-=(vec3<T> const& v);

	vec3<T>& operator*=(T s);
	vec3<T>& operator*=(vec3<T> const& v);

	vec3<T>& operator/=(T s);
	vec3<T>& operator/=(vec3<T> const& v);
};

template<typename T> vec3<T> operator*(T s, vec3<T> const&);
template<typename T> vec3<T> operator*(vec3<T> const& v0, vec3<T> const& v1);
template<typename T> vec3<T> operator/(T s, vec3<T> const&);
template<typename T> vec3<T> operator/(vec3<T> const& v0, vec3<T> const& v1);   

//////////////////////////////////////////////////////////////////////////

template<typename T> typename vec3<T>::ZUninitialized vec3<T>::uninitialized;

} //math

#if !defined ARDUINO
template<typename T> std::ostream& operator<<(std::ostream& stream, math::vec3<T> const& v);
template<typename T> std::istream& operator>>(std::istream& stream, math::vec3<T>& v);
#endif

