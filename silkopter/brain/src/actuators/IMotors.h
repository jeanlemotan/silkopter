#pragma once

namespace silk
{
namespace actuators
{

class IMotors : q::util::Noncopyable
{
public:
    virtual ~IMotors() {}

    //----------------------------------------------------------------------
    //motors

    virtual void set_throttles(float const* throttles, size_t count) = 0;
    virtual void cut_throttle() = 0;

    //----------------------------------------------------------------------
    virtual void process() = 0;
};

DECLARE_CLASS_PTR(IMotors);

}
}
