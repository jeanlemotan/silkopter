#pragma once

#include <stdint.h>
#include <stddef.h>
#include <algorithm>
#include <math.h>
#include "FString.h"

namespace util
{
namespace formatting
{
	struct Placeholder
	{
		Placeholder() : alignment(0), precision(0), base_case(0), base(10), filler(' ') {}
		int8_t alignment;
		int8_t precision;
		uint8_t base_case : 1; //lower == 0 / upper case == 1
		uint8_t base : 7;
		char filler;
	};

	namespace detail
	{
		extern const float s_pow_10[];
		extern const char s_digits[201];

		template<class Format_String_Adapter>
		auto parse_index(uint8_t& index, Format_String_Adapter& fmt_adapter) -> bool
		{
			if (fmt_adapter.is_done())
			{
				return false;
			}
			auto ch = fmt_adapter.get_and_advance();
			if (ch < '0' || ch > '9')
			{
				return false;
			}
			index = ch - '0';
			return true;
		}

		///

		template<class T>
		void decompose_digits(char* buf, int8_t length, T uvalue)
		{
			auto off_end = length - 1;
			while(uvalue >= 100)
			{
				const uint32_t i = (uvalue % 100) * 2;
				uvalue /= 100;
				buf[off_end] = detail::s_digits[i + 1];
				buf[off_end - 1] = detail::s_digits[i];
				off_end -= 2;
			}

			// Handle last 1-2 digits.
			if (uvalue < 10)
			{
				buf[off_end] = '0' + static_cast<char>(uvalue);
			}
			else
			{
				const uint32_t i = static_cast<uint32_t>(uvalue) * 2;
				buf[off_end] = detail::s_digits[i + 1];
				buf[off_end - 1] = detail::s_digits[i];
			}
		}


		//////////////////////////////////////////////////////////////////////////

		auto get_base_10_digit_count(uint64_t v) -> int8_t;
		auto get_base_10_digit_count(uint32_t v) -> int8_t;
		auto get_base_10_digit_count(uint16_t v) -> int8_t;
		auto get_base_10_digit_count(uint8_t v) -> int8_t;
	}

	//////////////////////////////////////////////////////////////////////////

	//this class is an adapter that is able to iterate over an input string
	template<class Format_String> struct Format_String_Adapter
	{
	public:
		typedef typename Format_String::value_type value_type;
		typedef typename Format_String::const_iterator iterator;
		Format_String_Adapter(Format_String const& fmt) : m_format_string(fmt), m_it(fmt.begin()), m_end(fmt.end()) {}
		auto is_done() const -> bool { return m_it == m_end; }
		auto get() -> value_type { return *m_it; }
		auto get_and_advance() -> value_type { return *m_it++; }
	private:
		Format_String const& m_format_string;
		iterator m_it;
		iterator m_end;
	};
	template<> struct Format_String_Adapter<char const*>
	{
	public:
		typedef char value_type;
		typedef char const* iterator;
		Format_String_Adapter(char const* fmt) : m_format_string(fmt), m_it(fmt) {}
		auto is_done() const -> bool { return *m_it == 0; }
		auto get() -> value_type { return *m_it; }
		auto get_and_advance() -> value_type { return *m_it++; }
	private:
		const char* m_format_string;
		const char* m_it;
	};

	//this is needed for const char[N]
	template<class T, size_t N> struct Format_String_Adapter<T[N]>
	{
	public:
		typedef T value_type;
		typedef T const* iterator;
		Format_String_Adapter(T const* fmt) : m_format_string(fmt), m_it(fmt) {}
		auto is_done() const -> bool { return *m_it == 0; }
		auto get() ->value_type { return *m_it; }
		auto get_and_advance() -> value_type { return *m_it++; }
	private:
		T const* m_format_string;
		T const* m_it;
	};


