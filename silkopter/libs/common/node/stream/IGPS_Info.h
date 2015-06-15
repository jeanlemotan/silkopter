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
    struct Value
    {
        enum class Fix
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
