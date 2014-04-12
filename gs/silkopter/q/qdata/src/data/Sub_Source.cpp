#include "QDataStdAfx.h"
#include "data/Sink.h"
#include "data/Source.h"
#include "data/Sub_Source.h"

using namespace q;
using namespace data;

Sub_Source::Sub_Source(Source_uptr source, size_t start, size_t size)
	: m_source(std::move(source))
	, m_start(start)
	, m_size(size)
{
}
Sub_Source::~Sub_Source()
{
}

size_t Sub_Source::get_size() const
{
	return std::min(m_source->get_size() - m_start, m_size);
}

size_t Sub_Source::read(uint8_t* buffer, size_t size)
{
	size = std::min(size, m_size - (m_source->get_offset() - m_start));
	if (size == 0)
	{
		return 0;
	}
	return m_source->read(buffer, size);
}

size_t Sub_Source::get_offset() const
{
	return m_source->get_offset() - m_start;
}
size_t Sub_Source::seek(size_t offset)
{
	offset = std::min(offset, m_size);
	m_source->seek(m_start + offset);
	return m_source->get_offset() - m_start;
}
size_t Sub_Source::seek_relative(int offset)
{
	size_t crt_offset = get_offset();
	if (offset < 0) 
	{
		if ((size_t)-offset > crt_offset) //seeking backwards too much?
		{
			return seek(0);
		}
		return seek(crt_offset - (-offset));
	}
	return seek(crt_offset + offset);
}

