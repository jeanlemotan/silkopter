#pragma once

#include "common/input/Camera_Input.h"

namespace silk
{

class HAL_Camera : q::util::Noncopyable
{
public:
    virtual ~HAL_Camera() {}

    //----------------------------------------------------------------------

    typedef std::function<void(uint8_t const* data, size_t size)> Data_Available_Callback;
    virtual void set_data_callback(Data_Available_Callback cb) = 0;

    enum class Result : uint8_t
    {
        OK,
        FAILED
    };

    virtual auto start_recording() -> Result = 0;
    virtual void stop_recording() = 0;

    virtual void set_iso(camera_input::Iso iso) = 0;
    virtual void set_shutter_speed(camera_input::Shutter_Speed ss) = 0;
    virtual void set_quality(camera_input::Stream_Quality sq) = 0;

    //----------------------------------------------------------------------
    virtual void process() = 0;
};

}
