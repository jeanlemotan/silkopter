#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IThermometer
{
public:
    virtual ~IThermometer();

    virtual auto get_thermometer_samples() const -> std::vector<Thermometer_Sample> const& = 0;
};

}
}
