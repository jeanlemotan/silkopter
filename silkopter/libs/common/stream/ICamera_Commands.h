#pragma once

#include "Stream_Base.h"

namespace silk
{
namespace stream
{


class ICamera_Commands : public IScalar_Stream<Semantic::CAMERA_COMMANDS>
{
public:
    typedef std::false_type can_be_filtered_t;

    enum class Quality : uint8_t
    {
        LOW,
        HIGH
    };

    struct Value
    {
        Quality quality = Quality::LOW;
        bool recording = false;
    };

    typedef stream::Sample<Value>     Sample;

    virtual ~ICamera_Commands() = default;

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}


namespace util
{
namespace serialization
{

template<> inline void serialize(Buffer_t& buffer, silk::stream::ICamera_Commands::Value const& value, size_t& off)
{
    uint8_t data = static_cast<uint8_t>(value.quality) & 0x3;
    data |= value.recording ? 0x4 : 0x0;
    serialize(buffer, data, off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::ICamera_Commands::Value& value, size_t& off) -> bool
{
    uint8_t data = 0;
    if (!deserialize(buffer, data, off))
    {
        return false;
    }

    value.quality = static_cast<silk::stream::ICamera_Commands::Quality>(data & 0x3);
    value.recording = (data & 0x4) ? true : false;

    return true;
}


}
}
