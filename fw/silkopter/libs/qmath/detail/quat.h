#pragma once

namespace math
{

template <typename T>
struct quat
{
	typedef T value_t;

	T x, y, z, w;

	static struct ZUninitialized {} uninitialized;
	static struct ZAxisX {} axisX;
	static struct ZAxisY {} axisY;
	static struct ZAxisZ {} axisZ;
	static struct ZAngleAxis {} angleAxis;
	static struct ZEulerXYZ {} eulerXYZ;
	static struct ZEulerXZY {} eulerXZY;
	static struct ZEulerYXZ {} eulerYXZ;
	static struct ZEulerYZX {} eulerYZX;
	static struct ZEulerZXY {} eulerZXY;
	static struct ZEulerZYX {} eulerZYX;

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

	//these use the enum to avoid ambiguities
	//use as quatf(quatf::axisX, pi)
	//or quatf(quatf::angleAxis, pi, vec3f(0, 0, 1))
	quat(ZAxisX, T angle);
	quat(ZAxisY, T angle);
	quat(ZAxisZ, T angle);
	quat(ZAngleAxis, T angle, vec3<T> const& axis);

	//These handle conversion from Euler (x, y, z) to quat
	//All possible orders of multiplication are handled.
	//NOTE - this doesn't imply any yaw/pitch/roll - the user gives that meaning.
	quat(ZEulerXYZ, vec3<T> const& angles);
	quat(ZEulerXZY, vec3<T> const& angles);
	quat(ZEulerYXZ, vec3<T> const& angles);
	quat(ZEulerYZX, vec3<T> const& angles);
	quat(ZEulerZXY, vec3<T> const& angles);
	quat(ZEulerZYX, vec3<T> const& angles);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//! Direct assignment
	quat<T>& set(T x, T y, T z, T w);

	//! Normalizes the quat
	quat<T>& normalize();

	//! Inverts this quat
	quat<T>& invert();

	//! Set quat to identity
	quat<T>& set_identity();

	//! cast from/to mat3
	quat<T>& set_from_mat3(mat3<T> const& mat);
	void  get_as_mat3(mat3<T>& mat) const;

	//! cast from/to mat4
	quat<T>& set_from_mat4(mat4<T> const& mat);
	void  get_as_mat4(mat4<T>& mat) const;

	//! cast from/to trans3d
	quat<T>& set_from_trans3d(trans3d<T> const& mat);
	void	get_as_trans3d(trans3d<T>& trans) const;

	//! cast from/to angle axis
	quat<T>& set_from_angle_axis(T angle, vec3<T> const& axis);
	void	 get_as_angle_axis(T& angle, vec3<T>& axis) const;

	//! cast from/to euler (polar rotations in a vector)
	//these handle conversion from Euler (x, y, z) to quat
	//All possible orders of multiplication are handled.
	//NOTE - this doesn't imply any yaw/pitch/roll - the user gives that meaning.
	quat<T>& set_from_euler_xyz(vec3<T> const& angles);
	void  get_as_euler_xyz(vec3<T>& euler) const;
	quat<T>& set_from_euler_xzy(vec3<T> const& angles);
	void  get_as_euler_xzy(vec3<T>& euler) const;

	quat<T>& set_from_euler_yxz(vec3<T> const& angles);
	void  get_as_euler_yxz(vec3<T>& euler) const;
	quat<T>& set_from_euler_yzx(vec3<T> const& angles);
	void  get_as_euler_yzx(vec3<T>& euler) const;

	quat<T>& set_from_euler_zxy(vec3<T> const& angles);
	void  get_as_euler_zxy(vec3<T>& euler) const;
	quat<T>& set_from_euler_zyx(vec3<T> const& angles);
	void  get_as_euler_zyx(vec3<T>& euler) const;

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
template <typename T>
typename quat<T>::ZAxisX quat<T>::axisX;
template <typename T>
typename quat<T>::ZAxisY quat<T>::axisY;
template <typename T>
typename quat<T>::ZAxisZ quat<T>::axisZ;
template <typename T>
typename quat<T>::ZAngleAxis quat<T>::angleAxis;
template <typename T>
typename quat<T>::ZEulerXYZ quat<T>::eulerXYZ;
template <typename T>
typename quat<T>::ZEulerXZY quat<T>::eulerXZY;
template <typename T>
typename quat<T>::ZEulerYXZ quat<T>::eulerYXZ;
template <typename T>
typename quat<T>::ZEulerYZX quat<T>::eulerYZX;
template <typename T>
typename quat<T>::ZEulerZXY quat<T>::eulerZXY;
template <typename T>
typename quat<T>::ZEulerZYX quat<T>::eulerZYX;

}//math
