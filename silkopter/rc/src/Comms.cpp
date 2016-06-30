#include "Comms.h"

#include "common/node/INode.h"
#include "common/node/IPilot.h"

#include "common/Comm_Data.h"

#include "utils/RCP_UDP_Socket.h"
#include "utils/RCP_RFMON_Socket.h"

#include "comms.def.h"
#include "def_lang/JSON_Serializer.h"

#include "def_lang/ast/Node.h"
#include "def_lang/ast/Builder.h"

#include "def_lang/IPoly_Type.h"
#include "def_lang/IPoly_Value.h"


using namespace silk;
//using namespace boost::asio;

constexpr uint8_t SETUP_CHANNEL = 10;
constexpr uint8_t PILOT_CHANNEL = 15;
constexpr uint8_t VIDEO_CHANNEL = 16;
constexpr uint8_t TELEMETRY_CHANNEL = 20;

Comms::Comms(ts::Type_System& ts)
    : m_ts(ts)
    , m_pilot_channel(PILOT_CHANNEL)
    , m_video_channel(VIDEO_CHANNEL)
    , m_telemetry_channel(TELEMETRY_CHANNEL)
{
    m_streams[stream::IAcceleration::TYPE] =            std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IAcceleration>());
    m_streams[stream::IENU_Acceleration::TYPE] =        std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IENU_Acceleration>());
    m_streams[stream::IECEF_Acceleration::TYPE] =       std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IECEF_Acceleration>());
    m_streams[stream::IADC::TYPE] =                     std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IADC>());
    m_streams[stream::IAngular_Velocity::TYPE] =        std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IAngular_Velocity>());
    m_streams[stream::IENU_Angular_Velocity::TYPE] =    std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IENU_Angular_Velocity>());
    m_streams[stream::IECEF_Angular_Velocity::TYPE] =   std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IECEF_Angular_Velocity>());
    m_streams[stream::IBattery_State::TYPE] =           std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IBattery_State>());
    m_streams[stream::ICurrent::TYPE] =                 std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::ICurrent>());
    m_streams[stream::IDistance::TYPE] =                std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IDistance>());
    m_streams[stream::IENU_Distance::TYPE] =            std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IENU_Distance>());
    m_streams[stream::IECEF_Distance::TYPE] =           std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IECEF_Distance>());
    m_streams[stream::IFloat::TYPE] =                   std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IFloat>());
    m_streams[stream::IBool::TYPE] =                    std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IBool>());
    m_streams[stream::IForce::TYPE] =                   std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IForce>());
    m_streams[stream::IENU_Force::TYPE] =               std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IENU_Force>());
    m_streams[stream::IECEF_Force::TYPE] =              std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IECEF_Force>());
    m_streams[stream::IUAV_Frame::TYPE] =               std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IUAV_Frame>());
    m_streams[stream::IGPS_Info::TYPE] =                std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IGPS_Info>());
    m_streams[stream::ILinear_Acceleration::TYPE] =     std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::ILinear_Acceleration>());
    m_streams[stream::IENU_Linear_Acceleration::TYPE] = std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IENU_Linear_Acceleration>());
    m_streams[stream::IECEF_Linear_Acceleration::TYPE] =std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IECEF_Linear_Acceleration>());
    m_streams[stream::IECEF_Position::TYPE] =           std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IECEF_Position>());
    m_streams[stream::IMagnetic_Field::TYPE] =          std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IECEF_Magnetic_Field>());
    m_streams[stream::IENU_Magnetic_Field::TYPE] =      std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IENU_Magnetic_Field>());
    m_streams[stream::IECEF_Magnetic_Field::TYPE] =     std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IECEF_Magnetic_Field>());
    m_streams[stream::IPressure::TYPE] =                std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IPressure>());
    m_streams[stream::IPWM::TYPE] =                     std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IPWM>());
    m_streams[stream::ITemperature::TYPE] =             std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::ITemperature>());
    m_streams[stream::IThrottle::TYPE] =                std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IThrottle>());
    m_streams[stream::ITorque::TYPE] =                  std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::ITorque>());
    m_streams[stream::IENU_Torque::TYPE] =              std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IENU_Torque>());
    m_streams[stream::IECEF_Torque::TYPE] =             std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IECEF_Torque>());
    m_streams[stream::IVelocity::TYPE] =                std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IVelocity>());
    m_streams[stream::IENU_Velocity::TYPE] =            std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IENU_Velocity>());
    m_streams[stream::IECEF_Velocity::TYPE] =           std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IECEF_Velocity>());
    m_streams[stream::IVoltage::TYPE] =                 std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IVoltage>());
    m_streams[stream::IVideo::TYPE] =                   std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IVideo>());
    m_streams[stream::IMultirotor_Commands::TYPE] =     std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IMultirotor_Commands>());
    m_streams[stream::IMultirotor_State::TYPE] =        std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IMultirotor_State>());
    m_streams[stream::IProximity::TYPE] =               std::unique_ptr<Comms::IStream_Data>(new Comms::Stream_Data<stream::IProximity>());

}

