#pragma once

#include "HAL.h"
#include "common/Comm_Data.h"

#include "utils/RCP.h"
#include "utils/Channel.h"


namespace silk
{
class HAL;

class Comms : q::util::Noncopyable
{
    friend class HAL;
public:
    Comms(boost::asio::io_service& io_service, HAL& hal);

    auto start_udp(boost::asio::ip::address const& address, uint16_t send_port, uint16_t receive_port) -> bool;
    auto start_rfmon(std::string const& interface) -> bool;

    void disconnect();
    auto is_connected() const -> bool;

    void request_all_node_configs();

    void process();

    //----------------------------------------------------------------------

    typedef util::Channel<comms::Setup_Message, uint16_t> Setup_Channel;
    typedef util::Channel<comms::Input_Message, uint16_t> Input_Channel;
    typedef util::Channel<comms::Telemetry_Message, uint16_t> Telemetry_Channel;
    typedef util::Channel<comms::Video_Message, uint32_t> Video_Channel;

    auto get_setup_channel() -> Setup_Channel&;
    uint32_t get_new_req_id();

private:
    HAL& m_hal;
    uint32_t m_last_req_id = 0;

    void configure_channels();


    void reset();
    void request_data();
    bool m_did_request_data = false;

    boost::asio::io_service& m_io_service;

    std::shared_ptr<util::RCP_Socket> m_socket;
    std::shared_ptr<util::RCP> m_rcp;

    mutable Setup_Channel m_setup_channel;
    mutable Input_Channel m_input_channel;
    mutable Telemetry_Channel m_telemetry_channel;
    mutable Video_Channel m_video_channel;

    auto unpack_node_data(Comms::Setup_Channel& channel, node::Node& node) -> bool;
    auto link_inputs(node::Node_ptr node) -> bool;
    auto publish_outputs(node::Node_ptr node) -> bool;
    auto unpublish_outputs(node::Node_ptr node) -> bool;

    void handle_stream_data();
    void handle_frame_data();

    void handle_multi_config();

    void handle_clock();
    void handle_enumerate_nodes();
    void handle_enumerate_node_defs();
    void handle_get_node_data();

    void handle_streams_telemetry_active();

    void handle_add_node();
    void handle_remove_node();
    void handle_node_message();
    void handle_node_config();
    void handle_node_input_stream_path();
};

}
