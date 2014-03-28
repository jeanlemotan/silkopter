#pragma once

///////////////////////////////////////////////////////////////////////////////
// Defines transform functions
///////////////////////////////////////////////////////////////////////////////

namespace math
{
	///! Creates a world space lookat matrix (front axis is yaxis, up axis is zaxis)
	template<typename T, class Policy = standard> mat3<T> look_at(vec3<T> const& front, vec3<T> const& up);
	template<typename T, class Policy = standard> quat<T> quat_look_at(vec3<T> const& front, vec3<T> const& up);
	template<typename T, class Policy = standard> quat<T> quat_axis_x(T rot);
	template<typename T, class Policy = standard> quat<T> quat_axis_y(T rot);
	template<typename T, class Policy = standard> quat<T> quat_axis_z(T rot);
	template <typename T, class Policy = standard> quat<T> quat_rotation_from_to(vec3<T> const& a, vec3<T> const& b);

}//namespace math