auto Comms::start_udp(boost::asio::ip::address const& address, uint16_t send_port, uint16_t receive_port) -> bool
{
    try
    {
        auto s = new util::RCP_UDP_Socket();
        m_socket.reset(s);
        m_rcp.reset(new util::RCP());

        util::RCP::Socket_Handle handle = m_rcp->add_socket(m_socket.get());
        if (handle < 0)
        {
            QASSERT(0);
            throw std::exception();
        }

        m_rcp->set_internal_socket_handle(handle);
        m_rcp->set_socket_handle(SETUP_CHANNEL, handle);
        m_rcp->set_socket_handle(PILOT_CHANNEL, handle);
        m_rcp->set_socket_handle(VIDEO_CHANNEL, handle);
        m_rcp->set_socket_handle(TELEMETRY_CHANNEL, handle);

        s->open(send_port, receive_port);
        s->set_send_endpoint(address, send_port);
        s->start_listening();

//        m_socket.open(ip::udp::v4());
//        m_socket.set_option(ip::udp::socket::reuse_address(true));
//        m_socket.set_option(socket_base::send_buffer_size(65536));
//        m_socket.bind(ip::udp::endpoint(ip::udp::v4(), receive_port));

//        m_rudp.set_send_endpoint();

//        m_rudp.start_listening();

        QLOGI("Started sending on port {} and receiving on port {}", send_port, receive_port);
    }
    catch(...)
    {
        m_socket.reset();
        m_rcp.reset();
        QLOGW("Connect failed");
        return false;
    }

    configure_channels();
    reset();

    return true;
}

auto Comms::start_rfmon(std::string const& interface, uint8_t id) -> bool
{
    bool is_connected = false;
    try
    {
        auto s = new util::RCP_RFMON_Socket(interface, id);
        m_socket.reset(s);
        m_rcp.reset(new util::RCP());

        util::RCP::Socket_Handle handle = m_rcp->add_socket(m_socket.get());
        if (handle < 0)
        {
            QASSERT(0);
            throw std::exception();
        }

        m_rcp->set_internal_socket_handle(handle);
        m_rcp->set_socket_handle(SETUP_CHANNEL, handle);
        m_rcp->set_socket_handle(PILOT_CHANNEL, handle);
        m_rcp->set_socket_handle(VIDEO_CHANNEL, handle);
        m_rcp->set_socket_handle(TELEMETRY_CHANNEL, handle);

        is_connected = s->start();
    }
    catch(std::exception e)
    {
        is_connected = false;
    }

    if (!is_connected)
    {
        m_socket.reset();
        m_rcp.reset();
        QLOGW("Cannot start comms on interface {}", interface);
        return false;
    }

    QLOGI("Started sending on interface {}", interface);
    configure_channels();

    return true;
}

void Comms::disconnect()
{
    reset();
    m_socket.reset();
}

auto Comms::is_connected() const -> bool
{
    return m_socket != nullptr;
}


void Comms::configure_channels()
{
    {
        util::RCP::Send_Params params;
        params.is_compressed = true;
        params.is_reliable = false;
        params.importance = 10;
        m_rcp->set_send_params(TELEMETRY_CHANNEL, params);
    }

    {
        util::RCP::Send_Params params;
        params.is_compressed = true;
        params.is_reliable = true;
        params.importance = 100;
        m_rcp->set_send_params(SETUP_CHANNEL, params);
    }
    {
        util::RCP::Send_Params params;
        params.is_compressed = true;
        params.is_reliable = true;
        params.importance = 100;
        params.cancel_after = std::chrono::milliseconds(100);
        m_rcp->set_send_params(PILOT_CHANNEL, params);
    }

    {
        util::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::seconds(999999);
        m_rcp->set_receive_params(SETUP_CHANNEL, params);
    }
    {
        util::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(300);
        m_rcp->set_receive_params(PILOT_CHANNEL, params);
    }
    {
        util::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(300);
        m_rcp->set_receive_params(VIDEO_CHANNEL, params);
    }

    {
        util::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(500);
        m_rcp->set_receive_params(TELEMETRY_CHANNEL, params);
    }

}

//auto Comms::get_setup_channel() -> Setup_Channel&
//{
//    return m_setup_channel;
//}

void Comms::reset()
{
    m_last_req_id = 0;
    sig_reset.execute();
}

//void Comms::add_node(std::string const& def_name,
//                     std::string const& name,
//                     rapidjson::Document const& init_params)
//{
//    auto& channel = m_setup_channel;
//    channel.begin_pack(comms::Setup_Message::ADD_NODE);
//    channel.pack_param(def_name);
//    channel.pack_param(name);
//    channel.pack_param(init_params);
//    channel.end_pack();
//}

//void Comms::remove_node(std::string const& name)
//{
//    auto& channel = m_setup_channel;
//    channel.begin_pack(comms::Setup_Message::REMOVE_NODE);
//    channel.pack_param(name);
//    channel.end_pack();
//}

//void Comms::set_node_input_stream_path(std::string const& name, std::string const& input_name, q::Path const& stream_path)
//{
//    auto& channel = m_setup_channel;
//    channel.begin_pack(comms::Setup_Message::NODE_INPUT_STREAM_PATH);
//    channel.pack_param(name);
//    channel.pack_param(input_name);
//    channel.pack_param(stream_path.get_as<std::string>());
//    channel.end_pack();
//}

//void Comms::set_node_config(std::string const& name, rapidjson::Document const& config)
//{
//    auto& channel = m_setup_channel;
//    channel.begin_pack(comms::Setup_Message::NODE_CONFIG);
//    channel.pack_param(name);
//    channel.pack_param(config);
//    channel.end_pack();
//}

