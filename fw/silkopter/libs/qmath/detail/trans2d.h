#pragma once

namespace math
{

///////////////////////////////////////////////////////////////////////////////
// forward declarations
///////////////////////////////////////////////////////////////////////////////
template<typename T> struct vec2;
template<typename T> struct mat2;
template<typename T> struct trans2d;
template<typename T> struct trans3d;

//	.--------------- packet0 (column0) - x axis
//	|   .----------- packet1 (column1) - y axis
//  |	|	.------- packet2 (column2) - translation
//  0   3   6
//  1   4   7
//  2   5   8 <--- always 0, 0, 1

template <typename T>
struct trans2d
{
	typedef T value_t;
	static struct ZUninitialized {} uninitialized;
	static trans2d<T> const zero;
	static trans2d<T> const one;
	static trans2d<T> const identity;


	///////////////////////////////////////////////////////////////////////////////
	// constructors
	///////////////////////////////////////////////////////////////////////////////
	trans2d();
	trans2d(ZUninitialized);
	trans2d(trans2d<T> const& other);
	template<typename U> explicit trans2d(trans2d<U> const& v);

	//casting from trans3d
	explicit trans2d(trans3d<T> const& v);

	trans2d(vec2<T> const& translation, T rotation, vec2<T> const& scale);
	trans2d(vec2<T> const& translation, mat2<T> const& rotation, vec2<T> const& scale);

	///////////////////////////////////////////////////////////////////////////////
	// arithmetic operators
	///////////////////////////////////////////////////////////////////////////////

	trans2d<T> operator*(trans2d<T> const& other) const;
	trans2d<T>& operator*=(trans2d<T> const& other);

	///////////////////////////////////////////////////////////////////////////////
	// methods
	///////////////////////////////////////////////////////////////////////////////

	trans2d<T>&	set_identity();
	trans2d<T>&	set_rotation_identity();
	bool		invert();
	trans2d<T>&	post_scale(vec2<T> const& scale);
	trans2d<T>&	post_translate(vec2<T> const& trans);

public:
	vec2<T> const& get_axis_x() const;
	trans2d<T>&	set_axis_x(vec2<T> const& axis);

	vec2<T> const& get_axis_y() const;
	trans2d<T>&	set_axis_y(vec2<T> const& axis);

	vec2<T> const& get_translation() const;
	trans2d<T>&	set_translation(vec2<T> const& trans);

	vec2<T>		get_scale() const;
	trans2d<T>&	set_scale(vec2<T> const& scale);

	mat2<T>		get_rotation() const;
	trans2d<T>&	set_rotation(mat2<T> const& rot);
	trans2d<T>&	set_rotation(T rot);

	void		repair();
	bool		is_valid() const;

	///////////////////////////////////////////////////////////////////////////////
	// operators

	bool operator==(trans2d<T> const& v) const;
	bool operator!=(trans2d<T> const& v) const;

	//casting from trans2d
	trans2d<T>& operator=(trans3d<T> const& m);

	///////////////////////////////////////////////////////////////////////////////
	//members
	///////////////////////////////////////////////////////////////////////////////

	mat3<T> mat;
};

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

template <typename T>
typename trans2d<T>::ZUninitialized trans2d<T>::uninitialized;

///////////////////////////////////////////////////////////////////////////////

}//namespace math
