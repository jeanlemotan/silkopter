#pragma once

namespace q
{
namespace video
{
namespace color
{

	struct A_8
	{
        A_8() = default;
		explicit A_8(uint8_t a) : a(a) {}
        uint8_t a = 0;
	};
	struct I_8
	{
        I_8() = default;
		explicit I_8(uint8_t i) : i(i) {}
        uint8_t i = 0;
	};
	union AI_8
	{
        AI_8() = default;
		explicit AI_8(uint8_t a, uint8_t i) : a(a), i(i) {}
		struct
		{
			uint8_t a;
			uint8_t i;
		};
        uint16_t all = 0;
	};
	union RGBA_4
	{
        RGBA_4() = default;
		explicit RGBA_4(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
		struct 
		{
			uint16_t r : 4;
			uint16_t g : 4;
			uint16_t b : 4;
			uint16_t a : 4;
		};
        uint16_t all = 0;
	};
	union RGB_565
	{
        RGB_565() = default;
		explicit RGB_565(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
		struct 
		{
			uint16_t r : 5;
			uint16_t g : 6;
			uint16_t b : 5;
		};	
        uint16_t all = 0;
	};
	union RGBA_5551
	{
        RGBA_5551() = default;
		explicit RGBA_5551(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
		struct
		{
			uint16_t r : 5;
			uint16_t g : 5;
			uint16_t b : 5;
			uint16_t a : 1;
		};
        uint16_t all = 0;
	};
	struct RGB_8
	{
        RGB_8() = default;
		explicit RGB_8(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
	};
	union RGBA_8
	{
        RGBA_8() = default;
		explicit RGBA_8(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
		struct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
        uint32_t all = 0;
	};
	struct RGB_32
	{
        RGB_32() = default;
		explicit RGB_32(float r, float g, float b) : r(r), g(g), b(b) {}
        float r = 0;
        float g = 0;
        float b = 0;
	};
	struct RGBA_32
	{
        RGBA_32() = default;
		explicit RGBA_32(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
        float r = 0;
        float g = 0;
        float b = 0;
        float a = 0;
	};
	struct I_32
	{
        I_32() = default;
		explicit I_32(float i) : i(i) {}
        float i = 0;
	};
	struct NORMAL_8
	{
        NORMAL_8() = default;
		explicit NORMAL_8(int8_t x, int8_t y, int8_t z) : x(x), y(y), z(z) {}
        int8_t x = 0;
        int8_t y = 0;
        int8_t z = 127;
	};
	struct NORMAL_32
	{
        NORMAL_32() = default;
		explicit NORMAL_32(float x, float y, float z) : x(x), y(y), z(z) {}
        float x = 0;
        float y = 0;
        float z = 1;
	};

	//////////////////////////////////////////////////////////////////////////

	template<class Color_T> struct Traits;
	template<> struct Traits<A_8>
	{
		typedef std::true_type	has_alpha_channel;
		typedef std::false_type has_color_channel;
		typedef std::false_type has_grayscale_channel;
		typedef std::false_type is_hdr;
		typedef std::integral_constant<int, 1> channel_count;
		static char const* get_name() { return "A 8"; }
	};
	template<> struct Traits<I_8>
	{
		typedef std::false_type has_alpha_channel;
		typedef std::false_type	has_color_channel;
		typedef std::true_type	has_grayscale_channel;
		typedef std::false_type is_hdr;
		typedef std::integral_constant<int, 1> channel_count;
		static char const* get_name() { return "I 8"; }
	};
	template<> struct Traits<AI_8>
	{
		typedef std::true_type	has_alpha_channel;
		typedef std::false_type	has_color_channel;
		typedef std::true_type	has_grayscale_channel;
		typedef std::false_type is_hdr;
		typedef std::integral_constant<int, 2> channel_count;
		static char const* get_name() { return "AI 8"; }
	};
	template<> struct Traits<RGBA_4>
	{
		typedef std::true_type	has_alpha_channel;
		typedef std::true_type	has_color_channel;
		typedef std::false_type has_grayscale_channel;
		typedef std::false_type is_hdr;
		typedef std::integral_constant<int, 4> channel_count;
		static char const* get_name() { return "RGBA 4"; }
	};
	template<> struct Traits<RGB_565>
	{
		typedef std::false_type has_alpha_channel;
		typedef std::true_type	has_color_channel;
		typedef std::false_type has_grayscale_channel;
		typedef std::false_type is_hdr;
		typedef std::integral_constant<int, 3> channel_count;
		static char const* get_name() { return "RGB 565"; }
	};
	template<> struct Traits<RGBA_5551>
	{
		typedef std::true_type	has_alpha_channel;
		typedef std::true_type	has_color_channel;
		typedef std::false_type has_grayscale_channel;
		typedef std::false_type is_hdr;
		typedef std::integral_constant<int, 4> channel_count;
		static char const* get_name() { return "RGBA 5551"; }
	};
	template<> struct Traits<RGB_8>
	{
		typedef std::false_type has_alpha_channel;
		typedef std::true_type	has_color_channel;
		typedef std::false_type has_grayscale_channel;
		typedef std::false_type is_hdr;
		typedef std::integral_constant<int, 3> channel_count;
		static char const* get_name() { return "RGB 8"; }
	};
	template<> struct Traits<RGBA_8>
	{
		typedef std::true_type	has_alpha_channel;
		typedef std::true_type	has_color_channel;
		typedef std::false_type has_grayscale_channel;
		typedef std::false_type is_hdr;
		typedef std::integral_constant<int, 4> channel_count;
		static char const* get_name() { return "RGBA 8"; }
	};
	template<> struct Traits<RGB_32>
	{
		typedef std::false_type has_alpha_channel;
		typedef std::true_type	has_color_channel;
		typedef std::false_type has_grayscale_channel;
		typedef std::true_type	is_hdr;
		typedef std::integral_constant<int, 3> channel_count;
		static char const* get_name() { return "RGB 32"; }
	};
	template<> struct Traits<RGBA_32>
	{
		typedef std::true_type has_alpha_channel;
		typedef std::true_type has_color_channel;
		typedef std::false_type has_grayscale_channel;
		typedef std::true_type is_hdr;
		typedef std::integral_constant<int, 4> channel_count;
		static char const* get_name() { return "RGBA 32"; }
	};
	template<> struct Traits<I_32>
	{
		typedef std::false_type has_alpha_channel;
		typedef std::true_type	has_color_channel;
		typedef std::true_type	has_grayscale_channel;
		typedef std::true_type	is_hdr;
		typedef std::integral_constant<int, 1> channel_count;
		static char const* get_name() { return "I 32"; }
	};
	template<> struct Traits<NORMAL_8>
	{
		typedef std::false_type has_alpha_channel;
		typedef std::false_type	has_color_channel;
		typedef std::false_type	has_grayscale_channel;
		typedef std::false_type	is_hdr;
		typedef std::integral_constant<int, 3> channel_count;
		static char const* get_name() { return "NORMAL 8"; }
	};
	template<> struct Traits<NORMAL_32>
	{
		typedef std::false_type has_alpha_channel;
		typedef std::false_type	has_color_channel;
		typedef std::false_type	has_grayscale_channel;
		typedef std::true_type	is_hdr;
		typedef std::integral_constant<int, 3> channel_count;
		static char const* get_name() { return "NORMAL 32"; }
	};


