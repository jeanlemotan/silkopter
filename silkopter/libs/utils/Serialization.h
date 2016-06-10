#pragma once

//#include "rapidjson/document.h"
//#include "rapidjson/stringbuffer.h"
//#include "rapidjson/writer.h"
#include "utils/Serialization.h"

namespace util
{
namespace serialization
{
typedef std::vector<uint8_t> Buffer_t;

template<class T> auto deserialize(Buffer_t const& buffer, T& val, size_t& off) -> bool
{
    static_assert(std::is_pod<T>::value, "you need a specialized packer/unpacker for T");
    static_assert(std::is_standard_layout<T>::value, "you need a specialized packer/unpacker for T");
    static_assert(!std::is_pointer<T>::value, "you cannot pack/unpack a pointer");
    if (off + sizeof(T) > buffer.size())
    {
        return false;
    }
    std::copy(buffer.begin() + off, buffer.begin() + off + sizeof(T), reinterpret_cast<uint8_t*>(&val));
    off += sizeof(T);
    return true;
}
template<> inline auto deserialize(Buffer_t const& buffer, std::string& val, size_t& off) -> bool
{
    uint32_t size;
    if (!deserialize(buffer, size, off))
    {
        return false;
    }
    if (off + size > buffer.size())
    {
        return false;
    }
    val.resize(size);
    std::copy(buffer.begin() + off, buffer.begin() + off + size, val.begin());
    off += size;
    return true;
}
template<class T> inline auto deserialize(Buffer_t const& buffer, math::vec2<T>& val, size_t& off) -> bool
{
    return deserialize(buffer, val.x, off) &&
           deserialize(buffer, val.y, off);
}
template<class T> inline auto deserialize(Buffer_t const& buffer, math::vec3<T>& val, size_t& off) -> bool
{
    return deserialize(buffer, val.x, off) &&
           deserialize(buffer, val.y, off) &&
           deserialize(buffer, val.z, off);
}
template<class T> inline auto deserialize(Buffer_t const& buffer, math::quat<T>& val, size_t& off) -> bool
{
    return deserialize(buffer, val.x, off) &&
           deserialize(buffer, val.y, off) &&
           deserialize(buffer, val.z, off) &&
           deserialize(buffer, val.w, off);
}
//template<> inline auto deserialize(Buffer_t const& buffer, rapidjson::Document& json, size_t& off) -> bool
//{
//    std::string str;
//    if (!deserialize(buffer, str, off))
//    {
//        return false;
//    }
//    json.SetObject();
//    if (!str.empty() && json.Parse(str.c_str()).HasParseError())
//    {
//        QLOGE("Failed to parse json: {}:{}", json.GetParseError(), json.GetErrorOffset());
//        return false;
//    }
//    return true;
//}
template<class T> inline auto deserialize(Buffer_t const& buffer, std::vector<T>& val, size_t& off) -> bool
{
    val.clear();
    uint32_t size = 0;
    if (!deserialize(buffer, size, off))
    {
        return false;
    }
    val.resize(size);
    for (auto& i: val)
    {
        if (!deserialize(buffer, i, off))
        {
            val.clear();
            return false;
        }
    }
    return true;
}

template<class T> void serialize(Buffer_t& buffer, T const& val, size_t& off)
{
    static_assert(std::is_pod<T>::value, "you need a specialized packer/unpacker for T");
    static_assert(std::is_standard_layout<T>::value, "you need a specialized packer/unpacker for T");
    static_assert(!std::is_pointer<T>::value, "you cannot pack/unpack a pointer");
    if (off + sizeof(T) > buffer.size())
    {
        buffer.resize(off + sizeof(T));
    }
    auto const* src = reinterpret_cast<uint8_t const*>(&val);
    std::copy(src, src + sizeof(T), buffer.data() + off);
    off += sizeof(T);
}
template<> inline void serialize(Buffer_t& buffer, std::string const& val, size_t& off)
{
    serialize(buffer, static_cast<uint32_t>(val.size()), off);
    if (off + val.size() > buffer.size())
    {
        buffer.resize(off + val.size());
    }
    std::copy(val.begin(), val.end(), buffer.data() + off);
    off += val.size();
}
template<class T> inline void serialize(Buffer_t& buffer, math::vec2<T> const& val, size_t& off)
{
    serialize(buffer, val.x, off);
    serialize(buffer, val.y, off);
}
template<class T> inline void serialize(Buffer_t& buffer, math::vec3<T> const& val, size_t& off)
{
    serialize(buffer, val.x, off);
    serialize(buffer, val.y, off);
    serialize(buffer, val.z, off);
}
template<class T> inline void serialize(Buffer_t& buffer, math::quat<T> const& val, size_t& off)
{
    serialize(buffer, val.x, off);
    serialize(buffer, val.y, off);
    serialize(buffer, val.z, off);
    serialize(buffer, val.w, off);
}
//template<> inline void serialize(Buffer_t& buffer, rapidjson::Document const& json, size_t& off)
//{
//    rapidjson::StringBuffer str_buf;
//    rapidjson::Writer<rapidjson::StringBuffer> writer(str_buf);
//    json.Accept(writer);
//    std::string str(str_buf.GetString(), str_buf.GetSize());
//    serialize(buffer, str, off);
//}
template<class T> inline void serialize(Buffer_t& buffer, std::vector<T> const& val, size_t& off)
{
    serialize(buffer, static_cast<uint32_t>(val.size()), off);
    for (auto const& i: val)
    {
        serialize(buffer, i, off);
    }
}

}
}

