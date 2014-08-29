#pragma once

#include <atomic>
#include "mem/Embedded_Allocator.h"

namespace q
{

//This is an implementation of an interned string. The strings are held in StringDB.
//It's thread safe - each thread has it's own DB and string are copied between them
//For average strings the performance is:
// - construction time - a bit better than std::string since most of the time it does a lookup than an allocation
// - copy time is O(1) - just copies 4 ints
// - hash lokuptime is excelent. The hash function is faster than int for some reason
// - std::map time also very good. There are 2 versions of the < operator - a fast one using the pointer as the key and a slower lexicographically correct one using the StringLexiLT predicates
//
//By default the string is case-insensitive but case sensitive predicates and functions are provided

class String_DB_Map;
class String_DB;

class String
{
public:

	typedef char value_type;
	typedef char const* const_iterator;

public:
	String();
	String(String const& str);
	String(String&& str);
	String(String const& str, size_t offset, size_t count);
	String(char const* from, char const* to);
	String(char const* str);
	String(char const* str, size_t count);
	explicit String(std::string const& str);

	auto operator==(String const& str) const -> bool;
	auto operator!=(String const& str) const -> bool;

	auto operator==(std::string const& str) const -> bool;
	auto operator!=(std::string const& str) const -> bool;

	auto operator==(char const* str) const -> bool;
	auto operator!=(char const* str) const -> bool;

	auto operator<(String const& str) const -> bool;

	auto operator=(String const& str) -> String&;
	auto operator=(String&& str) -> String&;
	auto operator=(char const* str) -> String&;
	auto operator=(std::string const& str) -> String&;

	auto operator+(String const& str) const -> String;
	auto operator+=(String const& str) -> String&;
	auto operator+(char const* cstr) const -> String;
	auto operator+=(char const* cstr) -> String&;
	auto operator+(char ch) const -> String;
	auto operator+=(char ch) -> String&;

	auto operator[](size_t idx) const -> char;

	//////////////////////////////////////////////////////////////////////////
	//equivalents of std::string
	auto c_str() const -> char const*;
	auto size() const -> size_t;
	auto length() const -> size_t;
	auto empty() const -> bool;
	void clear();
	void swap(String& other);
	void reserve(size_t size);
	void resize(size_t size);

	auto substr(size_t off = 0, size_t count = npos) const -> String;

	void push_back(char ch);

	void append(char ch);
	void append(char const* cstr);
	void append(char const* from, char const* to);
	void append(String const& str);

	auto find(char ch, size_t off = 0) const -> size_t;
	auto find(char const* cstr, size_t off = 0) const -> size_t;
	auto find(String const& str, size_t off = 0) const -> size_t;

	auto find_first_of(char ch, size_t off = 0) const -> size_t;
	auto find_first_of(char const* cstr, size_t off = 0) const -> size_t;
	auto find_first_of(String const& str, size_t off = 0) const -> size_t;

	auto find_first_not_of(char ch, size_t off = 0) const -> size_t;
	auto find_first_not_of(char const* cstr, size_t off = 0) const -> size_t;
	auto find_first_not_of(String const& str, size_t off = 0) const -> size_t;

	auto find_last_of(char ch, size_t off = npos) const -> size_t;
	auto find_last_of(char const* cstr, size_t off = npos) const -> size_t;
	auto find_last_of(String const& str, size_t off = npos) const -> size_t;

	auto find_last_not_of(char ch, size_t off = npos) const -> size_t;
	auto find_last_not_of(char const* cstr, size_t off = npos) const -> size_t;
	auto find_last_not_of(String const& str, size_t off = npos) const -> size_t;

	auto begin() const -> const_iterator;
	auto end() const -> const_iterator;

	//////////////////////////////////////////////////////////////////////////
	//specific methods

	//returns a case-insensitive hash value
	auto hash() const -> uint32_t;
	//returns a case-insensitive string equal to all strings from the same family
	auto key() const -> char const*;

	//Returns a new lowercase/uppercase version of this string
	auto lowercase() const -> String;
	auto uppercase() const -> String;

	//These are lexicographically correct - they compare the characters in the string
	//If you want just strict ordering, use s1.getKey() < s2.getKey()
	auto less(String const& str) const -> bool; //case insensitive less
	auto less_cs(String const& str) const -> bool; //case sensitive less
	auto less(char const* str) const -> bool; //case insensitive less
	auto less_cs(char const* str) const -> bool; //case sensitive less

