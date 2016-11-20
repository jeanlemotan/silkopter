#pragma once

#include "IInput_Device.h"

namespace silk
{

class ISticks : public IInput_Device
{
public:
    virtual ~ISticks() = default;

    virtual float get_raw_yaw() const = 0;
    virtual float get_yaw() const = 0;
    virtual void set_yaw_calibration(float center, float min, float max) = 0;

    virtual float get_raw_pitch() const = 0;
    virtual float get_pitch() const = 0;
    virtual void set_pitch_calibration(float center, float min, float max) = 0;

    virtual float get_raw_roll() const = 0;
    virtual float get_roll() const = 0;
    virtual void set_roll_calibration(float center, float min, float max) = 0;

    virtual float get_raw_throttle() const = 0;
    virtual float get_throttle() const = 0;
    virtual void set_throttle_calibration(float center, float min, float max) = 0;
};

}
