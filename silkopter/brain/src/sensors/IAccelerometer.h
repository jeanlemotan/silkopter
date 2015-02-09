#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IAccelerometer
{
public:
    virtual ~IAccelerometer() {}

    struct Calibration
    {
        math::vec3f bias;
        math::vec3f scale = math::vec3f(1);
    };

    virtual void set_accelerometer_calibration(Calibration const& calibration) = 0;
    virtual auto get_accelerometer_calibration() const -> Calibration const& = 0;

    virtual auto get_accelerometer_samples() const -> std::vector<Accelerometer_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IAccelerometer);

}
}
