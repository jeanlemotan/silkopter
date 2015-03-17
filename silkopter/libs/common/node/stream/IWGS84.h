#pragma once

#include "IStream.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{
namespace stream
{

class IWGS84 : public IStream
{
    DEFINE_RTTI_CLASS(IWGS84, IStream);
public:
    struct Value
    {
        static constexpr double MAX_VALID_ACCURACY = 999999.0;

        math::vec2d lat_lon; //radians
        double lat_lon_accuracy = MAX_VALID_ACCURACY * 2.0; //meters

        double altitude; //meters
        double altitude_accuracy = MAX_VALID_ACCURACY * 2.0; //meters
    };

    typedef stream::Sample<Value>     Sample;

    virtual ~IWGS84() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

};
DECLARE_CLASS_PTR(IWGS84);


}
}
}



namespace util
{
namespace dsp
{
template<> inline bool equals(silk::node::stream::IWGS84::Value const& a, silk::node::stream::IWGS84::Value const& b)
{
    return math::equals(a.lat_lon, b.lat_lon) &&
           math::equals(a.altitude, b.altitude);
}
template<> inline silk::node::stream::IWGS84::Value add(silk::node::stream::IWGS84::Value const& a, silk::node::stream::IWGS84::Value const& b)
{
    silk::node::stream::IWGS84::Value r = a;
    r.lat_lon  += b.lat_lon;
    r.lat_lon_accuracy += b.lat_lon_accuracy;
    r.altitude += b.altitude;
    r.altitude_accuracy += b.altitude_accuracy;
    return r;
}
template<> inline silk::node::stream::IWGS84::Value scale(silk::node::stream::IWGS84::Value const& a, double scale)
{
    silk::node::stream::IWGS84::Value r = a;
    r.lat_lon  *= scale;
    r.lat_lon_accuracy  *= scale;
    r.altitude *= scale;
    r.altitude_accuracy *= scale;
    return r;
}
template<> inline void fix(silk::node::stream::IWGS84::Value& a)
{
    a.lat_lon_accuracy = math::clamp(a.lat_lon_accuracy, 0.0, silk::node::stream::IWGS84::Value::MAX_VALID_ACCURACY * 2.0);
    a.altitude_accuracy = math::clamp(a.altitude_accuracy, 0.0, silk::node::stream::IWGS84::Value::MAX_VALID_ACCURACY * 2.0);
}

}
}

