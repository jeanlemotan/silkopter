#pragma once

namespace q
{
namespace util
{

	inline int toupper_ascii(int c)
	{
		return (c >= 'a' && c <= 'z') ? c & (~32) : c;
	}
	inline int tolower_ascii(int c)
	{
		return (c >= 'A' && c <= 'Z') ? c | 32 : c;
	}
	inline size_t strlen(char const* s)
	{
		return ::strlen(s);
	}
	inline int strcmp(char const* s1, char const* s2)
	{
		return ::strcmp(s1, s2); //this should be implemented in ASM by the OS
	}
	inline int memcmp(char const* s1, char const* s2, size_t n)
	{
		return ::memcmp(s1, s2, n); //this should be implemented in ASM by the OS
	}
	inline char const* memchr(char const* s, char ch, size_t n)
	{
		return (char const*)::memchr(s, ch, n);
	}
	inline char const* strstr(char const* in, char const* str)
	{
		return ::strstr(in, str);
	}

	extern int stricmp(char const* s1, char const* s2);
	extern int strnicmp(char const* s1, char const* s2, size_t len);
	extern int memicmp(char const* s1, char const* s2, size_t n);
	extern char const* memichr(char const* s, char ch, size_t n);
	extern char const* stristr(char const* in, char const* str);

	//-----------------------------------------------------------------------------
	// MurmurHash2, by Austin Appleby

	// Note - This code makes a few assumptions about how your machine behaves -

	// 1. We can read a 4-byte value from any address without crashing
	// 2. sizeof(int) == 4

	// And it has a few limitations -

	// 1. It will not work incrementally.
	// 2. It will not produce the same results on little-endian and big-endian
	//    machines.

	extern uint32_t murmur_hash(const void* key, int length, uint32_t seed);
	extern uint32_t murmur_hash_cs(const void* key, int length, uint32_t seed);

	extern void memcpy(uint8_t* dst, size_t dstStrideBytes, uint8_t const* src, size_t srcStrideBytes, size_t elementSizeBytes, size_t count);


	template<class T>
	std::pair<size_t, size_t> find_line_char_by_offset(T const& str, size_t off)
	{
		size_t line = 1;
		size_t pos = 0;
		size_t start = pos;
		do
		{
			pos = str.find_first_of("\n", pos + 1);
			//pos = str.find_first_not_of("\n", pos);
			if (pos <= off)
			{
				start = pos;
				line++;
			}
		} while (pos != T::npos && pos <= off);

		return std::make_pair(line, off - start);
	}

	template<class T>
	T get_line_annotated_string(T const& str)
	{
		T res;
		if (str.empty())
		{
			return res;
		}
		res.reserve(str.size() * 2);
		size_t line = 1;
		size_t pos = 0;
		bool done = false;
		do
		{
			size_t last = pos == 0 ? 0 : pos + 1;
			pos = str.find_first_of("\n", pos + 1);
			char buf[32];
            sprintf(buf, "%d:", static_cast<int>(line));
			res += buf;
			if (pos == T::npos)
			{
				pos = str.size() - 1;
				done = true;
			}
			res += str.substr(last, pos - last + 1);
			line++;
		} while (!done);

		return res;
	}

	template<class T>
	std::vector<std::pair<size_t, size_t>> tokenize_range(T const& str, T const& delimiters)
	{
		std::vector<T> tokens;
		tokens.reserve(1000);

		size_t p0 = 0, p1 = T::npos;
		p0 = str.find_first_not_of(delimiters, p0);
		while (p0 != T::npos)
		{
			p1 = str.find_first_of(delimiters, p0);
			if (p1 == T::npos)
			{
				p1 = str.size();
			}
			if (p1 != p0)
			{
				tokens.emplace_back(p0, p1 - p0);
			}
			p0 = str.find_first_not_of(delimiters, p1);
		}
		return tokens;		
	}
	template<class T>
	std::vector<T> tokenize(T const& str, T const& delimiters)
	{
		std::vector<T> tokens;
		tokens.reserve(1000);

		size_t p0 = 0, p1 = T::npos;
		p0 = str.find_first_not_of(delimiters, p0);
		while (p0 != T::npos)
		{
			p1 = str.find_first_of(delimiters, p0);
			if (p1 == T::npos)
			{
				p1 = str.size();
			}
			if (p1 != p0)
			{
				tokens.emplace_back(str.substr(p0, p1 - p0));
			}
			p0 = str.find_first_not_of(delimiters, p1);
		}
		return tokens;		
	}

}
}
