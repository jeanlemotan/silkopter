#pragma once

namespace math
{

template <typename T>
struct quat
{
	typedef T value_t;

	T x, y, z, w;

	static struct ZUninitialized {} uninitialized;

	static quat<T> const zero;
	static quat<T> const one;
	static quat<T> const identity;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	quat();
	quat(ZUninitialized);
	quat(T x, T y, T z, T w);
	quat(quat const&);

	//conversion constructors
	explicit quat(mat3<T> const&);
	explicit quat(mat4<T> const&);
	explicit quat(trans3d<T> const&);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//! Direct assignment
	void set(T x, T y, T z, T w);

	//! Normalizes the quat
	template<class Policy = standard> void normalize();

	//! Inverts this quat
	template<class Policy = standard> void invert();

	//! Set quat to identity
	void set_identity();

	//! cast from/to mat3
	template<class Policy = standard> void set_from_mat3(mat3<T> const& mat);
	template<class Policy = standard> void get_as_mat3(mat3<T>& mat) const;

	//! cast from/to mat4
	template<class Policy = standard> void set_from_mat4(mat4<T> const& mat);
	template<class Policy = standard> void get_as_mat4(mat4<T>& mat) const;

	//! cast from/to trans3d
	template<class Policy = standard> void set_from_trans3d(trans3d<T> const& mat);
	template<class Policy = standard> void get_as_trans3d(trans3d<T>& trans) const;

	//! cast from/to angle axis
	template<class Policy = standard> void set_from_angle_axis(T angle, vec3<T> const& axis);
	template<class Policy = standard> void get_as_angle_axis(T& angle, vec3<T>& axis) const;

	//! cast from/to euler (polar rotations in a vector)
	//these handle conversion from Euler (x, y, z) to quat
	//All possible orders of multiplication are handled.
	//NOTE - this doesn't imply any yaw/pitch/roll - the user gives that meaning.
	template<class Policy = standard> void set_from_euler_xyz(vec3<T> const& angles);
	template<class Policy = standard> void get_as_euler_xyz(vec3<T>& euler) const;
	template<class Policy = standard> void set_from_euler_xzy(vec3<T> const& angles);
	template<class Policy = standard> void get_as_euler_xzy(vec3<T>& euler) const;

	template<class Policy = standard> void set_from_euler_yxz(vec3<T> const& angles);
	template<class Policy = standard> void get_as_euler_yxz(vec3<T>& euler) const;
	template<class Policy = standard> void set_from_euler_yzx(vec3<T> const& angles);
	template<class Policy = standard> void get_as_euler_yzx(vec3<T>& euler) const;

	template<class Policy = standard> void set_from_euler_zxy(vec3<T> const& angles);
	template<class Policy = standard> void get_as_euler_zxy(vec3<T>& euler) const;
	template<class Policy = standard> void set_from_euler_zyx(vec3<T> const& angles);
	template<class Policy = standard> void get_as_euler_zyx(vec3<T>& euler) const;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// operators
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool operator==(quat<T> const& v) const;
	bool operator!=(quat<T> const& v) const;

	//! Assignment operator
	quat& operator=(quat const& other);

	//! Add operator
	quat operator+(quat const& other) const;

	//! Subtract operator
	quat operator-(quat const& other) const;

	//! Multiplication operator
	quat operator*(quat const& other) const;

	//! Multiplication operator
	quat& operator*=(quat const& other);

	//! Multiplication operator with scalar
	template <typename U> 
	quat<T> operator*(U s) const;

	//! Multiplication operator with scalar
	template <typename U> 
	quat& operator*=(U s);

	//! Negation (additive inverse) (-x,-y,-z,-w).
	// A + (-A) == A - A == identity
	quat operator-() const;

	//! Conjugation (spatial inverse) (-x,-y,-z,+w).
	// A * (~A) == identity
	quat operator~() const;
};//quaternion

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// global functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
T dot(quat<T> const& q1, quat<T> const& q2);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implementation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
typename quat<T>::ZUninitialized quat<T>::uninitialized;

}//math
