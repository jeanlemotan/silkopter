#pragma once

//#include <cassert>
#include "debug/debug.h"

#include <algorithm>
#include <limits>
#include <math.h>

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
	//fast and safe
	struct fast_safe {};
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
	typedef vec2<int32_t>		vec2i;
	typedef vec2<uint32_t>		vec2u;
	typedef vec2<float>			vec2f;

	typedef vec3<int8_t>		vec3s8;
	typedef vec3<uint8_t>		vec3u8;
	typedef vec3<int16_t>		vec3s16;
	typedef vec3<uint16_t>		vec3u16;
	typedef vec3<int32_t>		vec3i;
	typedef vec3<uint32_t>		vec3u;
	typedef vec3<float>			vec3f;

	typedef vec4<int8_t>		vec4s8;
	typedef vec4<uint8_t>		vec4u8;
	typedef vec4<int16_t>		vec4s16;
	typedef vec4<uint16_t>		vec4u16;
	typedef vec4<int32_t>		vec4i;
	typedef vec4<uint32_t>		vec4u;
	typedef vec4<float>			vec4f;

	//////////////////////////////////////////////////////////////////////////

	typedef quat<float>			quatf;

	typedef mat2<float>			mat2f;
	typedef mat3<float>			mat3f;
	typedef trans2d<float>		trans2df;
	typedef mat4<float>			mat4f;
	typedef trans3d<float>		trans3df;
	typedef line2<float>		line2f;
	typedef line3<float>		line3f;
	typedef rect<int32_t>		recti;
	typedef rect<uint32_t>		rectu;
	typedef rect<float>			rectf;
	typedef aabb2<float>		aabb2f;
	typedef aabb3<float>		aabb3f;
	typedef triangle3<float>	triangle3f;
}//namespace math

#include "detail/func_common.h"
#include "detail/func_test.h"
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


#include "detail/func_common.hpp"
#include "detail/func_range.hpp"
#include "detail/func_test.hpp"
#include "detail/func_trig.hpp"
#include "detail/func_interp.hpp"
#include "detail/func_projection.hpp"
#include "detail/func_transform.hpp"
#include "detail/func_binary.hpp"
#include "detail/func_static_constructors.hpp"
#include "detail/func_rayintersect.hpp"
#include "detail/func_string.hpp"

