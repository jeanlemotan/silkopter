#pragma once

namespace silk
{

class IStick_Actuators
{
public:
    virtual ~IStick_Actuators() = default;

    virtual bool set_target_yaw(boost::optional<float> value) = 0;
    virtual bool set_target_pitch(boost::optional<float> value) = 0;
    virtual bool set_target_roll(boost::optional<float> value) = 0;
    virtual bool set_target_throttle(boost::optional<float> value) = 0;
    virtual void process() = 0;
};

}
