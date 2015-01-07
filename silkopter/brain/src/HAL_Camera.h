#pragma once

#include "common/Comm_Data.h"

namespace silk
{

class HAL_Camera : q::util::Noncopyable
{
public:
    virtual ~HAL_Camera() {}

    virtual auto init() -> bool = 0;
    virtual void shutdown() = 0;

    //----------------------------------------------------------------------

    typedef std::function<void(uint8_t const* data, size_t size)> Data_Available_Callback;
    virtual void set_data_callback(Data_Available_Callback cb) = 0;

    virtual auto start_recording() -> bool = 0;
    virtual void stop_recording() = 0;

    virtual void set_iso(uint32_t iso) = 0;
    virtual void set_shutter_speed(q::Clock::duration ss) = 0;
    virtual void set_stream_quality(comms::Camera_Params::Stream_Quality sq) = 0;

    //----------------------------------------------------------------------
    virtual void process() = 0;
};

}
