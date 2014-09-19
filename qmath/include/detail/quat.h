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
	template<typename U> explicit quat(quat<U> const&);

    template<class Policy = standard> static quat<T> look_at(vec3<T> const& front, vec3<T> const& up);
    template<class Policy = standard> static quat<T> from_axis_x(angle<T> const& a);
    template<class Policy = standard> static quat<T> from_axis_y(angle<T> const& a);
    template<class Policy = standard> static quat<T> from_axis_z(angle<T> const& a);
    //returns a quaternion that will rotate a to b
    template<class Policy = standard> static quat<T> from_vectors(vec3<T> const& a, vec3<T> const& b);

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
	template<class Policy = standard> mat3<T> get_as_mat3() const;
	template<class Policy = standard> void get_as_mat3_and_inv(mat3<T>& mat, mat3<T>& inv) const;

	//! cast from/to angle axis
    template<class Policy = standard> void set_from_angle_axis(angle<T> const& angle, vec3<T> const& axis);
    template<class Policy = standard> void get_as_angle_axis(angle<T>& angle, vec3<T>& axis) const;

	//! cast from/to euler (polar rotations in a vector)
	//these handle conversion from Euler (x, y, z) to quat
	//All possible orders of multiplication are handled.
	//NOTE - this doesn't imply any yaw/pitch/roll - the user gives that meaning.
    template<class Policy = standard> void set_from_euler_xyz(angle<T> const& x, angle<T> const& y, angle<T> const& z);
    template<class Policy = standard> void get_as_euler_xyz(angle<T>& x, angle<T>& y, angle<T>& z) const;
    template<class Policy = standard> void set_from_euler_xzy(angle<T> const& x, angle<T> const& y, angle<T> const& z);
    template<class Policy = standard> void get_as_euler_xzy(angle<T>& x, angle<T>& y, angle<T>& z) const;

    template<class Policy = standard> void set_from_euler_yxz(angle<T> const& x, angle<T> const& y, angle<T> const& z);
    template<class Policy = standard> void get_as_euler_yxz(angle<T>& x, angle<T>& y, angle<T>& z) const;
    template<class Policy = standard> void set_from_euler_yzx(angle<T> const& x, angle<T> const& y, angle<T> const& z);
    template<class Policy = standard> void get_as_euler_yzx(angle<T>& x, angle<T>& y, angle<T>& z) const;

    template<class Policy = standard> void set_from_euler_zxy(angle<T> const& x, angle<T> const& y, angle<T> const& z);
    template<class Policy = standard> void get_as_euler_zxy(angle<T>& x, angle<T>& y, angle<T>& z) const;
    template<class Policy = standard> void set_from_euler_zyx(angle<T> const& x, angle<T> const& y, angle<T> const& z);
    template<class Policy = standard> void get_as_euler_zyx(angle<T>& x, angle<T>& y, angle<T>& z) const;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// operators
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool operator==(quat<T> const& v) const;
	bool operator!=(quat<T> const& v) const;

	//! Assignment operator
	quat& operator=(quat const& other);

	//! Add operator
	quat operator+(quat const& other) const;
	quat& operator+=(quat const& other);

	//! Subtract operator
	quat operator-(quat const& other) const;
	quat& operator-=(quat const& other);

	//! Multiplication operator
	quat operator*(quat const& other) const;
	quat& operator*=(quat const& other);

	//! Multiplication operator with scalar
	template <typename U> quat<T> operator*(U s) const;
	template <typename U> quat& operator*=(U s);

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

template <typename T> T dot(quat<T> const& q1, quat<T> const& q2);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implementation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T> typename quat<T>::ZUninitialized quat<T>::uninitialized;

}//math
