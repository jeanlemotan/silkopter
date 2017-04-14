#pragma once

#include "IInput_Device.h"

namespace silk
{

class IGimbal_Control : public IInput_Device
{
public:
    virtual ~IGimbal_Control() = default;

    virtual float get_raw_pitch() const = 0;
    virtual float get_pitch() const = 0;
    virtual void set_pitch_calibration(float min, float center, float max) = 0;

    virtual float get_raw_roll() const = 0;
    virtual float get_roll() const = 0;
    virtual void set_roll_calibration(float min, float center, float max) = 0;
};

}