	//Very fast (integer like) equality comparison
	auto equals(String const& str) const -> bool; //case insensitive equality: aAa == aaa
	auto equals_cs(String const& str) const -> bool; //case sensitive equality: aAa != aaa

	//These do a lexicographical comparison
	auto equals(std::string const& str) const -> bool; //case insensitive equality
	auto equals_cs(std::string const& str) const -> bool; //case sensitive equality
	auto equals(char const* str) const -> bool; //case insensitive equality
	auto equals_cs(char const* str) const -> bool; //case sensitive equality

	static String   null;
	static size_t const npos = (size_t)(-1);

protected:
	friend class String_DB_Map;
	friend class String_DB;

#if defined Q_WINDOWS
	typedef std::atomic<int> atomic_count;
#else
	typedef std::atomic_int atomic_count;
#endif


	//a cell is a case insensitive string family.
	struct Cell
	{
		Cell() 
            : key(get_null()), string(get_null()), counter(&counter_data)
		{
		}
		Cell(atomic_count* _counter)
            : key(get_null()), string(get_null()), counter(_counter)
		{
		}

        int length = 0; //length for both string and key

        uint32_t hash = 0; //hash for key
		char const* key; //case insensitive string - points to the string of the head of the family
		char const* string; //case sensitive string - unique for each cell

        Cell* next = nullptr; //points to the next family
        Cell* next_sibling = nullptr; //points to the next string in the family

		atomic_count counter_data;
        atomic_count* counter = nullptr;

		static int const string_storage_size = 32;
		char string_storage[string_storage_size];

		void reset()
		{
			length = 0;
			hash = 0;
			key = get_null();
			string = get_null();
			next = nullptr;
            next_sibling = nullptr;
			QASSERT((int)counter_data == 0);
			counter = &counter_data;
		}

	private:
		static auto get_null() -> char const*
		{
			return "";
		}

 		Cell(Cell const&) {}
 		Cell(Cell&&) {}
		Cell& operator=(Cell const&) { return *this; }
		Cell& operator=(Cell const&&) { return *this; }
	};

	//class that managed the string data and the atomic ref counter
    struct String_Data
	{
        String_Data() {}
        String_Data(Cell* _cell) : cell(_cell)
		{
			QASSERT(cell);
			if (cell)
			{
				++(*cell->counter);
			}
		}
        String_Data(String_Data const& other)
			: cell(other.cell)
		{
			if (cell && cell->counter)
			{
				++(*cell->counter);
			}
		}
        String_Data(String_Data&& other)
		{
			cell = other.cell;
			other.cell = nullptr;
		}
        ~String_Data()
		{
			if (cell && cell->counter)
			{
				--(*cell->counter);
			}
		}
        auto operator=(String_Data const& other) -> String_Data&
		{
            String_Data tmp(other);
			swap(tmp);
			return *this;
		}
        auto operator=(String_Data&& other) -> String_Data&
		{
			swap(other);
			return *this;
		}
        auto operator==(String_Data const& other) const -> bool
		{
			return cell == other.cell;
		}
        void swap(String_Data& other)
		{
			std::swap(cell, other.cell);
		}
		void reset()
		{
            String_Data tmp;
			swap(tmp);
		}
		auto is_empty() const -> bool
		{
			return cell == nullptr;
		}
		auto get_cell() const -> Cell const*
		{
			return cell;
		}
		auto operator->() const -> Cell const*
		{
			return cell;
		}

        Cell const* cell = nullptr;
	};


private:
	void			reset();

