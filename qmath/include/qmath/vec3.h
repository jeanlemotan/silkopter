#pragma once

#include <array>

namespace math
{
	template<typename T> struct vec2;
	template<typename T> struct vec4;

template<typename T>
struct vec3
{
	typedef T value_t;

	T x, y, z;

    static vec3<T> const zero;
    static vec3<T> const one;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////
	// Basic constructors
    constexpr vec3();

	//constructs the vector uninitialized
    constexpr vec3(math::ZUninitialized);

	//constructs from direct x, y, z values
    constexpr vec3(T x, T y, T z);

	//construct from a single value - x == y == z = s
	//this has to be explicit to avoid undesirable casts
    explicit constexpr vec3(T s);

    explicit constexpr vec3(std::array<T, 3> const& v);

	//copy constructor
    constexpr vec3(vec3<T> const& v) = default;
    //move constructor
    constexpr vec3(vec3<T>&& v) = default;

	// Conversion vector constructors
    template<typename U> explicit constexpr vec3(vec3<U> const& v);

	//cast
	//implementation is in cast.inl
    explicit constexpr vec3(vec2<T> const& v);
    explicit constexpr vec3(vec2<T> const& v, T z);
    explicit constexpr vec3(vec4<T> const& v);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Methods
	// Use only non-mutable methods here pls

	void set(T x, T y, T z);
	template<typename U> void set(vec3<U> const& v);

	void set_length(T len);
	template<class Policy = standard> void normalize();
	
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

    vec3<T>& operator=(vec3<T> const& v) = default;
    vec3<T>& operator=(vec3<T>&& v) = default;

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

} //math


