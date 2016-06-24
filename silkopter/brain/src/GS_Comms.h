#pragma once

#include "common/Comm_Data.h"
#include "HAL.h"

#include "common/Manual_Clock.h"
#include "common/node/ISource.h"
#include "common/stream/IMultirotor_Commands.h"
#include "common/stream/IMultirotor_State.h"
#include "common/stream/IVideo.h"

namespace util
{
class RCP;
class RCP_Socket;
}

namespace silk
{
namespace gs_comms
{
namespace setup
{
class Error;
class Node_Data;
class Get_AST_Req;
class Set_Clock_Req;
class Set_UAV_Descriptor_Req;
class Get_UAV_Descriptor_Req;
class Get_Node_Defs_Req;
class Remove_Node_Req;
class Get_Nodes_Req;
class Add_Node_Req;
class Set_Node_Input_Stream_Path_Req;
}
}
}


namespace silk
{

class GS_Comms : q::util::Noncopyable
{
public:
    GS_Comms(HAL& hal);

    auto start_udp(uint16_t send_port, uint16_t receive_port) -> bool;
    auto start_rfmon(std::string const& interface, uint8_t id) -> bool;

    auto is_connected() const -> bool;

    void process();

private:
    void configure_channels();

    struct Stream_Telemetry_Data
    {
        std::string stream_name;
        std::weak_ptr<stream::IStream> stream;
        uint32_t sample_count = 0;
        std::vector<uint8_t> data;
    };
    std::vector<Stream_Telemetry_Data> m_stream_telemetry_data;

    struct Telemetry
    {
        bool is_enabled = false;
        uint32_t sample_count = 0;
        std::vector<uint8_t> data;
    } m_telemetry_data;


    template<class Stream> auto gather_telemetry_stream(Stream_Telemetry_Data& ts, stream::IStream const& _stream) -> bool;
    void gather_telemetry_data();
    void pack_telemetry_data();

    std::vector<uint8_t> m_setup_buffer;
    std::string m_json_buffer;
    std::string m_base64_buffer;

    template<typename T>
    void serialize_and_send(size_t channel_idx, T const& res);
    template<class Format_String, typename... Params>
    gs_comms::setup::Error make_error(Format_String const& fmt, Params&&... params);
    std::string const& decode_json(std::string const& json_base64);
    std::string const& encode_json(std::string const& json);
    boost::variant<gs_comms::setup::Node_Data, gs_comms::setup::Error> get_node_data(std::string const& name, node::INode const& node);

    class Dispatch_Req_Visitor;
    friend class Dispatch_Req_Visitor;

    void handle_req(gs_comms::setup::Get_AST_Req const& req);
    void handle_req(gs_comms::setup::Set_Clock_Req const& req);
    void handle_req(gs_comms::setup::Set_UAV_Descriptor_Req const& req);
    void handle_req(gs_comms::setup::Get_UAV_Descriptor_Req const& req);
    void handle_req(gs_comms::setup::Get_Node_Defs_Req const& req);
    void handle_req(gs_comms::setup::Remove_Node_Req const& req);
    void handle_req(gs_comms::setup::Get_Nodes_Req const& req);
    void handle_req(gs_comms::setup::Add_Node_Req const& req);
    void handle_req(gs_comms::setup::Set_Node_Input_Stream_Path_Req const& req);

    HAL& m_hal;
    q::Clock::time_point m_uav_sent_tp = q::Clock::now();

    std::vector<stream::IMultirotor_Commands::Value> m_multirotor_commands_values;

    q::Clock::time_point m_last_rcp_tp = q::Clock::now();

    std::shared_ptr<util::RCP_Socket> m_socket;
    std::shared_ptr<util::RCP> m_rcp;

    bool m_is_connected = false;

};


}
