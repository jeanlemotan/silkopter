#pragma once

#include <string>

namespace util
{

//fixed size string
template<size_t SIZE>
class String
{
public:

	typedef char value_type;
	typedef char const* const_iterator;
	enum { MAX_SIZE = SIZE };

public:
	String();
	String(String<SIZE> const& str);
	String(String<SIZE> const& str, size_t offset, size_t count);
	String(char const* from, char const* to);
	String(char const* str);
	String(char const* str, size_t count);
	explicit String(std::string const& str);

	auto operator==(String<SIZE> const& str) const -> bool;
	auto operator!=(String<SIZE> const& str) const -> bool;

	auto operator==(std::string const& str) const -> bool;
	auto operator!=(std::string const& str) const -> bool;

	auto operator==(char const* str) const -> bool;
	auto operator!=(char const* str) const -> bool;

	auto operator<(String<SIZE> const& str) const -> bool;

	auto operator=(String<SIZE> const& str) -> String<SIZE>&;
	auto operator=(char const* str) -> String<SIZE>&;
	auto operator=(std::string const& str) -> String<SIZE>&;

	auto operator+(String<SIZE> const& str) const -> String<SIZE>;
	auto operator+=(String<SIZE> const& str) -> String<SIZE>&;
	auto operator+(char const* cstr) const -> String<SIZE>;
	auto operator+=(char const* cstr) -> String<SIZE>&;
	auto operator+(char ch) const -> String<SIZE>;
	auto operator+=(char ch) -> String<SIZE>&;

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

	auto substr(size_t off = 0, size_t count = npos) const -> String<SIZE>;

	void push_back(char ch);

	void append(char ch);
	void append(char const* cstr);
	void append(char const* from, char const* to);
	void append(String<SIZE> const& str);

	auto find(char ch, size_t off = 0) const -> size_t;
	auto find(char const* cstr, size_t off = 0) const -> size_t;
	auto find(String<SIZE> const& str, size_t off = 0) const -> size_t;

	auto find_first_of(char ch, size_t off = 0) const -> size_t;
	auto find_first_of(char const* cstr, size_t off = 0) const -> size_t;
	auto find_first_of(String<SIZE> const& str, size_t off = 0) const -> size_t;

	auto find_first_not_of(char ch, size_t off = 0) const -> size_t;
	auto find_first_not_of(char const* cstr, size_t off = 0) const -> size_t;
	auto find_first_not_of(String<SIZE> const& str, size_t off = 0) const -> size_t;

	auto find_last_of(char ch, size_t off = npos) const -> size_t;
	auto find_last_of(char const* cstr, size_t off = npos) const -> size_t;
	auto find_last_of(String<SIZE> const& str, size_t off = npos) const -> size_t;

	auto find_last_not_of(char ch, size_t off = npos) const -> size_t;
	auto find_last_not_of(char const* cstr, size_t off = npos) const -> size_t;
	auto find_last_not_of(String<SIZE> const& str, size_t off = npos) const -> size_t;

	auto begin() const -> const_iterator;
	auto end() const -> const_iterator;

	//////////////////////////////////////////////////////////////////////////
	//specific methods

