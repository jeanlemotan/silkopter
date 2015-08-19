#include "Coordinates.h"

namespace util
{
namespace coordinates
{

auto normal_distance(double latitude) -> double
{
    double N = constants::radius/( math::sqrt(1-constants::eccentricity_sq*math::square(math::sin(latitude))) );
    return N;
}

auto ecef_to_lla(ECEF const& ecef) -> LLA
{
    double x = ecef.x;
    double y = ecef.y;
    double z = ecef.z;

    double b = math::sqrt( constants::radius_sq * (1.0-constants::eccentricity_sq) );
    double bsq = math::square(b);
    double ep = math::sqrt( (constants::radius_sq - bsq)/bsq);
    double p = math::sqrt( math::square(x) + math::square(y) );
    double th = math::atan2(constants::radius*z, b*p);

    double lon = math::atan2(y, x);
    double lat = math::atan2( (z + math::square(ep)*b*math::pow(math::sin(th), 3.0) ), (p - constants::eccentricity_sq*constants::radius*math::pow(math::cos(th), 3.0)) );
    double N = normal_distance(lat);
    double alt = p / math::cos(lat) - N;

    // mod lat to 0-2pi
    //lon = lon % (2.0 * math::angled::pi);

    // correction for altitude near poles left out.

    LLA ret;
    ret.latitude = lat;
    ret.longitude = lon;
    ret.altitude = alt;
    return ret;
}

auto lla_to_ecef(LLA const& lla) -> ECEF
{
    double cos_long, sin_long;
    math::sin_cos(lla.longitude, sin_long, cos_long);
    double cos_lat, sin_lat;
    math::sin_cos(lla.latitude, sin_lat, cos_lat);

    double N = normal_distance(lla.latitude);

    ECEF ret;
    ret.x = (lla.altitude + N) * cos_lat * cos_long;
    ret.y = (lla.altitude + N) * cos_lat * sin_long;
    ret.z = (lla.altitude + ((1.0 - constants::eccentricity_sq) * N)) * sin_lat;

    return ret;
}

auto enu_to_ecef_transform(LLA const& lla) -> math::trans3dd
{
    double cos_long, sin_long;
    math::sin_cos(lla.longitude, sin_long, cos_long);
    double cos_lat, sin_lat;
    math::sin_cos(lla.latitude, sin_lat, cos_lat);


//ecef to enu
//    -sin_long               cos_long                0
//    -cos_long*sin_lat       -sin_lat*sin_long       cos_lat
//    cos_long*cos_lat        cos_lat*sin_long        sin_lat

//transpose - enu to ecef
//    -sin_long               -cos_long*sin_lat       cos_long*cos_lat
//    cos_long                -sin_lat*sin_long       cos_lat*sin_long
//    0                       cos_lat                 sin_lat

    math::trans3dd trans;

    trans.set_axis_x(math::vec3d(-sin_long,
                                  cos_long,
                                  0));
    trans.set_axis_y(math::vec3d(-cos_long*sin_lat,
                                 -sin_lat*sin_long,
                                  cos_lat));
    trans.set_axis_z(math::vec3d( cos_long*cos_lat,
                                  cos_lat*sin_long,
                                  sin_lat));

    math::vec3d ecef_p0;
    {
        double N = constants::radius/( math::sqrt(1-constants::eccentricity_sq*math::square(sin_lat)) );
        ecef_p0.x = (lla.altitude + N) * cos_lat * cos_long;
        ecef_p0.y = (lla.altitude + N) * cos_lat * sin_long;
        ecef_p0.z = (lla.altitude + ((1.0 - constants::eccentricity_sq) * N)) * sin_lat;
    }

    trans.set_translation(ecef_p0);

    return trans;
}

auto enu_to_ecef_rotation(LLA const& lla) -> math::mat3d
{
    double cos_long, sin_long;
    math::sin_cos(lla.longitude, sin_long, cos_long);
    double cos_lat, sin_lat;
    math::sin_cos(lla.latitude, sin_lat, cos_lat);


//ecef to enu
//    -sin_long               cos_long                0
//    -cos_long*sin_lat       -sin_lat*sin_long       cos_lat
//    cos_long*cos_lat        cos_lat*sin_long        sin_lat

//transpose - enu to ecef
//    -sin_long               -cos_long*sin_lat       cos_long*cos_lat
//    cos_long                -sin_lat*sin_long       cos_lat*sin_long
//    0                       cos_lat                 sin_lat

    math::mat3d mat;

    mat.set_axis_x(math::vec3d(-sin_long,
                                  cos_long,
                                  0));
    mat.set_axis_y(math::vec3d(-cos_long*sin_lat,
                                 -sin_lat*sin_long,
                                  cos_lat));
    mat.set_axis_z(math::vec3d( cos_long*cos_lat,
                                  cos_lat*sin_long,
                                  sin_lat));

    return mat;
}
}
}
