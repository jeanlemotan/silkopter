#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IGyroscope
{
public:
    virtual ~IGyroscope() {}

    struct Calibration
    {
        math::vec3f bias;
        math::vec3f scale = math::vec3f(1);
    };

    virtual void set_gyroscope_calibration(Calibration const& calibration) = 0;
    virtual auto get_gyroscope_calibration() const -> Calibration const& = 0;

    virtual auto get_gyroscope_samples() const -> std::vector<Gyroscope_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IGyroscope);

}
}
