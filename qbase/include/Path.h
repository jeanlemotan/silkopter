#pragma once

#include "Platform.h"
#include "Interned_String.h"
#include "util/Hash.h"
#include "mem/Embedded_Allocator.h"

namespace q
{

class Path
{
	friend std::size_t hash_value(Path const& p);
public:
	Path();
	explicit Path(char const* path);
	explicit Path(String const& path);
	explicit Path(std::string const& path);
	Path(Path const& other);
	Path(Path&& other);
	Path(Path const& from, Path const& to);
	~Path();

	//Positive index - get the path element at that index
	//Negative index - idx = get_count + idx (indexing from the end)
	auto operator[](int idx) const -> String;

	//Adding path elements
	auto operator+(String const& path) const -> Path;
	auto operator+(std::string const& path) const -> Path;
	auto operator+(char const* path) const -> Path;
	auto operator+(Path const& path) const -> Path;
	
	auto operator+=(String const& path) -> Path&;
	auto operator+=(std::string const& path) -> Path&;
	auto operator+=(char const* path) -> Path&;
	auto operator+=(Path const& path) -> Path&;

	//assignment
    auto operator=(Path&& other) -> Path&;
    auto operator=(Path const& other) -> Path&;
    auto operator=(String const& path) -> Path&;
    auto operator=(std::string const& path) -> Path&;
    auto operator=(char const* path) -> Path&;

	auto operator==(Path const& other) const -> bool;
	auto operator!=(Path const& other) const -> bool;

	void clear();
	void swap(Path& other);
		 
	//removes elements from the end of the path
	//if size is > get_count, the path will be empty
	void shrink(size_t size);

	//sets this path to be the relative path between from and to.
	//Example:
	//	set('a/b/c', 'a/b/c/d') == 'd'
	//	set('a/b/c', 'a/b/x') == '../x'
	void set(Path const& from, Path const& to);

	//returns the path in String form
	// Path(p.get_as_string()) == p
	auto get_as_string() const -> String;
	//returns the path in std::string form
	// Path(p.get_as_string()) == p
    template<class Str_T> auto get_as() const -> Str_T;

	//returns the number of elements in the path.
	auto get_count() const -> size_t;
	//same as get_count
	auto size() const -> size_t;

	//if idx >= 0 - returns the idx'th element from the path
	//else, returns the idx'th element from the back
	auto get(int idx) const -> String;
	//returns a new path with its elements starting from idx up to:
	// if count > 0, up to idx + count
	// else up to back - count
	auto get_sub_path(size_t idx, int count = 0) const -> Path;
	void get_sub_path(Path& dst, size_t idx, int count = 0) const;

	//returns true if the path doesn't have any elements (path == null)
	auto is_empty() const -> bool;
	auto empty() const -> bool;

	//returns true if the path starts with the prefix
	auto starts_with(Path const& prefix) const -> bool;
	//returns true if the path ends with the suffix
	auto ends_with(Path const& suffix) const -> bool;

	//collapses any '..' the path might have, if possible
	//Example:
	//	'a/b/c/../d' == 'a/b/d'
	//	'a/b/../../../d' == '../d'
	void collapse_relative_path();

	//returns a has for this path. Used for hash_maps
	auto get_hash() const -> std::size_t;

	//filesystem interaction
	//returns from the last path element - if it exists - the part after the last '.' - or null if not found. This usually represents the file extension
	auto get_extension_part() const -> String;
	//returns from the last path element - if it exists - the part before the last '.' - or null if not found. This usually represents the file name
	auto get_name_part() const -> String;
	//returns the last path element - if it exists or null if not found. This usually represents the filename (name + extension)
	auto get_filename() const -> String;

    auto pop_front() -> String;
    auto pop_back() -> String;

	static const Path				null;

private:
	static const String 			k_separator;
	static const String 			k_back;

	void parse(char const* path, size_t size);
    auto can_pop() const -> bool;

	typedef mem::Embedded_Allocator<String, 16*sizeof(String)> Allocator;
	Allocator::arena_type m_arena;