//void Comms::send_node_message(std::string const& name, rapidjson::Document const& json)
//{
//    auto& channel = m_setup_channel;
//    channel.begin_pack(comms::Setup_Message::NODE_MESSAGE);
//    channel.pack_param(name);
//    channel.pack_param(json);
//    channel.end_pack();
//}

//void Comms::set_stream_telemetry_active(q::Path const& stream_path, bool active)
//{
//    auto& channel = m_setup_channel;
//    channel.begin_pack(comms::Setup_Message::STREAM_TELEMETRY_ACTIVE);
//    channel.pack_param(stream_path.get_as<std::string>());
//    channel.pack_param(active);
//    channel.end_pack();
//}

template<typename T>
void Comms::serialize_and_send(size_t channel_idx, T const& message)
{
    if (m_rcp->is_connected())
    {
        ts::sz::Value sz_value = silk::comms::serialize(message);
        std::string json = ts::sz::to_json(sz_value, true);
        if (!m_rcp->send(channel_idx, json.data(), json.size()))
        {
            QLOGE("Failed to send message");
            return;
        }
    }
}

void Comms::request_all_data()
{
    //todo - ask for these in sequence, not all at once as the response is not guaranteed to arrive in order
    comms::setup::Brain_Req request;
    {
        uint64_t t = time(nullptr);
        comms::setup::Set_Clock_Req req;
        req.set_req_id(++m_last_req_id);
        req.set_time(t * 1000);
        request = req;
        serialize_and_send(SETUP_CHANNEL, request);
    }
    {
        comms::setup::Get_AST_Req req;
        req.set_req_id(++m_last_req_id);
        request = req;
        serialize_and_send(SETUP_CHANNEL, request);
    }
    {
        comms::setup::Get_UAV_Descriptor_Req req;
        req.set_req_id(++m_last_req_id);
        request = req;
        serialize_and_send(SETUP_CHANNEL, request);
    }
    {
        comms::setup::Get_Node_Defs_Req req;
        req.set_req_id(++m_last_req_id);
        request = req;
        serialize_and_send(SETUP_CHANNEL, request);
    }
    {
        comms::setup::Get_Nodes_Req req;
        req.set_req_id(++m_last_req_id);
        request = req;
        serialize_and_send(SETUP_CHANNEL, request);
    }
}

//void Comms::request_node_config(std::string const& name)
//{
//    m_setup_channel.pack_all(comms::Setup_Message::NODE_CONFIG, name);
//}

//void Comms::request_uav_config()
//{
////    m_setup_channel.pack_all(comms::Setup_Message::UAV_CONFIG);
//}

//void Comms::send_uav_config(boost::optional<silk::UAV_Config&> config)
//{
//    if (!config)
//    {
//        m_setup_channel.pack_all(comms::Setup_Message::UAV_CONFIG, false);
//    }
//    else if (silk::Multirotor_Config* multirotor_config = dynamic_cast<silk::Multirotor_Config*>(config.get_ptr()))
//    {
//        rapidjson::Document configj;
//        configj.SetObject();
//        autojsoncxx::to_document(*multirotor_config, configj);
//        m_setup_channel.pack_all(comms::Setup_Message::UAV_CONFIG, true, silk::UAV_Config::Type::MULTIROTOR, configj);
//    }
//}


//auto parse_json(std::string const& str) -> std::unique_ptr<rapidjson::Document>
//{
//    std::unique_ptr<rapidjson::Document> json(new rapidjson::Document);
//    json->SetObject();
//    if (!str.empty() && json->Parse(str.c_str()).HasParseError())
//    {
//        //            QLOGE("Failed to parse config: {}:{}", name, node->config.GetParseError(), node->config.GetErrorOffset());
//        return nullptr;
//    }
//    return std::move(json);
//}

//template<class T>
//auto unpack_def_inputs(Comms::Setup_Channel& channel, std::vector<T>& io) -> bool
//{
//    io.clear();
//    uint32_t size = 0;
//    if (!channel.unpack_param(size))
//    {
//        return false;
//    }
//    io.resize(size);
//    for (auto& i: io)
//    {
//        if (!channel.unpack_param(i.name) ||
//            !channel.unpack_param(i.type) ||
//            !channel.unpack_param(i.rate))
//        {
//            return false;
//        }
//    }
//    return true;
//}
//template<class T>
//auto unpack_inputs(Comms::Setup_Channel& channel, std::vector<T>& io) -> bool
//{
//    io.clear();
//    uint32_t size = 0;
//    if (!channel.unpack_param(size))
//    {
//        return false;
//    }
//    io.resize(size);
//    std::string stream_path;
//    for (auto& i: io)
//    {
//        if (!channel.unpack_param(i.name) ||
//                !channel.unpack_param(i.type) ||
//                !channel.unpack_param(i.rate) ||
//                !channel.unpack_param(stream_path))
//        {
//            return false;
//        }
//        i.stream_path = q::Path(stream_path);
//    }
//    return true;
//}

//template<class T>
//auto unpack_outputs(Comms::Setup_Channel& channel, std::vector<T>& io) -> bool
//{
//    io.clear();
//    uint32_t size = 0;
//    if (!channel.unpack_param(size))
//    {
//        return false;
//    }
//    io.resize(size);
//    for (auto& o: io)
//    {
//        if (!channel.unpack_param(o.name) ||
//            !channel.unpack_param(o.type) ||
//            !channel.unpack_param(o.rate))
//        {
//            return false;
//        }
//    }
//    return true;
//}

