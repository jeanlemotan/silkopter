#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/Comm_Data.h"
#include "common/stream/IVideo.h"
#include <thread>
#include <fstream>
#include <thread>
#include <atomic>

#include "common/stream/IPWM.h"

#include "common/stream/ICamera_Commands.h"
#include "Sample_Accumulator.h"

namespace silk
{
namespace hal
{
struct Raspicam_Descriptor;
struct Raspicam_Config;
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

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    ts::Result<std::shared_ptr<messages::INode_Message>> send_message(messages::INode_Message const& message) override;

    ts::Result<void> start(Clock::time_point tp) override;

    void shutdown();

    std::vector<Output> get_outputs() const override;

    ts::Result<void> set_input_stream_path(size_t, std::string const&) override;
    std::vector<Input> get_inputs() const override;

    struct Impl;
    void process();

    typedef std::function<void(uint8_t const* data, size_t size, math::vec2u16 const& resolution, bool is_keyframe)> Data_Available_Callback;

private:
    HAL& m_hal;

    ts::Result<void> init();

    std::shared_ptr<hal::Raspicam_Descriptor> m_descriptor;
    std::shared_ptr<hal::Raspicam_Config> m_config;

    Sample_Accumulator<stream::ICamera_Commands> m_commands_accumulator;
    void process_commands(stream::ICamera_Commands::Value const& i_commands);
    stream::ICamera_Commands::Value m_last_commands;

    void activate_streams();

    std::shared_ptr<Impl> m_impl;

    void streaming_callback(stream::IVideo::Quality quality, uint8_t const* data, size_t size, math::vec2u16 const& resolution, bool is_keyframe);
    void recording_callback(uint8_t const* data, size_t size, math::vec2u16 const& resolution, bool is_keyframe);
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

    struct Sample_Queue
    {
        std::mutex mutex;
        std::deque<Stream::Sample> samples;
        size_t count = 0;
    } m_sample_queue;


    struct Recording_Data
    {
        std::mutex data_in_mutex;
        std::vector<uint8_t> data_in;

        std::thread thread;
        std::atomic_bool should_stop = {false};
        std::vector<uint8_t> data_out;
        std::ofstream file_sink;
    } m_recording_data;

};


}
}