	static size_t const npos = (size_t)(-1);

protected:
	char m_data[SIZE + 1]; //+1 for the ending zero
	size_t m_size;
};

//////////////////////////////////////////////////////////////////////////

template<size_t SIZE>
inline String<SIZE>::String()
	: m_size(0)
{
	m_data[0] = 0;
}

template<size_t SIZE>
inline String<SIZE>::String(String<SIZE> const& str)
	: m_size(str.m_size)
{
	memcpy(m_data, str.m_data, m_size + 1); //copy ending zero as well
}

template<size_t SIZE>
inline String<SIZE>::String(String<SIZE> const& str, size_t offset, size_t count)
{
	offset = std::min(offset, str.size());
	count = std::min(count, str.size() - offset);
	m_size = std::min(count, SIZE);
	memcpy(m_data, str.m_data, m_size); //ending zero as well
}

template<size_t SIZE>
inline String<SIZE>::String(char const* str)
{
	char const* ptr = str ? str : "";
	m_size = std::min(strlen(ptr), SIZE);
	memcpy(m_data, ptr, m_size + 1); //ending zero as well
}

template<size_t SIZE>
inline String<SIZE>::String(char const* str, size_t count)
{
	char const* ptr = str ? str : "";
	m_size = std::min(count, SIZE);
	memcpy(m_data, ptr, m_size);
	m_data[m_size] = 0;
}

template<size_t SIZE>
inline String<SIZE>::String(char const* from, char const* to)
	: m_size(to - from)
{
	m_size = std::min(m_size, SIZE);
	memcpy(m_data, from, m_size);
	m_data[m_size] = 0;
}

template<size_t SIZE>
inline String<SIZE>::String(std::string const& str)
	: m_size(0)
{
	m_data[0] = 0;
	*this = str;
}

template<size_t SIZE>
inline auto String<SIZE>::operator=(std::string const& str) -> String<SIZE>&
{
	if (str.empty())
	{
		clear();
		return *this;
	}

	char const* cstr = str.c_str();
	*this = String<SIZE>(cstr, cstr + str.length());
	return *this;
}

template<size_t SIZE>
inline auto String<SIZE>::operator=(String const& str) -> String<SIZE>&
{
	m_size = str.m_size;
	memcpy(m_data, str.m_data, m_size + 1); //copy ending zero as well
	return *this;
}

template<size_t SIZE>
inline auto String<SIZE>::operator=(char const* str) -> String<SIZE>&
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
inline auto String<SIZE>::operator<(String<SIZE> const& str) const -> bool
{
	return strcmp(c_str(), str.c_str()) < 0;
}

template<size_t SIZE>
inline auto String<SIZE>::operator==(String<SIZE> const& str) const -> bool
{
	return m_size == str.m_size && !memcmp(m_data, str.m_data, m_size);
}

template<size_t SIZE>
inline auto String<SIZE>::operator!=(String<SIZE> const& str) const -> bool
{
	return !operator==(str);
}

template<size_t SIZE>
inline auto String<SIZE>::operator==(std::string const& str) const -> bool
{
	return m_size == str.size() && !memcmp(m_data, str.c_str(), m_size);
}

template<size_t SIZE>
inline auto String<SIZE>::operator!=(std::string const& str) const -> bool
{
	return !operator==(str);
}

template<size_t SIZE>
inline auto String<SIZE>::operator==(char const* str) const -> bool
{
	return !strcmp(c_str(), str);
}

template<size_t SIZE>
inline auto String<SIZE>::operator!=(char const* str) const -> bool
{
	return !operator==(str);
}

template<size_t SIZE>
inline auto String<SIZE>::operator+(String<SIZE> const& str) const -> String<SIZE>
{
	String ret(*this);
	ret.append(str);
	return ret;
}

template<size_t SIZE>
inline auto String<SIZE>::operator+=(String<SIZE> const& str) -> String<SIZE>&
{
	append(str);
	return *this;
}

template<size_t SIZE>
inline auto String<SIZE>::operator+(char const* cstr) const -> String<SIZE>
{
	String ret(*this);
	ret.append(cstr);
	return ret;
}

template<size_t SIZE>
inline auto String<SIZE>::operator+=(char const* cstr) -> String<SIZE>&
{
	append(cstr);
	return *this;
}

template<size_t SIZE>
inline auto String<SIZE>::operator+(char ch) const -> String<SIZE>
{
	String ret(*this);
	ret.append(ch);
	return ret;
}

template<size_t SIZE>
inline auto String<SIZE>::operator+=(char ch) -> String<SIZE>&
{
	append(ch);
	return *this;
}

template<size_t SIZE>
inline auto	String<SIZE>::operator[](size_t idx) const -> char
{
	return m_data[idx];
}

template<size_t SIZE>
inline auto	String<SIZE>::operator[](size_t idx) -> char&
{
	return m_data[idx];
}

template<size_t SIZE>
inline void String<SIZE>::push_back(char ch)
{
	append(ch);
}

template<size_t SIZE>
inline auto String<SIZE>::c_str() const -> char const*
{
	return m_data;
}

template<size_t SIZE>
inline void String<SIZE>::clear()
{
	m_size = 0;
	m_data[0] = 0;
}

template<size_t SIZE>
inline auto String<SIZE>::length() const -> size_t
{
	return m_size;
}

template<size_t SIZE>
inline auto String<SIZE>::size() const -> size_t
{
	return m_size;
}

template<size_t SIZE>
inline auto String<SIZE>::empty() const -> bool
{
	return m_size == 0;
}

template<size_t SIZE>
inline auto String<SIZE>::substr(size_t off, size_t count) const -> String<SIZE>
{
	if (count == npos)
	{
		count = size() - off;
	}

	if (count == 0)
	{
		return String<SIZE>();
	}
	
	count = std::min(count, SIZE);

	String<SIZE> ret;
	ret.m_size = count;
	memcpy(ret.m_data, c_str() + off, count + 1); //ending zero as well
	return ret;
}

template<size_t SIZE>
inline auto String<SIZE>::find(char ch, size_t off) const -> size_t
{
	return find_first_of(ch, off);
}

template<size_t SIZE>
inline auto String<SIZE>::find(char const* cstr, size_t off) const -> size_t
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
inline auto String<SIZE>::find(String<SIZE> const& str, size_t off) const -> size_t
{
	return find(str.c_str(), off);
}

template<size_t SIZE>
inline auto String<SIZE>::begin() const -> const_iterator
{
	return empty() ? 0 : c_str();
}

template<size_t SIZE>
inline auto String<SIZE>::end() const -> const_iterator
{
	return empty() ? 0 : c_str() + size();
}

template<size_t SIZE>
inline void String<SIZE>::reserve(size_t)
{
	; //nothing to do
}

template<size_t SIZE>
inline void String<SIZE>::resize(size_t s)
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
void String<SIZE>::append(char ch)
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
void String<SIZE>::append(char const* cstr)
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
void String<SIZE>::append(char const* from, char const* to)
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
	memcpy(m_data + m_size, from, cstrlen);
	m_size += cstrlen;
	m_data[m_size] = 0;
}

