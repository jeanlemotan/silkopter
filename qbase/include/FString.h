#pragma once

#include <string>
#include <cstring>

namespace q
{
	

//fixed size string
template<size_t SIZE>
class FString
{
public:

	typedef char value_type;
	typedef char const* const_iterator;
	enum { MAX_SIZE = SIZE };

public:
	FString();
	FString(FString<SIZE> const& str);
	explicit FString(FString<SIZE> const& str, size_t offset, size_t count);
	explicit FString(char const* from, char const* to);
	explicit FString(char const* str);
	explicit FString(char const* str, size_t count);
	explicit FString(std::string const& str);

	auto operator==(FString<SIZE> const& str) const -> bool;
	auto operator!=(FString<SIZE> const& str) const -> bool;

	auto operator==(std::string const& str) const -> bool;
	auto operator!=(std::string const& str) const -> bool;

	auto operator==(char const* str) const -> bool;
	auto operator!=(char const* str) const -> bool;

	auto operator<(FString<SIZE> const& str) const -> bool;

	auto operator=(FString<SIZE> const& str) -> FString<SIZE>&;
	auto operator=(char const* str) -> FString<SIZE>&;
	auto operator=(std::string const& str) -> FString<SIZE>&;

	auto operator+(FString<SIZE> const& str) const -> FString<SIZE>;
	auto operator+=(FString<SIZE> const& str) -> FString<SIZE>&;
	auto operator+(char const* cstr) const -> FString<SIZE>;
	auto operator+=(char const* cstr) -> FString<SIZE>&;
	auto operator+(char ch) const -> FString<SIZE>;
	auto operator+=(char ch) -> FString<SIZE>&;

	auto operator[](size_t idx) const -> char;
	auto operator[](size_t idx) -> char&;

	//////////////////////////////////////////////////////////////////////////
	//equivalents of std::string
	auto c_str() const -> char const*;
	auto size() const -> size_t;
	auto length() const -> size_t;
	auto empty() const -> bool;
	void clear();
	void reserve(size_t size);
	void resize(size_t size);
	auto data() const -> char const*;
	auto data() -> char*;

	auto substr(size_t off = 0, size_t count = npos) const -> FString<SIZE>;

	void push_back(char ch);

	void append(char ch);
	void append(char const* cstr);
	void append(char const* from, char const* to);
	void append(FString<SIZE> const& str);

	auto find(char ch, size_t off = 0) const -> size_t;
	auto find(char const* cstr, size_t off = 0) const -> size_t;
	auto find(FString<SIZE> const& str, size_t off = 0) const -> size_t;

	auto find_first_of(char ch, size_t off = 0) const -> size_t;
	auto find_first_of(char const* cstr, size_t off = 0) const -> size_t;
	auto find_first_of(FString<SIZE> const& str, size_t off = 0) const -> size_t;

	auto find_first_not_of(char ch, size_t off = 0) const -> size_t;
	auto find_first_not_of(char const* cstr, size_t off = 0) const -> size_t;
	auto find_first_not_of(FString<SIZE> const& str, size_t off = 0) const -> size_t;

	auto find_last_of(char ch, size_t off = npos) const -> size_t;
	auto find_last_of(char const* cstr, size_t off = npos) const -> size_t;
	auto find_last_of(FString<SIZE> const& str, size_t off = npos) const -> size_t;

	auto find_last_not_of(char ch, size_t off = npos) const -> size_t;
	auto find_last_not_of(char const* cstr, size_t off = npos) const -> size_t;
	auto find_last_not_of(FString<SIZE> const& str, size_t off = npos) const -> size_t;

	auto begin() const -> const_iterator;
	auto end() const -> const_iterator;

	//////////////////////////////////////////////////////////////////////////
	//specific methods

