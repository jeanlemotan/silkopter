#pragma once

#if !defined __AVR__
#	include <cassert>
#endif


#include <algorithm>
#include <limits>
#include <math.h>
#include <string.h>

#if defined ARDUINO
#	undef min
#	undef max
#	undef abs
#	undef round
#	undef radians
#	undef degrees
#endif

namespace math
{
	//standard precision
	struct standard {};
	//fast but less precise
	struct fast {};
	//standard precision but safe assumptions
	struct safe {};
}

#include "detail/constants.h"
#include "detail/standard_types.h"

// include files order matters, mat4 requires vec3 and vec4, watch out!
#include "detail/angle.h"
#include "detail/vec2.h"
#include "detail/vec3.h"
#include "detail/vec4.h"
#include "detail/rect.h"//requires vec2
#include "detail/mat2.h"
#include "detail/mat3.h"
#include "detail/mat4.h"
#include "detail/trans2d.h"
#include "detail/trans3d.h"
#include "detail/quat.h"
#include "detail/plane.h"
#include "detail/line2.h"
#include "detail/line3.h"
#include "detail/triangle3.h"
#include "detail/aabb3.h"
#include "detail/aabb2.h"
#include "detail/color.h"

namespace math
{
	typedef angle<float>		anglef;

	typedef vec2<int8_t>		vec2s8;
	typedef vec2<uint8_t>		vec2u8;
	typedef vec2<int16_t>		vec2s16;
	typedef vec2<uint16_t>		vec2u16;
	typedef vec2<int32_t>		vec2s32;
	typedef vec2<uint32_t>		vec2u32;
	typedef vec2<float>			vec2f;
	typedef vec2<double>		vec2d;

	typedef vec3<int8_t>		vec3s8;
	typedef vec3<uint8_t>		vec3u8;
	typedef vec3<int16_t>		vec3s16;
	typedef vec3<uint16_t>		vec3u16;
	typedef vec3<int32_t>		vec3s32;
	typedef vec3<uint32_t>		vec3u32;
	typedef vec3<float>			vec3f;
	typedef vec3<double>		vec3d;

	typedef vec4<int8_t>		vec4s8;
	typedef vec4<uint8_t>		vec4u8;
	typedef vec4<int16_t>		vec4s16;
	typedef vec4<uint16_t>		vec4u16;
	typedef vec4<int32_t>		vec4s32;
	typedef vec4<uint32_t>		vec4u32;
	typedef vec4<float>			vec4f;
	typedef vec4<double>		vec4d;

	//////////////////////////////////////////////////////////////////////////

	typedef quat<float>			quatf;
	typedef quat<double>		quatd;

	typedef mat2<float>			mat2f;
	typedef mat2<double>		mat2d;
	typedef mat3<float>			mat3f;
	typedef mat3<double>		mat3d;
	typedef trans2d<float>		trans2df;
	typedef trans2d<double>		trans2dd;
	typedef mat4<float>			mat4f;
	typedef mat4<double>		mat4d;
	typedef trans3d<float>		trans3df;
	typedef trans3d<double>		trans3dd;
	typedef line2<float>		line2f;
	typedef line2<double>		line2d;
	typedef line3<float>		line3f;
	typedef line3<double>		line3d;
	typedef rect<int32_t>		rects32;
	typedef rect<uint32_t>		rectu32;
	typedef rect<float>			rectf;
	typedef rect<double>		rectd;
	typedef aabb2<float>		aabb2f;
	typedef aabb2<double>		aabb2d;
	typedef aabb3<float>		aabb3f;
	typedef aabb3<double>		aabb3d;
	typedef triangle3<float>	triangle3f;
	typedef triangle3<double>	triangle3d;
}//namespace math

