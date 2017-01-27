#pragma once

namespace q
{
namespace util
{
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
        std::vector<std::pair<size_t, size_t>> tokens;
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
