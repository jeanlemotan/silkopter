#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class ISonar
{
public:
    virtual ~ISonar() {}

    virtual auto get_sonar_name() const -> q::String const& = 0;

    struct Config
    {
        float bias;
        float scale = 1;
    };

    virtual void set_sonar_config(Config const& config) = 0;
    virtual auto get_sonar_config() const -> Config const& = 0;

    virtual auto get_sonar_samples() const -> std::vector<Sonar_Sample> const& = 0;
};

DECLARE_CLASS_PTR(ISonar);

}
}
