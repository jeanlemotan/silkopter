#pragma once

#include "common/Comm_Data.h"
#include "HAL.h"
#include "utils/Channel.h"
#include "utils/RUDP.h"
#include "common/Manual_Clock.h"

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

    typedef util::Channel<comms::Setup_Message, uint16_t> Setup_Channel;
    typedef util::Channel<comms::Input_Message, uint16_t> Input_Channel;

    typedef std::function<void(Setup_Channel&)> Setup_Channel_Callback;
    void set_setup_message_callback(comms::Setup_Message message, Setup_Channel_Callback);

    typedef std::function<void(Input_Channel&)> Input_Channel_Callback;
    void set_input_message_callback(comms::Input_Message message, Input_Channel_Callback);

private:
    boost::asio::io_service& m_io_service;

    void handle_accept(boost::system::error_code const& error);

    template<class Stream> auto send_telemetry_stream(node::stream::IStream const& _stream) -> bool;
    void send_telemetry_streams();

    void handle_enumerate_sources();
    void handle_enumerate_sinks();
    void handle_enumerate_streams();
    void handle_enumerate_processors();

    template<class Registry, class Node_Base> void handle_node_config(comms::Setup_Message msg, Registry const& registry);

    void handle_source_config();
    void handle_sink_config();
    void handle_processor_config();
    void handle_stream_config();

    void handle_telemetry_streams();

    std::vector<node::stream::IStream*> m_telemetry_streams;


    std::vector<Setup_Channel_Callback> m_setup_channel_callbacks;
    std::vector<Input_Channel_Callback> m_input_channel_callbacks;

    HAL& m_hal;
    q::Clock::time_point m_uav_sent_time_point = q::Clock::now();

    Manual_Clock m_remote_clock;

    q::Clock::time_point m_last_rudp_time_stamp = q::Clock::now();

    uint16_t m_send_port = 0;
    uint16_t m_receive_port = 0;
    boost::asio::ip::udp::socket m_socket;
    util::RUDP m_rudp;

    typedef util::Channel<uint32_t, uint16_t> Telemetry_Channel;
    Setup_Channel m_setup_channel;
    Input_Channel m_input_channel;
    Telemetry_Channel m_telemetry_channel;

    bool m_is_connected = false;

};


}
