#pragma once

#include "common/Comm_Data.h"

#include "common/stream/IAcceleration.h"
#include "common/stream/IADC.h"
#include "common/stream/IFloat.h"
#include "common/stream/IBool.h"
#include "common/stream/IAngular_Velocity.h"
#include "common/stream/IBattery_State.h"
#include "common/stream/ICurrent.h"
#include "common/stream/IDistance.h"
#include "common/stream/IForce.h"
#include "common/stream/IFrame.h"
#include "common/stream/IGPS_Info.h"
#include "common/stream/ILinear_Acceleration.h"
#include "common/stream/IPosition.h"
#include "common/stream/IMagnetic_Field.h"
#include "common/stream/IPressure.h"
#include "common/stream/IPWM.h"
#include "common/stream/ITemperature.h"
#include "common/stream/IThrottle.h"
#include "common/stream/ITorque.h"
#include "common/stream/IVelocity.h"
#include "common/stream/IVideo.h"
#include "common/stream/IVoltage.h"
#include "common/stream/IProximity.h"
#include "common/stream/IMultirotor_Commands.h"
#include "common/stream/IMultirotor_State.h"

#include "utils/RCP.h"
#include "utils/Channel.h"
#include "common/Manual_Clock.h"

#include "common/node/INode.h"
#include "common/stream/IVideo.h"
#include "common/stream/IMultirotor_State.h"

#include "def_lang/IStruct_Value.h"

#include <boost/asio.hpp>

namespace silk
{
namespace gs_comms
{
namespace setup
{
class Error;
class Get_AST_Res;
class Set_Clock_Res;
class Set_UAV_Descriptor_Res;
class Get_UAV_Descriptor_Res;
class Get_Node_Defs_Res;
class Remove_Node_Res;
class Get_Nodes_Res;
class Add_Node_Res;
class Set_Node_Input_Stream_Path_Res;
}
}
}

namespace silk
{

class Comms : q::util::Noncopyable
{
public:
    Comms(ts::Type_System& ts);

    ts::Type_System& get_type_system();

    auto start_udp(boost::asio::ip::address const& address, uint16_t send_port, uint16_t receive_port) -> bool;

    void disconnect();
    auto is_connected() const -> bool;

    ts::Result<std::shared_ptr<ts::IStruct_Value>> request_uav_descriptor(std::chrono::high_resolution_clock::duration timeout = std::chrono::milliseconds(1000));
    ts::Result<std::shared_ptr<ts::IStruct_Value>> send_uav_descriptor(std::shared_ptr<ts::IStruct_Value> descriptor, std::chrono::high_resolution_clock::duration timeout = std::chrono::milliseconds(1000));

    //----------------------------------------------------------------------

    boost::signals2::signal<void()> sig_reset;
    boost::signals2::signal<void(Manual_Clock::time_point)> sig_clock_received;
    boost::signals2::signal<void()> sig_type_system_will_be_reset;
    boost::signals2::signal<void()> sig_type_system_reset;

    boost::signals2::signal<void(uint32_t req_id, std::string const& message)> sig_error_received;

    boost::signals2::signal<void(std::shared_ptr<ts::IStruct_Value>)> sig_uav_descriptor_received;


    void request_all_data();

    struct Node_Def
    {
        std::string name;
        node::Type type;
        std::shared_ptr<ts::IStruct_Value> default_descriptor;
        struct Input
        {
            stream::Type type;
            std::string name;
            //uint32_t rate = 0;
        };
        std::vector<Input> inputs;
        struct Output
        {
            stream::Type type;
            std::string name;
            //uint32_t rate = 0;
        };
        std::vector<Output> outputs;
    };

    boost::signals2::signal<void()> sig_node_defs_reset;
    boost::signals2::signal<void(std::vector<Node_Def> const&)> sig_node_defs_added;

    struct Node
    {
        std::string name;
        node::Type type;
        std::shared_ptr<ts::IStruct_Value> descriptor;
        std::shared_ptr<ts::IStruct_Value> config;

        struct Input
        {
            q::Path stream_path;
            stream::Type type;
            std::string name;
            uint32_t rate = 0;
        };
        std::vector<Input> inputs;
        struct Output
        {
            stream::Type type;
            std::string name;
            uint32_t rate = 0;
        };
        std::vector<Output> outputs;
    };

    boost::signals2::signal<void()> sig_nodes_reset;
    boost::signals2::signal<void(std::vector<Node> const&)> sig_nodes_added;
    boost::signals2::signal<void(Node const&)> sig_node_changed;
    boost::signals2::signal<void(std::string const& name)> sig_node_removed;
//    boost::signals2::signal<void(std::string const& name, rapidjson::Value const& message)> sig_node_message_received;
//    boost::signals2::signal<void(std::string const& name, rapidjson::Value const& json)> sig_node_config_received;

    struct IStream_Data
    {
        stream::Semantic type;
        uint32_t rate = 0;
        //virtual void unpack(Comms::Telemetry_Channel& channel, uint32_t sample_count) = 0;
    };

    template<class Stream_T>
    struct Stream_Data : public IStream_Data
    {
        static constexpr stream::Semantic TYPE = Stream_T::TYPE;
        typedef typename Stream_T::Value Value;
        typedef typename Stream_T::Sample Sample;
        typedef std::vector<Sample> Samples;

        Samples samples;
    private:
        //void unpack(Comms::Telemetry_Channel& channel, uint32_t sample_count) override;
    };

    boost::signals2::signal<void(IStream_Data const&)> sig_stream_data_received;


//    void request_node_config(std::string const& name);

//    void request_uav_config();
//    boost::signals2::signal<void(boost::optional<silk::UAV_Config&> config)> sig_uav_config_received;
//    void send_uav_config(boost::optional<silk::UAV_Config&> config);

    void process_rcp();
    void process();

private:
    ts::Type_System& m_ts;

    void configure_channels();

    std::map<stream::Type, std::unique_ptr<IStream_Data>> m_streams;

    struct Dispatch_Res_Visitor;
    struct Dispatch_Req_Visitor;

    void reset();
    bool m_did_request_data = false;

    template<typename T>
    void serialize_and_send(size_t channel_idx, T const& message);

    std::shared_ptr<util::RCP_Socket> m_socket;
    std::shared_ptr<util::RCP> m_rcp;

    uint32_t m_last_req_id = 0;

    std::string const& decode_json(std::string const& json_base64);
    std::string const& encode_json(std::string const& json);

    std::vector<uint8_t> m_setup_buffer;
    std::string m_base64_buffer;

    bool handle_uav_descriptor(std::string const& serialized_data);

    void handle_res(gs_comms::setup::Error const& res);
    void handle_res(gs_comms::setup::Get_AST_Res const& res);
    void handle_res(gs_comms::setup::Set_Clock_Res const& res);
    void handle_res(gs_comms::setup::Set_UAV_Descriptor_Res const& res);
    void handle_res(gs_comms::setup::Get_UAV_Descriptor_Res const& res);
    void handle_res(gs_comms::setup::Get_Node_Defs_Res const& res);
    void handle_res(gs_comms::setup::Remove_Node_Res const& res);
    void handle_res(gs_comms::setup::Get_Nodes_Res const& res);
    void handle_res(gs_comms::setup::Add_Node_Res const& res);
    void handle_res(gs_comms::setup::Set_Node_Input_Stream_Path_Res const& res);
};

}
