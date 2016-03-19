#pragma once

#include "Stream_Base.h"
#include "ICurrent.h"
#include "IVoltage.h"
#include <limits>

namespace silk
{
namespace stream
{

class IGPS_Info : public IScalar_Stream<Semantic::GPS_INFO>
{
public:
    typedef std::false_type can_be_filtered_t;

    struct Value
    {
        enum class Fix : uint8_t
        {
            INVALID,
            FIX_2D,
            FIX_3D
        } fix;

        uint16_t visible_satellites = 0; //how many satellites are visible
        uint16_t fix_satellites = 0;     //how many satellites are tracked (participate in the fix)
        float pacc = std::numeric_limits<float>::infinity(); //position accuracy (standard deviation)
        float vacc = std::numeric_limits<float>::infinity(); //velocity accuracy (standard deviation)

        float hdop = std::numeric_limits<float>::infinity(); //horizontal dillution of precision (the minimum precision obtainable form the sats in view)
        float vdop = std::numeric_limits<float>::infinity(); //vertical dillution of precision
        float pdop = std::numeric_limits<float>::infinity(); //position dillution of precision
    };
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}



namespace util
{
namespace serialization
{

template<> inline void serialize(Buffer_t& buffer, silk::stream::IGPS_Info::Value const& value, size_t& off)
{
    serialize(buffer, value.fix, off);
    serialize(buffer, value.visible_satellites, off);
    serialize(buffer, value.fix_satellites, off);
    serialize(buffer, value.pacc, off);
    serialize(buffer, value.vacc, off);
    serialize(buffer, value.hdop, off);
    serialize(buffer, value.vdop, off);
    serialize(buffer, value.pdop, off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::IGPS_Info::Value& value, size_t& off) -> bool
{
    return deserialize(buffer, value.fix, off) &&
            deserialize(buffer, value.visible_satellites, off) &&
            deserialize(buffer, value.fix_satellites, off) &&
            deserialize(buffer, value.pacc, off) &&
            deserialize(buffer, value.vacc, off) &&
            deserialize(buffer, value.hdop, off) &&
            deserialize(buffer, value.vdop, off) &&
            deserialize(buffer, value.pdop, off);
}


}
}

