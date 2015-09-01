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
    Comms(HAL& hal);

    auto start_udp(boost::asio::ip::address const& address, uint16_t send_port, uint16_t receive_port) -> bool;
    auto start_rfmon(std::string const& interface) -> bool;

    void disconnect();
    auto is_connected() const -> bool;

    void request_all_node_configs();

    void process();

    //----------------------------------------------------------------------

    typedef util::Channel<comms::Setup_Message, uint16_t> Setup_Channel;
    typedef util::Channel<comms::Pilot_Message, uint16_t> Pilot_Channel;
    typedef util::Channel<comms::Telemetry_Message, uint16_t> Telemetry_Channel;

    auto get_setup_channel() -> Setup_Channel&;

    auto get_multi_state_samples() const -> std::vector<stream::IMulti_State::Sample> const&;
    void send_multi_input_value(stream::IMulti_Input::Value const& value);

private:
    HAL& m_hal;

    void configure_channels();


    void reset();
    void request_data();
    bool m_did_request_data = false;

    std::shared_ptr<util::RCP_Socket> m_socket;
    std::shared_ptr<util::RCP> m_rcp;

    mutable Setup_Channel m_setup_channel;
    mutable Pilot_Channel m_pilot_channel;
    mutable Telemetry_Channel m_telemetry_channel;

    std::vector<stream::IMulti_State::Sample> m_multi_state_samples;

    auto unpack_node_data(Comms::Setup_Channel& channel, node::gs::Node& node) -> bool;
    auto link_inputs(node::gs::Node_ptr node) -> bool;
    auto publish_outputs(node::gs::Node_ptr node) -> bool;
    auto unpublish_outputs(node::gs::Node_ptr node) -> bool;

    void handle_stream_data();
    void handle_frame_data();

    void handle_multi_config();
    void handle_multi_state();

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
