#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class ISonar
{
public:
    virtual ~ISonar();

    virtual auto get_sonar_samples() const -> std::vector<Sonar_Sample> const& = 0;
};

}
}
