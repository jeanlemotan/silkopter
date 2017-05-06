#pragma once

#include "mem/auto_buffer.h"

namespace q
{
namespace data
{

class Source;
DECLARE_CLASS_PTR(Source);

class Source : util::Noncopyable
{
public:
    virtual ~Source() {}

    //tries to read size bytes into the destination pointer. Returns the actual number of bytes read
    virtual size_t		read(uint8_t* buffer, size_t size) = 0;

    virtual size_t		get_size() const = 0;

    //returns the source offset. It's always <= size
    virtual size_t		get_offset() const = 0;

    //changes the position to the specified offset. Returns the new offset
    //NOTE - you cannot seek past the end of the source
    virtual size_t		seek(size_t offset) = 0;
    virtual size_t		seek_relative(int offset) = 0;
};

//////////////////////////////////////////////////////////////////////////

inline std::vector<uint8_t> read_whole_source(Source& s)
{
    auto size = s.get_size();
    std::vector<uint8_t> buf(size);
    s.seek(0);
    s.read(buf.data(), size);
    return std::move(buf);
}

template<class String_T>
String_T read_whole_source_as_string(Source& s)
{
    auto size = s.get_size();
    boost::auto_buffer<char, boost::store_n_bytes<1024>> buffer(size);
    s.seek(0);
    s.read((uint8_t*)buffer.data(), size);
    String_T res(buffer.data(), buffer.data() + size);
    return res;
}

template<class T> Source& operator>>(Source& source, T& val)
{
    source.read((uint8_t*)&val, sizeof(val));
    return source;
}

template<class T, class U> Source& operator>>(Source& source, std::pair<T, U>& val)
{
    source.read((uint8_t*)&val.first, sizeof(val.first));
    source.read((uint8_t*)&val.second, sizeof(val.second));
    return source;
}

template<class T, class SIZE_T = uint32_t> Source& operator>>(Source& source, std::vector<T>& val)
{
    val.clear();
    SIZE_T size;
    source >> size;
    val.resize(size);
    for (auto& v: val)
    {
        source >> v;
    }
    return source;
}

inline Source& operator>>(Source& source, std::string& val)
{
    boost::auto_buffer<uint8_t, boost::store_n_bytes<1024>> buffer;
    uint16_t size;
    source >> size;
    buffer.uninitialized_resize(size + 1);
    source.read(buffer.data(), size);
    buffer.data()[size] = 0;
    val = (char*)buffer.data();
    return source;
}

}
}