//template<class T>
//auto process_config(rapidjson::Document const& config, T& node)
//{
//    for (auto const& i: node.inputs)
//    {
//        i.stream_name = std::string();
//        q::Path path("inputs");
//        path += i.name;
//        auto* value = jsonutil::find_value(document, path);
//        if (!value)
//        {
//            QLOGE("Node {}, cannot find input named {} in the config json.", node.name, i.name);
//            continue;
//        }
//        if (value)
//        {
//            QLOGE("Node {}, cannot find input named {} in the config json.", node.name, i.name);
//            continue;
//        }
//        i.stream_name = value->GetString();
//    }
//}

//static auto unpack_node_def_data(Comms::Setup_Channel& channel, Comms::Node_Def& node_def) -> bool
//{
//    bool ok = channel.unpack_param(node_def.name);
//    ok &= channel.unpack_param(node_def.type);
//    ok &= unpack_def_inputs(channel, node_def.inputs);
//    ok &= unpack_outputs(channel, node_def.outputs);
//    ok &= channel.unpack_param(node_def.default_init_params_json);
//    return ok;
//}

//auto Comms::unpack_node_data(Comms::Setup_Channel& channel, Comms::Node& node) -> bool
//{
//    bool ok = channel.unpack_param(node.type);
//    ok &= unpack_inputs(channel, node.inputs);
//    ok &= unpack_outputs(channel, node.outputs);
//    ok &= channel.unpack_param(node.init_params_json);
//    ok &= channel.unpack_param(node.config_json);

////    if (ok)
////    {
////        for (auto& os: node.outputs)
////        {
////            auto stream = m_hal->m_streams.find_by_name(node.name + "/" + os.name);
////            os.stream = stream;
////        }
////    }

//    return ok;
//}

//template<class T>
//auto unpack_io_stream(HAL& hal, Comms::Setup_Channel& channel, std::vector<T>& io) -> bool
//{
//    io.clear();
//    uint32_t size = 0;
//    if (!channel.unpack_param(size))
//    {
//        return false;
//    }
//    io.resize(size);
//    std::string name;
//    for (auto& i: io)
//    {
//        if (!channel.unpack_param(name))
//        {
//            return false;
//        }
//        i.stream = hal.get_streams().find_by_name<stream::Stream>(name);
//        if (!i.stream)
//        {
//            QLOGE("Cannot find stream name {}", i.name);
//            return false;
//        }
//    }
//    return true;
//}

//void Comms::handle_clock()
//{
//    uint64_t us;
//    if (m_setup_channel.unpack_all(us))
//    {
//        sig_clock_received.execute(Manual_Clock::time_point(std::chrono::microseconds(us)));
//        //m_hal->m_remote_clock.set_epoch(Manual_Clock::time_point(std::chrono::microseconds(us)));
//    }
//    else
//    {
//        QLOGE("Error in enumerating clock");
//    }
//}

//void Comms::handle_uav_config()
//{
//    bool has_config = false;
//    bool ok = m_setup_channel.begin_unpack() &&
//              m_setup_channel.unpack_param(has_config);
//    if (!ok)
//    {
//        QLOGE("Failed to unpack uav config");
//        return;
//    }

//    QLOGI("Uav config received");

//    if (has_config)
//    {
//        UAV_Config::Type type;
//        rapidjson::Document configj;
//        if (!m_setup_channel.unpack_param(type) || !m_setup_channel.unpack_param(configj))
//        {
//            QLOGE("Failed to unpack config json");
//        }

//        if (type == Multirotor_Config::TYPE)
//        {
//            Multirotor_Config config;
//            autojsoncxx::error::ErrorStack result;
//            if (!autojsoncxx::from_value(config, configj, result))
//            {
//                std::ostringstream ss;
//                ss << result;
//                QLOGE("Req Id: {} - Cannot deserialize multi config: {}", ss.str());
//                return;
//            }
//            sig_uav_config_received.execute(config);
//        }
//    }
//    else
//    {
//        sig_uav_config_received.execute(boost::none);
//    }

//    m_setup_channel.end_unpack();
//}


//void Comms::handle_enumerate_node_defs()
//{
//    //m_hal->m_node_defs.remove_all();

//    uint32_t node_count = 0;
//    if (!m_setup_channel.begin_unpack() ||
//        !m_setup_channel.unpack_param(node_count))
//    {
//        QLOGE("Error in enumerating node defs");
//        return;
//    }

//    QLOGI("Received defs for {} nodes", node_count);

//    std::vector<Node_Def> defs;
//    for (uint32_t i = 0; i < node_count; i++)
//    {
//        Node_Def def;
//        if (!unpack_node_def_data(m_setup_channel, def))
//        {
//            QLOGE("\t\tBad node");
//            return;
//        }
//        QLOGI("\tNode: {}", def.name);
//        //m_hal->m_node_defs.add(std::move(def));
//        defs.push_back(std::move(def));
//    }
//    sig_node_defs_reset.execute();
//    sig_node_defs_added.execute(defs);

//    m_setup_channel.end_unpack();
//}

