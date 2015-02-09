#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IVoltage
{
public:
    virtual ~IVoltage();

    virtual auto get_voltage_samples() const -> std::vector<Voltage_Sample> const& = 0;
};

}
}
