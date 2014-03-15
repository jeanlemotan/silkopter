#pragma once

namespace math
{
	//////////////////////////////////////////////////////////////////////////
	// mixing values based on a weight.
	// lerp(a, b, t):
	//		return a*(t-1) + b*t

	float lerp(float a, float b, float t);

	template<class T>
	T lerp(T const& a, T const& b, float t);

	template<typename T>
	angle<T> lerp(angle<T> const& a, angle<T> const& b, float ratio);

	//does simple linear interpolation (as opposed to spherical linear interpolation) of quaternions
	template<typename T>
	quat<T> nlerp(quat<T> const& a, quat<T> const& b, float t);

	//equivalent to lerp
	template<typename T>
	quat<T> slerp(quat<T> const& a, quat<T> const& b, float t);
}