////auto Comms::publish_streams(Node const& node) -> bool
////{
////    for (auto& os: node.outputs)
////    {
////        auto stream = create_stream_from_type(os.type);
////        if (!stream)
////        {
////            QLOGE("\t\tCannot create output stream '{}/{}', type {}", node.name, os.name, os.type);
////            return false;
////        }
////        //os.stream = stream;

////        //stream->node = node;
////        stream->rate = os.rate;
////        //m_hal->m_streams.add(stream);
////        //sig_stream_added.execute(stream);
////        m_stream_data_holders[node.name + "/" + os.name] = std::move(stream);
////    }
////    return true;
////}

////auto Comms::unpublish_streams(Node const& node) -> bool
////{
////    for (auto& os: node.outputs)
////    {
////        //auto stream = m_hal->m_streams.find_by_name(node->name + "/" + os.name);
////        //m_hal->m_streams.remove(stream);
////        //sig_stream_removed.execute(os);
////        m_stream_data_holders.erase(node.name + "/" + os.name);
////    }
////    return true;
////}

//void Comms::handle_enumerate_nodes()
//{
//    uint32_t node_count = 0;
//    if (!m_setup_channel.begin_unpack() ||
//        !m_setup_channel.unpack_param(node_count))
//    {
//        QLOGE("Error in unpacking enumerate nodes message");
//        return;
//    }

//    QLOGI("Received {} nodes", node_count);

//    std::vector<Node> nodes;
//    for (uint32_t i = 0; i < node_count; i++)
//    {
//        Node node;
//        bool ok = m_setup_channel.unpack_param(node.name);
//        ok &= unpack_node_data(m_setup_channel, node);
//        if (!ok)
//        {
//            QLOGE("\t\tBad node");
//            return;
//        }
//        QLOGI("\tNode: {}", node.name);
//        //nodes.push_back(std::move(node));
//    }

//    sig_nodes_reset.execute();
//    sig_nodes_added.execute(nodes);

//    m_setup_channel.end_unpack();
//}


//void Comms::handle_node_message()
//{
//    std::string name;
//    if (!m_setup_channel.begin_unpack() ||
//        !m_setup_channel.unpack_param(name))
//    {
//        QLOGE("Failed to unpack node message");
//        return;
//    }

////    auto node = m_hal->get_nodes().find_by_name(name);
////    if (!node)
////    {
////        QLOGE("Cannot find node '{}'", name);
////        return;
////    }

////    QLOGI("Node '{}' - message received", name);

//    rapidjson::Document json;
//    if (!m_setup_channel.unpack_param(json))
//    {
//        QLOGE("Node '{}' - failed to unpack message", name);
//    }

//    sig_node_message_received.execute(name, json);


//    //node->message_received_signal.execute(json);
//}

//void Comms::handle_node_config()
//{
//    std::string name;
//    rapidjson::Document json;
//    if (!m_setup_channel.begin_unpack() ||
//        !m_setup_channel.unpack_param(name) ||
//        !m_setup_channel.unpack_param(json))
//    {
//        QLOGE("Failed to unpack node config");
//        return;
//    }

//    sig_node_config_received.execute(name, json);
//    //auto node = m_hal->get_nodes().find_by_name(name);
//    //if (!node)
//    //{
//    //    QLOGE("Cannot find node '{}'", name);
//    //    return;
//    //}
//    //node->config.SetObject();
//    //jsonutil::clone_value(node->config, json, node->config.GetAllocator());
//    //node->changed_signal.execute();
//}


//void Comms::handle_get_node_data()
//{
//    std::string name;
//    bool ok = m_setup_channel.begin_unpack() &&
//              m_setup_channel.unpack_param(name);
//    if (!ok)
//    {
//        QLOGE("Failed to unpack node data");
//        return;
//    }

//    QLOGI("Node '{}' - config received", name);

//    Node node;
//    node.name = name;
//    if (!unpack_node_data(m_setup_channel, node))
//    {
//        QLOGE("Node '{}' - failed to unpack config", name);
//    }

//    sig_node_changed.execute(node);
//}

//void Comms::handle_node_input_stream_path()
//{
//    std::string name;
//    if (!m_setup_channel.begin_unpack() ||
//        !m_setup_channel.unpack_param(name))
//    {
//        QLOGE("Failed to unpack node input stream path");
//        return;
//    }

//    QLOGI("Node '{}' - message received", name);

//    Node node;
//    node.name = name;
//    if (!unpack_node_data(m_setup_channel, node))
//    {
//        QLOGE("Node '{}' - failed to unpack config", name);
//    }

//    sig_node_changed.execute(node);
//}

//void Comms::handle_add_node()
//{
//    std::string name;
//    if (!m_setup_channel.begin_unpack() ||
//        !m_setup_channel.unpack_param(name))
//    {
//        QLOGE("Failed to unpack add node request");
//        return;
//    }

//    Node node;
//    node.name = name;
//    bool ok = unpack_node_data(m_setup_channel, node);
//    if (!ok)
//    {
//        return;
//    }

//    std::vector<Node> nodes;
//    nodes.push_back(std::move(node));
//    sig_nodes_added.execute(nodes);
//}

//void Comms::handle_remove_node()
//{
//    std::string name;
//    bool ok = m_setup_channel.begin_unpack() &&
//              m_setup_channel.unpack_param(name);
//    if (!ok)
//    {
//        QLOGE("Failed to unpack remove node request");
//        return;
//    }

