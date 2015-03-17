#pragma once

#include "IStream.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{
namespace stream
{

class ILocationxxx : public IStream
{
    DEFINE_RTTI_CLASS(ILocationxxx, IStream);
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

    virtual ~ILocationxxx() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

};
DECLARE_CLASS_PTR(ILocationxxx);


}
}
}



