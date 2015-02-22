#pragma once

#include "HAL.h"
#include "common/node/source/ISource.h"
#include "common/Comm_Data.h"
#include "common/node/stream/IVideo.h"

namespace silk
{
namespace node
{
namespace source
{

class Raspicam : public ISource
{
public:
    Raspicam(HAL& hal);
    ~Raspicam();

    struct Quality
    {
        math::vec2u32 resolution;
        uint32_t bitrate = 0;
    };

    struct Init_Params
    {
        std::string name;
        uint32_t fps = 30;
        Quality low;
        Quality medium;
        Quality high;
        Quality recording;
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto init(Init_Params const& params) -> bool;
    void shutdown();

    auto get_name() const -> std::string const&;
    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::IStream&;

    //----------------------------------------------------------------------

    typedef std::function<void(uint8_t const* data, size_t size)> Data_Available_Callback;
    void set_data_callback(Data_Available_Callback cb);

    auto start_recording() -> bool;
    void stop_recording();

    void set_iso(uint32_t iso);
    void set_shutter_speed(q::Clock::duration ss);
    void set_stream_quality(comms::Camera_Params::Stream_Quality sq);

    //----------------------------------------------------------------------
    struct Impl;
    void process();

    auto get_stream() -> stream::IVideo& {}

private:
    HAL& m_hal;
    Init_Params m_params;

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


}
}
}
