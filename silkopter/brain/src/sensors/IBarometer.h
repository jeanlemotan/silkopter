#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IBarometer
{
public:
    virtual ~IBarometer() {}

    virtual auto get_barometer_name() const -> q::String const& = 0;

    struct Config
    {
        float bias;
        float scale = 1;
    };

    virtual void set_barometer_config(Config const& config) = 0;
    virtual auto get_barometer_config() const -> Config const& = 0;

    virtual auto get_barometer_samples() const -> std::vector<Barometer_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IBarometer);

}
}
