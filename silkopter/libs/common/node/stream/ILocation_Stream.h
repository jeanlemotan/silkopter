#pragma once

#include "IStream.h"
#include "utils/Butterworth.h"

namespace silk
{
namespace node
{

class IECEF_Location_Stream : public ISpatial_Stream<IStream::Type::LOCATION, Space::ECEF>
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
    typedef node::Sample<Value>     Sample;
    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};
DECLARE_CLASS_PTR(IECEF_Location_Stream);


}
}



namespace util
{
namespace dsp
{
template<> inline bool equals(silk::node::IECEF_Location_Stream::Value const& a, silk::node::IECEF_Location_Stream::Value const& b)
{
    return math::equals(a.position, b.position) &&
           math::equals(a.velocity, b.velocity) &&
           math::equals(a.direction, b.direction);
}
template<> inline silk::node::IECEF_Location_Stream::Value add(silk::node::IECEF_Location_Stream::Value const& a, silk::node::IECEF_Location_Stream::Value const& b)
{
    silk::node::IECEF_Location_Stream::Value r = a;
    r.position  += b.position;
    r.position_accuracy  += b.position_accuracy;
    r.velocity  += b.velocity;
    r.velocity_accuracy  += b.velocity_accuracy;
    r.direction += b.direction;
    return r;
}
template<> inline silk::node::IECEF_Location_Stream::Value scale(silk::node::IECEF_Location_Stream::Value const& a, double scale)
{
    silk::node::IECEF_Location_Stream::Value r = a;
    r.position  *= scale;
    r.position_accuracy  *= scale;
    r.velocity  *= scale;
    r.velocity_accuracy  *= scale;
    r.direction *= scale;
    return r;
}
template<> inline void fix(silk::node::IECEF_Location_Stream::Value& a)
{
    a.direction.normalize<math::safe>();
    a.position_accuracy = math::clamp(a.position_accuracy, 0.0, silk::node::IECEF_Location_Stream::Value::MAX_VALID_ACCURACY * 2.0);
    a.velocity_accuracy = math::clamp(a.velocity_accuracy, 0.0, silk::node::IECEF_Location_Stream::Value::MAX_VALID_ACCURACY * 2.0);
}

}
}