	template<class Dst_String, class Placeholder, size_t SIZE>
	void format_string(Dst_String& dst, Placeholder const& ph, FString<SIZE> const& p)
	{
		dst.append(p.begin(), p.end());
	}
	template<class Dst_String, class Placeholder>
	void format_string(Dst_String& dst, Placeholder const& ph, const char* p)
	{
		if (p)
		{
			dst.reserve(dst.size() + 64);
			for (; *p != 0; ++p)
			{
				dst.append(*p);
			}
		}
	}
	template<class Dst_String, class Placeholder>
	void format_string(Dst_String& dst, Placeholder const& ph, char p)
	{
		dst.append(p);
	}

	template<class Dst_String, class Placeholder>
	void format_string(Dst_String& dst, Placeholder const& ph, int8_t value)
	{
		if (ph.base == 16)
		{
			char buf[8];
			sprintf(buf, ph.base_case == 0 ? "%x" : "%X", value);
			format_string(dst, Placeholder(), buf);
			return;
		}

		// Take care of sign.
		uint8_t uvalue = (value < 0) ? -value : value;
		auto length = util::formatting::detail::get_base_10_digit_count(uvalue);
		auto aligned_length = std::max(length, ph.alignment);
		if (value < 0)
		{
			//to make space for the zero
			aligned_length--;
		}
		for (int8_t i = length; i < aligned_length; i++)
		{
			dst.append(ph.filler);
		}
		if (value < 0)
		{
			dst.append('-');
		}
		char buf[16];
		detail::decompose_digits(buf, length, uvalue);
		dst.append(buf, buf + length);
	}

	template<class Dst_Adapter, class Placeholder>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, uint8_t value)
	{
		if (ph.base == 16)
		{
			char buf[8];
			sprintf(buf, ph.base_case == 0 ? "%x" : "%X", value);
			format_string(dst, Placeholder(), buf);
			return;
		}
		auto uvalue = value;
		auto length = util::formatting::detail::get_base_10_digit_count(uvalue);
		auto aligned_length = std::max(length, ph.alignment);
		for (int8_t i = length; i < aligned_length; i++)
		{
			dst.append(ph.filler);
		}
		char buf[16];
		detail::decompose_digits(buf, length, uvalue);
		dst.append(buf, buf + length);
	}

