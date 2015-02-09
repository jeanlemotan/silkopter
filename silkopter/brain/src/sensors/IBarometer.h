#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IBarometer
{
public:
    virtual ~IBarometer() {}

    struct Calibration
    {
        float bias;
        float scale = 1;
    };

    virtual void set_barometer_calibration(Calibration const& calibration) = 0;
    virtual auto get_barometer_calibration() const -> Calibration const& = 0;

    virtual auto get_barometer_samples() const -> std::vector<Barometer_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IBarometer);

}
}
