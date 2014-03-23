#pragma once

#include "util/Noncopyable.h"
#include <util/crc16.h>

namespace util
{

//a buffer imlementation that calculates the crc of the data added inside
	
template<size_t SIZE>
class CRC_Buffer : util::Noncopyable
{
public:
	static const size_t MAX_SIZE = SIZE;
	
	CRC_Buffer() 
		: m_size(0)
	{
			
	}

	size_t get_size() const
	{
		return m_size;
	}
	
	void clear()
	{
		m_size = 0;
	}
	
	uint16_t compute_crc() const
	{
		uint16_t crc = 0;
		uint8_t const* ptr = m_data;
		uint8_t const* end = ptr + m_size;
		for (; ptr < end; ++ptr)
		{
			crc = _crc16_update(crc, *ptr);
		}
		return crc;
	}
	
	uint8_t const* get_data_ptr() const
	{
		return m_data;	
	}
	
	void write(uint8_t const* src, size_t size)
	{
		if (!src || !size)
		{
			return;
		}
		size = std::min(size, MAX_SIZE - m_size);
		if (size)
		{
			memcpy(m_data + m_size, src, size);
			m_size += size;
		}
	}

	template<typename T>
	void write(T t)
	{
		return write(reinterpret_cast<uint8_t*>(&t), sizeof(t));
	}

	void write_at(size_t off, uint8_t const* src, size_t size)
	{
		if (!src || !size)
		{
			return;
		}
		off = std::min(off, MAX_SIZE - 1);
		size = std::min(size, MAX_SIZE - off);
		if (size)
		{
			memcpy(m_data + off, src, size);
			m_size = std::max(m_size, off + size);
		}
	}
	
	template<typename T>
	void write_at(size_t off, T t)
	{
		return write_at(off, reinterpret_cast<uint8_t*>(&t), sizeof(t));
	}

private:
	size_t m_size;
	uint8_t m_data[MAX_SIZE];
};
	
}