#pragma once

#include "common/Comm_Data.h"
#include "HAL.h"
#include "utils/Channel.h"
#include "utils/RUDP.h"
#include "common/Manual_Clock.h"
#include "common/node/ISource.h"
#include "common/node/stream/ICommands_Stream.h"

namespace sz
{
namespace Comms
{
namespace Source
{
struct Init_Params;
struct Config;
}
}
}

namespace silk
{

class Comms : q::util::Noncopyable
{
public:
    Comms(boost::asio::io_service& io_service, HAL& hal);

    auto start(uint16_t send_port, uint16_t receive_port) -> bool;

    auto is_connected() const -> bool;
    auto get_remote_address() const -> boost::asio::ip::address;
    auto get_remote_clock() const -> Manual_Clock const&;

    auto get_rudp() -> util::RUDP&;

    void process();

    auto get_error_count() const -> size_t;

    enum class Video_Flag : uint8_t
    {
        FLAG_KEYFRAME = 1 << 0,
    };
    typedef q::util::Flag_Set<Video_Flag, uint8_t> Video_Flags;
    //sends a video frame.
    //The data needs to be alive only for the duration of this call.
    auto send_video_frame(Video_Flags flags, uint8_t const* data, size_t size) -> bool;

    struct Source : public node::ISource
    {
        Source(Comms& comms) : m_comms(comms) {}
        auto init(rapidjson::Value const& init_params) -> bool;
        auto get_init_params() const -> rapidjson::Document const&;
        auto set_config(rapidjson::Value const& json) -> bool;
        auto get_config() const -> rapidjson::Document;
        auto get_outputs() const -> std::vector<Output>;
        void process();
    private:
        Comms& m_comms;
    };

    typedef util::Channel<comms::Setup_Message, uint16_t> Setup_Channel;
    typedef util::Channel<comms::Input_Message, uint16_t> Input_Channel;
    typedef util::Channel<comms::Telemetry_Message, uint16_t> Telemetry_Channel;

    auto get_source() -> std::shared_ptr<Source>;

private:
    boost::asio::io_service& m_io_service;

    struct Commands : public node::ICommands_Stream
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        Sample last_sample;
        std::vector<Sample> samples;
        uint32_t rate = 0;
    };
    mutable std::shared_ptr<Commands> m_commands_stream;

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::Comms::Source::Init_Params> m_init_params;
    std::shared_ptr<sz::Comms::Source::Config> m_config;
    std::shared_ptr<Source> m_source;

    void handle_accept(boost::system::error_code const& error);


    struct Telemetry_Stream
    {
        std::string stream_name;
        node::IStream_wptr stream;
        uint32_t sample_count = 0;
        std::vector<uint8_t> data;
    };
    std::vector<Telemetry_Stream> m_telemetry_streams;

    template<class Stream> auto gather_telemetry_stream(Telemetry_Stream& ts, node::IStream const& _stream) -> bool;
    void gather_telemetry_streams();
    void pack_telemetry_streams();

    void handle_clock();

    void handle_enumerate_node_defs();
    void handle_enumerate_nodes();

    void handle_node_config();

    void handle_source_config();
    void handle_sink_config();
    void handle_processor_config();
    void handle_stream_config();

    void handle_add_source();
    void handle_add_sink();
    void handle_add_node();

    void handle_streams_telemetry_active();

    HAL& m_hal;
    q::Clock::time_point m_uav_sent_tp = q::Clock::now();

    Manual_Clock m_remote_clock;

    q::Clock::time_point m_last_rudp_tp = q::Clock::now();

    uint16_t m_send_port = 0;
    uint16_t m_receive_port = 0;
    boost::asio::ip::udp::socket m_socket;
    util::RUDP m_rudp;

    Setup_Channel m_setup_channel;
    Input_Channel m_input_channel;
    Telemetry_Channel m_telemetry_channel;

    q::Clock::time_point m_comms_start_tp;

    bool m_is_connected = false;

};


}
