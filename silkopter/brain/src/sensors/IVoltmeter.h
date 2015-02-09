#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensors
{

class IVoltmeter
{
public:
    virtual ~IVoltmeter() {}

    struct Calibration
    {
        float bias;
        float scale = 1;
    };

    virtual void set_voltmeter_calibration(Calibration const& calibration) = 0;
    virtual auto get_voltmeter_calibration() const -> Calibration const& = 0;

    virtual auto get_voltmeter_samples() const -> std::vector<Voltmeter_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IVoltmeter);

}
}
