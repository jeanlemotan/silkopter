#pragma once

#include "UAV.h"
#include "common/node/ISource.h"
#include "common/Comm_Data.h"
#include "common/stream/IVideo.h"


namespace silk
{
namespace uav
{
struct OpenCV_Capture_Descriptor;
struct OpenCV_Capture_Config;
}
}



namespace silk
{
namespace node
{

class OpenCV_Capture : public ISource
{
public:
    OpenCV_Capture(UAV& uav);
    ~OpenCV_Capture();

    bool init(std::shared_ptr<uav::INode_Descriptor> descriptor) override;
    std::shared_ptr<uav::INode_Descriptor> get_descriptor() const override;

    bool set_config(std::shared_ptr<uav::INode_Config> config) override;
    std::shared_ptr<uav::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void shutdown();

    auto get_outputs() const -> std::vector<Output>;

    //----------------------------------------------------------------------

    auto start_recording() -> bool;
    void stop_recording();

    //----------------------------------------------------------------------
    struct Impl;
    void process();

    typedef std::function<void(uint8_t const* data, size_t size, math::vec2u32 const& resolution, bool is_keyframe)> Data_Available_Callback;

private:
    UAV& m_uav;

    auto init() -> bool;

    std::shared_ptr<uav::OpenCV_Capture_Descriptor> m_descriptor;
    std::shared_ptr<uav::OpenCV_Capture_Config> m_config;

    void set_active_streams(bool recording, uint32_t quality);

    std::shared_ptr<Impl> m_impl;

    void streaming_callback(uint8_t const* data, size_t size, math::vec2u32 const& resolution, bool is_keyframe);
    void file_callback(uint8_t const* data, size_t size, math::vec2u32 const& resolution, bool is_keyframe);
    void create_file_sink();

    std::shared_ptr<q::data::File_Sink> m_file_sink;

    auto create_components() -> bool;

    struct Stream : public stream::IVideo
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
    };
    mutable std::shared_ptr<Stream> m_stream;

    struct
    {
        std::mutex mutex;
        std::vector<Stream::Sample> samples;
        size_t count = 0;
    } m_temp_samples;
};


}
}
