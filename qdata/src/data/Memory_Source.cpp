#include "QDataStdAfx.h"
#include "data/Sink.h"
#include "data/Source.h"
#include "data/Memory_Source.h"
#include "data/Memory_Sink.h"

using namespace q;
using namespace data;

Memory_Source::Memory_Source()
	: m_offset(0)
	, m_data_size(0)
	, m_data_ptr(nullptr)
{
	m_data = std::make_shared<std::vector<uint8_t>>();
	refresh_cache();
}
Memory_Source::Memory_Source(std::vector<uint8_t>&& data)
	: m_offset(0)
	, m_data_size(0)
	, m_data_ptr(nullptr)
{
	m_data = std::make_shared<std::vector<uint8_t>>();
	std::swap(*m_data, data);
	refresh_cache();
}
Memory_Source::Memory_Source(uint8_t const* data, size_t size)
	: m_offset(0)
	, m_data_size(0)
	, m_data_ptr(nullptr)
{
	m_data = std::make_shared<std::vector<uint8_t>>();
	if (data && size > 0)
	{
		m_data->resize(size);
		memcpy(m_data->data(), data, size);
	}
	refresh_cache();
}
Memory_Source::~Memory_Source()
{
}

size_t Memory_Source::get_size() const
{
	return m_data_size;
}
size_t Memory_Source::get_capacity() const
{
	return m_data->capacity();
}

size_t Memory_Source::read(uint8_t* buffer, size_t size)
{
	size = std::min(size, m_data_size - m_offset);
	if (size == 0)
	{
		return 0;
	}
	memcpy(buffer, m_data_ptr + m_offset, size);
	m_offset += size;
	return size;
}
size_t Memory_Source::get_offset() const
{
	return m_offset;
}
size_t Memory_Source::seek(size_t offset)
{
	m_offset = std::min(offset, m_data_size);
	return m_offset;
}
size_t Memory_Source::seek_relative(int offset)
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

uint8_t const* Memory_Source::get_data_ptr() const
{
	if (m_offset >= m_data_size)
	{
		return nullptr;
	}

	return m_data_ptr + m_offset;
}
std::vector<uint8_t> Memory_Source::get_data_copy() const
{
	return m_data_size > 0 ? *m_data : std::vector<uint8_t>();
}

void Memory_Source::swap_data(Memory_Sink& sink)
{
	sink.swap_data(*this);
	m_offset = 0;
}

void Memory_Source::refresh_cache() const
{
	m_data_size = m_data->size();
	m_data_ptr = m_data_size > 0 ? m_data->data() : nullptr;
}