	// some helpers first
	namespace helpers
	{
		inline uint8_t convert_4_1(uint8_t x) { return x >> 3; }
		inline uint8_t convert_8_1(uint8_t x) { return x >> 7; }
		inline uint8_t convert_f_1(float x) { return x > 0.5f ? 1 : 0;}

		inline uint8_t convert_8_4(uint8_t x) { return x >> 4; }
		inline uint8_t convert_5_4(uint8_t x) { return x >> 1; }
		inline uint8_t convert_6_4(uint8_t x) { return x >> 2; }
		inline uint8_t convert_f_4(float x) { return static_cast<uint8_t>(x * 15.f); }

		inline uint8_t convert_4_5(uint8_t x) { return x << 1; }
		inline uint8_t convert_6_5(uint8_t x) { return x >> 1; }
		inline uint8_t convert_8_5(uint8_t x) { return x >> 3; }
		inline uint8_t convert_s8_5(int x) { return static_cast<uint8_t>((x + 128) >> 3); }
		inline uint8_t convert_f_5(float x) { return static_cast<uint8_t>(x * 31.f); }
		inline uint8_t convert_sf_5(float x) { return static_cast<uint8_t>((x + 1.f) * 15.5f); }

		inline uint8_t convert_4_6(uint8_t x) { return x << 2; }
		inline uint8_t convert_5_6(uint8_t x) { return x << 1; }
		inline uint8_t convert_8_6(uint8_t x) { return x >> 2; }
		inline uint8_t convert_s8_6(int x) { return static_cast<uint8_t>((x + 128) >> 2); }
		inline uint8_t convert_f_6(float x) { return static_cast<uint8_t>(x * 63.f); }
		inline uint8_t convert_sf_6(float x) { return static_cast<uint8_t>((x + 1.f) * 31.5f); }

		inline uint8_t convert_1_8(uint8_t x) { return x ? 0xFF : 0; }
		inline uint8_t convert_1_4(uint8_t x) { return x ? 0xF : 0; }
		inline uint8_t convert_4_8(uint8_t x) { return static_cast<uint8_t>(x*272 >> 4); }
		inline uint8_t convert_5_8(uint8_t x) { return static_cast<uint8_t>(x*264 >> 5); }
		inline uint8_t convert_6_8(uint8_t x) { return static_cast<uint8_t>(x*260 >> 6); }
		inline uint8_t convert_s8_8(int x) { return static_cast<uint8_t>(x + 128); }
		inline uint8_t convert_f_8(float x) { return static_cast<uint8_t>(x * 255.f); }
		inline uint8_t convert_sf_8(float x) { return static_cast<uint8_t>((x + 1.f) * 127.5f); }

		inline int8_t convert_5_s8(uint8_t x) { return static_cast<uint8_t>((x*264 >> 5) - 128); }
		inline int8_t convert_6_s8(uint8_t x) { return static_cast<uint8_t>((x*260 >> 6) - 128); }
		inline int8_t convert_8_s8(uint8_t x) { return static_cast<uint8_t>(x - 128); }
		inline int8_t convert_f_s8(float x) { return static_cast<uint8_t>(x * 255.f) - 128; }
		inline int8_t convert_sf_s8(float x) { return static_cast<uint8_t>(x * 127.f); }

		inline float convert_1_f(uint8_t x) { return x ? 1.f : 0.f; }
		inline float convert_4_f(uint8_t x) { return static_cast<float>(x) * 0.066666667f; }
		inline float convert_5_f(uint8_t x) { return static_cast<float>(x) * 0.032258064516129f; }
		inline float convert_6_f(uint8_t x) { return static_cast<float>(x) * 0.015873015873016f; }
		inline float convert_8_f(uint8_t x) { return static_cast<float>(x) * 0.003921568627451f; }
		inline float convert_s8_f(int x) { return static_cast<float>(x + 128) * 0.003921568627451f; }
		inline float convert_sf_f(float x) { return (x + 1.f) * 0.5f; }

		inline float convert_5_sf(uint8_t x) { return static_cast<float>(x) * 0.064516129032258f - 1.f; }
		inline float convert_6_sf(uint8_t x) { return static_cast<float>(x) * 0.031746031746032f - 1.f; }
		inline float convert_8_sf(uint8_t x) { return static_cast<float>(x) * 0.007843137254902f - 1.f; }
		inline float convert_s8_sf(int x) { return static_cast<float>(x) * 0.007843137254902f; }
		inline float convert_f_sf(float x) { return x * 2.f - 1.f; }

