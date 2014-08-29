#include "QDataStdAfx.h"
#include "data/Sink.h"
#include "data/Source.h"
#include "data/File_Source.h"

using namespace q;
using namespace data;

File_Source::File_Source(Path const& path)
	: m_offset(0)
	, m_size(0)
{
	m_file = fopen(path.get_as_string().c_str(), "rb");
	if (m_file)
	{
		m_size = util::fs::get_file_size(path);
	}
}
File_Source::~File_Source()
{
	if (m_file)
	{
		fclose(m_file);
	}
}

size_t File_Source::get_size() const
{
	QASSERT(is_open());
	return m_size;
}

bool File_Source::is_open() const
{
	return m_file != nullptr;
}

size_t File_Source::read(uint8_t* buffer, size_t size)
{
	QASSERT(is_open());
	if (is_open())
	{
		size = std::min(size, m_size - m_offset);
		if (size == 0)
		{
			return 0;
		}
		auto r = fread(buffer, 1, size, m_file);
		m_offset += r;
		return r;
	}
	return 0;
}

size_t File_Source::get_offset() const
{
	QASSERT(is_open());
	return m_offset;
}
size_t File_Source::seek(size_t offset)
{
	QASSERT(is_open());
	offset = std::min(offset, m_size);
	if (is_open() && offset != m_offset)
	{
		m_offset = offset;
		fseek(m_file, m_offset, SEEK_SET);
	}
	return m_offset;
}
size_t File_Source::seek_relative(int offset)
{
	QASSERT(is_open());
	if (is_open())
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
	return 0;
}

