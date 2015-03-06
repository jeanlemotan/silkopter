#pragma once

namespace math
{
	template<typename T> struct vec2;
	template<typename T> struct vec3;
	
template<typename T>
struct vec4
{
	typedef T value_t;

	T x, y, z, w;

	static vec4<T> const zero;
	static vec4<T> const one;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////
	// Basic constructors
    constexpr vec4();

	//constructs the vector uninitialized
    constexpr vec4(math::ZUninitialized);

	//constructs from direct x, y, z values
    constexpr vec4(T x, T y, T z, T w);

	//construct from a single value - x == y == z = s
	//this has to be explicit to avoid undesirable casts
    explicit constexpr vec4(T s);

	//copy constructor
    constexpr vec4(vec4<T> const& v);
	
	// Conversion vector constructors
    template<typename U> explicit constexpr vec4(vec4<U> const& v);

	//cast
	//implementation in cast.inl
    explicit constexpr vec4(vec2<T> const& v);
    explicit constexpr vec4(vec2<T> const& v, T z, T w);
    explicit constexpr vec4(vec3<T> const& v);
    explicit constexpr vec4(vec3<T> const& v, T w);

	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Methods
	// Use only non-mutable methods here pls

    void set(T x, T y, T z, T w);
    template<typename U> void set(vec4<U> const& v);

	void set_length(T len);
    template<class Policy = standard> void normalize();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// operators
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	T& operator[](uint8_t i);
	T const& operator[](uint8_t i) const;

	bool operator==(vec4<T> const& v) const;
	bool operator!=(vec4<T> const& v) const;

	vec4<T> operator-() const;

	vec4<T> operator+(T s) const;
	vec4<T> operator+(vec4<T> const& v) const;

	vec4<T> operator-(T s) const;
	vec4<T> operator-(vec4<T> const& v) const;

	vec4<T> operator*(T s) const;
	vec4<T> operator/(T s) const;

	vec4<T>& operator=(vec4<T> const& v);
	vec4<T>& operator=(vec3<T> const& v);
	vec4<T>& operator=(vec2<T> const& v);

	vec4<T>& operator+=(T s);
	vec4<T>& operator+=(vec4<T> const& v);

	vec4<T>& operator-=(T s);
	vec4<T>& operator-=(vec4<T> const& v);

	vec4<T>& operator*=(T s);
	vec4<T>& operator*=(vec4<T> const& v);

	vec4<T>& operator/=(T s);
	vec4<T>& operator/=(vec4<T> const& v);
};

template <typename T> math::vec4<T> operator*(T s, math::vec4<T> const&);
template <typename T> math::vec4<T> operator/(T s, math::vec4<T> const&);

    
template <typename T> math::vec4<T> operator*(math::vec4<T> const&, math::vec4<T> const&);
template <typename T> math::vec4<T> operator/(math::vec4<T> const&, math::vec4<T> const&);

//////////////////////////////////////////////////////////////////////////

} //math

#if !defined ARDUINO
template <typename T> std::ostream& operator<<(std::ostream& stream, math::vec4<T> const& v);
template <typename T> std::istream& operator>>(std::istream& stream, math::vec4<T>& v);
#endif
