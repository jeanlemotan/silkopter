#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IGyroscope
{
public:
    virtual ~IGyroscope() {}

    virtual auto get_name() const -> q::String const& = 0;

    struct Config
    {
        math::vec3f bias;
        math::vec3f scale = math::vec3f(1);
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_samples() const -> std::vector<Gyroscope_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IGyroscope);

}
}
