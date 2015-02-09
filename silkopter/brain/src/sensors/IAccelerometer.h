#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IAccelerometer
{
public:
    virtual ~IAccelerometer();

    virtual auto get_accelerometer_samples() const -> std::vector<Accelerometer_Sample> const& = 0;
};

}
}
