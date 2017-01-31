#pragma once

#include <array>

namespace math
{

template <typename T>
struct quat
{
	typedef T value_t;

	T x, y, z, w;

	static quat<T> const zero;
	static quat<T> const one;
	static quat<T> const identity;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructors
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    constexpr quat();
    constexpr quat(math::ZUninitialized);
    constexpr quat(T x, T y, T z, T w);
    constexpr quat(quat<T> const&) = default;
    constexpr quat(quat<T>&&) = default;
    template<typename U> explicit constexpr quat(quat<U> const&);

    explicit constexpr quat(std::array<T, 4> const& v);

    template<class Policy = standard> static quat<T> look_at(vec3<T> const& front, vec3<T> const& up);
    template<class Policy = standard> static quat<T> from_axis_x(T const& a);
    template<class Policy = standard> static quat<T> from_axis_y(T const& a);
    template<class Policy = standard> static quat<T> from_axis_z(T const& a);
    //returns a quaternion that will rotate a to b
    template<class Policy = standard> static quat<T> from_a_to_b(vec3<T> const& a, vec3<T> const& b);
    //returns a quaternion that will rotate a to b
    template<class Policy = standard> static quat<T> from_a_to_b(quat<T> const& a, quat<T> const& b);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//! Direct assignment
	void set(T x, T y, T z, T w);

	//! Normalizes the quat
	template<class Policy = standard> void normalize();

	//! Inverts this quat
    void invert();

	//! Set quat to identity
	void set_identity();

	//! cast from/to mat3
	template<class Policy = standard> void set_from_mat3(mat3<T> const& mat);
	template<class Policy = standard> void get_as_mat3(mat3<T>& mat) const;
	template<class Policy = standard> mat3<T> get_as_mat3() const;
	template<class Policy = standard> void get_as_mat3_and_inv(mat3<T>& mat, mat3<T>& inv) const;

	//! cast from/to angle axis
    template<class Policy = standard> void set_from_angle_axis(T const& angle, vec3<T> const& axis);
    template<class Policy = standard> void get_as_angle_axis(T& angle, vec3<T>& axis) const;

	//! cast from/to euler (polar rotations in a vector)
	//these handle conversion from Euler (x, y, z) to quat
	//All possible orders of multiplication are handled.
	//NOTE - this doesn't imply any yaw/pitch/roll - the user gives that meaning.
    template<class Policy = standard> void set_from_euler_xyz(T const& x, T const& y, T const& z);
    template<class Policy = standard> void get_as_euler_xyz(T& x, T& y, T& z) const;
    template<class Policy = standard> void set_from_euler_xzy(T const& x, T const& y, T const& z);
    template<class Policy = standard> void get_as_euler_xzy(T& x, T& y, T& z) const;

    template<class Policy = standard> void set_from_euler_yxz(T const& x, T const& y, T const& z);
    template<class Policy = standard> void get_as_euler_yxz(T& x, T& y, T& z) const;
    template<class Policy = standard> void set_from_euler_yzx(T const& x, T const& y, T const& z);
    template<class Policy = standard> void get_as_euler_yzx(T& x, T& y, T& z) const;

    template<class Policy = standard> void set_from_euler_zxy(T const& x, T const& y, T const& z);
    template<class Policy = standard> void get_as_euler_zxy(T& x, T& y, T& z) const;
    template<class Policy = standard> void set_from_euler_zyx(T const& x, T const& y, T const& z);
    template<class Policy = standard> void get_as_euler_zyx(T& x, T& y, T& z) const;

    template<class Policy = standard> void set_from_euler_xyz(vec3<T> const& euler);
    template<class Policy = standard> void get_as_euler_xyz(vec3<T>& euler) const;
    template<class Policy = standard> void set_from_euler_xzy(vec3<T> const& euler);
    template<class Policy = standard> void get_as_euler_xzy(vec3<T>& euler) const;

    template<class Policy = standard> void set_from_euler_yxz(vec3<T> const& euler);
    template<class Policy = standard> void get_as_euler_yxz(vec3<T>& euler) const;
    template<class Policy = standard> void set_from_euler_yzx(vec3<T> const& euler);
    template<class Policy = standard> void get_as_euler_yzx(vec3<T>& euler) const;

    template<class Policy = standard> void set_from_euler_zxy(vec3<T> const& euler);
    template<class Policy = standard> void get_as_euler_zxy(vec3<T>& euler) const;
    template<class Policy = standard> void set_from_euler_zyx(vec3<T> const& euler);
    template<class Policy = standard> void get_as_euler_zyx(vec3<T>& euler) const;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// operators
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool operator==(quat<T> const& v) const;
	bool operator!=(quat<T> const& v) const;

	//! Assignment operator
    quat& operator=(quat<T> const&) = default;
    quat& operator=(quat<T>&&) = default;

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

}//math
