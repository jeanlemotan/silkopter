#pragma once

#include "Node_Data.h"

namespace silk
{
namespace node
{

class IGyroscope : q::util::Noncopyable
{
public:
    typedef math::vec3f                     Angular_Velocity;
    typedef Sample<Angular_Velocity>        Sample;
    typedef TP_Sample<Angular_Velocity>     TP_Sample;

    virtual ~IGyroscope() {}

    virtual auto get_name() const -> q::String const& = 0;

    struct Config
    {
        math::vec3f bias;
        math::vec3f scale = math::vec3f(1);
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

DECLARE_CLASS_PTR(IGyroscope);

}
}
