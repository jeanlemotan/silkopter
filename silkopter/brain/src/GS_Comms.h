#pragma once

#include "common/Comm_Data.h"
#include "HAL.h"

#include "common/Manual_Clock.h"
#include "common/node/ISource.h"
#include "common/stream/IMultirotor_Commands.h"
#include "common/stream/IMultirotor_State.h"
#include "common/stream/IVideo.h"
#include "utils/comms/RCP.h"
#include "utils/comms/RCP_Channel.h"
#include "utils/Clock.h"

namespace util
{
namespace comms
{
class RCP;
class ISocket;
}
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
class Set_Stream_Telemetry_Enabled_Req;
class Set_Node_Config_Req;
}
}
}


namespace silk
{

class GS_Comms
{
    GS_Comms(GS_Comms const&) = delete;
    GS_Comms operator=(GS_Comms const&) = delete;

public:
    GS_Comms(HAL& hal);

    auto start_udp(uint16_t send_port, uint16_t receive_port) -> bool;

    auto is_connected() const -> bool;

    void process();

private:
    void configure_channels();

    struct Stream_Telemetry_Data
    {
        std::string stream_path;
        stream::Type stream_type;
        std::weak_ptr<stream::IStream> stream;
        uint32_t sample_count = 0;
        std::vector<uint8_t> data;
    };
    std::vector<Stream_Telemetry_Data> m_stream_telemetry_data;

    struct Telemetry
    {
        size_t version = 0;
        bool is_enabled = false;
        uint32_t sample_count = 0;
        std::vector<uint8_t> data;
    } m_internal_telemetry_data;


    template<class Stream> auto gather_telemetry_stream(Stream_Telemetry_Data& ts, stream::IStream const& _stream) -> bool;
    void gather_telemetry_data();
    void pack_telemetry_data();

    std::vector<uint8_t> m_setup_buffer;
    std::string m_json_buffer;
    std::string m_base64_buffer;

    template<typename T>
    void serialize_and_send(size_t channel_idx, T const& res);
    template<class Format_String, typename... Params>
    gs_comms::setup::Error make_error_response(uint32_t req_id, Format_String const& fmt, Params&&... params);

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
    void handle_req(gs_comms::setup::Set_Stream_Telemetry_Enabled_Req const& req);
    void handle_req(gs_comms::setup::Set_Node_Config_Req const& req);

    HAL& m_hal;
    Clock::time_point m_uav_sent_tp = Clock::now();

    std::vector<stream::IMultirotor_Commands::Value> m_multirotor_commands_values;

    Clock::time_point m_last_rcp_tp = Clock::now();

    std::shared_ptr<util::comms::ISocket> m_socket;
    std::shared_ptr<util::comms::RCP> m_rcp;

    typedef util::comms::RCP_Channel<uint32_t> Telemetry_Channel;
    Telemetry_Channel m_telemetry_channel;

    bool m_is_connected = false;

};


}
