#pragma once

#include "IInput_Device.h"

namespace silk
{

class ISticks : public IInput_Device
{
public:
    virtual ~ISticks() = default;

    enum class Raw_Type
    {
        WITHOUT_REMAPPING,
        WITHOUT_DEADBAND
    };

    virtual float get_raw_yaw(Raw_Type raw_type) const = 0;
    virtual float get_yaw() const = 0;
    virtual void set_yaw_calibration(float min, float center, float max, float deadband, float curve) = 0;

    virtual float get_raw_pitch(Raw_Type raw_type) const = 0;
    virtual float get_pitch() const = 0;
    virtual void set_pitch_calibration(float min, float center, float max, float deadband, float curve) = 0;

    virtual float get_raw_roll(Raw_Type raw_type) const = 0;
    virtual float get_roll() const = 0;
    virtual void set_roll_calibration(float min, float center, float max, float deadband, float curve) = 0;

    enum class Deadband_Position
    {
        LOW,
        MIDDLE,
    };

    virtual float get_raw_throttle(Raw_Type raw_type) const = 0;
    virtual float get_throttle() const = 0;
    virtual void set_throttle_calibration(float min, float center, float max, float deadband, float curve) = 0;
    virtual void set_throttle_deadband_position(Deadband_Position position) = 0;
    virtual Deadband_Position get_throttle_deadband_position() const = 0;
};

}
