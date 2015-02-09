#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class ICurrent
{
public:
    virtual ~ICurrent();

    virtual auto get_current_samples() const -> std::vector<Current_Sample> const& = 0;
};

}
}
