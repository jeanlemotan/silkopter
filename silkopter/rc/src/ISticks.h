#pragma once

namespace silk
{

class ISticks
{
public:
    virtual ~ISticks() = default;

    virtual float get_yaw() const = 0;
    virtual float get_pitch() const = 0;
    virtual float get_roll() const = 0;
    virtual float get_throttle() const = 0;
    virtual void process() = 0;
};

}