		inline uint8_t grayscale_8(uint8_t r, uint8_t g, uint8_t b) { return ((r*76) >> 8) + ((g*150) >> 8) + ((b*29) >> 8); }
		inline uint8_t grayscale_4(uint8_t r, uint8_t g, uint8_t b) { return ((r*76) >> 4) + ((g*150) >> 4) + ((b*29) >> 4); }
		inline uint8_t grayscale_565(uint8_t r, uint8_t g, uint8_t b) { return ((r*76) >> 5) + ((g*150) >> 6) + ((b*29) >> 5); }
		inline uint8_t grayscale_555(uint8_t r, uint8_t g, uint8_t b) { return ((r*76) >> 5) + ((g*150) >> 5) + ((b*29) >> 5); }
		inline float grayscale_f(float r, float g, float b) { return r*0.222f + g*0.707f + b*0.071f; }
	}

	//////////////////////////////////////////////////////////////////////////

	template<class Color_T> struct Ratio
	{
		Ratio(float f) : ratio(static_cast<int>(f * 256.f)) {}
		int ratio;
	};
	template<> struct Ratio<RGB_32>
	{
        explicit Ratio(float f) : ratio(f) {}
		float ratio;
	};
	template<> struct Ratio<RGBA_32>
	{
        explicit Ratio(float f) : ratio(f) {}
		float ratio;
	};
	template<> struct Ratio<I_32>
	{
        explicit Ratio(float f) : ratio(f) {}
		float ratio;
	};
	template<> struct Ratio<NORMAL_32>
	{
        explicit Ratio(float f) : ratio(f) {}
		float ratio;
	};

	template<class Color_T> auto lerp(Color_T a, Color_T b, Ratio<Color_T> t) -> Color_T;
	template<> inline auto lerp<A_8>(A_8 a, A_8 b, Ratio<A_8> t) -> A_8
	{
		auto it = 256 - t.ratio;
		int ra = (a.a*it>>8) + ((b.a*t.ratio)>>8);
		return A_8(static_cast<uint8_t>(ra));
	}
	template<> inline auto lerp<I_8>(I_8 a, I_8 b, Ratio<I_8> t) -> I_8
	{
		auto it = 256 - t.ratio;
		int ri = (a.i*it>>8) + ((b.i*t.ratio)>>8);
		return I_8(static_cast<uint8_t>(ri));
	}
	template<> inline auto lerp<AI_8>(AI_8 a, AI_8 b, Ratio<AI_8> t) -> AI_8
	{
		auto it = 256 - t.ratio;
		int ra = (a.a*it>>8) + ((b.a*t.ratio)>>8);
		int ri = (a.i*it>>8) + ((b.i*t.ratio)>>8);
		return AI_8(static_cast<uint8_t>(ra), static_cast<uint8_t>(ri));
	}
	template<> inline auto lerp<RGBA_4>(RGBA_4 a, RGBA_4 b, Ratio<RGBA_4> t) -> RGBA_4
	{
		auto it = 256 - t.ratio;
		int rr = (a.r*it>>8) + (b.r*t.ratio>>8);
		int rg = (a.g*it>>8) + (b.g*t.ratio>>8);
		int rb = (a.b*it>>8) + (b.b*t.ratio>>8);
		int ra = (a.a*it>>8) + (b.a*t.ratio>>8);
		return RGBA_4(	static_cast<uint8_t>(rr),
						static_cast<uint8_t>(rg),
						static_cast<uint8_t>(rb),
						static_cast<uint8_t>(ra));
	}
	template<> inline auto lerp<RGB_565>(RGB_565 a, RGB_565 b, Ratio<RGB_565> t) -> RGB_565
	{
		auto it = 256 - t.ratio;
		int rr = (a.r*it>>8) + (b.r*t.ratio>>8);
		int rg = (a.g*it>>8) + (b.g*t.ratio>>8);
		int rb = (a.b*it>>8) + (b.b*t.ratio>>8);
		return RGB_565(	static_cast<uint8_t>(rr),
						static_cast<uint8_t>(rg),
						static_cast<uint8_t>(rb));
	}
	template<> inline auto lerp<RGBA_5551>(RGBA_5551 a, RGBA_5551 b, Ratio<RGBA_5551> t) -> RGBA_5551
	{
		auto it = 256 - t.ratio;
		int rr = (a.r*it>>8) + (b.r*t.ratio>>8);
		int rg = (a.g*it>>8) + (b.g*t.ratio>>8);
		int rb = (a.b*it>>8) + (b.b*t.ratio>>8);
		int ra = (a.a*it>>8) + (b.a*t.ratio>>8);
		return RGBA_5551(	static_cast<uint8_t>(rr),
							static_cast<uint8_t>(rg),
							static_cast<uint8_t>(rb),
							static_cast<uint8_t>(ra));
	}
	template<> inline auto lerp<RGB_8>(RGB_8 a, RGB_8 b, Ratio<RGB_8> t) -> RGB_8
	{
		auto it = 256 - t.ratio;
		int rr = (a.r*it>>8) + (b.r*t.ratio>>8);
		int rg = (a.g*it>>8) + (b.g*t.ratio>>8);
		int rb = (a.b*it>>8) + (b.b*t.ratio>>8);
		return RGB_8(	static_cast<uint8_t>(rr),
						static_cast<uint8_t>(rg),
						static_cast<uint8_t>(rb));
	}
	template<> inline auto lerp<RGBA_8>(RGBA_8 a, RGBA_8 b, Ratio<RGBA_8> t) -> RGBA_8
	{
		auto it = 256 - t.ratio;
		int rr = (a.r*it>>8) + (b.r*t.ratio>>8);
		int rg = (a.g*it>>8) + (b.g*t.ratio>>8);
		int rb = (a.b*it>>8) + (b.b*t.ratio>>8);
		int ra = (a.a*it>>8) + (b.a*t.ratio>>8);
		return RGBA_8(	static_cast<uint8_t>(rr),
						static_cast<uint8_t>(rg),
						static_cast<uint8_t>(rb),
						static_cast<uint8_t>(ra));
	}
	template<> inline auto lerp<RGB_32>(RGB_32 a, RGB_32 b, Ratio<RGB_32> t) -> RGB_32
	{
		auto it = 1.f - t.ratio;
		return RGB_32((a.r*it) + (b.r*t.ratio),
			(a.g*it) + (b.g*t.ratio),
			(a.b*it) + (b.b*t.ratio));
	}
	template<> inline auto lerp<RGBA_32>(RGBA_32 a, RGBA_32 b, Ratio<RGBA_32> t) -> RGBA_32
	{
		auto it = 1.f - t.ratio;
		return RGBA_32((a.r*it) + (b.r*t.ratio),
			(a.g*it) + (b.g*t.ratio),
			(a.b*it) + (b.b*t.ratio),
			(a.a*it) + (b.a*t.ratio));
	}
	template<> inline auto lerp<I_32>(I_32 a, I_32 b, Ratio<I_32> t) -> I_32
	{
		auto it = 1.f - t.ratio;
		return I_32((a.i*it) + (b.i*t.ratio));
	}

