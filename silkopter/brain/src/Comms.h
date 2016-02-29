#pragma once

#include "common/Comm_Data.h"
#include "HAL.h"

#include "common/Manual_Clock.h"
#include "common/node/ISource.h"
#include "common/stream/IMulti_Commands.h"
#include "common/stream/IMulti_State.h"
#include "common/stream/IVideo.h"

namespace sz
{
namespace Comms
{
namespace Multi_Comms
{
struct Init_Params;
struct Config;
}
}
}

namespace util
{
class RCP;
class RCP_Socket;
}

namespace silk
{

class Comms : q::util::Noncopyable
{
public:
    Comms(HAL& hal);

    auto start_udp(uint16_t send_port, uint16_t receive_port) -> bool;
    auto start_rfmon(std::string const& interface, uint8_t id) -> bool;

    auto is_connected() const -> bool;

    void process();

    auto get_multi_commands_values() const -> std::vector<stream::IMulti_Commands::Value> const&;
    void add_multi_state_sample(stream::IMulti_State::Sample const& sample);
    void add_video_sample(stream::IVideo::Sample const& sample);

    struct Channels; //this needs to be public...
private:
    void configure_channels();

    void handle_accept(boost::system::error_code const& error);


    struct Stream_Telemetry_Data
    {
        std::string stream_name;
        stream::IStream_wptr stream;
        uint32_t sample_count = 0;
        std::vector<uint8_t> data;
    };
    std::vector<Stream_Telemetry_Data> m_stream_telemetry_data;

    struct HAL_Telemetry
    {
        bool is_enabled = false;
        uint32_t sample_count = 0;
        std::vector<uint8_t> data;
    } m_hal_telemetry_data;


//    auto send_video_stream(Stream_Telemetry_Data& ts, stream::IStream const& _stream) -> bool;

    template<class Stream> auto gather_telemetry_stream(Stream_Telemetry_Data& ts, stream::IStream const& _stream) -> bool;
    void gather_telemetry_data();
    void pack_telemetry_data();

    void handle_clock();

    void handle_enumerate_node_defs();
    void handle_enumerate_nodes();
    void handle_get_node_data();

    void handle_node_config();
    void handle_node_message();
    void handle_node_input_stream_path();

    void handle_add_node();
    void handle_remove_node();

    void handle_multi_config();
    void handle_multi_commands();

    void handle_streams_telemetry_active();
    void handle_hal_telemetry_active();


    HAL& m_hal;
    q::Clock::time_point m_uav_sent_tp = q::Clock::now();

    std::vector<stream::IMulti_Commands::Value> m_multi_commands_values;

    q::Clock::time_point m_last_rcp_tp = q::Clock::now();

    std::shared_ptr<util::RCP_Socket> m_socket;
    std::shared_ptr<util::RCP> m_rcp;

    std::shared_ptr<Channels> m_channels;

    bool m_is_connected = false;

};


}
