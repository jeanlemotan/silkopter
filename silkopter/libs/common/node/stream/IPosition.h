#pragma once

#include "Stream_Base.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{
namespace stream
{

class IECEF_Position : public ISpatial_Stream<Type::POSITION, Space::ECEF>
{
public:
    typedef std::true_type can_be_filtered_t;

    typedef math::vec3d Value; //meters
    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Position);

class ILLA_Position : public ISpatial_Stream<Type::POSITION, Space::LLA>
{
public:
    typedef std::false_type can_be_filtered_t;

    struct Value
    {
        double latitude = 0; //radians
        double longitude = 0; //radians
        double altitude = 0; //meters
    };

    typedef stream::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(ILLA_Position);


inline ILLA_Position::Value ecef_to_lla(IECEF_Position::Value const& ecef)
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

    ILLA_Position::Value ret;
    ret.latitude = lat;
    ret.longitude = lon;
    ret.altitude = alt;
    return ret;
}


}
}
}



namespace util
{
namespace dsp
{
template<> inline void apply_coefficients(silk::node::stream::IECEF_Position::Value& x,
                                   silk::node::stream::IECEF_Position::Value& w0,
                                   silk::node::stream::IECEF_Position::Value& w1,
                                   silk::node::stream::IECEF_Position::Value& w2, float d1, float d2, float A)
{
    w0 = double(d1)*w1 + double(d2)*w2 + x;
    x = double(A)*(w0 + 2.0*w1 + w2);
    w2 = w1;
    w1 = w0;
}

}
}

