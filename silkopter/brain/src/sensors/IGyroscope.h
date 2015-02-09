#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IGyroscope
{
public:
    virtual ~IGyroscope();

    virtual auto get_gyroscope_samples() const -> std::vector<Gyroscope_Sample> const& = 0;
};

}
}
