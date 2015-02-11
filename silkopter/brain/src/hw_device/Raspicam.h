#pragma once

#include "common/node/ICamera.h"

namespace silk
{
namespace hw_device
{

class Raspicam : public node::ICamera
{
public:
    Raspicam(q::String const& name);
    ~Raspicam();

    struct Quality
    {
        math::vec2u32 resolution;
        size_t bitrate = 0;
    };

    struct Init_Params
    {
        size_t fps = 30;
        Quality low;
        Quality medium;
        Quality high;
        Quality recording;
    };

    auto init(Init_Params const& params) -> bool;
    void shutdown();

    auto get_name() const -> q::String const&;

    //----------------------------------------------------------------------

    void set_data_callback(Data_Available_Callback cb);

    auto start_recording() -> bool;
    void stop_recording();

    void set_iso(uint32_t iso);
    void set_shutter_speed(q::Clock::duration ss);
    void set_stream_quality(comms::Camera_Params::Stream_Quality sq);

    //----------------------------------------------------------------------
    struct Impl;
    void process();

private:
    q::String m_name;

    void set_active_streams(bool high, bool medium, bool low);

    std::shared_ptr<Impl> m_impl;
    size_t m_fps = 30;

    void file_callback(uint8_t const* data, size_t size);
    void create_file_sink();

    std::shared_ptr<q::data::File_Sink> m_file_sink;
    uint32_t m_iso = 0;
    q::Clock::duration m_shutter_speed{0};
    comms::Camera_Params::Stream_Quality m_stream_quality = comms::Camera_Params::Stream_Quality::MEDIUM;

    auto create_components() -> bool;
};


DECLARE_CLASS_PTR(Raspicam);

}
}