//    sig_node_removed.execute(name);
//}

//void Comms::handle_streams_telemetry_active()
//{
//    bool is_active = false;
//    bool ok = m_setup_channel.begin_unpack() &&
//              m_setup_channel.unpack_param(is_active);
//    if (!ok)
//    {
//        QLOGE("Failed to unpack remove node request");
//        return;
//    }
//}

template<typename Stream>
void Comms::Stream_Data<Stream>::unpack(Comms::Telemetry_Channel& channel, uint32_t sample_count)
{
    samples.clear();
    Sample sample;
    for (uint32_t i = 0; i < sample_count; i++)
    {
        if (!channel.unpack_param(sample))
        {
            QLOGE("Error unpacking samples!!!");
        }
        samples.push_back(sample);
    }
}

//void Comms::handle_stream_data()
//{
//    std::string stream_name;
//    stream::Type stream_type;
//    uint32_t sample_count = 0;
//    bool ok = m_telemetry_channel.begin_unpack() &&
//              m_telemetry_channel.unpack_param(stream_name) &&
//              m_telemetry_channel.unpack_param(stream_type) &&
//              m_telemetry_channel.unpack_param(sample_count);
//    if (!ok)
//    {
//        QLOGE("Failed to unpack stream telemetry");
//        return;
//    }

//    IStream_Data* stream_data = nullptr;
//    auto it = m_streams.find(stream_type);
//    if (it != m_streams.end())
//    {
//        stream_data = it->second.get();
//    }
//    else
//    {
//        QLOGE("Failed to find a stream data holder for semantic {}, space {} for stream {}", stream_type.get_semantic(), stream_type.get_space(), stream_name);
//        return;
//    }

//    stream_data->unpack(m_telemetry_channel, sample_count);
//    sig_stream_data_received.execute(*stream_data);
//}

//void Comms::handle_multirotor_state()
//{
//    auto& channel = m_pilot_channel;

//    stream::IMultirotor_State::Sample sample;
//    if (!channel.unpack_all(sample))
//    {
//        QLOGE("Error in unpacking multirotor state");
//        return;
//    }

//    m_multirotor_state_samples.push_back(sample);
//}
//void Comms::handle_video()
//{
//    auto& channel = m_video_channel;

//    stream::IVideo::Sample sample;
//    if (!channel.unpack_all(sample))
//    {
//        QLOGE("Error in unpacking video");
//        return;
//    }

//    m_video_samples.push_back(sample);
//}

//auto Comms::get_video_samples() const -> std::vector<stream::IVideo::Sample> const&
//{
//    return m_video_samples;
//}
//auto Comms::get_multirotor_state_samples() const -> std::vector<stream::IMultirotor_State::Sample> const&
//{
//    return m_multirotor_state_samples;
//}
//void Comms::send_multirotor_commands_value(stream::IMultirotor_Commands::Value const& value)
//{
//    m_pilot_channel.pack_all(silk::comms::Pilot_Message::MULTIROTOR_COMMANDS, value);
//    m_pilot_channel.try_sending(*m_rcp);
//}

std::string const& Comms::decode_json(std::string const& json_base64)
{
    m_base64_buffer.resize(q::util::compute_base64_max_decoded_size(json_base64.size()));
    auto last_it = q::util::decode_base64(json_base64.data(), json_base64.data() + json_base64.size(), m_base64_buffer.begin());
    m_base64_buffer.erase(last_it, m_base64_buffer.end());
    return m_base64_buffer;
}
std::string const& Comms::encode_json(std::string const& json)
{
    m_base64_buffer.resize(q::util::compute_base64_encoded_size(json.size()));
    q::util::encode_base64(json.data(), json.size(), &m_base64_buffer[0]);
    return m_base64_buffer;
}

void Comms::handle_res(comms::setup::Error const& res)
{
    QLOGI("Error {}", res.get_req_id());

    sig_error_occurred.execute(res.get_req_id(), res.get_message());
}

void Comms::handle_res(comms::setup::Get_AST_Res const& res)
{
    QLOGI("Get_AST_Res {}", res.get_req_id());

    std::string const& json = decode_json(res.get_data());
    auto json_result = ts::sz::from_json(json);
    if (json_result != ts::success)
    {
        QLOGE("Cannot deserialize ast json data: {}", json_result.error().what());
        return;
    }

    ts::ast::Builder builder;
    auto deserialize_result = builder.get_ast_root_node().deserialize(json_result.payload());
    if (deserialize_result != ts::success)
    {
        QLOGE("Cannot deserialize ast root: {}", deserialize_result.error().what());
        return;
    }

    ts::Type_System ts;
    ts.populate_builtin_types();

    auto compile_result = builder.compile(ts);
    if (compile_result != ts::success)
    {
        QLOGE("Cannot compile type system: {}", compile_result.error().what());
        return;
    }

    sig_type_system_will_be_reset.execute();

    m_ts = ts;

    sig_type_system_reset.execute();
}

void Comms::handle_res(comms::setup::Set_Clock_Res const& res)
{
    QLOGI("Set_Clock_Res {}", res.get_req_id());

    Manual_Clock clock;
    clock.set_epoch(Manual_Clock::time_point(Manual_Clock::duration(std::chrono::milliseconds(res.get_time()))));
    sig_clock_received.execute(clock.now());
}


