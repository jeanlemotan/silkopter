#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/Comm_Data.h"
#include "common/stream/IVideo.h"


namespace silk
{
namespace hal
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
    OpenCV_Capture(HAL& hal);
    ~OpenCV_Capture();

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    ts::Result<void> start(Clock::time_point tp) override;

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
    HAL& m_hal;

    ts::Result<void> init();

    std::shared_ptr<hal::OpenCV_Capture_Descriptor> m_descriptor;
    std::shared_ptr<hal::OpenCV_Capture_Config> m_config;

    void set_active_streams(bool recording, uint32_t quality);

    std::shared_ptr<Impl> m_impl;

    void streaming_callback(uint8_t const* data, size_t size, math::vec2u32 const& resolution, bool is_keyframe);
    void file_callback(uint8_t const* data, size_t size, math::vec2u32 const& resolution, bool is_keyframe);
    void create_file_sink();

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
