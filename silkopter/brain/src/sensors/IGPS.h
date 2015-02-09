#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IGPS
{
public:
    virtual ~IGPS() {}

    struct Calibration
    {
    };

    virtual void set_gps_calibration(Calibration const& calibration) = 0;
    virtual auto get_gps_calibration() const -> Calibration const& = 0;

    virtual auto get_gps_samples() const -> std::vector<GPS_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IGPS);

}
}