	template<> inline auto lerp<NORMAL_8>(NORMAL_8 a, NORMAL_8 b, Ratio<NORMAL_8> t) -> NORMAL_8
	{
		auto it = 256 - t.ratio;
		int rx = (a.x*it>>8) + (b.x*t.ratio>>8);
		int ry = (a.y*it>>8) + (b.y*t.ratio>>8);
		int rz = (a.z*it>>8) + (b.z*t.ratio>>8);
		NORMAL_8 res(	static_cast<int8_t>(rx),
						static_cast<int8_t>(ry),
						static_cast<int8_t>(rx));

		//normalize
		int length_sq = rx*rx + ry*ry + rz*rz;
		if (length_sq > 0)
		{
			auto fl = static_cast<float>(length_sq) / 16129.f; //(127*127)
			auto i = 1.f / std::sqrt(fl);
			res.x = static_cast<int8_t>(static_cast<float>(rx) * i);
			res.y = static_cast<int8_t>(static_cast<float>(ry) * i);
			res.z = static_cast<int8_t>(static_cast<float>(rz) * i);
		}
		return res;
	}

	template<> inline auto lerp<NORMAL_32>(NORMAL_32 a, NORMAL_32 b, Ratio<NORMAL_32> t) -> NORMAL_32
	{
		auto it = 1.f - t.ratio;
		NORMAL_32 res((a.x*it) + (b.x*t.ratio),
			(a.y*it) + (b.y*t.ratio),
			(a.z*it) + (b.z*t.ratio));

		//normalize
		float length_sq = res.x*res.x + res.y*res.y + res.z*res.z;
		if (length_sq > 0.f)
		{
			float i = 1.f / std::sqrt(length_sq);
			res.x *= i;
			res.y *= i;
			res.z *= i;
		}
		return res;
	}


	//conversion routines

	//////////////////////////////////////////////////////////////////////////
	template<class Dst_Color_T> Dst_Color_T convert_to(A_8 x);
	template<> inline A_8		convert_to<A_8>			(A_8 x) { return x; }
	template<> inline I_8		convert_to<I_8>			(A_8  ) { return I_8(); }
	template<> inline AI_8		convert_to<AI_8>		(A_8 x) { return AI_8(x.a, 0); }
	template<> inline RGBA_4	convert_to<RGBA_4>		(A_8 x) { RGBA_4 y; y.a = helpers::convert_8_4(x.a); return y; }
	template<> inline RGB_565	convert_to<RGB_565>		(A_8  ) { return RGB_565(); }
	template<> inline RGBA_5551 convert_to<RGBA_5551>	(A_8 x) { RGBA_5551 y; y.a = helpers::convert_8_1(x.a); return y; }
	template<> inline RGB_8		convert_to<RGB_8>		(A_8  ) { return RGB_8(); }
	template<> inline RGBA_8	convert_to<RGBA_8>		(A_8 x) { RGBA_8 y; y.a = x.a; return y; }
	template<> inline RGB_32	convert_to<RGB_32>		(A_8  ) { return RGB_32(); }
	template<> inline RGBA_32	convert_to<RGBA_32>		(A_8 x) { return RGBA_32(0.f, 0.f, 0.f, helpers::convert_8_f(x.a)); }
	template<> inline I_32		convert_to<I_32>		(A_8  ) { return I_32(); }

	//////////////////////////////////////////////////////////////////////////
	template<class Dst_Color_T> Dst_Color_T convert_to(I_8 x);
	template<> inline A_8		convert_to<A_8>			(I_8  ) { return A_8(0xFF); }
	template<> inline I_8		convert_to<I_8>			(I_8 x) { return x; }
	template<> inline AI_8		convert_to<AI_8>		(I_8 x) { return AI_8(0xFF, x.i); }
	template<> inline RGBA_4	convert_to<RGBA_4>		(I_8 x) { uint8_t i = helpers::convert_8_4(x.i); return RGBA_4(i, i, i, 0xF); }
	template<> inline RGB_565	convert_to<RGB_565>		(I_8 x) { uint8_t i = helpers::convert_8_5(x.i); return RGB_565(i, helpers::convert_8_6(x.i), i); }
	template<> inline RGBA_5551 convert_to<RGBA_5551>	(I_8 x) { uint8_t i = helpers::convert_8_5(x.i); return RGBA_5551(i, i, i, 1); }
	template<> inline RGB_8		convert_to<RGB_8>		(I_8 x) { return RGB_8(x.i, x.i, x.i); }
	template<> inline RGBA_8	convert_to<RGBA_8>		(I_8 x) { return RGBA_8(x.i, x.i, x.i, 0xFF); }
	template<> inline RGB_32	convert_to<RGB_32>		(I_8 x) { float i = helpers::convert_8_f(x.i); return RGB_32(i, i, i); }
	template<> inline RGBA_32	convert_to<RGBA_32>		(I_8 x) { float i = helpers::convert_8_f(x.i); return RGBA_32(i, i, i, 1.f); }
	template<> inline I_32		convert_to<I_32>		(I_8 x) { return I_32(helpers::convert_8_f(x.i)); }

