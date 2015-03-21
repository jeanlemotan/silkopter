#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/Comm_Data.h"
#include "common/node/stream/IVideo.h"


namespace sz
{
namespace Raspicam
{
struct Init_Params;
struct Config;
}
}



namespace silk
{
namespace node
{

class Raspicam : public ISource
{
public:
    Raspicam(HAL& hal);
    ~Raspicam();

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    void shutdown();

    auto get_outputs() const -> std::vector<Output>;

    //----------------------------------------------------------------------

    typedef std::function<void(uint8_t const* data, size_t size)> Data_Available_Callback;
    void set_data_callback(Data_Available_Callback cb);

    auto start_recording() -> bool;
    void stop_recording();

    void set_iso(uint32_t iso);
    void set_shutter_speed(q::Clock::duration ss);
    //void set_stream_quality(comms::Camera_Params::Stream_Quality sq);

    //----------------------------------------------------------------------
    struct Impl;
    void process();

    auto get_stream() -> stream::IVideo& {}

private:
    HAL& m_hal;

    auto init() -> bool;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::Raspicam::Init_Params> m_init_params;
    std::shared_ptr<sz::Raspicam::Config> m_config;

    void set_active_streams(bool high, bool medium, bool low);

    std::shared_ptr<Impl> m_impl;
    size_t m_fps = 30;

    void file_callback(uint8_t const* data, size_t size);
    void create_file_sink();

    std::shared_ptr<q::data::File_Sink> m_file_sink;
    uint32_t m_iso = 0;
    q::Clock::duration m_shutter_speed{0};
    //comms::Camera_Params::Stream_Quality m_stream_quality = comms::Camera_Params::Stream_Quality::MEDIUM;

    auto create_components() -> bool;
};


}
}