	//interned data
    String_Data	m_data;
};

//////////////////////////////////////////////////////////////////////////

inline String::String()
{
}

inline String::String(String const& str)
	: m_data(str.m_data)
{
}
inline String::String(String const& str, size_t offset, size_t count)
{
	QASSERT(offset + count <= str.size());
	*this = String(str.c_str() + offset, str.c_str() + offset + count);
}

inline String::String(String&& str)
	: m_data(std::move(str.m_data))
{
}

inline String::String(char const* str)
{
	*this = str;
}
inline String::String(char const* str, size_t count)
{
	*this = String(str, str + count);
}

inline String::String(std::string const& str)
{
	*this = str;
}

inline auto String::operator=(std::string const& str) -> String&
{
	if (str.empty())
	{
		reset();
		return *this;
	}

	char const* cstr = str.c_str();
	*this = String(cstr, cstr + str.length());
	return *this;
}

inline auto String::operator=(String const& str) -> String&
{
	m_data = str.m_data;
	return *this;
}
inline auto String::operator=(String&& str) -> String&
{
	m_data.swap(str.m_data);
	return *this;
}

inline auto String::operator<(String const& str) const -> bool
{
	return less(str);
}

inline auto String::operator==(String const& str) const -> bool
{
	return equals(str);
}

inline auto String::operator!=(String const& str) const -> bool
{
	return !operator==(str);
}

inline auto String::operator==(std::string const& str) const -> bool
{
	return equals(str);
}

inline auto String::operator!=(std::string const& str) const -> bool
{
	return !operator==(str);
}

inline auto String::operator==(char const* str) const -> bool
{
	return equals(str);
}

inline auto String::operator!=(char const* str) const -> bool
{
	return !operator==(str);
}
inline auto String::operator+(String const& str) const -> String
{
	String ret(*this);
	ret.append(str);
	return ret;
}
inline auto String::operator+=(String const& str) -> String&
{
	append(str);
	return *this;
}
inline auto String::operator+(char const* cstr) const -> String
{
	String ret(*this);
	ret.append(cstr);
	return ret;
}
inline auto String::operator+=(char const* cstr) -> String&
{
	append(cstr);
	return *this;
}
inline auto String::operator+(char ch) const -> String
{
	String ret(*this);
	ret.append(ch);
	return ret;
}
inline auto String::operator+=(char ch) -> String&
{
	append(ch);
	return *this;
}

inline auto	String::operator[](size_t idx) const -> char
{
	QASSERT(idx < size());
	return c_str()[idx];
}
inline void String::reset()
{
	m_data.reset();
}

inline void String::push_back(char ch)
{
	append(ch);
}
inline auto String::c_str() const -> char const*
{
	return m_data.cell ? m_data->string : "";
}

inline void String::clear()
{
	*this = String::null;
}

inline auto String::length() const -> size_t
{
	return m_data.cell ? m_data->length : 0;
}

inline auto String::size() const -> size_t
{
	return m_data.cell ? m_data->length : 0;
}

inline auto String::empty() const -> bool
{
	return m_data.is_empty() ? true : m_data->length == 0;
}
inline auto String::substr(size_t off, size_t count) const -> String
{
	QASSERT(off + count <= size());
	if (count == npos)
	{
		count = size() - off;
	}

	if (count == 0)
	{
		return String::null;
	}

	boost::auto_buffer<char, boost::store_n_bytes<1024>> buffer;
	buffer.uninitialized_resize(count + 1);
	char* b = buffer.data();
	memcpy(b, c_str() + off, count);

	String ret(b, b + count);
	return ret;
}
inline auto String::find(char ch, size_t off) const -> size_t
{
	return find_first_of(ch, off);
}
inline auto String::find(char const* cstr, size_t off) const -> size_t
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
	char const* c = util::stristr(string + off, cstr);
	return c ? c - string : npos;
}
inline auto String::find(String const& str, size_t off) const -> size_t
{
	return find(str.c_str(), off);
}

inline auto String::begin() const -> String::const_iterator
{
	return empty() ? 0 : c_str();
}
inline auto String::end() const -> String::const_iterator
{
	return empty() ? 0 : c_str() + size();
}

inline void String::reserve(size_t)
{
	; //nothing to do
}
inline void String::resize(size_t s)
{
	if (size() == s)
	{
		return;
	}

	if (s == 0)
	{
		clear();
		return;
	}

	boost::auto_buffer<char, boost::store_n_bytes<1024>> buffer;
	buffer.uninitialized_resize(s + 1);
	char* b = buffer.data();

	if (s < size())
	{
		memcpy(b, c_str(), s);
		b[s] = '\0';
		*this = b;
	}
	else
	{
		memcpy(b, c_str(), size());
		memset(b + size(), 0, s - size());
		b[s] = '\0';
		*this = b;
	}
}
inline void String::swap(String& other)
{
	std::swap(m_data, other.m_data);
}

inline auto String::less(String const& str) const -> bool
{
	if (equals(str))
	{
		return false;
	}
	return util::stricmp(c_str(), str.c_str()) < 0;
}
inline auto String::less_cs(String const& str) const -> bool
{
	return util::strcmp(c_str(), str.c_str()) < 0;
}
inline auto String::less(char const* str) const -> bool
{
	if (!str) return false;
	return util::stricmp(c_str(), str) < 0;
}
inline auto String::less_cs(char const* str) const -> bool
{
	if (!str) return false;
	return util::strcmp(c_str(), str) < 0;
}

