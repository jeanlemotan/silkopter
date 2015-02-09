#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IAmmeter
{
public:
    virtual ~IAmmeter() {}

    virtual auto get_ammeter_name() const -> q::String const& = 0;

    struct Config
    {
        float bias;
        float scale = 1;
    };

    virtual void set_ammeter_config(Config const& config) = 0;
    virtual auto get_ammeter_config() const -> Config const& = 0;

    virtual auto get_ammeter_samples() const -> std::vector<Ammeter_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IAmmeter);

}
}