	template<class Dst_Adapter, class Placeholder>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, int16_t value)
	{
		if (ph.base == 16)
		{
			char buf[32];
			sprintf(buf, ph.base_case == 0 ? "%x" : "%X", value);
			format_string(dst, Placeholder(), buf);
			return;
		}
		// Take care of sign.
		uint16_t uvalue = (value < 0) ? -value : value;
		auto length = util::formatting::detail::get_base_10_digit_count(uvalue);
		auto aligned_length = std::max(length, ph.alignment);
		if (value < 0)
		{
			//to make space for the zero
			aligned_length--;
		}
		for (int8_t i = length; i < aligned_length; i++)
		{
			dst.append(ph.filler);
		}
		if (value < 0)
		{
			dst.append('-');
		}
		char buf[16];
		detail::decompose_digits(buf, length, uvalue);
		dst.append(buf, buf + length);
	}

	template<class Dst_Adapter, class Placeholder>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, uint16_t value)
	{
		if (ph.base == 16)
		{
			char buf[32];
			sprintf(buf, ph.base_case == 0 ? "%x" : "%X", value);
			format_string(dst, Placeholder(), buf);
			return;
		}
		auto uvalue = value;
		auto length = util::formatting::detail::get_base_10_digit_count(uvalue);
		auto aligned_length = std::max(length, ph.alignment);
		for (int8_t i = length; i < aligned_length; i++)
		{
			dst.append(ph.filler);
		}
		char buf[16];
		detail::decompose_digits(buf, length, uvalue);
		dst.append(buf, buf + length);
	}

	template<class Dst_Adapter, class Placeholder>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, int32_t value)
	{
		//if (ph.base == 16)
		//{
			//char buf[32];
			//sprintf(buf, ph.base_case == 0 ? "%x" : "%X", value);
			//format_string(dst, Placeholder(), buf);
			//return;
		//}

		// Take care of sign.
		uint32_t uvalue = (value < 0) ? -value : value;
		auto length = util::formatting::detail::get_base_10_digit_count(uvalue);
		auto aligned_length = std::max(length, ph.alignment);
		if (value < 0)
		{
			//to make space for the zero
			aligned_length--;
		}
		for (int8_t i = length; i < aligned_length; i++)
		{
			dst.append(ph.filler);
		}
		if (value < 0)
		{
			dst.append('-');
		}
		char buf[16];
		detail::decompose_digits(buf, length, uvalue);
		dst.append(buf, buf + length);
	}

	template<class Dst_Adapter, class Placeholder>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, uint32_t value)
	{
// 		if (ph.base == 16)
// 		{
// 			char buf[32];
// 			sprintf(buf, ph.baseCase == 0 ? "%x" : "%X", value);
// 			format_string(dst, Placeholder(), buf);
// 			return;
// 		}

		auto uvalue = value;
		auto length = util::formatting::detail::get_base_10_digit_count(uvalue);
		auto aligned_length = std::max(length, ph.alignment);
		for (int8_t i = length; i < aligned_length; i++)
		{
			dst.append(ph.filler);
		}
		char buf[16];
		detail::decompose_digits(buf, length, uvalue);
		dst.append(buf, buf + length);
	}

	template<class Dst_Adapter, class Placeholder>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, int64_t value)
	{
		// Take care of sign.
		uint64_t uvalue = (value < 0) ? -value : value;
		auto length = util::formatting::detail::get_base_10_digit_count(uvalue);
		auto aligned_length = std::max(length, ph.alignment);
		if (value < 0)
		{
			//to make space for the zero
			aligned_length--;
		}
		for (int8_t i = length; i < aligned_length; i++)
		{
			dst.append(ph.filler);
		}
		if (value < 0)
		{
			dst.append('-');
		}
		char buf[32];
		detail::decompose_digits(buf, length, uvalue);
		dst.append(buf, buf + length);
	}

	template<class Dst_Adapter, class Placeholder>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, uint64_t value)
	{
		auto uvalue = value;
		auto length = util::formatting::detail::get_base_10_digit_count(uvalue);
		auto aligned_length = std::max(length, ph.alignment);
		for (int8_t i = length; i < aligned_length; i++)
		{
			dst.append(ph.filler);
		}
		char buf[16];
		detail::decompose_digits(buf, length, uvalue);
		dst.append(buf, buf + length);
	}

	template<class Dst_Adapter, class Placeholder>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, float value)
	{
		if (!(value == value))
		{
			// Handle the 'width' parameter.
			format_string(dst, ph, "nan");
			return;
		}

		//to format the parts without alignment or precision
		Placeholder ph2;
		ph2.filler = '0';
		ph2.alignment = 9;

		//the float is formatted as a series of integers
		int32_t whole_parts[16]; //each will hold 9 decimals
		int8_t whole_parts_count = 0;

		int32_t frac_parts[16];
		uint8_t last_frac_part_alignment = 9;
		int8_t frac_parts_count = 0;

		uint8_t precision = ph.precision;
		if (precision == 0)
		{
			precision = 5;
		}
		else if (precision > 9)
		{
			precision = 9;
		}

		bool is_negative = value < 0.0;
		value = is_negative ? -value : value;

		float whole = floorf(value);
		float frac = value - whole;

		//store the whole parts
		float v = whole;
		if (v > 0.0f)
		{
			do
			{
				whole_parts[whole_parts_count++] = (int32_t)fmod(v, 1000000);
				if (v < 1000000)
				{
					break;
				}
				v /= 1000000;
			} while (true);
		}

		//store the fract parts
		//we transform the frac part like this (For a precision of 5):
		//	1.	0.0004272
		//	2. 	0.0004272 * 100000 == 42
		v = floor(frac * detail::s_pow_10[precision] + 0.5f);
		if (v > 0.0f)
		{
			//	3.	42 + 100000 = 100042
			v += detail::s_pow_10[precision];
			do
			{
				int32_t r = (int32_t)fmod(v, 1000000);
				bool last = (v < 1000000);
				if (r > 0)
				{
					if (last)
					{
						last_frac_part_alignment = 9;
						//remove trailing zeros
						int32_t r2 = r / 10;
						while (r2 * 10 == r)
						{
							r = r2;
							r2 /= 10;
							last_frac_part_alignment--;
						}
					}
					frac_parts[frac_parts_count++] = r;
				}
				if (last)
				{
					break;
				}
				v /= 1000000;
			} while (true);
		}

		if (whole_parts_count > 0)
		{
			if (is_negative)
			{
				whole_parts[whole_parts_count - 1] *= -1;
			}

			//first part doesn't need '0' padding
			ph2.alignment = 0;
			format_string(dst, ph2, whole_parts[whole_parts_count - 1]);

			//next parts are 9 digits each
			ph2.alignment = 9;
			for (int8_t i = whole_parts_count - 2; i >= 0; i--)
			{
				format_string(dst, ph2, whole_parts[i]);
			}
		}
		else
		{
			format_string(dst, ph, is_negative ? "-0" : "0");
		}

		if (frac_parts_count > 0)
		{
			auto decimal_point_off = dst.size();
			//parseToString(dst, off, Placeholder(), '.');

			//first part - no alignment needed
			ph2.alignment = 0;
			format_string(dst, ph2, frac_parts[frac_parts_count - 1]);

			//middle part - alignment is 9
			ph2.alignment = 9;
			for (int8_t i = frac_parts_count - 2; i >= 1; i--)
			{
				format_string(dst, ph2, frac_parts[i]);
			}

			if (frac_parts_count > 1)
			{
				//the last part might be cut as we remove zeroes from it
				ph2.alignment = last_frac_part_alignment;
				format_string(dst, ph2, frac_parts[0]);
			}

			//overwrite the 1 from the frac part with the decimal point
			dst[decimal_point_off] = '.';
		}
	}

	template<class Dst_Adapter, class Placeholder, class P0, class P1>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, std::pair<P0, P1> const& pair)
	{
		dst.append('<');
		format_string(dst, ph, pair.first);
		dst.append(',');
		dst.append(' ');
		format_string(dst, ph, pair.second);
		dst.append('>');
	}

