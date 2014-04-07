#pragma once

namespace math
{
namespace color
{

	template<typename T>
	vec4<T> u32_to_rgba(uint32_t);
	
	template<typename T>
	vec4<T> u32_to_argb(uint32_t);
	
	template<typename T>
	vec4<T> u32_to_bgra(uint32_t);
	
	template<typename T>
	vec4<T> u32_to_abgr(uint32_t);

	template<typename T>
	uint32_t rgba_to_u32(const vec4<T>& v);
	
	template<typename T>
	uint32_t argb_to_u32(const vec4<T>& v);
	
	template<typename T>
	uint32_t bgra_to_u32(const vec4<T>& v);
	
	template<typename T>
	uint32_t abgr_to_u32(const vec4<T>& v);


	template<typename T>
	vec3<T> u32_to_rgb(uint32_t);

	template<typename T>
	vec3<T> u32_to_bgr(uint32_t);

	template<typename T>
	uint32_t rgb_to_u32(const vec3<T>& v);

	template<typename T>
	uint32_t bgr_to_u32(const vec3<T>& v);

	uint32_t argb_to_abgr(uint32_t v);
	uint32_t abgr_to_argb(uint32_t v);
	uint32_t rgba_to_bgra(uint32_t v);
	uint32_t bgra_to_rgba(uint32_t v);

	uint32_t swap_rb(uint32_t v);

	uint32_t lerp_color(uint32_t a, uint32_t b, uint32_t t);
}
}
