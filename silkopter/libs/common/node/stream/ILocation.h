#pragma once

#include "IStream.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{
namespace stream
{

class ILocation : public IStream
{
    DEFINE_RTTI_CLASS(ILocation, IStream);
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

};
DECLARE_CLASS_PTR(ILocation);


}
}
}



namespace util
{
namespace dsp
{
template<> inline bool equals(silk::node::stream::ILocation::Value const& a, silk::node::stream::ILocation::Value const& b)
{
    return math::equals(a.longitude, b.longitude) ||
           math::equals(a.latitude, b.latitude) ||
           math::equals(a.altitude, b.altitude) ||
           math::equals(a.velocity_2d, b.velocity_2d) ||
           math::equals(a.velocity, b.velocity) ||
           math::equals(a.direction, b.direction);
}
template<> inline silk::node::stream::ILocation::Value add(silk::node::stream::ILocation::Value const& a, silk::node::stream::ILocation::Value const& b)
{
    silk::node::stream::ILocation::Value r = a;
    r.longitude += b.longitude;
    r.latitude += b.latitude;
    r.altitude += b.altitude;
    r.velocity_2d += b.velocity_2d;
    r.velocity += b.velocity;
    r.direction += b.direction;
    return r;
}
template<> inline silk::node::stream::ILocation::Value scale(silk::node::stream::ILocation::Value const& a, double scale)
{
    silk::node::stream::ILocation::Value r = a;
    r.longitude *= scale;
    r.latitude *= scale;
    r.altitude *= scale;
    r.velocity_2d *= scale;
    r.velocity *= scale;
    r.direction *= scale;
    return r;
}
}
}

