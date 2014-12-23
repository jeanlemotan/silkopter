#pragma once

#include "HAL_Camera.h"

namespace silk
{

class HAL_Raspicam : public HAL_Camera
{
public:
    HAL_Raspicam();
    ~HAL_Raspicam();

    auto init() -> bool;
    void shutdown();

    //----------------------------------------------------------------------

    void set_data_callback(Data_Available_Callback cb);

    auto start_recording() -> bool;
    void stop_recording();

    void set_iso(camera_input::Iso iso);
    void set_shutter_speed(camera_input::Shutter_Speed ss);
    void set_quality(camera_input::Stream_Quality sq);

    //----------------------------------------------------------------------
    struct Impl;
    void process();

    struct Quality
    {
        math::vec2u32 resolution;
        size_t bitrate = 0;
    };

private:
    void set_active_streams(bool high, bool medium, bool low);

    struct Params
    {
        size_t fps = 30;
        Quality recording;//{{1280, 960}, 16000000};
        Quality high;//{{1280, 960}, 4000000};
        Quality medium;//{{640, 480}, 2000000};
        Quality low;//{{320, 240}, 160000};
    };

    std::shared_ptr<Impl> m_impl;
    size_t m_fps = 30;

    void file_callback(uint8_t const* data, size_t size);
    void create_file_sink();

    std::shared_ptr<q::data::File_Sink> m_file_sink;
    camera_input::Iso m_iso = 0;
    camera_input::Shutter_Speed m_shutter_speed{0};
    camera_input::Stream_Quality m_stream_quality = camera_input::Stream_Quality::MEDIUM;

    auto create_components() -> bool;
};

}
