#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IBarometer
{
public:
    virtual ~IBarometer();

    virtual auto get_barometer_samples() const -> std::vector<Barometer_Sample> const& = 0;
};

}
}
