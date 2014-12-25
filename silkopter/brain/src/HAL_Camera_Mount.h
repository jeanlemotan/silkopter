#pragma once

namespace silk
{

class HAL_Camera_Mount : q::util::Noncopyable
{
public:
    virtual ~HAL_Camera_Mount() {}

    virtual auto init() -> bool = 0;
    virtual void shutdown() = 0;

    //----------------------------------------------------------------------
    //mount

    virtual void set_rotation(math::vec3f const& rot) = 0;

    //----------------------------------------------------------------------
    
    virtual void process() = 0;
};

}
