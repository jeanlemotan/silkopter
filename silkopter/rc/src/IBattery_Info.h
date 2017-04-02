#pragma once

namespace silk
{

class IBattery_Info
{
public:
    virtual ~IBattery_Info() = default;

    virtual float get_voltage() const = 0;

    virtual float get_low_voltage() const = 0;
    virtual void set_low_voltage(float voltage) = 0;

    virtual float get_critical_voltage() const = 0;
    virtual void set_critical_voltage(float voltage) = 0;

    struct Calibration_Data
    {
        float bias = 0.f;
        float scale = 1.f;
    };

    virtual Calibration_Data get_calibration_data() const = 0;
    virtual void set_calibration_data(Calibration_Data const& data) = 0;

    virtual void process() = 0;
};

}