	//////////////////////////////////////////////////////////////////////////
	template<class Dst_Color_T> Dst_Color_T convert_to(AI_8 x);
	template<> inline A_8		convert_to<A_8>			(AI_8 x) { return A_8(x.a); }
	template<> inline I_8		convert_to<I_8>			(AI_8 x) { return I_8(x.i); }
	template<> inline AI_8		convert_to<AI_8>		(AI_8 x) { return x; }
	template<> inline RGBA_4	convert_to<RGBA_4>		(AI_8 x) { uint8_t i = helpers::convert_8_4(x.i); return RGBA_4(i, i, i, helpers::convert_8_4(x.a)); }
	template<> inline RGB_565	convert_to<RGB_565>		(AI_8 x) { uint8_t i = helpers::convert_8_5(x.i); return RGB_565(i, helpers::convert_8_6(x.i), i); }
	template<> inline RGBA_5551 convert_to<RGBA_5551>	(AI_8 x) { uint8_t i = helpers::convert_8_5(x.i); return RGBA_5551(i, i, i, helpers::convert_8_1(x.a)); }
	template<> inline RGB_8		convert_to<RGB_8>		(AI_8 x) { return RGB_8(x.i, x.i, x.i); }
	template<> inline RGBA_8	convert_to<RGBA_8>		(AI_8 x) { return RGBA_8(x.i, x.i, x.i, x.a); }
	template<> inline RGB_32	convert_to<RGB_32>		(AI_8 x) { float i = helpers::convert_8_f(x.i); return RGB_32(i, i, i); }
	template<> inline RGBA_32	convert_to<RGBA_32>		(AI_8 x) { float i = helpers::convert_8_f(x.i); return RGBA_32(i, i, i, helpers::convert_8_f(x.a)); }
	template<> inline I_32		convert_to<I_32>		(AI_8 x) { return I_32(helpers::convert_8_f(x.i)); }

	//////////////////////////////////////////////////////////////////////////
	template<class Dst_Color_T> Dst_Color_T convert_to(RGBA_4 x);
	template<> inline A_8		convert_to<A_8>			(RGBA_4 x) { return A_8(helpers::convert_4_8(x.a)); }
	template<> inline I_8		convert_to<I_8>			(RGBA_4 x) { return I_8(helpers::grayscale_4(x.r, x.g, x.b)); }
	template<> inline AI_8		convert_to<AI_8>		(RGBA_4 x) { return AI_8(helpers::convert_4_8(x.a), helpers::grayscale_4(x.r, x.g, x.b)); }
	template<> inline RGBA_4	convert_to<RGBA_4>		(RGBA_4 x) { return x; }
	template<> inline RGB_565	convert_to<RGB_565>		(RGBA_4 x) { return RGB_565(helpers::convert_4_5(x.r), helpers::convert_4_6(x.g), helpers::convert_4_5(x.b)); }
	template<> inline RGBA_5551 convert_to<RGBA_5551>	(RGBA_4 x) { return RGBA_5551(helpers::convert_4_5(x.r), helpers::convert_4_5(x.g), helpers::convert_4_5(x.b), helpers::convert_4_1(x.a)); }
	template<> inline RGB_8		convert_to<RGB_8>		(RGBA_4 x) { return RGB_8(helpers::convert_4_8(x.r), helpers::convert_4_8(x.g), helpers::convert_4_8(x.b)); }
	template<> inline RGBA_8	convert_to<RGBA_8>		(RGBA_4 x) { return RGBA_8(helpers::convert_4_8(x.r), helpers::convert_4_8(x.g), helpers::convert_4_8(x.b), helpers::convert_4_8(x.a)); }
	template<> inline RGB_32	convert_to<RGB_32>		(RGBA_4 x) { return RGB_32(helpers::convert_4_f(x.r), helpers::convert_4_f(x.g), helpers::convert_4_f(x.b)); }
	template<> inline RGBA_32	convert_to<RGBA_32>		(RGBA_4 x) { return RGBA_32(helpers::convert_4_f(x.r), helpers::convert_4_f(x.g), helpers::convert_4_f(x.b), helpers::convert_4_f(x.a)); }
	template<> inline I_32		convert_to<I_32>		(RGBA_4 x) { return I_32(helpers::convert_8_f(helpers::grayscale_4(x.r, x.g, x.b))); }

	//////////////////////////////////////////////////////////////////////////
	template<class Dst_Color_T> Dst_Color_T convert_to(RGB_565 x);
	template<> inline A_8		convert_to<A_8>			(RGB_565  ) { return A_8(0xFF); }
	template<> inline I_8		convert_to<I_8>			(RGB_565 x) { return I_8(helpers::grayscale_565(x.r, x.g, x.b)); }
	template<> inline AI_8		convert_to<AI_8>		(RGB_565 x) { return AI_8(0xFF, helpers::grayscale_565(x.r, x.g, x.b)); }
	template<> inline RGBA_4	convert_to<RGBA_4>		(RGB_565 x) { return RGBA_4(helpers::convert_5_4(x.r), helpers::convert_6_4(x.g), helpers::convert_5_4(x.b), 0xF); }
	template<> inline RGB_565	convert_to<RGB_565>		(RGB_565 x) { return x; }
	template<> inline RGBA_5551 convert_to<RGBA_5551>	(RGB_565 x) { return RGBA_5551(x.r, helpers::convert_6_5(x.g), x.b, 1); }
	template<> inline RGB_8		convert_to<RGB_8>		(RGB_565 x) { return RGB_8(helpers::convert_5_8(x.r), helpers::convert_6_8(x.g), helpers::convert_5_8(x.b)); }
	template<> inline RGBA_8	convert_to<RGBA_8>		(RGB_565 x) { return RGBA_8(helpers::convert_5_8(x.r), helpers::convert_6_8(x.g), helpers::convert_5_8(x.b), 0xFF); }
	template<> inline RGB_32	convert_to<RGB_32>		(RGB_565 x) { return RGB_32(helpers::convert_5_f(x.r), helpers::convert_6_f(x.g), helpers::convert_5_f(x.b)); }
	template<> inline RGBA_32	convert_to<RGBA_32>		(RGB_565 x) { return RGBA_32(helpers::convert_5_f(x.r), helpers::convert_6_f(x.g), helpers::convert_5_f(x.b), 1.f); }
	template<> inline I_32		convert_to<I_32>		(RGB_565 x) { return I_32(helpers::convert_8_f(helpers::grayscale_565(x.r, x.g, x.b))); }
	template<> inline NORMAL_8	convert_to<NORMAL_8>	(RGB_565 x) { return NORMAL_8(helpers::convert_5_s8(x.r), helpers::convert_6_s8(x.g), helpers::convert_5_s8(x.b)); }
	template<> inline NORMAL_32	convert_to<NORMAL_32>	(RGB_565 x) { return NORMAL_32(helpers::convert_5_sf(x.r), helpers::convert_6_sf(x.g), helpers::convert_5_sf(x.b)); }

