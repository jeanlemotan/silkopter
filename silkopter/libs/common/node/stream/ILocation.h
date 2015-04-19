#pragma once

#include "IStream.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{
namespace stream
{

class IECEF_Location : public ISpatial_Stream<Type::LOCATION, Space::ECEF>
{
public:
    struct Value
    {
        static constexpr double MAX_VALID_ACCURACY = 999999.0;

        math::vec3d position; //meters
        double position_accuracy = MAX_VALID_ACCURACY * 2.0; //meters

        math::vec3d velocity; //meters/second
        double velocity_accuracy = MAX_VALID_ACCURACY * 2.0; //meters

        math::vec2d direction; //normalized
    };
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Location);


inline std::array<double, 3> ecef_to_lla(math::vec3d const& ecef)
{
    // WGS84 ellipsoid constants
    static constexpr double a = 6378137; // radius
    static constexpr double e = 8.1819190842622e-2;  // eccentricity
    double asq = math::square(a);
    double esq = math::square(e);

    double x = ecef.x;
    double y = ecef.y;
    double z = ecef.z;

    double b = math::sqrt( asq * (1-esq) );
    double bsq = math::square(b);
    double ep = math::sqrt( (asq - bsq)/bsq);
    double p = math::sqrt( math::square(x) + math::square(y) );
    double th = math::atan2(a*z, b*p);

    double lon = math::atan2(y, x);
    double lat = math::atan2( (z + math::square(ep)*b*math::pow(math::sin(th), 3.0) ), (p - esq*a*math::pow(math::cos(th), 3.0)) );
    double N = a/( math::sqrt(1-esq*math::square(math::sin(lat))) );
    double alt = p / math::cos(lat) - N;

    // mod lat to 0-2pi
    //lon = lon % (2.0 * math::angled::pi);

    // correction for altitude near poles left out.

    std::array<double, 3> ret = {lat, lon, alt};
    return ret;
}


}
}
}



namespace util
{
namespace dsp
{
template<> inline bool equals(silk::node::stream::IECEF_Location::Value const& a, silk::node::stream::IECEF_Location::Value const& b)
{
    return math::equals(a.position, b.position) &&
           math::equals(a.velocity, b.velocity) &&
           math::equals(a.direction, b.direction);
}
template<> inline void apply_coefficients(silk::node::stream::IECEF_Location::Value& x,
                                   silk::node::stream::IECEF_Location::Value& w0,
                                   silk::node::stream::IECEF_Location::Value& w1,
                                   silk::node::stream::IECEF_Location::Value& w2, float d1, float d2, float A)
{
}
//template<> inline silk::node::stream::IECEF_Location::Value add(silk::node::stream::IECEF_Location::Value const& a, silk::node::stream::IECEF_Location::Value const& b)
//{
//    silk::node::stream::IECEF_Location::Value r = a;
//    r.position  += b.position;
//    r.position_accuracy  += b.position_accuracy;
//    r.velocity  += b.velocity;
//    r.velocity_accuracy  += b.velocity_accuracy;
//    r.direction += b.direction;
//    return r;
//}
//template<> inline silk::node::stream::IECEF_Location::Value scale(silk::node::stream::IECEF_Location::Value const& a, double scale)
//{
//    silk::node::stream::IECEF_Location::Value r = a;
//    r.position  *= scale;
//    r.position_accuracy  *= scale;
//    r.velocity  *= scale;
//    r.velocity_accuracy  *= scale;
//    r.direction *= scale;
//    return r;
//}
//template<> inline void fix(silk::node::stream::IECEF_Location::Value& a)
//{
//    a.direction.normalize<math::safe>();
//    a.position_accuracy = math::clamp(a.position_accuracy, 0.0, silk::node::stream::IECEF_Location::Value::MAX_VALID_ACCURACY * 2.0);
//    a.velocity_accuracy = math::clamp(a.velocity_accuracy, 0.0, silk::node::stream::IECEF_Location::Value::MAX_VALID_ACCURACY * 2.0);
//}

}
}

