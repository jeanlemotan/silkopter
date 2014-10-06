#pragma once

namespace silk
{

class HAL_Camera : q::util::Noncopyable
{
public:
    virtual ~HAL_Camera() {}

    struct Params
    {
        size_t fps = 30;
    };

    //----------------------------------------------------------------------

    typedef std::function<void(uint8_t const* data, size_t size)> Data_Available_Callback;

    virtual void setup_record_quality(math::vec2u32 const& resolution, size_t bitrate) = 0;
    virtual void setup_high_quality(math::vec2u32 const& resolution, size_t bitrate) = 0;
    virtual void setup_medium_quality(math::vec2u32 const& resolution, size_t bitrate) = 0;
    virtual void setup_low_quality(math::vec2u32 const& resolution, size_t bitrate) = 0;

    virtual void set_data_callback(Data_Available_Callback cb) = 0;

    virtual auto start_capture() -> Result = 0;
    virtual void stop_capture() = 0;

    virtual auto start_recording() -> Result = 0;
    virtual void stop_recording() = 0;

    virtual void set_iso(camera_input::Iso iso) = 0;
    virtual void set_shutter_speed(camera_input::Shutter_Speed ss) = 0;
    virtual void set_quality(camera_input::Stream_Quality sq) = 0;

    //----------------------------------------------------------------------
    virtual void process() = 0;
};

}