bool Comms::handle_uav_descriptor(std::string const& serialized_data)
{
    std::shared_ptr<const ts::IPoly_Type> descriptor_poly_type = m_ts.get_root_scope()->find_specialized_symbol_by_path<ts::IPoly_Type>("Poly_IUAV_Descriptor");
    if (!descriptor_poly_type)
    {
        QLOGE("Cannot find 'Poly_IUAV_Descriptor' type in the type system");
        return false;
    }

    std::shared_ptr<ts::IPoly_Value> descriptor_poly_value = descriptor_poly_type->create_specialized_value();
    QASSERT(descriptor_poly_value);
    auto construction_result = descriptor_poly_value->construct();
    if (construction_result != ts::success)
    {
        QLOGE("Cannot construct a 'Poly_IUAV_Descriptor' value: {}", construction_result.error().what());
        return false;
    }

    std::string const& json = decode_json(serialized_data);
    auto json_result = ts::sz::from_json(json);
    if (json_result != ts::success)
    {
        QLOGE("Cannot deserialize ast json data: {}", json_result.error().what());
        return false;
    }

    auto deserialize_result = descriptor_poly_value->deserialize(json_result.payload());
    if (deserialize_result != ts::success)
    {
        QLOGE("Cannot deserialize descriptor: {}", deserialize_result.error().what());
        return false;
    }

    if (descriptor_poly_value->get_value())
    {
        std::shared_ptr<ts::IStruct_Value> descriptor_value = std::dynamic_pointer_cast<ts::IStruct_Value>(descriptor_poly_value->get_value());
        if (!descriptor_value)
        {
            QLOGE("Invalid descriptor value. Expected struct, got something else");
            return false;
        }
        sig_uav_descriptor_received.execute(descriptor_value);
    }
    else
    {
        sig_uav_descriptor_received.execute(nullptr);
    }

    return true;
}

void Comms::handle_res(comms::setup::Set_UAV_Descriptor_Res const& res)
{
    QLOGI("Set_UAV_Descriptor_Res {}", res.get_req_id());

    handle_uav_descriptor(res.get_data());
}

void Comms::handle_res(comms::setup::Get_UAV_Descriptor_Res const& res)
{
    QLOGI("Get_UAV_Descriptor_Res {}", res.get_req_id());

    handle_uav_descriptor(res.get_data());
}

void Comms::handle_res(comms::setup::Get_Node_Defs_Res const& res)
{
    QLOGI("Get_Node_Defs_Res {}", res.get_req_id());


    std::shared_ptr<const ts::IPoly_Type> descriptor_poly_type = m_ts.get_root_scope()->find_specialized_symbol_by_path<ts::IPoly_Type>("Poly_INode_Descriptor");
    if (!descriptor_poly_type)
    {
        QLOGE("Cannot find 'Poly_INode_Descriptor' type in the type system");
        return;
    }

    std::shared_ptr<ts::IPoly_Value> descriptor_poly_value = descriptor_poly_type->create_specialized_value();
    QASSERT(descriptor_poly_value);
    auto construction_result = descriptor_poly_value->construct();
    if (construction_result != ts::success)
    {
        QLOGE("Cannot construct a 'Poly_INode_Descriptor' value: {}", construction_result.error().what());
        return;
    }


    std::vector<Node_Def> node_defs;

    std::vector<comms::setup::Node_Def_Data> const& node_def_datas = res.get_node_def_datas();
    node_defs.reserve(node_def_datas.size());

    for (comms::setup::Node_Def_Data const& node_def_data: node_def_datas)
    {
        Node_Def node_def;
        node_def.name = node_def_data.get_name();
        node_def.type = static_cast<node::Type>(node_def_data.get_type());

        std::vector<comms::setup::Node_Def_Data::Input> const& input_datas = node_def_data.get_inputs();
        node_def.inputs.reserve(input_datas.size());
        for (comms::setup::Node_Def_Data::Input const& input_data: input_datas)
        {
            Node_Def::Input input;
            input.name = input_data.get_name();
            input.type = stream::Type(static_cast<stream::Semantic>(input_data.get_semantic()), static_cast<stream::Space>(input_data.get_space()));
            node_def.inputs.push_back(std::move(input));
        }

        std::vector<comms::setup::Node_Def_Data::Output> const& output_datas = node_def_data.get_outputs();
        node_def.outputs.reserve(output_datas.size());
        for (comms::setup::Node_Def_Data::Output const& output_data: output_datas)
        {
            Node_Def::Output output;
            output.name = output_data.get_name();
            output.type = stream::Type(static_cast<stream::Semantic>(output_data.get_semantic()), static_cast<stream::Space>(output_data.get_space()));
            node_def.outputs.push_back(std::move(output));
        }

        std::string const& json = decode_json(node_def_data.get_descriptor_data());
        auto json_result = ts::sz::from_json(json);
        if (json_result != ts::success)
        {
            QLOGE("Cannot deserialize ast json data: {}", json_result.error().what());
            return;
        }

        auto deserialize_result = descriptor_poly_value->deserialize(json_result.payload());
        if (deserialize_result != ts::success)
        {
            QLOGE("Cannot deserialize descriptor: {}", deserialize_result.error().what());
            return;
        }

        node_def.default_descriptor = std::dynamic_pointer_cast<ts::IStruct_Value>(descriptor_poly_value->get_value());
        if (!node_def.default_descriptor)
        {
            QLOGE("Invalid descriptor value. Expected struct, got something else");
            return;
        }

        node_defs.push_back(std::move(node_def));
    }

    sig_node_defs_added.execute(node_defs);
}

