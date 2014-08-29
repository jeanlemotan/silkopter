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

	static struct ZUninitialized {} uninitialized;
	static vec4<T> const zero;
	static vec4<T> const one;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////
	// Basic constructors
	vec4();

	//constructs the vector uninitialized
	vec4(ZUninitialized);

	//constructs from direct x, y, z values
	vec4(T x, T y, T z, T w);

	//construct from a single value - x == y == z = s
	//this has to be explicit to avoid undesirable casts
	explicit vec4(T s);

	//copy constructor
	vec4(vec4<T> const& v);
	
	// Conversion vector constructors
	template<typename U>
	explicit vec4(vec4<U> const& v);

	//cast
	//implementation in cast.inl
	explicit vec4(vec2<T> const& v);
	explicit vec4(vec2<T> const& v, T z, T w);
	explicit vec4(vec3<T> const& v);
	explicit vec4(vec3<T> const& v, T w);

	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Methods
	// Use only non-mutable methods here pls

	vec4<T>& set(T x, T y, T z, T w);
	template<typename U> vec4<T>& set(vec4<U> const& v);

	void set_length(T len);
	vec4<T>& normalize();

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

template <typename T>
typename vec4<T>::ZUninitialized vec4<T>::uninitialized;

} //math

#if !defined ARDUINO
template <typename T> std::ostream& operator<<(std::ostream& stream, math::vec4<T> const& v);
template <typename T> std::istream& operator>>(std::istream& stream, math::vec4<T>& v);
#endif
