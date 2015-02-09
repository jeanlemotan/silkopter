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

    virtual auto get_gyroscope_name() const -> q::String const& = 0;

    struct Config
    {
        math::vec3f bias;
        math::vec3f scale = math::vec3f(1);
    };

    virtual void set_gyroscope_config(Config const& config) = 0;
    virtual auto get_gyroscope_config() const -> Config const& = 0;

    virtual auto get_gyroscope_samples() const -> std::vector<Gyroscope_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IGyroscope);

}
}
