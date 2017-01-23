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

#include "utils/comms/RCP.h"
#include "utils/comms/RCP_Channel.h"
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
class Node_Data;
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
class Set_Stream_Telemetry_Enabled_Res;
class Set_Node_Config_Res;
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

    struct Node_Def
    {
        std::string name;
        node::Type type;
        std::shared_ptr<ts::IStruct_Value> default_descriptor;
        struct Input
        {
            stream::Type type;
            std::string name;
        };
        std::vector<Input> inputs;
        struct Output
        {
            stream::Type type;
            std::string name;
        };
        std::vector<Output> outputs;
    };
    struct Node
    {
        std::string name;
        node::Type type;
        std::shared_ptr<ts::IStruct_Value> descriptor;
        std::shared_ptr<ts::IStruct_Value> config;

        struct Input
        {
            std::string stream_path;
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

    ts::Result<std::vector<Node_Def>> request_node_defs(std::chrono::high_resolution_clock::duration timeout = std::chrono::milliseconds(1000));
    ts::Result<std::vector<Node>> request_nodes(std::chrono::high_resolution_clock::duration timeout = std::chrono::milliseconds(1000));

    ts::Result<Node> add_node(std::string const& name, std::string const def_name, std::shared_ptr<ts::IStruct_Value> descriptor, std::chrono::high_resolution_clock::duration timeout = std::chrono::milliseconds(1000));
    ts::Result<void> remove_node(std::string const& name, std::chrono::high_resolution_clock::duration timeout = std::chrono::milliseconds(1000));

    ts::Result<Comms::Node> set_node_input_stream_path(std::string const& node_name, std::string const& input_name, std::string const& stream_path, std::chrono::high_resolution_clock::duration timeout = std::chrono::milliseconds(1000));

    ts::Result<void> set_stream_telemetry_enabled(std::string const& stream_path, bool enabled, std::chrono::high_resolution_clock::duration timeout = std::chrono::milliseconds(1000));

    ts::Result<Node> set_node_config(std::string const& name, std::shared_ptr<ts::IStruct_Value> config, std::chrono::high_resolution_clock::duration timeout = std::chrono::milliseconds(1000));

    //----------------------------------------------------------------------

    boost::signals2::signal<void()> sig_reset;
    boost::signals2::signal<void(Manual_Clock::time_point)> sig_clock_received;
    boost::signals2::signal<void()> sig_type_system_will_be_reset;
    boost::signals2::signal<void()> sig_type_system_reset;

    boost::signals2::signal<void(uint32_t req_id, std::string const& message)> sig_error_received;

    boost::signals2::signal<void(std::shared_ptr<ts::IStruct_Value>)> sig_uav_descriptor_received;

    void request_all_data();

    boost::signals2::signal<void(std::vector<Node_Def> const&)> sig_node_defs_received;
    boost::signals2::signal<void(std::vector<Node> const&)> sig_nodes_received;
    boost::signals2::signal<void(Node const&)> sig_node_added;
    boost::signals2::signal<void()> sig_node_removed;
    boost::signals2::signal<void(Node const&)> sig_node_changed;


    typedef util::comms::RCP_Channel<uint32_t> Telemetry_Channel;

    struct ITelemetry_Stream
    {
        virtual ~ITelemetry_Stream() = default;

        std::string stream_path;
        stream::Type stream_type;

    protected:
        friend class Comms;
        virtual bool unpack(Telemetry_Channel& channel, size_t sample_count) = 0;
    };

    template<class Stream_T>
    struct Telemetry_Stream : public ITelemetry_Stream
    {
        static constexpr stream::Semantic TYPE = Stream_T::TYPE;
        typedef typename Stream_T::Value Value;
        typedef typename Stream_T::Sample Sample;
        typedef std::vector<Sample> Samples;

        Samples samples;
    private:
        bool unpack(Telemetry_Channel& channel, size_t sample_count) override;
    };

    boost::signals2::signal<void(ITelemetry_Stream const&)> sig_telemetry_samples_available;

    boost::signals2::signal<void()> sig_stream_telemetry_done;


    struct Internal_Telementry_Sample
    {
        q::Clock::duration total_duration;
        q::Clock::duration max_total_duration;
        struct Node
        {
            std::string name;
            q::Clock::duration duration;
            q::Clock::duration max_duration;
        };
        std::vector<Node> nodes;
    };

    boost::signals2::signal<void(std::vector<Internal_Telementry_Sample> const&)> sig_internal_telemetry_samples_available;


    //boost::signals2::signal<void(IStream_Data const&)> sig_stream_data_received;


//    void request_node_config(std::string const& name);

//    void request_uav_config();
//    boost::signals2::signal<void(boost::optional<silk::UAV_Config&> config)> sig_uav_config_received;
//    void send_uav_config(boost::optional<silk::UAV_Config&> config);

    void process_rcp();
    void process();

private:
    ts::Type_System& m_ts;

    void configure_channels();

    std::map<stream::Type, std::unique_ptr<ITelemetry_Stream>> m_streams;

    struct Dispatch_Res_Visitor;
    struct Dispatch_Req_Visitor;

    void reset();
    bool m_did_request_data = false;

    template<typename T>
    void serialize_and_send(size_t channel_idx, T const& message);

    std::shared_ptr<util::comms::ISocket> m_socket;
    std::shared_ptr<util::comms::RCP> m_rcp;

    uint32_t m_last_req_id = 0;

    std::string const& decode_json(std::string const& json_base64);
    std::string const& encode_json(std::string const& json);

    std::vector<uint8_t> m_setup_buffer;
    std::string m_base64_buffer;

    Telemetry_Channel m_telemetry_channel;
    void handle_telemetry_stream();
    std::map<std::string, size_t> m_stream_telemetry_ref_count;

    void handle_internal_telemetry_stream();
    std::vector<Internal_Telementry_Sample> m_internal_telemetry_samples;

    bool handle_uav_descriptor(std::string const& serialized_data);
    ts::Result<Node> handle_node_data(gs_comms::setup::Node_Data const& node_data);

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
    void handle_res(gs_comms::setup::Set_Stream_Telemetry_Enabled_Res const& res);
    void handle_res(gs_comms::setup::Set_Node_Config_Res const& res);
};

}
