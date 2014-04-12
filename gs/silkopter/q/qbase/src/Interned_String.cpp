#include "QBaseStdAfx.h"
#include "QBase.h"
#include "Interned_String_DB.h"
#include <stdarg.h>

using namespace q;

String	String::null;

//////////////////////////////////////////////////////////////////////////

String::String(char const* from, char const* to)
{
	QASSERT(from && to && to >= from);
	String_DB& tldb = String_DB::inst();
	tldb.internalize_string(from, to, m_data);
}

auto String::operator=(char const* str) -> String&
{
	QASSERT(str);
	if (*str == '\0')
	{
		reset();
		return *this;
	}

	size_t length = strlen(str);

	String_data data;

	String_DB& tldb = String_DB::inst();  //get TLDB
	tldb.internalize_string(str, str + length, data);

	m_data = data;

	return *this;
}

void String::append(char ch)
{
	size_t s = size() + 1;

	boost::auto_buffer<char, boost::store_n_bytes<1024>> buffer;
	buffer.uninitialized_resize(s);
	char* b = buffer.data();
	memcpy(b, c_str(), s - 1);
	b[s - 1] = ch;

	*this = String(b, b + s);
}
void String::append(char const* cstr)
{
	QASSERT(cstr);
	size_t cstrlen = strlen(cstr);
	size_t s = size() + cstrlen;

	boost::auto_buffer<char, boost::store_n_bytes<1024>> buffer;
	buffer.uninitialized_resize(s);
	char* b = buffer.data();
	memcpy(b, c_str(), size());
	memcpy(b + size(), cstr, cstrlen);

	*this = String(b, b + s);
}
void String::append(char const* from, char const* to)
{
	QASSERT(from && to && to >= from);
	size_t cstrlen = to - from;
	size_t s = size() + cstrlen;

	boost::auto_buffer<char, boost::store_n_bytes<1024>> buffer;
	buffer.uninitialized_resize(s);
	char* b = buffer.data();

	memcpy(b, c_str(), size());
	memcpy(b + size(), from, cstrlen);

	*this = String(b, b + s);
}
void String::append(String const& str)
{
	size_t sz = size();
	size_t strsz = str.size();

	//first some early outs
	if (strsz == 0)
	{
		return;
	}
	if (sz == 0)
	{
		*this = str;
		return;
	}

	size_t totalsz = sz + strsz;

	boost::auto_buffer<char, boost::store_n_bytes<1024>> buffer;
	buffer.uninitialized_resize(totalsz);
	char* b = buffer.data();
	memcpy(b, c_str(), sz);
	memcpy(b + sz, str.c_str(), strsz);

	*this = String(b, b + totalsz);
}

auto String::find_first_of(char ch, size_t off) const -> size_t
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
auto String::find_first_of(char const* cstr, size_t off) const -> size_t
{
	QASSERT(cstr);
	if (*cstr == '\0')
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
		if (util::memichr(cstr, ch, l) != 0)
		{
			return string - begin;
		}
	}
	return npos;
}
auto String::find_first_of(String const& str, size_t off) const -> size_t
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
		if (util::memichr(cstr, ch, l) != 0)
		{
			return string - begin;
		}
	}
	return npos;
}
auto String::find_first_not_of(char ch, size_t off) const -> size_t
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
auto String::find_first_not_of(char const* cstr, size_t off) const -> size_t
{
	QASSERT(cstr);
	if (*cstr == '\0')
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
		if (util::memichr(cstr, ch, l) == 0)
		{
			return string - begin;
		}
	}
	return npos;
}
auto String::find_first_not_of(String const& str, size_t off) const -> size_t
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
		if (util::memichr(cstr, ch, l) == 0)
		{
			return string - begin;
		}
	}
	return npos;
}

//////////////////////////////////////////////////////////////////////////

auto String::find_last_of(char ch, size_t pos) const -> size_t
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
auto String::find_last_of(char const* cstr, size_t pos) const -> size_t
{
	QASSERT(cstr);
	if (empty())
	{
		return npos;
	}
	if (pos >= size())
	{
		pos = size() - 1;
	}
	if (*cstr == '\0')
	{
		return pos;
	}
	size_t l = strlen(cstr);
	size_t i = pos;
	char const* string = c_str();
	do
	{
		char ch = string[i];
		if (util::memichr(cstr, ch, l) != 0)
		{
			return i;
		}
	} while(i-- > 0);
	return npos;
}
auto String::find_last_of(String const& str, size_t pos) const -> size_t
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
		if (util::memichr(cstr, ch, l) != 0)
		{
			return i;
		}
	} while(i-- > 0);
	return npos;
}
auto String::find_last_not_of(char ch, size_t pos) const -> size_t
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
auto String::find_last_not_of(char const* cstr, size_t pos) const -> size_t
{
	QASSERT(cstr);
	if (empty())
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
		if (util::memichr(cstr, ch, l) == 0)
		{
			return i;
		}
	} while(i-- > 0);
	return npos;
}
auto String::find_last_not_of(String const& str, size_t pos) const -> size_t
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
		if (util::memichr(cstr, ch, l) == 0)
		{
			return i;
		}
	} while(i-- > 0);
	return npos;
}

auto String::lowercase() const -> String
{
	if (empty())
	{
		return String::null;
	}

	size_t s = size();
	boost::auto_buffer<char, boost::store_n_bytes<1024>> buffer;
	buffer.uninitialized_resize(s);

	char const* src = c_str();
	for (char* dst = buffer.data(); *src;)
	{
		*dst++ = (char)util::tolower_ascii(*src++);
	}

	return String(buffer.data(), buffer.data() + s);
}
auto String::uppercase() const -> String
{
	if (empty())
	{
		return String::null;
	}

	size_t s = size();
	boost::auto_buffer<char, boost::store_n_bytes<1024>> buffer;
	buffer.uninitialized_resize(s);

	char const* src = c_str();
	for (char* dst = buffer.data(); *src;)
	{
		*dst++ = (char)util::toupper_ascii(*src++);
	}

	return String(buffer.data(), buffer.data() + s);
}

