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
		, m_crc(0)
	{
			
	}

	size_t get_size() const
	{
		return m_size;
	}
	
	void clear()
	{
		m_size = 0;
		m_crc = 0;
	}
	
	uint16_t get_crc() const
	{
		return m_crc;
	}
	
	uint8_t const* get_data_ptr() const
	{
		return m_data;	
	}
	
	void write(uint8_t const* src, size_t size)
	{
		if (!src)
		{
			return;
		}
		size = std::min(size, MAX_SIZE - m_size);
		for (size_t i = 0; i < size; i++)
		{
			uint8_t x = *src;
			m_data[m_size++] = x;
			m_crc = _crc16_update(m_crc, x);
		}
	}
	
	template<typename T>
	void write(T t)
	{
		return write(reinterpret_cast<uint8_t*>(&t), sizeof(t));
	}

private:
	size_t m_size;
	uint16_t m_crc;
	uint8_t m_data[MAX_SIZE];
};
	
}