// 	template<class Dst_Adapter, class Placeholder, class rep, class period>
// 	void format_string(Dst_Adapter& dst, auto& off, Placeholder const& ph, std::chrono::duration<rep, period> const& duration)
// 	{
// 		using namespace std::chrono;
// 
// 		auto d = duration;
// 		auto h = duration_cast<hours>(d);
// 		d -= duration_cast<std::chrono::duration<rep, period>>(h);
// 
// 		auto m = duration_cast<minutes>(d);
// 		d -= duration_cast<std::chrono::duration<rep, period>>(m);
// 
// 		auto s = duration_cast<seconds>(d);
// 		d -= duration_cast<std::chrono::duration<rep, period>>(s);
// 
// 		auto ms = duration_cast<milliseconds>(d);
// 
// 		Placeholder ph2 = ph;
// 		ph2.filler = '0';
// 		ph2.alignment = 2;
// 		format_string(dst, off, ph2, h.count());
// 		format_string(dst, off, ph, ':');
// 		format_string(dst, off, ph2, (int32_t)m.count());
// 		format_string(dst, off, ph, ':');
// 		format_string(dst, off, ph2, (int32_t)s.count());
// 		format_string(dst, off, ph, '.');
// 		format_string(dst, off, ph, (int32_t)ms.count());
// 	}
// 
// 	template<class Dst_Adapter, class Placeholder, class clock>
// 	void format_string(Dst_Adapter& dst, auto& off, Placeholder const& ph, std::chrono::time_point<clock> const& time_point)
// 	{
// 		format_string(dst, off, ph, time_point.time_since_epoch());
// 	}

	template<class Dst_Adapter, class P>
	struct Argument_Parser
	{
		void execute(Dst_Adapter& dst, Placeholder const& ph, P const& p)
		{
			format_string(dst, ph, p);
		}
	};
}