void Comms::handle_res(comms::setup::Remove_Node_Res const& res)
{
    QLOGI("Remove_Node_Res {}", res.get_req_id());

}

void Comms::handle_res(comms::setup::Get_Nodes_Res const& res)
{
    QLOGI("Get_Nodes_Res {}", res.get_req_id());

}

void Comms::handle_res(comms::setup::Add_Node_Res const& res)
{
    QLOGI("Add_Node_Res {}", res.get_req_id());

}

void Comms::handle_res(comms::setup::Set_Node_Input_Stream_Path_Res const& res)
{
    QLOGI("Set_Node_Input_Stream_Path_Res {}", res.get_req_id());

}



void Comms::process_rcp()
{
    if (!is_connected())
    {
        return;
    }

    auto result = m_socket->process();
    if (result != util::RCP_Socket::Result::OK)
    {
        m_rcp->reconnect();
    }

    m_rcp->process();
}

struct Comms::Dispatch_Res_Visitor : boost::static_visitor<void>
{
    Dispatch_Res_Visitor(Comms& comms) : m_comms(comms) {}
    template <typename T> void operator()(T const& t) const { m_comms.handle_res(t); }
    Comms& m_comms;
};
struct Comms::Dispatch_Req_Visitor : boost::static_visitor<void>
{
    Dispatch_Req_Visitor(Comms& comms) : m_comms(comms) {}
//    template <typename T> void operator()(T const& t) const { m_comms.handle_req(t); }
    Comms& m_comms;
};

void Comms::process()
{
    if (!is_connected())
    {
        return;
    }

    m_multirotor_state_samples.clear();
    m_video_samples.clear();

    while (auto msg = m_pilot_channel.get_next_message(*m_rcp))
    {
        switch (msg.get())
        {
        //case comms::Pilot_Message::MULTIROTOR_STATE : handle_multirotor_state(); break;
        default: break;
        }
    }
    while (auto msg = m_video_channel.get_next_message(*m_rcp))
    {
        switch (msg.get())
        {
        //case comms::Video_Message::FRAME_DATA : handle_video(); break;
        default: break;
        }
    }

    auto start = q::Clock::now();
    while (auto msg = m_telemetry_channel.get_next_message(*m_rcp))
    {
        //process only the first 10 ms worh of data and discard the rest
        if (q::Clock::now() - start < std::chrono::milliseconds(100))
        {
            switch (msg.get())
            {
//            case comms::Telemetry_Message::STREAM_DATA : handle_stream_data(); break;
            default: break;
            }
        }
    }

    Dispatch_Res_Visitor dispatcher(*this);
    while (m_rcp->receive(SETUP_CHANNEL, m_setup_buffer))
    {
        const char* data = (const char*)m_setup_buffer.data();
//        QLOGI("{}", std::string(data, data + m_setup_buffer.size()));

        auto parse_result = ts::sz::from_json(m_setup_buffer.data(), m_setup_buffer.size());
        if (parse_result != ts::success)
        {
            QLOGE("Cannot parse setup req: {}", parse_result.error().what());
        }
        else
        {
            silk::comms::setup::Brain_Res res;
            auto result = silk::comms::deserialize(res, parse_result.payload());
            if (result != ts::success)
            {
                QLOGE("Cannot deserialize setup res: {}", result.error().what());
            }
            boost::apply_visitor(dispatcher, res);
        }
        m_setup_buffer.clear();
    }

//    while (auto msg = m_setup_channel.get_next_message(*m_rcp))
//    {
//        switch (msg.get())
//        {
//        case comms::Setup_Message::UAV_CONFIG: handle_uav_config(); break;

//        case comms::Setup_Message::CLOCK: handle_clock(); break;

//        case comms::Setup_Message::ENUMERATE_NODE_DEFS: handle_enumerate_node_defs(); break;
//        case comms::Setup_Message::ENUMERATE_NODES: handle_enumerate_nodes(); break;
//        case comms::Setup_Message::GET_NODE_DATA: handle_get_node_data(); break;

//        case comms::Setup_Message::ADD_NODE: handle_add_node(); break;
//        case comms::Setup_Message::REMOVE_NODE: handle_remove_node(); break;
//        case comms::Setup_Message::NODE_CONFIG: handle_node_config(); break;
//        case comms::Setup_Message::NODE_MESSAGE: handle_node_message(); break;
//        case comms::Setup_Message::NODE_INPUT_STREAM_PATH: handle_node_input_stream_path(); break;

//        case comms::Setup_Message::STREAM_TELEMETRY_ACTIVE: handle_streams_telemetry_active(); break;

//        default: break;
//        }
//    }
    //    QLOGI("*********** LOOP: {}", xxx);

    if (m_rcp->is_connected())
    {
        if (!m_did_request_data)
        {
            m_did_request_data = true;
            request_all_data();
        }
    }
    else
    {
        m_did_request_data = false;
    }

    //m_setup_channel.send(*m_rcp);
    m_telemetry_channel.try_sending(*m_rcp);
}
