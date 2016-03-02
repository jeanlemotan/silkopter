#include "QDataStdAfx.h"
#include "data/Sink.h"
#include "data/File_Sink.h"

using namespace q;
using namespace data;

File_Sink::File_Sink(Path const& path)
{
	m_file = fopen(path.get_as_string().c_str(), "wb");
    if (!m_file)
    {
        QLOGW("Cannot open {}: {}", path, strerror(errno));
    }
}
File_Sink::~File_Sink()
{
	if (m_file)
	{
		fclose(m_file);
	}
}

size_t File_Sink::get_size() const
{
	QASSERT(is_open());
	return m_size;
}

bool File_Sink::is_open() const
{
	return m_file != nullptr;
}

size_t File_Sink::write(uint8_t const* buffer, size_t size)
{
	QASSERT(is_open());
	if (is_open())
	{
		auto r = fwrite(buffer, size, 1, m_file);
		m_offset += r;
		m_size = std::max(m_size, m_offset);
		return r;
	}
	return 0;
}

size_t File_Sink::get_offset() const
{
	QASSERT(is_open());
	return m_offset;
}
size_t File_Sink::seek(size_t offset)
{
	QASSERT(is_open());
	if (is_open() && m_offset != offset)
	{
		m_offset = offset;
		fseek(m_file, m_offset, SEEK_SET);
	}
	return m_offset;
}
size_t File_Sink::seek_relative(int offset)
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

void File_Sink::flush()
{
    QASSERT(is_open());
    if (is_open())
    {
        fflush(m_file);
    }
}
