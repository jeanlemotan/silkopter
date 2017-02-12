#pragma once

namespace q
{
namespace data
{

class Sub_Source : public Source
{
public:
    Sub_Source(Source_uptr source, size_t start, size_t size);
    virtual ~Sub_Source();

    virtual size_t		read(uint8_t* buffer, size_t size);

    virtual size_t		get_size() const;

    virtual size_t		get_offset() const;

    virtual size_t		seek(size_t offset);
    virtual size_t		seek_relative(int offset);

private:
    Source_uptr			m_source;
    size_t				m_start;
    size_t				m_size;
};

//////////////////////////////////////////////////////////////////////////

}
}
