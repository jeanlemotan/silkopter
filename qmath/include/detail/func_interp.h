#pragma once

namespace math
{
	//////////////////////////////////////////////////////////////////////////
	// mixing values based on a weight.
	// lerp(a, b, t):
	//		return a*(t-1) + b*t

	template<class T, class Policy = standard> T lerp(T const& a, T const& b, float t);

	//does simple linear interpolation (as opposed to spherical linear interpolation) of quaternions
	template<typename T, class Policy = standard> quat<T> nlerp(quat<T> const& a, quat<T> const& b, float t);

	//equivalent to lerp
	template<typename T, class Policy = standard> quat<T> slerp(quat<T> const& a, quat<T> const& b, float t);
}
