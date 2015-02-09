#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IGPS
{
public:
    virtual ~IGPS();

    virtual auto get_gps_samples() const -> std::vector<GPS_Sample> const& = 0;
};

}
}
