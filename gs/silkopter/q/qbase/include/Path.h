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
	Path& operator=(Path&& other);
	Path& operator=(Path const& other);
	Path& operator=(String const& path);
	Path& operator=(char const* path);

	~Path();

	//Positive index - get the path element at that index
	//Negative index - idx = GetCount + idx (indexing from the end)
	auto operator[](int idx) const -> String;

	//Adding path elements
	auto operator+(String const& path) const -> Path;
	auto operator+(char const* path) const -> Path;
	auto operator+(Path const& path) const -> Path;
	auto operator+=(String const& path) -> Path&;
	auto operator+=(char const* path) -> Path&;
	auto operator+=(Path const& path) -> Path&;

	auto operator==(Path const& other) const -> bool;
	auto operator!=(Path const& other) const -> bool;

	void clear();
	void swap(Path& other);
		 
	void shrink(size_t size);
	void set(Path const& from, Path const& to);
	auto get_as_string() const -> String;
	auto get_count() const -> size_t;
	auto size() const -> size_t;
	auto get(int idx) const -> String;
	auto get_sub_path(size_t idx, int count = 0) const -> Path;
	void get_sub_path(Path& dst, size_t idx, int count = 0) const;
	auto is_empty() const -> bool;
	auto empty() const -> bool;

	auto get_hash() const -> std::size_t;
	auto get_extension_part() const -> String;
	auto get_name_part() const -> String;
	auto get_filename() const -> String;

	auto starts_with(Path const& prefix) const -> bool;
	auto ends_with(Path const& suffix) const -> bool;

	static Path				null;

private:
	void parse(char const* path, size_t size);

	typedef mem::Embedded_Allocator<String, 16*sizeof(String)> Allocator;
	Allocator::arena_type m_arena;

	std::vector<String, Allocator>		m_elements;
	mutable String		m_path;
	mutable size_t		m_path_reserved;
	mutable size_t		m_hash;
};

//////////////////////////////////////////////////////////////////////////

inline Path::Path() 
	: m_elements(Allocator(m_arena))
	, m_path_reserved(8)
	, m_hash(0)
{}

inline Path::Path(char const* path) 
	: m_elements(Allocator(m_arena))
	, m_path_reserved(8)
	, m_hash(0)
{
	QASSERT(path);
	parse(path, strlen(path));
}
inline Path::Path(String const& path) 
	: m_elements(Allocator(m_arena))
	, m_path_reserved(8)
	, m_hash(0)
{
	parse(path.c_str(), path.size());
}
inline Path::Path(std::string const& path) 
	: m_elements(Allocator(m_arena))
	, m_path_reserved(8)
	, m_hash(0)
{
	parse(path.c_str(), path.size());
}
inline Path::Path(Path const& other)
	: m_elements(Allocator(m_arena))
	, m_path(other.m_path)
	, m_path_reserved(other.m_path_reserved)
	, m_hash(other.m_hash)
{
	m_elements = other.m_elements;
}
inline Path::Path(Path&& other) 
	: m_elements(Allocator(m_arena))
	, m_path(std::move(other.m_path))
	, m_path_reserved(other.m_path_reserved)
	, m_hash(other.m_hash)
{
	m_elements.resize(other.m_elements.size());
	std::copy(std::make_move_iterator(other.m_elements.begin()), std::make_move_iterator(other.m_elements.end()), m_elements.begin());
}
inline Path::Path(Path const& from, Path const& to) 
	: m_elements(Allocator(m_arena))
	, m_path_reserved(8)
	, m_hash(0)
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

	std::swap(m_path, other.m_path);
	m_path_reserved = other.m_path_reserved;
	m_hash = other.m_hash;
	return *this;
}
inline auto Path::operator=(Path const& other) -> Path&
{
	m_elements = other.m_elements;
	m_path = other.m_path;
	m_path_reserved = other.m_path_reserved;
	m_hash = other.m_hash;
	return *this;
}
inline auto Path::operator=(String const& path) -> Path&
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

	Path p = *this;

	p.m_elements.reserve(p.m_elements.size() + path.m_elements.size());
	p.m_elements.insert(p.m_elements.end(), path.m_elements.begin(), path.m_elements.end());
	p.m_path_reserved = p.m_elements.size() * 8;
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
	if (!path.empty())
	{
		m_elements.reserve(m_elements.size() + path.m_elements.size());
		m_elements.insert(m_elements.end(), path.m_elements.begin(), path.m_elements.end());
		m_path_reserved = m_elements.size() * 8;
		m_hash = 0;
	}
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
	m_path = String::null;
	//mElements.resize(0);
	m_elements.clear();
	m_path_reserved = 8;
	m_hash = 0;
}
inline void Path::shrink(size_t size)
{
	if (size < get_count())
	{
		m_path = String::null;
		m_elements.resize(size);
		m_path_reserved = 8;
		m_hash = 0;
	}
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
		*this += "..";
	}
	for (size_t i = last; i < to.get_count(); i++)
	{
		*this += to[i];
	}
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
	for (; *i; ++i)
	{
		char c = *i;
		if (c == '/' || c == '\\')
		{
			if (i != start)
			{
				m_elements.emplace_back(start, i);
			}
			start = i + 1;
		}
	}
	if (i != start)
	{
		m_elements.emplace_back(start, i);
	}
}

inline auto Path::get_as_string() const -> String
{
	if (m_path_reserved)
	{
		std::string str;
		str.reserve(m_path_reserved * 2 + 32);

		if (!m_elements.empty())
		{
			for (auto const& el: m_elements)
			{
				str += el.c_str();
				str.push_back('/');
			}
			str.pop_back();
		}
		m_path = str;
		m_path_reserved = 0;
	}

	return m_path;
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
	dst.m_path.clear();
	dst.m_path_reserved = 8;
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

		dst.m_path_reserved = dst.m_elements.size() * 8;
	}
	else
	{
		//dst.mElements.resize(0);
		dst.m_elements.clear();
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
	std::swap(m_path, other.m_path);
	std::swap(m_path_reserved, other.m_path_reserved);
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
	stream << path.get_as_string();
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
	for (size_t i = 0; i < p.get_count(); i++)
	{
		format_string(dst, ph, p[i]);
		dst.append('/');
	}
}

}
}
}
