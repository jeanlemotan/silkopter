#include "QDataStdAfx.h"
#include "data/Sink.h"
#include "data/Source.h"
#include "data/Memory_Sink.h"
#include "data/Memory_Source.h"

using namespace q;
using namespace data;

Memory_Sink::Memory_Sink(size_t reserve_size)
	: m_data_ptr(nullptr)
	, m_data_size(0)
	, m_offset(0)
{
	m_data = std::make_shared<std::vector<uint8_t>>();
	if (reserve_size > 0)
	{
		m_data->resize(reserve_size);
	}
	refresh_cache();
}
Memory_Sink::~Memory_Sink()
{
	m_data->clear();
	refresh_cache();
}

size_t Memory_Sink::get_size() const
{
	return m_data_size;
}
void Memory_Sink::set_size(size_t size)
{
	m_data_size = size;
	if (m_data->size() < size)
	{
		m_data->resize(size);
		refresh_cache();
	}
}
size_t Memory_Sink::get_capacity() const
{
	return m_data->size();
}

size_t Memory_Sink::write(uint8_t const* buffer, size_t size)
{
	if (size > 0)
	{
		if (m_offset + size > m_data_size)
		{
			if (m_data->size() <= m_offset + size)
			{
				if (m_offset + size < 1000000)
				{
					m_data->resize((m_offset + size) * 2);
				}
				else
				{
					//if the file is huge, grow less
					m_data->resize(((m_offset + size) * 300) >> 8);
				}
				refresh_cache();
			}

			m_data_size = m_offset + size;
		}

		memcpy(m_data_ptr + m_offset, buffer, size);
		m_offset += size;
	}
	return size;
}

void Memory_Sink::swap_data(Memory_Source& source)
{
	m_data->resize(m_data_size);
	m_data_size = 0;
	m_offset = 0;

	std::swap(m_data, source.m_data);

	refresh_cache();
	source.refresh_cache();
}

uint8_t const* Memory_Sink::get_data_ptr() const
{
	if (m_offset >= m_data_size)
	{
		return nullptr;
	}
	return m_data_ptr + m_offset;
}
size_t Memory_Sink::get_offset() const
{
	return m_offset;
}
size_t Memory_Sink::seek(size_t offset)
{
	m_offset = offset;
	return m_offset;
}
size_t Memory_Sink::seek_relative(int offset)
{
	if (offset < 0)
	{
		if ((size_t)-offset > m_offset) //seeking backwards too much?)
		{
			return seek(0);
		}
		return seek(m_offset - (-offset));
	}
	return seek(m_offset + offset);
}

std::vector<uint8_t> Memory_Sink::get_data_copy() const
{
	m_data->resize(m_data_size);
	refresh_cache();
	return *m_data;
}

void Memory_Sink::refresh_cache() const
{
	m_data_ptr = m_data->empty() ? nullptr : m_data->data();
}