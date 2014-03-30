namespace math
{


//////////////////////////////////////////////////////////////////////////
// scalars

template<>
inline float degrees(float const& radians)
{
	return radians * (180.0f / anglef::pi.radians);
}
template<>
inline float radians(float const& degrees)
{
	return degrees * anglef::pi.radians / 180.f;
}
template<typename T>
inline T degrees(T const& radians)
{
	return radians * T(180) / angle<T>::pi.radians;
}
template<typename T>
inline T radians(T const& degrees)
{
	return degrees * angle<T>::pi.radians / T(180);
}

//////////////////////////////////////////////////////////////////////////

namespace detail
{
	//one version of fast_sin - around 1100 cycles on atmega2560
	//max_error = 0.001091, avg_error = 0.000505
// 	inline float _fast_sin(float x)
// 	{
// 		float res = x * (1.27323954f - 0.405284735f * math::abs(x));
// 		return res * (.225f * (math::abs(res) - 1.f) + 1.f);
// 	}
// 
// 	inline float fast_sin(float x)
// 	{
// 		//always wrap input angle to -PI..PI
// 		if (is_negative(x) && x < -3.14159266f)
// 		{
// 			do { x += 6.28318531f; } while (x < -3.14159266f);
// 		}
// 		else if (x > 3.14159266f)
// 		{
// 			do { x -= 6.28318531f; } while (x > 3.14159266f);
// 		}
// 		return _fast_sin(x);
// 	}
// 	inline float fast_cos(float x)
// 	{
// 		return fast_sin(x + 1.5707963268f);
// 	}
// 	inline void fast_sin_cos(float x, float& s, float& c)
// 	{
// 		//always wrap input angle to -PI..PI
// 		if (is_negative(x) && x < -3.14159266f)
// 		{
// 			do { x += 6.28318531f; } while (x < -3.14159266f);
// 
// 			s = _fast_sin(x);
// 			c = _fast_sin(x + 1.5707963268f); //no need to check for overflow
// 		}
// 		else if (x > 3.14159266f)
// 		{
// 			do { x -= 6.28318531f; } while (x > 3.14159266f);
// 
// 			s = _fast_sin(x);
// 			c = _fast_sin(x + 1.5707963268f); //no need to check for overflow
// 		}
// 		else
// 		{
// 			s = _fast_sin(x);
// 			x += 1.5707963268f;
// 			if (is_positive(x) && x > 3.14159266f)
// 			{
// 				x -= 6.28318531f;
// 			}
// 			c = _fast_sin(x);
// 		}
// 	}

//////////////////////////////////////////////////////////////////////////
// lookup table version: around 600 cycles
	//max_error = 0.000538  avg_error = 0.000347
	
	//mantissa of (sin(x = 0 - pi/2) + 1) + 1
	//the last element is to allow interpolating without checking for out of bounds
	static const uint16_t k_sin16[806] =
	{
		0x0, 0x7f, 0xff, 0x17f, 0x1ff, 0x27f, 0x2ff, 0x37f, 0x3ff, 0x47e, 0x4fe, 0x57e, 0x5fe, 0x67e, 0x6fe, 0x77d,
		0x7fd, 0x87d, 0x8fd, 0x97d, 0x9fc, 0xa7c, 0xafc, 0xb7c, 0xbfc, 0xc7b, 0xcfb, 0xd7b, 0xdfa, 0xe7a, 0xefa, 0xf79,
		0xff9, 0x1079, 0x10f8, 0x1178, 0x11f7, 0x1277, 0x12f7, 0x1376, 0x13f6, 0x1475, 0x14f4, 0x1574, 0x15f3, 0x1673, 0x16f2, 0x1771,
		0x17f1, 0x1870, 0x18ef, 0x196f, 0x19ee, 0x1a6d, 0x1aec, 0x1b6b, 0x1beb, 0x1c6a, 0x1ce9, 0x1d68, 0x1de7, 0x1e66, 0x1ee5, 0x1f64,
		0x1fe3, 0x2061, 0x20e0, 0x215f, 0x21de, 0x225d, 0x22db, 0x235a, 0x23d9, 0x2457, 0x24d6, 0x2554, 0x25d3, 0x2651, 0x26d0, 0x274e,
		0x27cc, 0x284b, 0x28c9, 0x2947, 0x29c5, 0x2a44, 0x2ac2, 0x2b40, 0x2bbe, 0x2c3c, 0x2cba, 0x2d38, 0x2db5, 0x2e33, 0x2eb1, 0x2f2f,
		0x2fac, 0x302a, 0x30a8, 0x3125, 0x31a3, 0x3220, 0x329d, 0x331b, 0x3398, 0x3415, 0x3492, 0x3510, 0x358d, 0x360a, 0x3687, 0x3704,
		0x3780, 0x37fd, 0x387a, 0x38f7, 0x3973, 0x39f0, 0x3a6c, 0x3ae9, 0x3b65, 0x3be2, 0x3c5e, 0x3cda, 0x3d56, 0x3dd3, 0x3e4f, 0x3ecb,
		0x3f47, 0x3fc2, 0x403e, 0x40ba, 0x4136, 0x41b1, 0x422d, 0x42a8, 0x4324, 0x439f, 0x441a, 0x4496, 0x4511, 0x458c, 0x4607, 0x4682,
		0x46fd, 0x4778, 0x47f3, 0x486d, 0x48e8, 0x4962, 0x49dd, 0x4a57, 0x4ad2, 0x4b4c, 0x4bc6, 0x4c40, 0x4cba, 0x4d34, 0x4dae, 0x4e28,
		0x4ea2, 0x4f1b, 0x4f95, 0x500e, 0x5088, 0x5101, 0x517a, 0x51f4, 0x526d, 0x52e6, 0x535f, 0x53d8, 0x5450, 0x54c9, 0x5542, 0x55ba,
		0x5633, 0x56ab, 0x5723, 0x579b, 0x5814, 0x588c, 0x5904, 0x597b, 0x59f3, 0x5a6b, 0x5ae2, 0x5b5a, 0x5bd1, 0x5c49, 0x5cc0, 0x5d37,
		0x5dae, 0x5e25, 0x5e9c, 0x5f13, 0x5f89, 0x6000, 0x6077, 0x60ed, 0x6163, 0x61da, 0x6250, 0x62c6, 0x633c, 0x63b1, 0x6427, 0x649d,
		0x6512, 0x6588, 0x65fd, 0x6672, 0x66e8, 0x675d, 0x67d1, 0x6846, 0x68bb, 0x6930, 0x69a4, 0x6a19, 0x6a8d, 0x6b01, 0x6b75, 0x6be9,
		0x6c5d, 0x6cd1, 0x6d45, 0x6db8, 0x6e2c, 0x6e9f, 0x6f13, 0x6f86, 0x6ff9, 0x706c, 0x70df, 0x7151, 0x71c4, 0x7236, 0x72a9, 0x731b,
		0x738d, 0x73ff, 0x7471, 0x74e3, 0x7555, 0x75c6, 0x7638, 0x76a9, 0x771a, 0x778c, 0x77fd, 0x786e, 0x78de, 0x794f, 0x79c0, 0x7a30,
		0x7aa0, 0x7b10, 0x7b81, 0x7bf0, 0x7c60, 0x7cd0, 0x7d40, 0x7daf, 0x7e1e, 0x7e8e, 0x7efd, 0x7f6c, 0x7fdb, 0x8049, 0x80b8, 0x8126,
		0x8195, 0x8203, 0x8271, 0x82df, 0x834d, 0x83bb, 0x8428, 0x8496, 0x8503, 0x8570, 0x85dd, 0x864a, 0x86b7, 0x8724, 0x8790, 0x87fd,
		0x8869, 0x88d5, 0x8941, 0x89ad, 0x8a19, 0x8a84, 0x8af0, 0x8b5b, 0x8bc6, 0x8c31, 0x8c9c, 0x8d07, 0x8d72, 0x8ddc, 0x8e47, 0x8eb1,
		0x8f1b, 0x8f85, 0x8fef, 0x9058, 0x90c2, 0x912b, 0x9195, 0x91fe, 0x9267, 0x92d0, 0x9338, 0x93a1, 0x9409, 0x9471, 0x94da, 0x9542,
		0x95a9, 0x9611, 0x9679, 0x96e0, 0x9747, 0x97ae, 0x9815, 0x987c, 0x98e3, 0x9949, 0x99b0, 0x9a16, 0x9a7c, 0x9ae2, 0x9b47, 0x9bad,
		0x9c12, 0x9c78, 0x9cdd, 0x9d42, 0x9da7, 0x9e0b, 0x9e70, 0x9ed4, 0x9f38, 0x9f9d, 0xa000, 0xa064, 0xa0c8, 0xa12b, 0xa18e, 0xa1f2,
		0xa255, 0xa2b7, 0xa31a, 0xa37c, 0xa3df, 0xa441, 0xa4a3, 0xa505, 0xa567, 0xa5c8, 0xa629, 0xa68b, 0xa6ec, 0xa74d, 0xa7ad, 0xa80e,
		0xa86e, 0xa8ce, 0xa92e, 0xa98e, 0xa9ee, 0xaa4e, 0xaaad, 0xab0c, 0xab6b, 0xabca, 0xac29, 0xac88, 0xace6, 0xad44, 0xada2, 0xae00,
		0xae5e, 0xaebb, 0xaf19, 0xaf76, 0xafd3, 0xb030, 0xb08c, 0xb0e9, 0xb145, 0xb1a2, 0xb1fe, 0xb259, 0xb2b5, 0xb310, 0xb36c, 0xb3c7,
		0xb422, 0xb47d, 0xb4d7, 0xb532, 0xb58c, 0xb5e6, 0xb640, 0xb69a, 0xb6f3, 0xb74c, 0xb7a6, 0xb7ff, 0xb857, 0xb8b0, 0xb909, 0xb961,
		0xb9b9, 0xba11, 0xba69, 0xbac0, 0xbb18, 0xbb6f, 0xbbc6, 0xbc1d, 0xbc73, 0xbcca, 0xbd20, 0xbd76, 0xbdcc, 0xbe22, 0xbe77, 0xbecd,
		0xbf22, 0xbf77, 0xbfcc, 0xc020, 0xc075, 0xc0c9, 0xc11d, 0xc171, 0xc1c4, 0xc218, 0xc26b, 0xc2be, 0xc311, 0xc364, 0xc3b6, 0xc409,
		0xc45b, 0xc4ad, 0xc4ff, 0xc550, 0xc5a2, 0xc5f3, 0xc644, 0xc695, 0xc6e5, 0xc736, 0xc786, 0xc7d6, 0xc826, 0xc875, 0xc8c5, 0xc914,
		0xc963, 0xc9b2, 0xca00, 0xca4f, 0xca9d, 0xcaeb, 0xcb39, 0xcb87, 0xcbd4, 0xcc22, 0xcc6f, 0xccbb, 0xcd08, 0xcd55, 0xcda1, 0xcded,
		0xce39, 0xce85, 0xced0, 0xcf1b, 0xcf66, 0xcfb1, 0xcffc, 0xd046, 0xd091, 0xd0db, 0xd124, 0xd16e, 0xd1b8, 0xd201, 0xd24a, 0xd293,
		0xd2db, 0xd324, 0xd36c, 0xd3b4, 0xd3fc, 0xd443, 0xd48b, 0xd4d2, 0xd519, 0xd560, 0xd5a6, 0xd5ec, 0xd633, 0xd679, 0xd6be, 0xd704,
		0xd749, 0xd78e, 0xd7d3, 0xd818, 0xd85c, 0xd8a0, 0xd8e4, 0xd928, 0xd96c, 0xd9af, 0xd9f2, 0xda35, 0xda78, 0xdabb, 0xdafd, 0xdb3f,
		0xdb81, 0xdbc3, 0xdc04, 0xdc46, 0xdc87, 0xdcc7, 0xdd08, 0xdd48, 0xdd89, 0xddc9, 0xde08, 0xde48, 0xde87, 0xdec6, 0xdf05, 0xdf44,
		0xdf82, 0xdfc1, 0xdfff, 0xe03c, 0xe07a, 0xe0b7, 0xe0f5, 0xe132, 0xe16e, 0xe1ab, 0xe1e7, 0xe223, 0xe25f, 0xe29a, 0xe2d6, 0xe311,
		0xe34c, 0xe387, 0xe3c1, 0xe3fb, 0xe436, 0xe46f, 0xe4a9, 0xe4e2, 0xe51c, 0xe554, 0xe58d, 0xe5c6, 0xe5fe, 0xe636, 0xe66e, 0xe6a5,
		0xe6dd, 0xe714, 0xe74b, 0xe782, 0xe7b8, 0xe7ee, 0xe824, 0xe85a, 0xe890, 0xe8c5, 0xe8fa, 0xe92f, 0xe964, 0xe998, 0xe9cc, 0xea00,
		0xea34, 0xea68, 0xea9b, 0xeace, 0xeb01, 0xeb33, 0xeb66, 0xeb98, 0xebca, 0xebfb, 0xec2d, 0xec5e, 0xec8f, 0xecc0, 0xecf0, 0xed21,
		0xed51, 0xed81, 0xedb0, 0xede0, 0xee0f, 0xee3e, 0xee6c, 0xee9b, 0xeec9, 0xeef7, 0xef25, 0xef52, 0xef80, 0xefad, 0xefd9, 0xf006,
		0xf032, 0xf05e, 0xf08a, 0xf0b6, 0xf0e1, 0xf10d, 0xf138, 0xf162, 0xf18d, 0xf1b7, 0xf1e1, 0xf20b, 0xf234, 0xf25e, 0xf287, 0xf2af,
		0xf2d8, 0xf300, 0xf328, 0xf350, 0xf378, 0xf39f, 0xf3c7, 0xf3ed, 0xf414, 0xf43b, 0xf461, 0xf487, 0xf4ac, 0xf4d2, 0xf4f7, 0xf51c,
		0xf541, 0xf566, 0xf58a, 0xf5ae, 0xf5d2, 0xf5f5, 0xf619, 0xf63c, 0xf65f, 0xf681, 0xf6a4, 0xf6c6, 0xf6e8, 0xf709, 0xf72b, 0xf74c,
		0xf76d, 0xf78e, 0xf7ae, 0xf7ce, 0xf7ee, 0xf80e, 0xf82d, 0xf84d, 0xf86c, 0xf88a, 0xf8a9, 0xf8c7, 0xf8e5, 0xf903, 0xf921, 0xf93e,
		0xf95b, 0xf978, 0xf994, 0xf9b1, 0xf9cd, 0xf9e9, 0xfa04, 0xfa20, 0xfa3b, 0xfa56, 0xfa70, 0xfa8b, 0xfaa5, 0xfabf, 0xfad8, 0xfaf2,
		0xfb0b, 0xfb24, 0xfb3c, 0xfb55, 0xfb6d, 0xfb85, 0xfb9d, 0xfbb4, 0xfbcb, 0xfbe2, 0xfbf9, 0xfc0f, 0xfc26, 0xfc3c, 0xfc51, 0xfc67,
		0xfc7c, 0xfc91, 0xfca6, 0xfcba, 0xfccf, 0xfce3, 0xfcf6, 0xfd0a, 0xfd1d, 0xfd30, 0xfd43, 0xfd55, 0xfd68, 0xfd7a, 0xfd8c, 0xfd9d,
		0xfdae, 0xfdbf, 0xfdd0, 0xfde1, 0xfdf1, 0xfe01, 0xfe11, 0xfe20, 0xfe30, 0xfe3f, 0xfe4e, 0xfe5c, 0xfe6b, 0xfe79, 0xfe86, 0xfe94,
		0xfea1, 0xfeae, 0xfebb, 0xfec8, 0xfed4, 0xfee0, 0xfeec, 0xfef8, 0xff03, 0xff0e, 0xff19, 0xff24, 0xff2e, 0xff38, 0xff42, 0xff4b,
		0xff55, 0xff5e, 0xff67, 0xff6f, 0xff78, 0xff80, 0xff88, 0xff8f, 0xff97, 0xff9e, 0xffa5, 0xffab, 0xffb2, 0xffb8, 0xffbe, 0xffc3,
		0xffc8, 0xffce, 0xffd2, 0xffd7, 0xffdb, 0xffe0, 0xffe3, 0xffe7, 0xffea, 0xffee, 0xfff0, 0xfff3, 0xfff5, 0xfff8, 0xfff9, 0xfffb,
		0xfffc, 0xfffe, 0xfffe, 0xffff, 0xffff, 0xffff,
	};

	inline float fast_sin_pi2(float x) //x = 64 .. 64 + pi/2
	{
		//0	1234567 0 1234567 01234567 01234567
		//s eeeeeee e ------t tttttttt iiiiiiii

		uint8_t* __restrict x1_ptr = reinterpret_cast<uint8_t*>(&x);
		uint8_t it = *x1_ptr; //low 8 bits are for interpolating between table entries
		uint16_t fx = *reinterpret_cast<uint16_t*>(x1_ptr + 1)&0x7FFF; //high 9 bits are for table lookup
	
		//now fx should be between 0 and 804

		//compose the float from a precomputed exponent (1.0) by appending the decimals
		uint16_t t1 = k_sin16[fx];
		uint16_t t2 = k_sin16[fx + 1];
		//v = a + (b - a)*t;
		uint32_t v = (uint32_t(t1) << 7) + ((uint32_t(t2 - t1) * it) >> 1);

		uint32_t f = 0x3F800000 | v;
		return *reinterpret_cast<float*>(&f); //1 .. 2
	}
	inline float fast_sin(float x)
	{
		while (is_positive(x) && x >= anglef::_2pi.radians) x -= anglef::_2pi.radians;
		while (is_negative(x) && x < 0) x += anglef::_2pi.radians;

		if (x >= anglef::pi.radians)
		{
			if (x >= anglef::_3pi2.radians)
			{
				//64 + anglef::_2pi.radians
				return 1.f - fast_sin_pi2(64.f + anglef::_2pi.radians - x);
			}
			else
			{
				//64 + x - anglef::pi.radians
				return 1.f - fast_sin_pi2(64.f - anglef::pi.radians + x);
			}
		}
		else
		{
			if (x >= anglef::pi2.radians)
			{
				//64 + anglef::pi.radians - x
				return fast_sin_pi2(64.f + anglef::pi.radians - x) - 1.f;
			}
			else
			{
				return fast_sin_pi2(64 + x) - 1.f;
			}
		}
	}

	inline float fast_cos(float x)
	{
		return fast_sin(x + anglef::pi2.radians);
	}
	inline void fast_sin_cos(float x, float& s, float& c)
	{
		while (is_positive(x) && x >= anglef::_2pi.radians) x -= anglef::_2pi.radians;
		while (is_negative(x) && x < 0) x += anglef::_2pi.radians;
		
		if (x >= anglef::pi.radians)
		{
			if (x >= anglef::_3pi2.radians)
			{
				//64 + anglef::_2pi.radians
				s = 1.f - fast_sin_pi2(64.f + anglef::_2pi.radians - x);
				c = fast_sin_pi2(64.f - anglef::_3pi2.radians + x) - 1.f;
			}
			else
			{
				//64 + x - anglef::pi.radians
				s = 1.f - fast_sin_pi2(64.f - anglef::pi.radians + x);
				c = 1.f - fast_sin_pi2(64.f + anglef::_3pi2.radians - x);
			}
		}
		else
		{
			if (x >= anglef::pi2.radians)
			{
				//64 + anglef::pi.radians - x
				s = fast_sin_pi2(64.f + anglef::pi.radians - x) - 1.f;
				c = 1.f - fast_sin_pi2(64.f - anglef::pi2.radians + x);
			}
			else
			{
				s = fast_sin_pi2(64.f + x) - 1.f;
				c = fast_sin_pi2(64.f + anglef::pi2.radians - x) - 1.f;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//standard

template<> inline float cos<float, standard>(float const& s) { return ::cosf(s); }
template<> inline float sin<float, standard>(float const& s) { return ::sinf(s); }
template<> inline float tan<float, standard>(float const& s) { return ::tanf(s); }
template<> inline float acos<float, standard>(float const& s) { return ::acosf(s); }
template<> inline float asin<float, standard>(float const& s) { return ::asinf(s); }
template<> inline float atan<float, standard>(float const& s) { return ::atanf(s); }
template<> inline float atan2<float, standard>(float const& y, float const& x) { return ::atan2f(y, x); }
template<> inline void sin_cos<float, standard>(float const& angle, float& s, float& c)
{
	s = ::sinf(angle);
	c = ::cosf(angle);
}

//////////////////////////////////////////////////////////////////////////
//fast

template<> inline float cos<float, fast>(float const& s) { return detail::fast_cos(s); }
template<> inline float sin<float, fast>(float const& s) { return detail::fast_sin(s); }
template<> inline float tan<float, fast>(float const& s) { return ::tanf(s); }
template<> inline float acos<float, fast>(float const& s) { return ::acosf(s); }
template<> inline float asin<float, fast>(float const& s) { return ::asinf(s); }
template<> inline float atan<float, fast>(float const& s) { return ::atanf(s); }
template<> inline float atan2<float, fast>(float const& y, float const& x) { return ::atan2f(y, x); }
template<> inline void sin_cos<float, fast>(float const& angle, float& s, float& c)
{
	detail::fast_sin_cos(angle, s, c);
}

//////////////////////////////////////////////////////////////////////////
//safe

template<> inline float cos<float, safe>(float const& s) { return ::cosf(s); }
template<> inline float sin<float, safe>(float const& s) { return ::sinf(s); }
template<> inline float tan<float, safe>(float const& s) { return ::tanf(s); }
template<> inline float acos<float, safe>(float const& s) { return ::acosf(s); }
template<> inline float asin<float, safe>(float const& s) { return ::asinf(s); }
template<> inline float atan<float, safe>(float const& s) { return ::atanf(s); }
template<> inline float atan2<float, safe>(float const& y, float const& x) { return ::atan2f(y, x); }
template<> inline void sin_cos<float, safe>(float const& angle, float& s, float& c)
{
	s = ::sinf(angle);
	c = ::cosf(angle);
}

//////////////////////////////////////////////////////////////////////////
//fast_safe

template<> inline float cos<float, fast_safe>(float const& s) { return detail::fast_cos(s); }
template<> inline float sin<float, fast_safe>(float const& s) { return detail::fast_sin(s); }
template<> inline float tan<float, fast_safe>(float const& s) { return ::tanf(s); }
template<> inline float acos<float, fast_safe>(float const& s) { return ::acosf(s); }
template<> inline float asin<float, fast_safe>(float const& s) { return ::asinf(s); }
template<> inline float atan<float, fast_safe>(float const& s) { return ::atanf(s); }
template<> inline float atan2<float, fast_safe>(float const& y, float const& x) { return ::atan2f(y, x); }
template<> inline void sin_cos<float, fast_safe>(float const& angle, float& s, float& c)
{
	detail::fast_sin_cos(angle, s, c);
}
	
	
template<typename T, class Policy = standard> inline void sin_cos(T const& angle, T& s, T& c)
{
	s = sin<T, Policy>(angle);
	c = cos<T, Policy>(angle);
}

//////////////////////////////////////////////////////////////////////////
// vec2
template<typename T>
inline vec2<T> degrees(vec2<T> const& v)
{
	return vec2<T>(degrees(v.x), degrees(v.y));
}
template<typename T>
inline vec2<T> radians(vec2<T> const& v)
{
	return vec2<T>(radians(v.x), radians(v.y));
}

template<typename T, class Policy = standard>
inline vec2<T> cos(vec2<T> const& v)
{
	return vec2<T>(cos<T, Policy>(v.x), cos<T, Policy>(v.y));
}
template<typename T, class Policy = standard>
inline vec2<T> sin(vec2<T> const& v)
{
	return vec2<T>(sin<T, Policy>(v.x), sin<T, Policy>(v.y));
}
template<typename T, class Policy = standard>
inline vec2<T> tan(vec2<T> const& v)
{
	return vec2<T>(tan<T, Policy>(v.x), tan<T, Policy>(v.y));
}
template<typename T, class Policy = standard>
inline vec2<T> acos(vec2<T> const& v)
{
	return vec2<T>(acos<T, Policy>(v.x), acos<T, Policy>(v.y));
}
template<typename T, class Policy = standard>
inline vec2<T> asin(vec2<T> const& v)
{
	return vec2<T>(asin<T, Policy>(v.x), asin<T, Policy>(v.y));
}
template<typename T, class Policy = standard>
inline vec2<T> atan(vec2<T> const& v)
{
	return vec2<T>(atan<T, Policy>(v.x), atan<T, Policy>(v.y));
}
template<typename T, class Policy = standard>
inline vec2<T> atan2(vec2<T> const& y, vec2<T> const& x)
{
	return vec2<T>(atan2<T, Policy>(y.x, x.x), atan2<T, Policy>(y.y, x.y));
}
template<typename T, class Policy = standard>
inline void sin_cos(vec2<T> const& angle, vec2<T>& s, vec2<T>& c)
{
	s = sin<T, Policy>(angle);
	c = cos<T, Policy>(angle);
}

//////////////////////////////////////////////////////////////////////////
// vec3
template<typename T>
inline vec3<T> degrees(vec3<T> const& v)
{
	return vec3<T>(degrees(v.x), degrees(v.y), degrees(v.z));
}
template<typename T>
inline vec3<T> radians(vec3<T> const& v)
{
	return vec3<T>(radians(v.x), radians(v.y), radians(v.z));
}
template<typename T, class Policy = standard>
inline vec3<T> cos(vec3<T> const& v)
{
	return vec3<T>(cos<T, Policy>(v.x), cos<T, Policy>(v.y), cos<T, Policy>(v.z));
}
template<typename T, class Policy = standard>
inline vec3<T> sin(vec3<T> const& v)
{
	return vec3<T>(sin<T, Policy>(v.x), sin<T, Policy>(v.y), sin<T, Policy>(v.z));
}
template<typename T, class Policy = standard>
inline vec3<T> tan(vec3<T> const& v)
{
	return vec3<T>(tan<T, Policy>(v.x), tan<T, Policy>(v.y), tan<T, Policy>(v.z));
}
template<typename T, class Policy = standard>
inline vec3<T> acos(vec3<T> const& v)
{
	return vec3<T>(acos<T, Policy>(v.x), acos<T, Policy>(v.y), acos<T, Policy>(v.z));
}
template<typename T, class Policy = standard>
inline vec3<T> asin(vec3<T> const& v)
{
	return vec3<T>(asin<T, Policy>(v.x), asin<T, Policy>(v.y), asin<T, Policy>(v.z));
}
template<typename T, class Policy = standard>
inline vec3<T> atan(vec3<T> const& v)
{
	return vec3<T>(atan<T, Policy>(v.x), atan<T, Policy>(v.y), atan<T, Policy>(v.z));
}
template<typename T, class Policy = standard>
inline vec3<T> atan2(vec3<T> const& y, vec3<T> const& x)
{
	return vec3<T>(atan2<T, Policy>(y.x, x.x), atan2<T, Policy>(y.y, x.y), atan2<T, Policy>(y.z, x.z));
}
template<typename T, class Policy = standard>
inline void sin_cos(vec3<T> const& angle, vec3<T>& s, vec3<T>& c)
{
	s = sin<T, Policy>(angle);
	c = cos<T, Policy>(angle);
}

//////////////////////////////////////////////////////////////////////////
// vec4
template<typename T>
inline vec4<T> degrees(vec4<T> const& v)
{
	return vec4<T>(degrees(v.x), degrees(v.y), degrees(v.z), degrees(v.w));
}
template<typename T>
inline vec4<T> radians(vec4<T> const& v)
{
	return vec4<T>(radians(v.x), radians(v.y), radians(v.z), radians(v.w));
}
template<typename T, class Policy = standard>
inline vec4<T> cos(vec4<T> const& v)
{
	return vec4<T>(cos<T, Policy>(v.x), cos<T, Policy>(v.y), cos<T, Policy>(v.z), cos<T, Policy>(v.w));
}
template<typename T, class Policy = standard>
inline vec4<T> sin(vec4<T> const& v)
{
	return vec4<T>(sin<T, Policy>(v.x), sin<T, Policy>(v.y), sin<T, Policy>(v.z), sin<T, Policy>(v.w));
}
template<typename T, class Policy = standard>
inline vec4<T> tan(vec4<T> const& v)
{
	return vec4<T>(tan<T, Policy>(v.x), tan<T, Policy>(v.y), tan<T, Policy>(v.z), tan<T, Policy>(v.w));
}
template<typename T, class Policy = standard>
inline vec4<T> acos(vec4<T> const& v)
{
	return vec4<T>(acos<T, Policy>(v.x), acos<T, Policy>(v.y), acos<T, Policy>(v.z), acos<T, Policy>(v.w));
}
template<typename T, class Policy = standard>
inline vec4<T> asin(vec4<T> const& v)
{
	return vec4<T>(asin<T, Policy>(v.x), asin<T, Policy>(v.y), asin<T, Policy>(v.z), asin<T, Policy>(v.w));
}
template<typename T, class Policy = standard>
inline vec4<T> atan(vec4<T> const& v)
{
	return vec4<T>(atan<T, Policy>(v.x), atan<T, Policy>(v.y), atan<T, Policy>(v.z), atan<T, Policy>(v.w));
}
template<typename T, class Policy = standard>
inline vec4<T> atan2(vec4<T> const& y, vec4<T> const& x)
{
	return vec4<T>(atan2<T, Policy>(y.x, x.x), atan2<T, Policy>(y.y, x.y), atan2<T, Policy>(y.z, x.z), atan2<T, Policy>(y.w, x.w));
}
template<typename T, class Policy = standard>
inline void sin_cos(vec4<T> const& angle, vec4<T>& s, vec4<T>& c)
{
	s = sin<T, Policy>(angle);
	c = cos<T, Policy>(angle);
}

}