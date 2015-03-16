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

        struct WGS84
        {
            static constexpr double MAX_VALID_ACCURACY = 999999.0;

            math::vec2d lat_lon; //radians
            double lat_lon_accuracy = MAX_VALID_ACCURACY * 2.0; //meters

            double altitude; //meters
            double altitude_accuracy = MAX_VALID_ACCURACY * 2.0; //meters
        } wgs84;

        struct ECEF
        {
            static constexpr double MAX_VALID_ACCURACY = 999999.0;

            math::vec3d position; //meters
            double position_accuracy = MAX_VALID_ACCURACY * 2.0; //meters

            math::vec3d velocity; //meters/second
            double velocity_accuracy = MAX_VALID_ACCURACY * 2.0; //meters

            math::vec2d direction; //normalized
        } ecef;
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
    return math::equals(a.wgs84.lat_lon, b.wgs84.lat_lon) &&
           math::equals(a.wgs84.altitude, b.wgs84.altitude) &&
           math::equals(a.ecef.position, b.ecef.position) &&
           math::equals(a.ecef.velocity, b.ecef.velocity) &&
           math::equals(a.ecef.direction, b.ecef.direction);
}
template<> inline silk::node::stream::ILocation::Value add(silk::node::stream::ILocation::Value const& a, silk::node::stream::ILocation::Value const& b)
{
    silk::node::stream::ILocation::Value r = a;
    r.wgs84.lat_lon  += b.wgs84.lat_lon;
    r.wgs84.lat_lon_accuracy += b.wgs84.lat_lon_accuracy;
    r.wgs84.altitude += b.wgs84.altitude;
    r.wgs84.altitude_accuracy += b.wgs84.altitude_accuracy;
    r.ecef.position  += b.ecef.position;
    r.ecef.position_accuracy  += b.ecef.position_accuracy;
    r.ecef.velocity  += b.ecef.velocity;
    r.ecef.velocity_accuracy  += b.ecef.velocity_accuracy;
    r.ecef.direction += b.ecef.direction;
    return r;
}
template<> inline silk::node::stream::ILocation::Value scale(silk::node::stream::ILocation::Value const& a, double scale)
{
    silk::node::stream::ILocation::Value r = a;
    r.wgs84.lat_lon  *= scale;
    r.wgs84.lat_lon_accuracy  *= scale;
    r.wgs84.altitude *= scale;
    r.wgs84.altitude_accuracy *= scale;
    r.ecef.position  *= scale;
    r.ecef.position_accuracy  *= scale;
    r.ecef.velocity  *= scale;
    r.ecef.velocity_accuracy  *= scale;
    r.ecef.direction *= scale;
    return r;
}
template<> inline void fix(silk::node::stream::ILocation::Value& a)
{
    a.ecef.direction.normalize<math::safe>();
    a.wgs84.lat_lon_accuracy = math::clamp(a.wgs84.lat_lon_accuracy, 0.0, silk::node::stream::ILocation::Value::WGS84::MAX_VALID_ACCURACY * 2.0);
    a.wgs84.altitude_accuracy = math::clamp(a.wgs84.altitude_accuracy, 0.0, silk::node::stream::ILocation::Value::WGS84::MAX_VALID_ACCURACY * 2.0);
    a.ecef.position_accuracy = math::clamp(a.ecef.position_accuracy, 0.0, silk::node::stream::ILocation::Value::ECEF::MAX_VALID_ACCURACY * 2.0);
    a.ecef.velocity_accuracy = math::clamp(a.ecef.velocity_accuracy, 0.0, silk::node::stream::ILocation::Value::ECEF::MAX_VALID_ACCURACY * 2.0);
}

}
}

