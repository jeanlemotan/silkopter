#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IAmmeter
{
public:
    virtual ~IAmmeter() {}

    struct Calibration
    {
        float bias;
        float scale = 1;
    };

    virtual void set_ammeter_calibration(Calibration const& calibration) = 0;
    virtual auto get_ammeter_calibration() const -> Calibration const& = 0;

    virtual auto get_ammeter_samples() const -> std::vector<Ammeter_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IAmmeter);

}
}