	std::vector<String, Allocator>		m_elements;
	mutable size_t		m_hash = 0;
};

//////////////////////////////////////////////////////////////////////////

inline Path::Path() 
	: m_elements(Allocator(m_arena))
{}

inline Path::Path(char const* path) 
	: m_elements(Allocator(m_arena))
{
	QASSERT(path);
	parse(path, strlen(path));
}
inline Path::Path(String const& path) 
	: m_elements(Allocator(m_arena))
{
	parse(path.c_str(), path.size());
}
inline Path::Path(std::string const& path) 
	: m_elements(Allocator(m_arena))
{
	parse(path.c_str(), path.size());
}
inline Path::Path(Path const& other)
	: m_elements(Allocator(m_arena))
	, m_hash(other.m_hash)
{
	m_elements = other.m_elements;
}
inline Path::Path(Path&& other) 
	: m_elements(Allocator(m_arena))
	, m_hash(other.m_hash)
{
	m_elements.resize(other.m_elements.size());
	std::copy(std::make_move_iterator(other.m_elements.begin()), std::make_move_iterator(other.m_elements.end()), m_elements.begin());
}
inline Path::Path(Path const& from, Path const& to) 
	: m_elements(Allocator(m_arena))
{
	set(from, to);
}
inline Path::~Path()
{
	m_elements.clear();
}
inline auto Path::operator[](int idx) const -> String
{
	return get(idx);
}
inline auto Path::operator=(Path&& other) -> Path&
{
	m_elements.resize(other.m_elements.size());
	std::copy(std::make_move_iterator(other.m_elements.begin()), std::make_move_iterator(other.m_elements.end()), m_elements.begin());

	m_hash = other.m_hash;
	return *this;
}
inline auto Path::operator=(Path const& other) -> Path&
{
	m_elements = other.m_elements;
	m_hash = other.m_hash;
	return *this;
}
inline auto Path::operator=(String const& path) -> Path&
{
	parse(path.c_str(), path.size());
	return *this;
}
inline auto Path::operator=(std::string const& path) -> Path&
{
	parse(path.c_str(), path.size());
	return *this;
}
inline auto Path::operator=(char const* path) -> Path&
{
	parse(path, strlen(path));
	return *this;
}
inline auto Path::operator+(String const& path) const -> Path
{
	if (path.empty())
	{
		return *this;
	}
	return *this + Path(path);
}
inline auto Path::operator+(std::string const& path) const -> Path
{
	if (path.empty())
	{
		return *this;
	}
	return *this + Path(path);
}
inline auto Path::operator+(char const* path) const -> Path
{
	if (!path || path[0] == '\0')
	{
		return *this;
	}
	return operator+(Path(path));
}
inline auto Path::operator+(Path const& path) const -> Path
{
	if (path.empty())
	{
		return *this;
	}
	if (empty())
	{
		return path;
	}

	Path p = *this;

	p.m_elements.reserve(p.m_elements.size() + path.m_elements.size());

	//avoid the case of double '/'
	if (path.m_elements.front() == k_separator)
	{
		p.m_elements.pop_back();
	}

	size_t i = 0;
	//skip the first '/'
	if (path.m_elements.front() == k_separator)
	{
		i = 1;
	}
	//collapse all the '..' from at the front of path
	for (; i < path.m_elements.size(); i++)
	{
		auto const& e = path.m_elements[i];
		if (e == k_back && p.can_pop())
		{
			//if elements is not empty and the last element is not '..', then remove it
			p.m_elements.pop_back();	
		}
		else
		{
			break;
		}
	}

	std::copy(path.m_elements.begin() + i, path.m_elements.end(), std::back_inserter(p.m_elements));
	p.m_hash = 0;

	return p;
}
inline auto Path::operator+=(String const& path) -> Path&
{
	if (!path.empty())
	{
		return operator+=(Path(path));
	}
	return *this;
}
inline auto Path::operator+=(std::string const& path) -> Path&
{
	if (!path.empty())
	{
		return operator+=(Path(path));
	}
	return *this;
}
inline auto Path::operator+=(char const* path) -> Path&
{
	if (path && path[0] != '\0')
	{
		return operator+=(Path(path));
	}
	return *this;
}
inline auto Path::operator+=(Path const& path) -> Path&
{
	if (path.empty())
	{
		return *this;
	}
	if (empty())
	{
		*this = path;
		return *this;
	}

	m_elements.reserve(m_elements.size() + path.m_elements.size());

	//avoid the case of double '/'
	if (path.m_elements.front() == k_separator)
	{
		m_elements.pop_back();
	}

	size_t i = 0;
	//skip the first '/'
	if (path.m_elements.front() == k_separator)
	{
		i = 1;
	}
	//collapse all the '..' from at the front of path
	for (; i < path.m_elements.size(); i++)
	{
		auto const& e = path.m_elements[i];
		if (e == k_back && can_pop())
		{
			//if elements is not empty and the last element is not '..', then remove it
			m_elements.pop_back();	
		}
		else
		{
			break;
		}
	}

	std::copy(path.m_elements.begin() + i, path.m_elements.end(), std::back_inserter(m_elements));

	return *this;
}
inline auto Path::operator==(Path const& other) const -> bool
{
	return (get_hash() == other.get_hash()) && m_elements == other.m_elements;
}
inline auto Path::operator!=(Path const& other) const -> bool
{
	return !(*this == other);
}

inline void Path::clear()
{
	m_elements.clear();
	m_hash = 0;
}
inline void Path::shrink(size_t size)
{
	m_elements.resize(std::min(size, get_count()));
	m_hash = 0;
}
inline void Path::set(Path const& from, Path const& to)
{
	if (from.empty())
	{
		*this = to;
		return;
	}
	if (to.empty())
	{
		*this = from;
		return;
	}
	auto count = std::min(from.get_count(), to.get_count());

	clear();

	size_t last = 0;
	for (size_t i = 0; i < count; i++)
	{
		if (from.get(i) != to.get(i))
		{
			break;
		}
		last = i + 1;
	}

	for (size_t i = last; i < from.get_count(); i++)
	{
		m_elements.push_back(k_back);
	}
	for (size_t i = last; i < to.get_count(); i++)
	{
		m_elements.push_back(to.m_elements[i]);
	}

	m_hash = 0;
}
inline void Path::parse(char const* path, size_t size)
{
	clear();
	if (size == 0 || !path || path[0] == 0)
	{
		return;
	}

	char const* start = path;
	char const* i = path;

	//special case for the first '/'
	if (*i == '/' || *i == '\\')
	{
		m_elements.push_back(k_separator);
		i++;
		start++;
	}

	for (; *i; ++i)
	{
		char c = *i;
		if (c == '/' || c == '\\')
		{
			size_t count = (size_t)(i - start);
			QASSERT(count < size);
			if (count > 0)
			{
				if (count == 2 && start[0] == '.' && start[1] == '.' && can_pop())
				{
					m_elements.pop_back();
				}
				else
				{
					m_elements.emplace_back(start, i);
				}
			}
			start = i + 1;
		}
	}
	if (i != start)
	{
		m_elements.emplace_back(start, i);
	}
}

inline auto Path::can_pop() const -> bool
{
	return !m_elements.empty() && m_elements.back() != k_back && m_elements.back() != k_separator;
}

inline auto Path::get_as_string() const -> String
{
	return String(get_as<std::string>());
}
template<class Str_T>
inline auto Path::get_as() const -> Str_T
{
	Str_T buffer;
	if (!m_elements.empty())
	{
		size_t size = 0;
		for (auto const& el: m_elements)
		{
			size += el.size();
			size++; //the separator
		}

		buffer.resize(size);
		size_t idx = 0;
		for (auto const& el: m_elements)
		{
			std::copy(el.begin(), el.end(), buffer.begin() + idx);
			idx += el.size();
			buffer[idx] = '/';
			idx++;
		}
		buffer.pop_back(); //remove trailing '/'
	}
	return buffer;
}
inline auto Path::size() const -> size_t
{
	return m_elements.size();
}
inline auto Path::get_count() const -> size_t
{
	return size();
}
inline auto Path::get(int idx) const -> String
{
	return idx < 0 ? m_elements[get_count() + idx] : m_elements[idx];
}

inline auto Path::empty() const -> bool
{
	return m_elements.empty();
}
inline auto Path::is_empty() const -> bool
{
	return empty();
}

inline auto Path::get_sub_path(size_t idx, int count) const -> Path
{
	Path path;
	get_sub_path(path, idx, count);
	return path;
}
inline void Path::get_sub_path(Path& dst, size_t idx, int count /* = 0 */) const
{
	dst.m_hash = 0;

	if (count == 0)
	{
		count = (int)get_count() - (int)idx;
	}
	if (count < 0)
	{
		count = (int)get_count() - (int)idx + count;
	}
	if (count > 0 && idx < get_count())
	{
		dst.m_elements.resize(count);
		for (int i = 0; i < count; i++)
		{
			dst.m_elements[i] = m_elements[idx + i];
		}
	}
	else
	{
		dst.m_elements.clear();
	}
}
inline void Path::collapse_relative_path()
{
	if (!is_empty())
	{
		m_hash = 0;
		for (size_t i = 1; i < m_elements.size();)
		{
			if (m_elements[i] == k_back)
			{
				m_elements.erase(m_elements.begin() + i); //remove the ..
				m_elements.erase(m_elements.begin() + i - 1);//remove the parent
				i -= 1;
			}
			else
			{
				i++;
			}
		}
	}
}

inline auto Path::starts_with(Path const& prefix) const -> bool
{
	if (empty() || prefix.empty())
	{
		return false;
	}
	if (get_count() < prefix.get_count())
	{
		return false;
	}
	for (size_t i = 0; i < prefix.get_count(); i++)
	{
		if (get(i) != prefix.get(i))
		{
			return false;
		}
	}
	return true;
}
inline auto Path::ends_with(Path const& suffix) const -> bool
{
	if (empty() || suffix.empty())
	{
		return false;
	}
	if (get_count() < suffix.get_count())
	{
		return false;
	}
	for (int i = 0; i < (int)suffix.get_count(); i++)
	{
		if (get(0 - i - 1) != suffix.get(0 - i - 1))
		{
			return false;
		}
	}
	return true;
}

inline void Path::swap(Path& other)
{
	std::swap(m_elements, other.m_elements);
	std::swap(m_hash, other.m_hash);
}

inline auto Path::get_filename() const -> String
{
	return empty() ? String::null : m_elements.back();
}
inline auto Path::get_extension_part() const -> String
{
	if (m_elements.empty())
	{
		return String::null;
	}

	String const& fn = m_elements.back();
	auto off = fn.find_last_of('.');
	if (off != String::npos)
	{
		return fn.substr(off + 1);
	}
	return String::null;
}
inline auto Path::get_name_part() const -> String
{
	if (m_elements.empty())
	{
		return String::null;
	}

	String const& fn = m_elements.back();
	auto off = fn.find_last_of('.');
	if (off != String::npos)
	{
		return fn.substr(0, off);
	}
	return String::null;
}

inline auto Path::pop_front() -> String
{
    if (m_elements.empty())
    {
        return String::null;
    }
    auto s = m_elements.front();
    m_elements.erase(m_elements.begin());
    return s;
}
inline auto Path::pop_back() -> String
{
    if (m_elements.empty())
    {
        return String::null;
    }
    auto s = m_elements.back();
    m_elements.pop_back();
    return s;
}


inline auto Path::get_hash() const -> size_t
{
	size_t hash = m_hash;
	if (hash == 0)
	{
		hash = 10;
		for (size_t i = 0, sz = get_count(); i < sz; i++)
		{
			util::hash_combine(hash, m_elements[i]);
		}

		m_hash = hash;
	}
	return hash;
}

inline void swap(Path& a, Path& b)
{
	a.swap(b);
}

}

namespace std 
{
	template <> 
	struct hash<q::Path>
	{
		auto operator()(q::Path const& p) const -> size_t
		{
			return p.get_hash();
		}
	};
}


inline auto operator<<(std::ostream& stream, q::Path const& path) -> std::ostream&
{
	stream << path.get_as<std::string>();
	return stream;
}

namespace q
{
namespace util
{
namespace formatting
{

template<class Dst_Adapter, class Placeholder>
void format_string(Dst_Adapter& dst, Placeholder const& ph, q::Path const& p)
{
	QASSERT(ph.alignment == 0);
    if (!p.empty())
    {
        size_t i = 0;
        if (p[0] == "/")
        {
            format_string(dst, ph, p[0]);
            i++;
        }
        for (; i < p.get_count(); i++)
        {
            format_string(dst, ph, p[i]);
            if (i + 1 < p.get_count())
            {
                dst.append('/');
            }
        }
    }
}

}
}
}
