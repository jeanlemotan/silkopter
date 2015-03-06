#pragma once

namespace math
{
	template<typename T> struct vec3;
	template<typename T> struct vec4;
	
template<typename T>
struct vec2
{
	typedef T value_t;
	T x, y;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static vec2<T> const zero;
	static vec2<T> const one;

	//////////////////////////////////////
	// Basic constructors
    constexpr vec2();

	//constructs the vector uninitialized
    constexpr vec2(math::ZUninitialized);

	//constructs from direct x, y values
    constexpr vec2(T x, T y);

	//construct from a single value - x == y == s
	//this has to be explicit to avoid undesirable casts
    explicit constexpr vec2(T s);

	//copy constructor
    constexpr vec2(vec2<T> const& v);
	
	// Conversion vector constructors
    template<typename U> explicit constexpr vec2(vec2<U> const& v);

	//casting
	//implementation is in cast.inl
    explicit constexpr vec2(vec3<T> const& v);
    explicit constexpr vec2(vec4<T> const& v);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void set(T x, T y);
    template<typename U> void set(vec2<U> const& v);

	void set_length(T len);
    template<class Policy = standard> void normalize();

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

} //math

#if !defined ARDUINO
template<typename T> std::ostream& operator<<(std::ostream& stream, math::vec2<T> const& v);
template<typename T> std::istream& operator>>(std::istream& stream, math::vec2<T>& v);
#endif

