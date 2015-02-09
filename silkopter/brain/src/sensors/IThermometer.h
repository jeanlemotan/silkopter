#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IThermometer
{
public:
    virtual ~IThermometer() {}

    struct Calibration
    {
        float bias;
        float scale = 1;
    };

    virtual void set_thermometer_calibration(Calibration const& calibration) = 0;
    virtual auto get_thermometer_calibration() const -> Calibration const& = 0;

    virtual auto get_thermometer_samples() const -> std::vector<Thermometer_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IThermometer);

}
}
