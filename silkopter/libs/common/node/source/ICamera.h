#pragma once

#include "ISource.h"
#include "common/Comm_Data.h"

namespace silk
{
namespace node
{
namespace source
{


class ICamera : public ISource
{
public:
    virtual ~ICamera() {}

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
}
}