#define FORMAT_BEGIN																			\
dst.clear();																					\
util::formatting::Format_String_Adapter<Format_String> fmt_adapter(fmt);						\
if (fmt_adapter.is_done())																		\
{																								\
	return;																						\
}																								\
																								\
do																								\
{																								\
	auto ch = fmt_adapter.get_and_advance();													\
	if (ch != '{')																				\
	{																							\
		dst.append(ch);																			\
		continue;																				\
	}																							\
																								\
	ch = fmt_adapter.get();																		\
	if (ch == '{') 																				\
	{																							\
		dst.append('{');																		\
		dst.append('{');																		\
		fmt_adapter.get_and_advance();															\
		continue;																				\
	}																							\
																								\
	uint8_t index;																				\
	bool ok = util::formatting::detail::parse_index(index, fmt_adapter);						\
	if (!ok)																					\
	{																							\
		break;																					\
	}																							\
	util::formatting::Placeholder ph;															\
	ch = fmt_adapter.get_and_advance();															\
	if (ch == ':')																				\
	{																							\
		ch = fmt_adapter.get_and_advance();														\
		if (ch == 'x')																			\
		{																						\
			ph.base = 16;																		\
			ch = fmt_adapter.get_and_advance();													\
		}																						\
		else if (ch == 'X')																		\
		{																						\
			ph.base_case = 1;																	\
			ph.base = 16;																		\
			ch = fmt_adapter.get_and_advance();													\
		}																						\
	}																							\
	if (ch != '}')																				\
	{																							\
		break;																					\
	}																							\
																								\
	switch (index)																				\
	{

#define FORMAT_PARAM(idx, T, p) case idx: util::formatting::Argument_Parser<Dst_String, T>().execute(dst, ph, p); break;

#define FORMAT_END																				\
	default:																					\
		break;																					\
	}																							\
} while (!fmt_adapter.is_done());


template<class Dst_String>
void format(Dst_String& dst, Dst_String const& fmt)
{
	dst = fmt;
}

template<class Dst_String, class Format_String>
void format(Dst_String& dst, Format_String const& fmt)
{
	dst = Dst_String(fmt);
}

template<class Dst_String, class Format_String, typename P0>
void format(Dst_String& dst, Format_String const& fmt, P0 const& p0)
{
	FORMAT_BEGIN
	FORMAT_PARAM(0, P0, p0)
	FORMAT_END
}

template<class Dst_String, class Format_String, typename P0, typename P1>
void format(Dst_String& dst, Format_String const& fmt, P0 const& p0, P1 const& p1)
{
	FORMAT_BEGIN
	FORMAT_PARAM(0, P0, p0)
	FORMAT_PARAM(1, P1, p1)
	FORMAT_END
}

template<class Dst_String, class Format_String, typename P0, typename P1, typename P2>
void format(Dst_String& dst, Format_String const& fmt, P0 const& p0, P1 const& p1, P2 const& p2)
{
	FORMAT_BEGIN
	FORMAT_PARAM(0, P0, p0)
	FORMAT_PARAM(1, P1, p1)
	FORMAT_PARAM(2, P2, p2)
	FORMAT_END
}

template<class Dst_String, class Format_String, typename P0, typename P1, typename P2, typename P3>
void format(Dst_String& dst, Format_String const& fmt, P0 const& p0, P1 const& p1, P2 const& p2, P3 const& p3)
{
	FORMAT_BEGIN
	FORMAT_PARAM(0, P0, p0)
	FORMAT_PARAM(1, P1, p1)
	FORMAT_PARAM(2, P2, p2)
	FORMAT_PARAM(3, P3, p3)
	FORMAT_END
}

template<class Dst_String, class Format_String, typename P0, typename P1, typename P2, typename P3, typename P4>
void format(Dst_String& dst, Format_String const& fmt, P0 const& p0, P1 const& p1, P2 const& p2, P3 const& p3, P4 const& p4)
{
	FORMAT_BEGIN
	FORMAT_PARAM(0, P0, p0)
	FORMAT_PARAM(1, P1, p1)
	FORMAT_PARAM(2, P2, p2)
	FORMAT_PARAM(3, P3, p3)
	FORMAT_PARAM(4, P4, p4)
	FORMAT_END
}

