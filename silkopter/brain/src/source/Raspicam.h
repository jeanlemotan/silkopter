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

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void shutdown();

    auto get_outputs() const -> std::vector<Output>;

    //----------------------------------------------------------------------

    auto start_recording() -> bool;
    void stop_recording();

    //----------------------------------------------------------------------
    struct Impl;
    void process();

    typedef std::function<void(uint8_t const* data, size_t size, math::vec2u32 const& resolution)> Data_Available_Callback;

private:
    HAL& m_hal;

    auto init() -> bool;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::Raspicam::Init_Params> m_init_params;
    std::shared_ptr<sz::Raspicam::Config> m_config;

    void set_active_streams(bool recording, bool high, bool low);

    std::shared_ptr<Impl> m_impl;

    void streaming_callback(uint8_t const* data, size_t size, math::vec2u32 const& resolution);
    void file_callback(uint8_t const* data, size_t size, math::vec2u32 const& resolution);
    void create_file_sink();

    std::shared_ptr<q::data::File_Sink> m_file_sink;

    auto create_components() -> bool;

    struct Stream : public stream::IVideo
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        uint32_t sample_idx = 0;
        std::vector<Sample> samples;
    };
    mutable std::shared_ptr<Stream> m_stream;

    struct
    {
        std::mutex mutex;
        std::vector<Stream::Sample> samples;
    } m_temp_samples;
};


}
}