#include "detail/func_test.h"
#include "detail/func_common.h"
#include "detail/func_range.h"
#include "detail/func_trig.h"
#include "detail/func_interp.h"
#include "detail/func_projection.h"
#include "detail/func_transform.h"
#include "detail/func_binary.h"
#include "detail/func_static_constructors.h"
#include "detail/func_rayintersect.h"
#include "detail/func_string.h"

#include "detail/cast.h"

#include "detail/angle.hpp"
#include "detail/vec2.hpp"
#include "detail/vec3.hpp"
#include "detail/vec4.hpp"
#include "detail/rect.hpp"
#include "detail/mat2.hpp"
#include "detail/mat3.hpp"
#include "detail/mat4.hpp"
#include "detail/trans2d.hpp"
#include "detail/trans3d.hpp"
#include "detail/quat.hpp"
#include "detail/plane.hpp"
#include "detail/line2.hpp"
#include "detail/line3.hpp"
#include "detail/triangle3.hpp"
#include "detail/aabb3.hpp"
#include "detail/aabb2.hpp"
#include "detail/color.hpp"
#include "detail/cast.hpp"


#include "detail/func_range.hpp"
#include "detail/func_test.hpp"
#include "detail/func_common.hpp"
#include "detail/func_trig.hpp"
#include "detail/func_interp.hpp"
#include "detail/func_projection.hpp"
#include "detail/func_transform.hpp"
#include "detail/func_binary.hpp"
#include "detail/func_static_constructors.hpp"
#include "detail/func_rayintersect.hpp"
#include "detail/func_string.hpp"

/*
namespace math
{

#define DECLARE_CONSTANTS(T)																							   \
	template<> angle<T> const angle<T>::zero(0);																	   \
	template<> T const angle<T>::pi(T(3.1415926535897932384626433832795028841971));							   \
	template<> T const angle<T>::pi2(T(3.1415926535897932384626433832795028841971) / T(2));					   \
	template<> T const angle<T>::_2pi(T(3.1415926535897932384626433832795028841971) * T(2));					   \
	template<> T const angle<T>::_3pi2(T(3.1415926535897932384626433832795028841971) * T(3) / T(2));			   \
	\
	template<> mat2<T> const mat2<T>::zero(0);																		   \
	template<> mat2<T> const mat2<T>::one(1);																		   \
	template<> mat2<T> const mat2<T>::identity;																		   \
	\
	template<> mat3<T> const mat3<T>::zero(0);																		   \
	template<> mat3<T> const mat3<T>::one(1);																		   \
	template<> mat3<T> const mat3<T>::identity;																		   \
	\
	template<> mat4<T> const mat4<T>::zero(0);																		   \
	template<> mat4<T> const mat4<T>::one(1);																		   \
	template<> mat4<T> const mat4<T>::identity;																		   \
	\
	template<> quat<T> const quat<T>::zero(0, 0, 0, 1);																   \
	template<> quat<T> const quat<T>::one(1, 1, 1, 1);																   \
	template<> quat<T> const quat<T>::identity;																		   \
	\
	template<> trans2d<T> const trans2d<T>::identity;																   \
	\
	template<> trans3d<T> const trans3d<T>::identity;																   \
	\
	template<> vec2<T> const vec2<T>::zero(0);																		   \
	template<> vec2<T> const vec2<T>::one(1);																		   \
	\
	template<> vec3<T> const vec3<T>::zero(0);																		   \
	template<> vec3<T> const vec3<T>::one(1);																		   \
	\
	template<> vec4<T> const vec4<T>::zero(0);																		   \
	template<> vec4<T> const vec4<T>::one(1);																		   

DECLARE_CONSTANTS(float)
DECLARE_CONSTANTS(double)
DECLARE_CONSTANTS(uint8_t)
DECLARE_CONSTANTS(uint16_t)
DECLARE_CONSTANTS(uint32_t)
DECLARE_CONSTANTS(int8_t)
DECLARE_CONSTANTS(int16_t)
DECLARE_CONSTANTS(int32_t)

}

*/
