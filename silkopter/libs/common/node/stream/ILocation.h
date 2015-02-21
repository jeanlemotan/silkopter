#pragma once

#include "IStream.h"

namespace silk
{
namespace node
{
namespace stream
{

class ILocation : public IStream
{
public:
    struct Value
    {
        enum class Fix : uint8_t
        {
            NONE,
            FIX_2D,
            FIX_3D
        };

        Fix fix;
        uint8_t sattelite_count = 0;

        double longitude; //radians
        double latitude; //radians
        float precision = std::numeric_limits<float>::max();

        float altitude; //meters
        float altitude_precision = std::numeric_limits<float>::max();

        math::vec2f velocity_2d; //meters/second but x/y only
        math::vec3f velocity; //meters/second
        math::vec2f direction; //normalized
    };

    typedef stream::Sample<Value>     Sample;

    virtual ~ILocation() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    //filter helpers
    static const size_t FILTER_CHANNELS = 10;
    static auto get_channels_from_value(std::array<double, FILTER_CHANNELS>& channels, Value const& value) -> bool
    {
        channels[0] = value.longitude;
        channels[1] = value.latitude;
        channels[2] = value.altitude;
        channels[3] = value.velocity_2d.x;
        channels[4] = value.velocity_2d.y;
        channels[5] = value.velocity.x;
        channels[6] = value.velocity.y;
        channels[7] = value.velocity.z;
        channels[8] = value.direction.x;
        channels[9] = value.direction.y;
        return true;/*value.fix != Value::Fix::NONE &&
                value.precision < 100.f &&
                value.altitude_precision < 100.f;*/
    }
    static void get_value_from_channels(Value& value, std::array<double, FILTER_CHANNELS> const& channels)
    {
        value.longitude      = channels[0];
        value.latitude       = channels[1];
        value.altitude       = channels[2];
        value.velocity_2d.x  = channels[3];
        value.velocity_2d.y  = channels[4];
        value.velocity.x     = channels[5];
        value.velocity.y     = channels[6];
        value.velocity.z     = channels[7];
        value.direction.x    = channels[8];
        value.direction.y    = channels[9];
    }
};


}
}
}
