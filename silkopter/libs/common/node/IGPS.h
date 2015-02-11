#pragma once

#include "Node_Data.h"

namespace silk
{
namespace node
{

class IGPS : q::util::Noncopyable
{
public:
    struct Location
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
    typedef Sample<Location>         Sample;
    typedef TP_Sample<Location>      TP_Sample;


    virtual ~IGPS() {}

    virtual auto get_name() const -> q::String const& = 0;

    struct Config
    {
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

DECLARE_CLASS_PTR(IGPS);

}
}