inline auto String::equals(String const& str) const -> bool
{
	return key() == str.key();
}
inline auto String::equals_cs(String const& str) const -> bool
{
	return c_str() == str.c_str();
}
inline auto String::equals(std::string const& str) const -> bool
{
	return size() == str.size() && (empty() || !util::memicmp(str.c_str(), c_str(), size()));
}
inline auto String::equals_cs(std::string const& str) const -> bool
{
	return size() == str.size() && (empty() || !util::memcmp(str.c_str(), c_str(), size()));
}
inline auto String::equals(char const* str) const -> bool
{
	if (!str) return empty();
	size_t len = strlen(str);
	return size() == len && (empty() || !util::memicmp(str, c_str(), size()));
}
inline auto String::equals_cs(char const* str) const -> bool
{
	if (!str) return empty();
	size_t len = strlen(str);
	return size() == len && (empty() || !util::memcmp(str, c_str(), size()));
}

inline auto String::hash() const -> uint32_t
{
	return m_data.cell ? m_data->hash : 0;
}
inline auto String::key() const -> char const*
{
	return  m_data.cell ? m_data->key : nullptr;
}

// Members end

//Returns true if s1 is less than s2
struct String_LT
{
	bool operator()(String const& s1, String const& s2) const
	{
		return s1.less(s2);
	}
};
struct String_LT_CS
{
	bool operator()(String const& s1, String const& s2) const
	{
		return s1.less_cs(s2);
	}
};

//Returns true if s1 is greater than s2
struct String_GT
{
	bool operator()(String const& s1, String const& s2) const
	{			 
		return s2.less(s1);
	}
};
struct String_GT_CS
{
	bool operator()(String const& s1, String const& s2) const
	{
		return s2.less_cs(s1);
	}
};


} //end namespace

namespace std 
{
	template <> 
	struct hash<q::String>
	{
		auto operator()(q::String const& s) const -> size_t
		{
			return (size_t)s.hash();
		}
	};
}


//global operator+ for char const* + String
//used for expressions like: String s = "my char" + name;
inline auto operator+(char const* cstr, q::String const& str) -> q::String
{
	//first some early outs
	if (str.empty())
	{
		return (cstr == 0 || cstr[0] == '\0') ? q::String::null : q::String(cstr);
	}
	if (cstr == 0 || cstr[0] == '\0')
	{
		return str;
	}

	size_t cstrlen = strlen(cstr);
	size_t s = str.size() + cstrlen;
	boost::auto_buffer<char, boost::store_n_bytes<1024>> buffer;
	buffer.uninitialized_resize(s + 1);
	char* b = buffer.data();

	memcpy(b, cstr, cstrlen);
	memcpy(b + cstrlen, str.c_str(), str.size());
	b[s] = '\0';

	return q::String(b);
}

//global operator== for char const* and String
inline auto operator==(char const* cstr, q::String const& str) -> bool
{
	return str == cstr;
}

inline void swap(q::String& a, q::String& b)
{
	a.swap(b);
}

inline auto operator<<(std::ostream& stream, q::String const& str) -> std::ostream&
{
	stream << str.c_str();
	return stream;
}

namespace q
{
namespace util
{
namespace formatting
{

	template<> struct Dst_Adapter<q::String>
	{
		typedef mem::Embedded_Allocator<char, 1024> alloc;
		alloc::arena_type m_arena;
		typedef	std::vector<char, alloc> buffer_type;
		buffer_type m_buffer;
		q::String& m_dst;

		Dst_Adapter(q::String& dst)
            : m_buffer(m_arena)
            , m_dst(dst)
        {
		}

		void append(char ch) { m_buffer.push_back(ch); }
		void append(char const* start, char const* end) { m_buffer.insert(m_buffer.end(), start, end); }
		void clear() { m_dst.clear(); m_buffer.clear();  }
		void finish() { m_dst = !m_buffer.empty() ? q::String(m_buffer.data(), m_buffer.data() + m_buffer.size()) : q::String::null; }
	};

	template<class Dst_Adapter, class Placeholder>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, q::String const& p)
	{
		QASSERT(ph.alignment == 0);
		dst.append(p.begin(), p.end());
	}

}
}
}

