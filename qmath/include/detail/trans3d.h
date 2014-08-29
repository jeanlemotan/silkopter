#pragma once

namespace math
{

///////////////////////////////////////////////////////////////////////////////
// forward declarations
///////////////////////////////////////////////////////////////////////////////
template<typename T> struct vec3;
template<typename T> struct vec4;
template<typename T> struct quat;
template<typename T> struct trans2d;
template<typename T> struct trans3d;

//column-major (OpenGL) layout

//	.--------------- column0 - x axis
//	|   .----------- column1 - y axis
//	|   |   .------- column2 - z axis
//  |	|	|	.--- column3 - translation
//  0   4   8  12
//  1   5   9  13
//  2   6  10  14
//  3   7  11  15 <-- this is 0, 0, 0, 1

template <typename T>
struct trans3d
{
	typedef T value_t;
	static struct ZUninitialized {} uninitialized;
	static trans3d<T> const identity;


	///////////////////////////////////////////////////////////////////////////////
	// constructors
	///////////////////////////////////////////////////////////////////////////////
	trans3d();
	trans3d(ZUninitialized);
	trans3d(trans3d<T> const& other);
	template<typename U> explicit trans3d(trans3d<U> const& v);

	//casting from trans2d
	explicit trans3d(trans2d<T> const& v);

	trans3d(vec3<T> const& translation, quat<T> const& rotation, vec3<T> const& scale);
	trans3d(vec3<T> const& translation, mat3<T> const& rotation, vec3<T> const& scale);

	///////////////////////////////////////////////////////////////////////////////
	// arithmetic operators
	///////////////////////////////////////////////////////////////////////////////

	trans3d<T> operator*(trans3d<T> const& other) const;
	trans3d<T>& operator*=(trans3d<T> const& other);

	///////////////////////////////////////////////////////////////////////////////
	// methods
	///////////////////////////////////////////////////////////////////////////////

	trans3d<T>&	set_identity();
	trans3d<T>&	set_rotation_identity();
	template<class Polict = standard> bool	invert();
	trans3d<T>&	post_scale(vec3<T> const& scale);
	trans3d<T>&	post_translate(vec3<T> const& translation);

public:
	vec3<T> const& get_axis_x() const;
	trans3d<T>&	set_axis_x(vec3<T> const& axis);

	vec3<T> const& get_axis_y() const;
	trans3d<T>&	set_axis_y(vec3<T> const& axis);

	vec3<T> const& get_axis_z() const;
	trans3d<T>&	set_axis_z(vec3<T> const& axis);

	vec3<T> const& get_translation() const;
	trans3d<T>&	set_translation(vec3<T> const& translation);

	vec3<T>		get_scale() const;
	trans3d<T>&	set_scale(vec3<T> const& scale);

	mat3<T>		get_rotation() const;
	trans3d<T>&	set_rotation(mat3<T> const& rotation);
	trans3d<T>&	set_rotation(quat<T> const& rotation);

	void		repair();
	bool		is_valid() const;

	///////////////////////////////////////////////////////////////////////////////
	// operators

	bool operator==(trans3d<T> const& v) const;
	bool operator!=(trans3d<T> const& v) const;

	//casting from trans2d
	trans3d<T>& operator=(trans2d<T> const& m);

	///////////////////////////////////////////////////////////////////////////////
	//members
	///////////////////////////////////////////////////////////////////////////////

	mat4<T> mat;
};

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////

template <typename T>
typename trans3d<T>::ZUninitialized trans3d<T>::uninitialized;

///////////////////////////////////////////////////////////////////////////////

}//namespace math
