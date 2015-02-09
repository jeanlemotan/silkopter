#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{


class ICompass
{
public:
    virtual ~ICompass() {}

    struct Calibration
    {
        math::vec3f bias;
        math::vec3f scale = math::vec3f(1);
    };

    virtual void set_compass_calibration(Calibration const& calibration) = 0;
    virtual auto get_compass_calibration() const -> Calibration const& = 0;

    virtual auto get_compass_samples() const -> std::vector<Compass_Sample> const& = 0;
};

DECLARE_CLASS_PTR(ICompass);

}
}