template<class Dst_String, class Format_String, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
void format(Dst_String& dst, Format_String const& fmt, P0 const& p0, P1 const& p1, P2 const& p2, P3 const& p3, P4 const& p4, P5 const& p5)
{
	FORMAT_BEGIN
	FORMAT_PARAM(0, P0, p0)
	FORMAT_PARAM(1, P1, p1)
	FORMAT_PARAM(2, P2, p2)
	FORMAT_PARAM(3, P3, p3)
	FORMAT_PARAM(4, P4, p4)
	FORMAT_PARAM(5, P5, p5)
	FORMAT_END
}

template<class Dst_String, class Format_String, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
void format(Dst_String& dst, Format_String const& fmt, P0 const& p0, P1 const& p1, P2 const& p2, P3 const& p3, P4 const& p4, P5 const& p5, P6 const& p6)
{
	FORMAT_BEGIN
	FORMAT_PARAM(0, P0, p0)
	FORMAT_PARAM(1, P1, p1)
	FORMAT_PARAM(2, P2, p2)
	FORMAT_PARAM(3, P3, p3)
	FORMAT_PARAM(4, P4, p4)
	FORMAT_PARAM(5, P5, p5)
	FORMAT_PARAM(6, P6, p6)
	FORMAT_END
}

template<class Dst_String, class Format_String, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
void format(Dst_String& dst, Format_String const& fmt, P0 const& p0, P1 const& p1, P2 const& p2, P3 const& p3, P4 const& p4, P5 const& p5, P6 const& p6, P7 const& p7)
{
	FORMAT_BEGIN
	FORMAT_PARAM(0, P0, p0)
	FORMAT_PARAM(1, P1, p1)
	FORMAT_PARAM(2, P2, p2)
	FORMAT_PARAM(3, P3, p3)
	FORMAT_PARAM(4, P4, p4)
	FORMAT_PARAM(5, P5, p5)
	FORMAT_PARAM(6, P6, p6)
	FORMAT_PARAM(7, P7, p7)
	FORMAT_END
}
template<class Dst_String, class Format_String, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
void format(Dst_String& dst, Format_String const& fmt, P0 const& p0, P1 const& p1, P2 const& p2, P3 const& p3, P4 const& p4, P5 const& p5, P6 const& p6, P7 const& p7, P8 const& p8)
{
	FORMAT_BEGIN
	FORMAT_PARAM(0, P0, p0)
	FORMAT_PARAM(1, P1, p1)
	FORMAT_PARAM(2, P2, p2)
	FORMAT_PARAM(3, P3, p3)
	FORMAT_PARAM(4, P4, p4)
	FORMAT_PARAM(5, P5, p5)
	FORMAT_PARAM(6, P6, p6)
	FORMAT_PARAM(7, P7, p7)
	FORMAT_PARAM(8, P8, p8)
	FORMAT_END
}
template<class Dst_String, class Format_String, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
void format(Dst_String& dst, Format_String const& fmt, P0 const& p0, P1 const& p1, P2 const& p2, P3 const& p3, P4 const& p4, P5 const& p5, P6 const& p6, P7 const& p7, P8 const& p8, P9 const& p9)
{
	FORMAT_BEGIN
	FORMAT_PARAM(0, P0, p0)
	FORMAT_PARAM(1, P1, p1)
	FORMAT_PARAM(2, P2, p2)
	FORMAT_PARAM(3, P3, p3)
	FORMAT_PARAM(4, P4, p4)
	FORMAT_PARAM(5, P5, p5)
	FORMAT_PARAM(6, P6, p6)
	FORMAT_PARAM(7, P7, p7)
	FORMAT_PARAM(8, P8, p8)
	FORMAT_PARAM(9, P9, p9)
	FORMAT_END
}

#undef FORMAT_BEGIN
#undef FORMAT_PARAM
#undef FORMAT_END

}
	