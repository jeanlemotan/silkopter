#pragma once

#include "HAL.h"
#include "common/Comm_Data.h"

namespace silk
{
class HAL;

class Comms : q::util::Noncopyable
{
    friend class HAL;
public:
    Comms(boost::asio::io_service& io_service, HAL& hal);

    auto start(boost::asio::ip::address const& address, uint16_t send_port, uint16_t receive_port) -> bool;
    void disconnect();

    auto is_connected() const -> bool;
    auto get_remote_address() const -> boost::asio::ip::address;

    auto get_rudp() -> util::RUDP&;

    void request_all_node_configs();

    void process();

    //----------------------------------------------------------------------

    typedef util::Channel<comms::Setup_Message, uint16_t> Setup_Channel;
    typedef util::Channel<comms::Input_Message, uint16_t> Input_Channel;
    typedef util::Channel<comms::Telemetry_Message, uint16_t> Telemetry_Channel;

    auto get_setup_channel() -> Setup_Channel&;

private:
    HAL& m_hal;
    uint32_t m_last_req_id = 0;

    void reset();
    void request_data();

    void send_hal_requests();

    boost::asio::io_service& m_io_service;
    boost::asio::ip::udp::socket m_socket;
    boost::asio::ip::udp::endpoint m_remote_endpoint;

    util::RUDP m_rudp;

    mutable Setup_Channel m_setup_channel;
    mutable Input_Channel m_input_channel;
    mutable Telemetry_Channel m_telemetry_channel;

    auto link_input_streams(node::Node_ptr node) -> bool;
    auto publish_output_streams(node::Node_ptr node) -> bool;

    void handle_stream_data();

    void handle_clock();
    void handle_enumerate_nodes();
    void handle_enumerate_node_defs();
    void handle_streams_telemetry_active();

    void handle_node_data();
    void handle_add_node();
    void handle_remove_node();
};

}
