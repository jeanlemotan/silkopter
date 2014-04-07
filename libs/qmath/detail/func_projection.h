#pragma once

namespace math
{
	///! Creates a RH offseted orthographic projection matrix
	template<typename T> mat4<T> ortho(T left, T right, T bottom, T top, T z_near, T z_far);

	///! Creates a RH perspective projection matrix
	template<typename T> mat4<T> perspective(T fov, T aspect_ratio, T z_near, T z_far);

	///! Creates a RH asymmetric frustum projection matrix
	template<typename T> mat4<T> perspective(T left, T right, T bottom, T top, T z_near, T z_far);

}//namespace math
