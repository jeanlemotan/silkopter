#pragma once

namespace silk
{

class HAL_Motors : q::util::Noncopyable
{
public:
    virtual ~HAL_Motors() {}

    virtual auto init() -> bool = 0;
    virtual void shutdown() = 0;

    //----------------------------------------------------------------------
    //motors

    virtual void set_throttles(float const* throttles, size_t count) = 0;
    virtual void cut_throttle() = 0;

    //----------------------------------------------------------------------
    virtual void process() = 0;
};

}
