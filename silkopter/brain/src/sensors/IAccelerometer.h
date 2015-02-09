#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IAccelerometer
{
public:
    virtual ~IAccelerometer() {}

    virtual auto get_accelerometer_name() const -> q::String const& = 0;

    struct Config
    {
        math::vec3f bias;
        math::vec3f scale = math::vec3f(1);
    };

    virtual void set_accelerometer_config(Config const& config) = 0;
    virtual auto get_accelerometer_config() const -> Config const& = 0;

    virtual auto get_accelerometer_samples() const -> std::vector<Accelerometer_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IAccelerometer);

}
}
