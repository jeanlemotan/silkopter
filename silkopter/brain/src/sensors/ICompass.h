#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{


class ICompass
{
public:
    virtual ~ICompass();

    virtual auto get_compass_samples() const -> std::vector<Compass_Sample> const& = 0;
};

}
}