	//////////////////////////////////////////////////////////////////////////
	template<class Dst_Color_T> Dst_Color_T convert_to(RGBA_5551 x);
	template<> inline A_8		convert_to<A_8>			(RGBA_5551 x) { return A_8(helpers::convert_1_8(x.a)); }
	template<> inline I_8		convert_to<I_8>			(RGBA_5551 x) { return I_8(helpers::grayscale_555(x.r, x.g, x.b)); }
	template<> inline AI_8		convert_to<AI_8>		(RGBA_5551 x) { return AI_8(helpers::convert_1_8(x.a), helpers::grayscale_555(x.r, x.g, x.b)); }
	template<> inline RGBA_4	convert_to<RGBA_4>		(RGBA_5551 x) { return RGBA_4(helpers::convert_5_4(x.r), helpers::convert_5_4(x.g), helpers::convert_5_4(x.b), helpers::convert_1_4(x.a)); }
	template<> inline RGB_565	convert_to<RGB_565>		(RGBA_5551 x) { return RGB_565(x.r, helpers::convert_5_6(x.g), x.b); }
	template<> inline RGBA_5551 convert_to<RGBA_5551>	(RGBA_5551 x) { return x; }
	template<> inline RGB_8		convert_to<RGB_8>		(RGBA_5551 x) { return RGB_8(helpers::convert_5_8(x.r), helpers::convert_5_8(x.g), helpers::convert_5_8(x.b)); }
	template<> inline RGBA_8	convert_to<RGBA_8>		(RGBA_5551 x) { return RGBA_8(helpers::convert_5_8(x.r), helpers::convert_5_8(x.g), helpers::convert_5_8(x.b), helpers::convert_1_8(x.a)); }
	template<> inline RGB_32	convert_to<RGB_32>		(RGBA_5551 x) { return RGB_32(helpers::convert_5_f(x.r), helpers::convert_5_f(x.g), helpers::convert_5_f(x.b)); }
	template<> inline RGBA_32	convert_to<RGBA_32>		(RGBA_5551 x) { return RGBA_32(helpers::convert_5_f(x.r), helpers::convert_5_f(x.g), helpers::convert_5_f(x.b), helpers::convert_1_f(x.a)); }
	template<> inline I_32		convert_to<I_32>		(RGBA_5551 x) { return I_32(helpers::convert_8_f(helpers::grayscale_555(x.r, x.g, x.b))); }

	//////////////////////////////////////////////////////////////////////////
	template<class Dst_Color_T> Dst_Color_T convert_to(RGB_8 x);
	template<> inline A_8		convert_to<A_8>			(RGB_8  ) { return A_8(0xFF); }
	template<> inline I_8		convert_to<I_8>			(RGB_8 x) { return I_8(helpers::grayscale_8(x.r, x.g, x.b)); }
	template<> inline AI_8		convert_to<AI_8>		(RGB_8 x) { return AI_8(0xFF, helpers::grayscale_8(x.r, x.g, x.b)); }
	template<> inline RGBA_4	convert_to<RGBA_4>		(RGB_8 x) { return RGBA_4(helpers::convert_8_4(x.r), helpers::convert_8_4(x.g), helpers::convert_8_4(x.b), 0xF); }
	template<> inline RGB_565	convert_to<RGB_565>		(RGB_8 x) { return RGB_565(helpers::convert_8_5(x.r), helpers::convert_8_6(x.g), helpers::convert_8_5(x.b)); }
	template<> inline RGBA_5551 convert_to<RGBA_5551>	(RGB_8 x) { return RGBA_5551(helpers::convert_8_5(x.r), helpers::convert_8_5(x.g), helpers::convert_8_5(x.b), 1); }
	template<> inline RGB_8		convert_to<RGB_8>		(RGB_8 x) { return x; }
	template<> inline RGBA_8	convert_to<RGBA_8>		(RGB_8 x) { return RGBA_8(x.r, x.g, x.b, 0xFF); }
	template<> inline RGB_32	convert_to<RGB_32>		(RGB_8 x) { return RGB_32(helpers::convert_8_f(x.r), helpers::convert_8_f(x.g), helpers::convert_8_f(x.b)); }
	template<> inline RGBA_32	convert_to<RGBA_32>		(RGB_8 x) { return RGBA_32(helpers::convert_8_f(x.r), helpers::convert_8_f(x.g), helpers::convert_8_f(x.b), 1.f); }
	template<> inline I_32		convert_to<I_32>		(RGB_8 x) { return I_32(helpers::convert_8_f(helpers::grayscale_8(x.r, x.g, x.b))); }
	template<> inline NORMAL_8	convert_to<NORMAL_8>	(RGB_8 x) { return NORMAL_8(helpers::convert_8_s8(x.r), helpers::convert_8_s8(x.g), helpers::convert_8_s8(x.b)); }
	template<> inline NORMAL_32	convert_to<NORMAL_32>	(RGB_8 x) { return NORMAL_32(helpers::convert_8_sf(x.r), helpers::convert_8_sf(x.g), helpers::convert_8_sf(x.b)); }

