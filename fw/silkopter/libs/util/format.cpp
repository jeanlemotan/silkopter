#include "format.h"

namespace util
{
namespace formatting
{
namespace detail
{

	const float s_pow_10[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 10000000000 };
	const char s_digits[201] =
				"0001020304050607080910111213141516171819"
				"2021222324252627282930313233343536373839"
				"4041424344454647484950515253545556575859"
				"6061626364656667686970717273747576777879"
				"8081828384858687888990919293949596979899";

	int8_t get_base_10_digit_count(uint64_t v)
	{
		if (v < 10) return 1;
		if (v < 100) return 2;
		if (v < 1000) return 3;
		if (v < 1000000000000)
		{
			if (v < 100000000)
			{
				if (v < 1000000)
				{
					return (v < 10000) ? 4 : 5 + (v >= 100000);
				}
				return 7 + (v >= 10000000);
			}
			return (v < 10000000000) ? 9 + (v >= 1000000000) : 11 + (v >= 100000000000);
		}
		return 12 + get_base_10_digit_count(v / 1000000000000);
	}
	int8_t get_base_10_digit_count(uint8_t v)
	{
		if (v < 100) 
		{
			return (v < 10) ? 1 : 2;
		}
		return 3;
	}
	int8_t get_base_10_digit_count(uint16_t v)
	{
		if (v < 10000)
		{
			if (v < 1000)
			{
				if (v < 100)
				{
					if (v < 10)
					{ 
						return 1;
					}
					return 2;
				}
				return 3;
			}
			return 4;
		}
		return 5;
	}
	int8_t get_base_10_digit_count(uint32_t v)
	{
		if (v < 100000)
		{
			if (v < 10000)
			{
				if (v < 1000)
				{
					if (v < 100)
					{
						return (v < 10) ? 1 : 2;
					}
					return 3;
				}
				return 4;
			}
			return 5;
		}
		if (v < 1000000000)
		{
			if (v < 100000000)
			{
				if (v < 10000000)
				{
					return (v < 1000000) ? 6 : 7;
				}
				return 8;
			}
			return 9;
		}
		return 10;
	}

}
}
}
