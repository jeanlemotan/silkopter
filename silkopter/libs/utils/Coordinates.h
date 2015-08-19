#pragma once

namespace util
{
namespace coordinates
{
namespace constants
{
    // WGS84 ellipsoid constants
    static constexpr double radius = 6378137;
    static constexpr double radius_sq = radius * radius;

    static constexpr double eccentricity = 8.1819190842622e-2;
    static constexpr double eccentricity_sq = eccentricity * eccentricity;
}

typedef math::vec3d ECEF;

struct LLA
{
    LLA() = default;
    LLA(double latitude, double longitude, double altitude)
        : latitude(latitude), longitude(longitude), altitude(altitude) {}

    double latitude = 0; //radians
    double longitude = 0; //radians
    double altitude = 0; //meters
};

auto normal_distance(double latitude) -> double;
auto ecef_to_lla(ECEF const& ecef) -> LLA;
auto lla_to_ecef(LLA const& lla) -> ECEF;
auto enu_to_ecef_transform(LLA const& lla) -> math::trans3dd;
auto enu_to_ecef_rotation(LLA const& lla) -> math::mat3d;

}
}
