#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IThermometer
{
public:
    virtual ~IThermometer() {}

    virtual auto get_thermometer_name() const -> q::String const& = 0;

    struct Config
    {
        float bias;
        float scale = 1;
    };

    virtual void set_thermometer_config(Config const& config) = 0;
    virtual auto get_thermometer_config() const -> Config const& = 0;

    virtual auto get_thermometer_samples() const -> std::vector<Thermometer_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IThermometer);

}
}
