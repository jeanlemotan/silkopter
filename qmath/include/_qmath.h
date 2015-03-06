#pragma once

#include "QBase.h"

#include <algorithm>
#include <limits>
#include <math.h>
#include <string.h>

namespace math
{
	//standard precision
	struct standard {};
	//fast but less precise
	struct fast {};
	//standard precision but safe assumptions
	struct safe {};
}

#include "qmath/constants.h"
#include "qmath/standard_types.h"

// include files order matters, mat4 requires vec3 and vec4, watch out!
#include "qmath/angle.h"
#include "qmath/vec2.h"
#include "qmath/vec3.h"
#include "qmath/vec4.h"
#include "qmath/rect.h"//requires vec2
#include "qmath/mat2.h"
#include "qmath/mat3.h"
#include "qmath/mat4.h"
#include "qmath/trans2d.h"
#include "qmath/trans3d.h"
#include "qmath/quat.h"
#include "qmath/plane.h"
#include "qmath/line2.h"
#include "qmath/line3.h"
#include "qmath/triangle3.h"
#include "qmath/aabb3.h"
#include "qmath/aabb2.h"
#include "qmath/color.h"

namespace math
{
	static struct ZUninitialized {} uninitialized;

	typedef angle<float>		anglef;
    typedef angle<double>		angled;

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

#include "qmath/func_test.h"
#include "qmath/func_common.h"
#include "qmath/func_range.h"
#include "qmath/func_trig.h"
#include "qmath/func_interp.h"
#include "qmath/func_projection.h"
#include "qmath/func_transform.h"
#include "qmath/func_binary.h"
#include "qmath/func_static_constructors.h"
#include "qmath/func_rayintersect.h"
#include "qmath/func_string.h"

#include "qmath/cast.h"

#include "qmath/angle.hpp"
#include "qmath/vec2.hpp"
#include "qmath/vec3.hpp"
#include "qmath/vec4.hpp"
#include "qmath/rect.hpp"
#include "qmath/mat2.hpp"
#include "qmath/mat3.hpp"
#include "qmath/mat4.hpp"
#include "qmath/trans2d.hpp"
#include "qmath/trans3d.hpp"
#include "qmath/quat.hpp"
#include "qmath/plane.hpp"
#include "qmath/line2.hpp"
#include "qmath/line3.hpp"
#include "qmath/triangle3.hpp"
#include "qmath/aabb3.hpp"
#include "qmath/aabb2.hpp"
#include "qmath/color.hpp"
#include "qmath/cast.hpp"


#include "qmath/func_range.hpp"
#include "qmath/func_test.hpp"
#include "qmath/func_common.hpp"
#include "qmath/func_trig.hpp"
#include "qmath/func_interp.hpp"
#include "qmath/func_projection.hpp"
#include "qmath/func_transform.hpp"
#include "qmath/func_binary.hpp"
#include "qmath/func_static_constructors.hpp"
#include "qmath/func_rayintersect.hpp"
#include "qmath/func_string.hpp"

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

