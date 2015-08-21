#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{

class IVideo : public IScalar_Stream<Type::VIDEO>
{
public:
    typedef std::false_type can_be_filtered_t;

    struct Value
    {
        enum class Type : uint8_t
        {
            H264
        };

        Type type;
        bool is_keyframe = false;
        math::vec2u32 resolution;
        std::vector<uint8_t> data;
    };
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IVideo);


}
}


namespace util
{
namespace serialization
{

template<> inline void serialize(Buffer_t& buffer, silk::stream::IVideo::Value const& value, size_t& off)
{
    serialize(buffer, value.type, off);
    serialize(buffer, value.resolution, off);
    serialize(buffer, value.is_keyframe, off);
    serialize(buffer, static_cast<uint32_t>(value.data.size()), off);
    if (buffer.size() < off + value.data.size())
    {
        buffer.resize(off + value.data.size());
    }
    std::copy(value.data.begin(), value.data.end(), buffer.begin() + off);
    off += value.data.size();
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::IVideo::Value& value, size_t& off) -> bool
{
    uint32_t size = 0;
    if (!deserialize(buffer, value.type, off) ||
        !deserialize(buffer, value.resolution, off) ||
        !deserialize(buffer, value.is_keyframe, off) ||
        !deserialize(buffer, size, off) ||
        size + off > buffer.size())
    {
        return false;
    }
    value.data.resize(size);
    std::copy(buffer.begin() + off, buffer.begin() + off + size, value.data.begin());
    off += size;
    return true;
}


}
}