	//////////////////////////////////////////////////////////////////////////
	template<class Dst_Color_T> Dst_Color_T convert_to(RGBA_8 x);
	template<> inline A_8		convert_to<A_8>			(RGBA_8 x) { return A_8(x.a); }
	template<> inline I_8		convert_to<I_8>			(RGBA_8 x) { return I_8(helpers::grayscale_8(x.r, x.g, x.b)); }
	template<> inline AI_8		convert_to<AI_8>		(RGBA_8 x) { return AI_8(x.a, helpers::grayscale_8(x.r, x.g, x.b)); }
	template<> inline RGBA_4	convert_to<RGBA_4>		(RGBA_8 x) { return RGBA_4(helpers::convert_8_4(x.r), helpers::convert_8_4(x.g), helpers::convert_8_4(x.b), helpers::convert_8_4(x.a)); }
	template<> inline RGB_565	convert_to<RGB_565>		(RGBA_8 x) { return RGB_565(helpers::convert_8_5(x.r), helpers::convert_8_6(x.g), helpers::convert_8_5(x.b)); }
	template<> inline RGBA_5551 convert_to<RGBA_5551>	(RGBA_8 x) { return RGBA_5551(helpers::convert_8_5(x.r), helpers::convert_8_5(x.g), helpers::convert_8_5(x.b), helpers::convert_8_1(x.a)); }
	template<> inline RGB_8		convert_to<RGB_8>		(RGBA_8 x) { return RGB_8(x.r, x.g, x.b); }
	template<> inline RGBA_8	convert_to<RGBA_8>		(RGBA_8 x) { return x; }
	template<> inline RGB_32	convert_to<RGB_32>		(RGBA_8 x) { return RGB_32(helpers::convert_8_f(x.r), helpers::convert_8_f(x.g), helpers::convert_8_f(x.b)); }
	template<> inline RGBA_32	convert_to<RGBA_32>		(RGBA_8 x) { return RGBA_32(helpers::convert_8_f(x.r), helpers::convert_8_f(x.g), helpers::convert_8_f(x.b), helpers::convert_8_f(x.a)); }
	template<> inline I_32		convert_to<I_32>		(RGBA_8 x) { return I_32(helpers::convert_8_f(helpers::grayscale_8(x.r, x.g, x.b))); }

	//////////////////////////////////////////////////////////////////////////
	template<class Dst_Color_T> Dst_Color_T convert_to(RGB_32 x);
	template<> inline A_8		convert_to<A_8>			(RGB_32  ) { return A_8(0xFF); }
	template<> inline I_8		convert_to<I_8>			(RGB_32 x) { return I_8(helpers::convert_f_8(helpers::grayscale_f(x.r, x.g, x.b))); }
	template<> inline AI_8		convert_to<AI_8>		(RGB_32 x) { return AI_8(0xFF, helpers::convert_f_8(helpers::grayscale_f(x.r, x.g, x.b))); }
	template<> inline RGBA_4	convert_to<RGBA_4>		(RGB_32 x) { return RGBA_4(helpers::convert_f_4(x.r), helpers::convert_f_4(x.g), helpers::convert_f_4(x.b), 0xF); }
	template<> inline RGB_565	convert_to<RGB_565>		(RGB_32 x) { return RGB_565(helpers::convert_f_5(x.r), helpers::convert_f_6(x.g), helpers::convert_f_5(x.b)); }
	template<> inline RGBA_5551 convert_to<RGBA_5551>	(RGB_32 x) { return RGBA_5551(helpers::convert_f_5(x.r), helpers::convert_f_5(x.g), helpers::convert_f_5(x.b), 1); }
	template<> inline RGB_8		convert_to<RGB_8>		(RGB_32 x) { return RGB_8(helpers::convert_f_8(x.r), helpers::convert_f_8(x.g), helpers::convert_f_8(x.b)); }
	template<> inline RGBA_8	convert_to<RGBA_8>		(RGB_32 x) { return RGBA_8(helpers::convert_f_8(x.r), helpers::convert_f_8(x.g), helpers::convert_f_8(x.b), 0xFF); }
	template<> inline RGB_32	convert_to<RGB_32>		(RGB_32 x) { return x; }
	template<> inline RGBA_32	convert_to<RGBA_32>		(RGB_32 x) { return RGBA_32(x.r, x.g, x.b, 1.f); }
	template<> inline I_32		convert_to<I_32>		(RGB_32 x) { return I_32(helpers::grayscale_f(x.r, x.g, x.b)); }
	template<> inline NORMAL_8	convert_to<NORMAL_8>	(RGB_32 x) { return NORMAL_8(helpers::convert_f_s8(x.r), helpers::convert_f_s8(x.g), helpers::convert_f_s8(x.b)); }
	template<> inline NORMAL_32	convert_to<NORMAL_32>	(RGB_32 x) { return NORMAL_32(helpers::convert_f_sf(x.r), helpers::convert_f_sf(x.g), helpers::convert_f_sf(x.b)); }

