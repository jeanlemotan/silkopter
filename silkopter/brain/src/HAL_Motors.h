#pragma once

namespace silk
{

class HAL_Motors : q::util::Noncopyable
{
public:
    virtual ~HAL_Motors() {}

    //----------------------------------------------------------------------
    //motors

    virtual void set_throttles(float const* throttles, size_t count) = 0;
    virtual void cut_throttle() = 0;

    //----------------------------------------------------------------------
    virtual void process() = 0;
};

}
