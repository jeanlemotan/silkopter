#pragma once

#include "util/Noncopyable.h"

namespace util
{

//a buffer imlementation that calculates the crc of the data added inside
	
template<class T, size_t SIZE>
class Buffer : util::Noncopyable
{
public:
	static const size_t MAX_SIZE = SIZE;
	
	size_t size() const
	{
		return m_size;
	}
	
	void clear()
	{
		m_size = 0;
	}
	
	T const* data() const
	{
		return m_data;	
	}
	T* data()
	{
		return m_data;
	}
	
	T const& operator[](size_t idx) const
	{
		ASSERT(idx < m_size);
		return m_data[idx];
	}
	T& operator[](size_t idx)
	{
		ASSERT(idx < m_size);
		return m_data[idx];
	}
	
	void append(T const* src, size_t size)
	{
		if (!src || !size)
		{
			return;
		}
		size = std::min(size, MAX_SIZE - m_size);
		if (size)
		{
			std::copy(src, src + size, m_data + m_size);
			m_size += size;
		}
	}
	template <class U>
	void append(U const& u)
	{
		ASSERT(sizeof(U) % sizeof(T) == 0);
		append(reinterpret_cast<T const*>(&u), sizeof(U) / sizeof(T));
	}

	void write_at(size_t off, T const* src, size_t size)
	{
		if (!src || !size)
		{
			return;
		}
		off = std::min(off, MAX_SIZE - 1);
		size = std::min(size, MAX_SIZE - off);
		if (size)
		{
			std::copy(src, src + size, m_data + off);
			m_size = std::max(m_size, off + size);
		}
	}

	template <class U>
	void write_at(size_t off, U const& u)
	{
		ASSERT(sizeof(U) % sizeof(T) == 0);
		write_at(off, reinterpret_cast<T const*>(&u), sizeof(U) / sizeof(T));
	}
	
private:
	size_t m_size = 0;
	T m_data[MAX_SIZE];
};
	
}