	//////////////////////////////////////////////////////////////////////////
	template<class Dst_Color_T> Dst_Color_T convert_to(RGBA_32 x);
	template<> inline A_8		convert_to<A_8>			(RGBA_32 x) { return A_8(helpers::convert_f_8(x.a)); }
	template<> inline I_8		convert_to<I_8>			(RGBA_32 x) { return I_8(helpers::convert_f_8(helpers::grayscale_f(x.r, x.g, x.b))); }
	template<> inline AI_8		convert_to<AI_8>		(RGBA_32 x) { return AI_8(helpers::convert_f_8(x.a), helpers::convert_f_8(helpers::grayscale_f(x.r, x.g, x.b))); }
	template<> inline RGBA_4	convert_to<RGBA_4>		(RGBA_32 x) { return RGBA_4(helpers::convert_f_4(x.r), helpers::convert_f_4(x.g), helpers::convert_f_4(x.b), helpers::convert_f_4(x.a)); }
	template<> inline RGB_565	convert_to<RGB_565>		(RGBA_32 x) { return RGB_565(helpers::convert_f_5(x.r), helpers::convert_f_6(x.g), helpers::convert_f_5(x.b)); }
	template<> inline RGBA_5551 convert_to<RGBA_5551>	(RGBA_32 x) { return RGBA_5551(helpers::convert_f_5(x.r), helpers::convert_f_5(x.g), helpers::convert_f_5(x.b), helpers::convert_f_1(x.a)); }
	template<> inline RGB_8		convert_to<RGB_8>		(RGBA_32 x) { return RGB_8(helpers::convert_f_8(x.r), helpers::convert_f_8(x.g), helpers::convert_f_8(x.b)); }
	template<> inline RGBA_8	convert_to<RGBA_8>		(RGBA_32 x) { return RGBA_8(helpers::convert_f_8(x.r), helpers::convert_f_8(x.g), helpers::convert_f_8(x.b), helpers::convert_f_8(x.a)); }
	template<> inline RGB_32	convert_to<RGB_32>		(RGBA_32 x) { return RGB_32(x.r, x.g, x.b); }
	template<> inline RGBA_32	convert_to<RGBA_32>		(RGBA_32 x) { return x; }
	template<> inline I_32		convert_to<I_32>		(RGBA_32 x) { return I_32(helpers::grayscale_f(x.r, x.g, x.b)); }

	//////////////////////////////////////////////////////////////////////////
	template<class Dst_Color_T> Dst_Color_T convert_to(I_32 x);
	template<> inline A_8		convert_to<A_8>			(I_32  ) { return A_8(0xFF); }
	template<> inline I_8		convert_to<I_8>			(I_32 x) { return I_8(helpers::convert_f_8(x.i)); }
	template<> inline AI_8		convert_to<AI_8>		(I_32 x) { return AI_8(0xFF, helpers::convert_f_8(x.i)); }
	template<> inline RGBA_4	convert_to<RGBA_4>		(I_32 x) { uint8_t i = helpers::convert_f_4(x.i); return RGBA_4(i, i, i, 0xF); }
	template<> inline RGB_565	convert_to<RGB_565>		(I_32 x) { uint8_t i = helpers::convert_f_5(x.i); return RGB_565(i, helpers::convert_f_6(x.i), i); }
	template<> inline RGBA_5551 convert_to<RGBA_5551>	(I_32 x) { uint8_t i = helpers::convert_f_5(x.i); return RGBA_5551(i, i, i, 1); }
	template<> inline RGB_8		convert_to<RGB_8>		(I_32 x) { uint8_t i = helpers::convert_f_8(x.i); return RGB_8(i, i, i); }
	template<> inline RGBA_8	convert_to<RGBA_8>		(I_32 x) { uint8_t i = helpers::convert_f_8(x.i); return RGBA_8(i, i, i, 0xFF); }
	template<> inline RGB_32	convert_to<RGB_32>		(I_32 x) { return RGB_32(x.i, x.i, x.i); }
	template<> inline RGBA_32	convert_to<RGBA_32>		(I_32 x) { return RGBA_32(x.i, x.i, x.i, 1.f); }
	template<> inline I_32		convert_to<I_32>		(I_32 x) { return x; }

	//////////////////////////////////////////////////////////////////////////
	template<class Dst_Color_T> Dst_Color_T convert_to(NORMAL_8 x);
	template<> inline RGB_565	convert_to<RGB_565>		(NORMAL_8 x) { return RGB_565(helpers::convert_s8_5(x.x), helpers::convert_s8_6(x.y), helpers::convert_s8_5(x.z)); }
	template<> inline RGB_8		convert_to<RGB_8>		(NORMAL_8 x) { return RGB_8(helpers::convert_s8_8(x.x), helpers::convert_s8_8(x.y), helpers::convert_s8_8(x.z)); }
	template<> inline RGB_32	convert_to<RGB_32>		(NORMAL_8 x) { return RGB_32(helpers::convert_s8_f(x.x), helpers::convert_s8_f(x.y), helpers::convert_s8_f(x.z)); }
	template<> inline NORMAL_8	convert_to<NORMAL_8>	(NORMAL_8 x) { return x; }
	template<> inline NORMAL_32	convert_to<NORMAL_32>	(NORMAL_8 x) { return NORMAL_32(helpers::convert_s8_sf(x.x), helpers::convert_s8_sf(x.y), helpers::convert_s8_sf(x.z)); }

	//////////////////////////////////////////////////////////////////////////
	template<class Dst_Color_T> Dst_Color_T convert_to(NORMAL_32 x);
	template<> inline RGB_565	convert_to<RGB_565>		(NORMAL_32 x) { return RGB_565(helpers::convert_sf_5(x.x), helpers::convert_sf_6(x.y), helpers::convert_sf_5(x.z)); }
	template<> inline RGB_8		convert_to<RGB_8>		(NORMAL_32 x) { return RGB_8(helpers::convert_sf_8(x.x), helpers::convert_sf_8(x.y), helpers::convert_sf_8(x.z)); }
	template<> inline RGB_32	convert_to<RGB_32>		(NORMAL_32 x) { return RGB_32(helpers::convert_sf_f(x.x), helpers::convert_sf_f(x.y), helpers::convert_sf_f(x.z)); }
	template<> inline NORMAL_8	convert_to<NORMAL_8>	(NORMAL_32 x) { return NORMAL_8(helpers::convert_sf_s8(x.x), helpers::convert_sf_s8(x.y), helpers::convert_sf_s8(x.z)); }
	template<> inline NORMAL_32	convert_to<NORMAL_32>	(NORMAL_32 x) { return x; }

}
}
}