template<size_t SIZE>
void String<SIZE>::append(String<SIZE> const& str)
{
	append(str.m_data, str.m_data + str.m_size);
}

template<size_t SIZE>
auto String<SIZE>::find_first_of(char ch, size_t off) const -> size_t
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
auto String<SIZE>::find_first_of(char const* cstr, size_t off) const -> size_t
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
auto String<SIZE>::find_first_of(String<SIZE> const& str, size_t off) const -> size_t
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
auto String<SIZE>::find_first_not_of(char ch, size_t off) const -> size_t
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
auto String<SIZE>::find_first_not_of(char const* cstr, size_t off) const -> size_t
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
auto String<SIZE>::find_first_not_of(String<SIZE> const& str, size_t off) const -> size_t
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
auto String<SIZE>::find_last_of(char ch, size_t pos) const -> size_t
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
auto String<SIZE>::find_last_of(char const* cstr, size_t pos) const -> size_t
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
auto String<SIZE>::find_last_of(String<SIZE> const& str, size_t pos) const -> size_t
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
auto String<SIZE>::find_last_not_of(char ch, size_t pos) const -> size_t
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
auto String<SIZE>::find_last_not_of(char const* cstr, size_t pos) const -> size_t
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
auto String<SIZE>::find_last_not_of(String<SIZE> const& str, size_t pos) const -> size_t
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
inline auto operator+(char const* cstr, String<SIZE> const& str) -> String<SIZE>
{
	//first some early outs
	if (str.empty())
	{
		return (cstr == 0 || cstr[0] == '\0') ? String<SIZE>() : String<SIZE>(cstr);
	}
	if (cstr == 0 || cstr[0] == '\0')
	{
		return str;
	}
	
	size_t cstrlen = strlen(cstr);
	size_t strsize = std::min(str.m_size, SIZE - cstrlen);
	String<SIZE> ret;
	ret.m_size = cstrlen + strsize;
	memcpy(ret.m_data, cstr, cstrlen);
	memcpy(ret.m_data + cstrlen, str.c_str(), strsize + 1); //end zero
	return ret;
}

//global operator== for char const* and String
template<size_t SIZE>
inline auto operator==(char const* cstr, String<SIZE> const& str) -> bool
{
	return str == cstr;
}

}
