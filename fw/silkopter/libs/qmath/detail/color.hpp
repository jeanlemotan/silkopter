namespace math
{
namespace color
{


template<typename T>
inline vec4<T> u32_to_rgba(uint32_t v)
{
	return vec4<T>(vec4<uint32_t>((v >> 16) & 0xFF, (v >> 8) & 0xFF, (v) & 0xFF, (v >> 24) & 0xFF)) * (T)0.003921568627451;
}
template<typename T>
inline vec4<T> u32_to_argb(uint32_t v)
{
	return vec4<T>(vec4<uint32_t>((v >> 24) & 0xFF, (v >> 16) & 0xFF, (v >> 8) & 0xFF, (v) & 0xFF)) * (T)0.003921568627451;
}
template<typename T>
inline vec4<T> u32_to_bgra(uint32_t v)
{
	return vec4<T>(vec4<uint32_t>((v) & 0xFF, (v >> 8) & 0xFF, (v >> 16) & 0xFF, (v >> 24) & 0xFF)) * (T)0.003921568627451;
}
template<typename T>
inline vec4<T> u32_to_abgr(uint32_t v)
{
	return vec4<T>(vec4<uint32_t>((v >> 24) & 0xFF, (v) & 0xFF, (v >> 8) & 0xFF, (v >> 16) & 0xFF)) * (T)0.003921568627451;
}

template<typename T>
inline uint32_t rgba_to_u32(const vec4<T>& v)
{
	vec4<uint32_t> uv(v * (T)255);
	return ((uv.w  & 0xFF) << 24) | ((uv.x  & 0xFF) << 16) | ((uv.y  & 0xFF) << 8) | ((uv.z  & 0xFF));
}
template<typename T>
inline uint32_t argb_to_u32(const vec4<T>& v)
{
	vec4<uint32_t> uv(v * (T)255);
	return ((uv.x  & 0xFF) << 24) | ((uv.y  & 0xFF) << 16) | ((uv.z  & 0xFF) << 8) | ((uv.w  & 0xFF));
}
template<typename T>
inline uint32_t bgra_to_u32(const vec4<T>& v)
{
	vec4<uint32_t> uv(v * (T)255);
	return ((uv.w  & 0xFF) << 24) | ((uv.z  & 0xFF) << 16) | ((uv.y  & 0xFF) << 8) | ((uv.x  & 0xFF));
}
template<typename T>
inline uint32_t abgr_to_u32(const vec4<T>& v)
{
	vec4<uint32_t> uv(v * (T)255);
	return ((uv.x  & 0xFF) << 24) | ((uv.w  & 0xFF) << 16) | ((uv.z  & 0xFF) << 8) | ((uv.y  & 0xFF));
}

//////////////////////////////////////////////////////////////////////////

template<typename T>
inline vec3<T> u32_to_rgb(uint32_t v)
{	
	return vec3<T>(vec3<uint32_t>((v >> 16) & 0xFF, (v >> 8) & 0xFF, (v) & 0xFF)) * (T)0.003921568627451;
}
template<typename T>
inline vec3<T> u32_to_bgr(uint32_t v)
{
	return vec3<T>(vec3<uint32_t>((v) & 0xFF, (v >> 8) & 0xFF, (v >> 16) & 0xFF)) * (T)0.003921568627451;
}

template<typename T>
inline uint32_t rgb_to_u32(const vec3<T>& v)
{
	vec3<uint32_t> uv(v * (T)255);
	return ((uv.x  & 0xFF) << 16) | ((uv.y  & 0xFF) << 8) | ((uv.z  & 0xFF)) | (uint32_t(0xFF) << 24);
}

template<typename T>
inline uint32_t bgr_to_u32(const vec3<T>& v)
{
	vec3<uint32_t> uv(v * (T)255);
	return ((uv.z  & 0xFF) << 16) | ((uv.y  & 0xFF) << 8) | ((uv.x  & 0xFF)) | (uint32_t(0xFF) << 24);
}

inline uint32_t argb_to_abgr(uint32_t v)
{
	uint32_t a = (v) >> 24;
	uint32_t r = (v >> 16) & 0xFF;
	uint32_t g = (v >> 8) & 0xFF;
	uint32_t b = (v) & 0xFF;
	return (a << 24) | (b << 16) | (g << 8) | (r);
}
inline uint32_t abgr_to_argb(uint32_t v)
{
	uint32_t a = (v) >> 24;
	uint32_t b = (v >> 16) & 0xFF;
	uint32_t g = (v >> 8) & 0xFF;
	uint32_t r = (v) & 0xFF;
	return (a << 24) | (r << 16) | (g << 8) | (b);
}
inline uint32_t rgba_to_bgra(uint32_t v)
{
	uint32_t r = (v >> 24) & 0xFF;
	uint32_t g = (v >> 16) & 0xFF;
	uint32_t b = (v >> 8) & 0xFF;
	uint32_t a = (v) & 0xFF;
	return (b << 24) | (g << 16) | (r << 8) | (a);
}

inline uint32_t bgra_to_rgba(uint32_t v)
{
	uint32_t b = (v >> 24) & 0xFF;
	uint32_t g = (v >> 16) & 0xFF;
	uint32_t r = (v >> 8) & 0xFF;
	uint32_t a = (v) & 0xFF;
	return (r << 24) | (g << 16) | (b << 8) | (a);
}

inline uint32_t swap_rb(uint32_t v)
{
	uint32_t x = (v >> 16) & 0xFF;
	uint32_t y = v & 0xFF;
	return (v & 0xFF00FF00) | (y << 16) | x;
}

inline uint32_t lerp_color(uint32_t a, uint32_t b, uint32_t factor)
{
	uint32_t af = 256 - factor;
	uint32_t bf = factor;

	// uint32_t abgr -> 0a0g0b0r
	uint32_t al = (a & 0x00ff00ff);
	uint32_t ah = (a & 0xff00ff00) >> 8;
	uint32_t bl = (b & 0x00ff00ff);
	uint32_t bh = (b & 0xff00ff00) >> 8;

	// -> a_g_b_r_
	uint32_t ml = (al * af + bl * bf);
	uint32_t mh = (ah * af + bh * bf);

	// shift & combine back into 32-bit quantity
	uint32_t result = (mh & 0xff00ff00) | ((ml & 0xff00ff00) >> 8);
	return result;
}


} //color
} //math