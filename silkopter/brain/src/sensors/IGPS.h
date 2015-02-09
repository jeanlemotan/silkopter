#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IGPS
{
public:
    virtual ~IGPS() {}

    virtual auto get_gps_name() const -> q::String const& = 0;

    struct Config
    {
    };

    virtual void set_gps_config(Config const& config) = 0;
    virtual auto get_gps_config() const -> Config const& = 0;

    virtual auto get_gps_samples() const -> std::vector<GPS_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IGPS);

}
}