	static size_t const npos = (size_t)(-1);

protected:
	char m_data[SIZE + 1]; //+1 for the ending zero
	size_t m_size = 0;
};

//////////////////////////////////////////////////////////////////////////

template<size_t SIZE>
inline FString<SIZE>::FString()
{
	m_data[0] = 0;
}

template<size_t SIZE>
inline FString<SIZE>::FString(FString<SIZE> const& str)
	: m_size(str.m_size)
{
	memcpy(m_data, str.m_data, m_size + 1); //copy ending zero as well
}

template<size_t SIZE>
inline FString<SIZE>::FString(FString<SIZE> const& str, size_t offset, size_t count)
{
	offset = std::min(offset, str.size());
	count = std::min(count, str.size() - offset);
	m_size = std::min(count, SIZE);
	memcpy(m_data, str.m_data, m_size); //ending zero as well
}

template<size_t SIZE>
inline FString<SIZE>::FString(char const* str)
{
	char const* ptr = str ? str : "";
	m_size = std::min(strlen(ptr), SIZE);
	memcpy(m_data, ptr, m_size + 1); //ending zero as well
}

template<size_t SIZE>
inline FString<SIZE>::FString(char const* str, size_t count)
{
	char const* ptr = str ? str : "";
	m_size = std::min(count, SIZE);
	memcpy(m_data, ptr, m_size);
	m_data[m_size] = 0;
}

template<size_t SIZE>
inline FString<SIZE>::FString(char const* from, char const* to)
	: m_size(to - from)
{
	m_size = std::min(m_size, SIZE);
	memcpy(m_data, from, m_size);
	m_data[m_size] = 0;
}

template<size_t SIZE>
inline FString<SIZE>::FString(std::string const& str)
{
	m_data[0] = 0;
	*this = str;
}

template<size_t SIZE>
inline auto FString<SIZE>::operator=(std::string const& str) -> FString<SIZE>&
{
	if (str.empty())
	{
		clear();
		return *this;
	}

	char const* cstr = str.c_str();
	*this = FString<SIZE>(cstr, cstr + str.length());
	return *this;
}

template<size_t SIZE>
inline auto FString<SIZE>::operator=(FString const& str) -> FString<SIZE>&
{
	m_size = str.m_size;
	memcpy(m_data, str.m_data, m_size + 1); //copy ending zero as well
	return *this;
}

template<size_t SIZE>
inline auto FString<SIZE>::operator=(char const* str) -> FString<SIZE>&
{
	if (!str || !*str)
	{
		clear();
		return *this;
	}

	size_t length = std::min((size_t)strlen(str), SIZE);
	m_size = length;
	memcpy(m_data, str, m_size + 1); //copy ending zero as well
	return *this;
}

template<size_t SIZE>
inline auto FString<SIZE>::operator<(FString<SIZE> const& str) const -> bool
{
	return strcmp(c_str(), str.c_str()) < 0;
}

template<size_t SIZE>
inline auto FString<SIZE>::operator==(FString<SIZE> const& str) const -> bool
{
	return m_size == str.m_size && !memcmp(m_data, str.m_data, m_size);
}

template<size_t SIZE>
inline auto FString<SIZE>::operator!=(FString<SIZE> const& str) const -> bool
{
	return !operator==(str);
}

template<size_t SIZE>
inline auto FString<SIZE>::operator==(std::string const& str) const -> bool
{
	return m_size == str.size() && !memcmp(m_data, str.c_str(), m_size);
}

template<size_t SIZE>
inline auto FString<SIZE>::operator!=(std::string const& str) const -> bool
{
	return !operator==(str);
}

template<size_t SIZE>
inline auto FString<SIZE>::operator==(char const* str) const -> bool
{
	return !strcmp(c_str(), str);
}

template<size_t SIZE>
inline auto FString<SIZE>::operator!=(char const* str) const -> bool
{
	return !operator==(str);
}

template<size_t SIZE>
inline auto FString<SIZE>::operator+(FString<SIZE> const& str) const -> FString<SIZE>
{
	FString ret(*this);
	ret.append(str);
	return ret;
}

template<size_t SIZE>
inline auto FString<SIZE>::operator+=(FString<SIZE> const& str) -> FString<SIZE>&
{
	append(str);
	return *this;
}

template<size_t SIZE>
inline auto FString<SIZE>::operator+(char const* cstr) const -> FString<SIZE>
{
	FString ret(*this);
	ret.append(cstr);
	return ret;
}

template<size_t SIZE>
inline auto FString<SIZE>::operator+=(char const* cstr) -> FString<SIZE>&
{
	append(cstr);
	return *this;
}

template<size_t SIZE>
inline auto FString<SIZE>::operator+(char ch) const -> FString<SIZE>
{
	FString ret(*this);
	ret.append(ch);
	return ret;
}

template<size_t SIZE>
inline auto FString<SIZE>::operator+=(char ch) -> FString<SIZE>&
{
	append(ch);
	return *this;
}

template<size_t SIZE>
inline auto	FString<SIZE>::operator[](size_t idx) const -> char
{
	return m_data[idx];
}

template<size_t SIZE>
inline auto	FString<SIZE>::operator[](size_t idx) -> char&
{
	return m_data[idx];
}

template<size_t SIZE>
inline void FString<SIZE>::push_back(char ch)
{
	append(ch);
}

template<size_t SIZE>
inline auto FString<SIZE>::c_str() const -> char const*
{
	return m_data;
}

template<size_t SIZE>
inline void FString<SIZE>::clear()
{
	m_size = 0;
	m_data[0] = 0;
}

template<size_t SIZE>
inline auto FString<SIZE>::length() const -> size_t
{
	return m_size;
}

template<size_t SIZE>
inline auto FString<SIZE>::size() const -> size_t
{
	return m_size;
}

template<size_t SIZE>
inline auto FString<SIZE>::empty() const -> bool
{
	return m_size == 0;
}

template<size_t SIZE>
inline auto FString<SIZE>::substr(size_t off, size_t count) const -> FString<SIZE>
{
	if (count == npos)
	{
		count = size() - off;
	}

	if (count == 0)
	{
		return FString<SIZE>();
	}
	
	count = std::min(count, SIZE);

	FString<SIZE> ret;
	ret.m_size = count;
	memcpy(ret.m_data, c_str() + off, count + 1); //ending zero as well
	return ret;
}

template<size_t SIZE>
inline auto FString<SIZE>::find(char ch, size_t off) const -> size_t
{
	return find_first_of(ch, off);
}

template<size_t SIZE>
inline auto FString<SIZE>::find(char const* cstr, size_t off) const -> size_t
{
	if (!cstr || cstr[0] == '\0')
	{
		return 0;
	}
	if (off >= size())
	{
		return npos;
	}

	char const* string = c_str();
	char const* c = strstr(string + off, cstr);
	return c ? c - string : npos;
}

template<size_t SIZE>
inline auto FString<SIZE>::find(FString<SIZE> const& str, size_t off) const -> size_t
{
	return find(str.c_str(), off);
}

template<size_t SIZE>
inline auto FString<SIZE>::begin() const -> const_iterator
{
	return empty() ? 0 : c_str();
}

template<size_t SIZE>
inline auto FString<SIZE>::end() const -> const_iterator
{
	return empty() ? 0 : c_str() + size();
}

template<size_t SIZE>
inline void FString<SIZE>::reserve(size_t)
{
	; //nothing to do
}

template<size_t SIZE>
inline void FString<SIZE>::resize(size_t s)
{
	s = std::min(s, SIZE);
	if (m_size == s)
	{
		return;
	}

	m_size = s;
	m_data[m_size] = 0;
}

template<size_t SIZE>
auto FString<SIZE>::data() const -> char const*
{
	return m_data;
}

template<size_t SIZE>
auto FString<SIZE>::data() -> char*
{
	return m_data;
}

template<size_t SIZE>
void FString<SIZE>::append(char ch)
{
	auto s = std::min(m_size + 1, SIZE);
	if (s == m_size)
	{
		return;
	}
	m_data[m_size] = ch;
	m_size = s;
	m_data[s] = 0;
}

template<size_t SIZE>
void FString<SIZE>::append(char const* cstr)
{
	if (!cstr)
	{
		return;
	}
	size_t cstrlen = strlen(cstr);
	cstrlen = std::min(cstrlen, SIZE - m_size);
	if (cstrlen == 0)
	{
		return;
	}
	memcpy(m_data + m_size, cstr, cstrlen + 1); //end zero as well
	m_size += cstrlen;
}

template<size_t SIZE>
void FString<SIZE>::append(char const* from, char const* to)
{
	if (!from || !to)
	{
		return;
	}
	size_t cstrlen = to - from;
	cstrlen = std::min(cstrlen, SIZE - m_size);
	if (cstrlen == 0)
	{
		return;
	}
	::memcpy(m_data + m_size, from, cstrlen);
	m_size += cstrlen;
	m_data[m_size] = 0;
}

template<size_t SIZE>
void FString<SIZE>::append(FString<SIZE> const& str)
{
	append(str.m_data, str.m_data + str.m_size);
}

template<size_t SIZE>
auto FString<SIZE>::find_first_of(char ch, size_t off) const -> size_t
{
	if (off >= size())
	{
		return npos;
	}
	size_t s = size();
	char const* begin = c_str();
	char const* end = c_str() + s;
	for (char const* string = begin + off; string < end; string++)
	{
		if (*string == ch)
		{
			return string - begin;
		}
	}
	return npos;
}

template<size_t SIZE>
auto FString<SIZE>::find_first_of(char const* cstr, size_t off) const -> size_t
{
	if (!cstr || !*cstr)
	{
		return off;
	}
	if (off >= size())
	{
		return npos;
	}
	size_t s = size();
	size_t l = strlen(cstr);
	char const* begin = c_str();
	char const* end = c_str() + s;
	for (char const* string = begin + off; string < end; string++)
	{
		char ch = *string;
		if (memchr(cstr, ch, l) != 0)
		{
			return string - begin;
		}
	}
	return npos;
}

template<size_t SIZE>
auto FString<SIZE>::find_first_of(FString<SIZE> const& str, size_t off) const -> size_t
{
	if (str.empty())
	{
		return off;
	}
	if (off >= size())
	{
		return npos;
	}
	size_t s = size();
	size_t l = str.size();
	char const* cstr = str.c_str();
	char const* begin = c_str();
	char const* end = c_str() + s;
	for (char const* string = begin + off; string < end; string++)
	{
		char ch = *string;
		if (memchr(cstr, ch, l) != 0)
		{
			return string - begin;
		}
	}
	return npos;
}

template<size_t SIZE>
auto FString<SIZE>::find_first_not_of(char ch, size_t off) const -> size_t
{
	if (off >= size())
	{
		return npos;
	}
	size_t s = size();
	char const* begin = c_str();
	char const* end = c_str() + s;
	for (char const* string = begin + off; string < end; string++)
	{
		if (*string != ch)
		{
			return string - begin;
		}
	}
	return npos;
}

template<size_t SIZE>
auto FString<SIZE>::find_first_not_of(char const* cstr, size_t off) const -> size_t
{
	if (!cstr || !*cstr)
	{
		return off;
	}
	if (off >= size())
	{
		return npos;
	}
	size_t s = size();
	size_t l = strlen(cstr);
	char const* begin = c_str();
	char const* end = c_str() + s;
	for (char const* string = begin + off; string < end; string++)
	{
		char ch = *string;
		if (memchr(cstr, ch, l) == 0)
		{
			return string - begin;
		}
	}
	return npos;
}

template<size_t SIZE>
auto FString<SIZE>::find_first_not_of(FString<SIZE> const& str, size_t off) const -> size_t
{
	if (str.empty())
	{
		return off;
	}
	if (off >= size())
	{
		return npos;
	}
	size_t s = size();
	size_t l = str.size();
	char const* cstr = str.c_str();
	char const* begin = c_str();
	char const* end = c_str() + s;
	for (char const* string = begin + off; string < end; string++)
	{
		char ch = *string;
		if (memchr(cstr, ch, l) == 0)
		{
			return string - begin;
		}
	}
	return npos;
}

//////////////////////////////////////////////////////////////////////////

template<size_t SIZE>
auto FString<SIZE>::find_last_of(char ch, size_t pos) const -> size_t
{
	if (empty())
	{
		return npos;
	}
	if (pos >= size())
	{
		pos = size() - 1;
	}
	size_t i = pos;
	char const* string = c_str();
	do
	{
		if (string[i] == ch)
		{
			return i;
		}
	} while(i-- > 0);

	return npos;
}

template<size_t SIZE>
auto FString<SIZE>::find_last_of(char const* cstr, size_t pos) const -> size_t
{
	if (empty())
	{
		return npos;
	}
	if (pos >= size())
	{
		pos = size() - 1;
	}
	if (!cstr || !*cstr)
	{
		return pos;
	}
	size_t l = strlen(cstr);
	size_t i = pos;
	char const* string = c_str();
	do
	{
		char ch = string[i];
		if (memchr(cstr, ch, l) != 0)
		{
			return i;
		}
	} while(i-- > 0);
	return npos;
}

template<size_t SIZE>
auto FString<SIZE>::find_last_of(FString<SIZE> const& str, size_t pos) const -> size_t
{
	if (empty())
	{
		return npos;
	}
	if (str.empty())
	{
		return pos;
	}
	if (pos >= size())
	{
		pos = size() - 1;
	}
	size_t l = str.size();
	char const* cstr = str.c_str();
	size_t i = pos;
	char const* string = c_str();
	do
	{
		char ch = string[i];
		if (memchr(cstr, ch, l) != 0)
		{
			return i;
		}
	} while(i-- > 0);
	return npos;
}

template<size_t SIZE>
auto FString<SIZE>::find_last_not_of(char ch, size_t pos) const -> size_t
{
	if (empty())
	{
		return npos;
	}
	if (pos >= size())
	{
		pos = size() - 1;
	}
	size_t i = pos;
	char const* string = c_str();
	do
	{
		if (string[i] != ch)
		{
			return i;
		}
	} while(i-- > 0);

	return npos;
}

template<size_t SIZE>
auto FString<SIZE>::find_last_not_of(char const* cstr, size_t pos) const -> size_t
{
	if (!cstr || empty())
	{
		return npos;
	}
	if (pos >= size())
	{
		pos = size() - 1;
	}
	size_t l = strlen(cstr);
	size_t i = pos;
	char const* string = c_str();
	do
	{
		char ch = string[i];
		if (memchr(cstr, ch, l) == 0)
		{
			return i;
		}
	} while(i-- > 0);
	return npos;
}

template<size_t SIZE>
auto FString<SIZE>::find_last_not_of(FString<SIZE> const& str, size_t pos) const -> size_t
{
	if (empty())
	{
		return npos;
	}
	if (str.empty())
	{
		return pos;
	}
	if (pos >= size())
	{
		pos = size() - 1;
	}
	size_t l = str.size();
	char const* cstr = str.c_str();
	size_t i = pos;
	char const* string = c_str();
	do
	{
		char ch = string[i];
		if (memchr(cstr, ch, l) == 0)
		{
			return i;
		}
	} while(i-- > 0);
	return npos;
}

//global operator+ for char const* + String
//used for expressions like: String s = "my char" + name;
template<size_t SIZE>
inline auto operator+(char const* cstr, FString<SIZE> const& str) -> FString<SIZE>
{
	//first some early outs
	if (str.empty())
	{
		return (cstr == 0 || cstr[0] == '\0') ? FString<SIZE>() : FString<SIZE>(cstr);
	}
	if (cstr == 0 || cstr[0] == '\0')
	{
		return str;
	}
	
	size_t cstrlen = strlen(cstr);
	size_t strsize = std::min(str.m_size, SIZE - cstrlen);
	FString<SIZE> ret;
	ret.m_size = cstrlen + strsize;
	memcpy(ret.m_data, cstr, cstrlen);
	memcpy(ret.m_data + cstrlen, str.c_str(), strsize + 1); //end zero
	return ret;
}

//global operator== for char const* and String
template<size_t SIZE>
inline auto operator==(char const* cstr, FString<SIZE> const& str) -> bool
{
	return str == cstr;
}

}
