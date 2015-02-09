#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class ISonar
{
public:
    virtual ~ISonar() {}

    struct Calibration
    {
        float bias;
        float scale = 1;
    };

    virtual void set_sonar_calibration(Calibration const& calibration) = 0;
    virtual auto get_sonar_calibration() const -> Calibration const& = 0;

    virtual auto get_sonar_samples() const -> std::vector<Sonar_Sample> const& = 0;
};

DECLARE_CLASS_PTR(ISonar);

}
}
