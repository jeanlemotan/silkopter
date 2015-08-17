#pragma once

#include "Stream_Base.h"
#include "ICurrent.h"
#include "IVoltage.h"
#include <limits>

namespace silk
{
namespace node
{
namespace stream
{

class IGPS_Info : public IScalar_Stream<Type::GPS_INFO>
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

        uint32_t visible_satellites = 0; //how many satellites are visible
        uint32_t fix_satellites = 0;     //how many satellites are tracked (participate in the fix)
        float position_accuracy = std::numeric_limits<float>::infinity(); //accuracy of the position, in meters
        float velocity_accuracy = std::numeric_limits<float>::infinity(); //accuracy of the velocity in meters
        float hdop = std::numeric_limits<float>::infinity(); //horizontal dillution of precision (the minimum precision obtainable form the sats in view)
        float vdop = std::numeric_limits<float>::infinity(); //vertical dillution of precision
        float pdop = std::numeric_limits<float>::infinity(); //position dillution of precision
    };
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IGPS_Info);


}
}
}



namespace util
{
namespace serialization
{

template<> inline void serialize(Buffer_t& buffer, silk::node::stream::IGPS_Info::Value const& value, size_t& off)
{
    serialize(buffer, value.fix, off);
    serialize(buffer, value.visible_satellites, off);
    serialize(buffer, value.fix_satellites, off);
    serialize(buffer, value.position_accuracy, off);
    serialize(buffer, value.velocity_accuracy, off);
    serialize(buffer, value.hdop, off);
    serialize(buffer, value.vdop, off);
    serialize(buffer, value.pdop, off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::node::stream::IGPS_Info::Value& value, size_t& off) -> bool
{
    return deserialize(buffer, value.fix, off) &&
            deserialize(buffer, value.visible_satellites, off) &&
            deserialize(buffer, value.fix_satellites, off) &&
            deserialize(buffer, value.position_accuracy, off) &&
            deserialize(buffer, value.velocity_accuracy, off) &&
            deserialize(buffer, value.hdop, off) &&
            deserialize(buffer, value.vdop, off) &&
            deserialize(buffer, value.pdop, off);
}


}
}

