#pragma once

#include "IStream.h"
#include "Sample.h"

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
        float precision = std::numeric_limits<float>::max();

        boost::optional<double> longitude; //radians
        boost::optional<double> latitude; //radians
        boost::optional<float> altitude; //meters
        boost::optional<math::vec2f> velocity_2d; //meters/second but x/y only
        boost::optional<math::vec3f> velocity; //meters/second
        boost::optional<math::vec2f> direction; //normalized
    };

    typedef Sample<Value>     Sample;

    virtual ~ILocation() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

DECLARE_CLASS_PTR(ILocation);

}
}
}
