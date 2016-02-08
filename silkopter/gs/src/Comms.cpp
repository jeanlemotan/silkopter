#include "Comms.h"

#include "sz_math.hpp"
#include "sz_Multi_Config.hpp"

#include "utils/RCP_UDP_Socket.h"
#include "utils/RCP_RFMON_Socket.h"


using namespace silk;
using namespace boost::asio;

constexpr uint8_t SETUP_CHANNEL = 10;
constexpr uint8_t PILOT_CHANNEL = 15;
constexpr uint8_t VIDEO_CHANNEL = 16;
constexpr uint8_t TELEMETRY_CHANNEL = 20;

Comms::Comms(HAL& hal)
    : m_hal(hal)
    , m_setup_channel(SETUP_CHANNEL)
    , m_pilot_channel(PILOT_CHANNEL)
    , m_video_channel(PILOT_CHANNEL)
    , m_telemetry_channel(TELEMETRY_CHANNEL)
{
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
        s->set_send_endpoint(ip::udp::endpoint(address, send_port));
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

auto Comms::get_setup_channel() -> Setup_Channel&
{
    return m_setup_channel;
}

void Comms::reset()
{
    m_hal.m_nodes.remove_all();
    m_hal.m_streams.remove_all();
}

void Comms::request_data()
{
    m_setup_channel.pack_all(comms::Setup_Message::CLOCK);
    m_setup_channel.pack_all(comms::Setup_Message::ENUMERATE_NODE_DEFS);
    m_setup_channel.pack_all(comms::Setup_Message::ENUMERATE_NODES);
    m_setup_channel.pack_all(comms::Setup_Message::MULTI_CONFIG);
}

void Comms::request_all_node_configs()
{
    auto nodes = m_hal.get_nodes().get_all();
    for (auto const& n: nodes)
    {
        m_setup_channel.pack_all(comms::Setup_Message::NODE_CONFIG, n->name);
    }
}


auto parse_json(std::string const& str) -> std::unique_ptr<rapidjson::Document>
{
    std::unique_ptr<rapidjson::Document> json(new rapidjson::Document);
    json->SetObject();
    if (!str.empty() && json->Parse(str.c_str()).HasParseError())
    {
        //            QLOGE("Failed to parse config: {}:{}", name, node->config.GetParseError(), node->config.GetErrorOffset());
        return nullptr;
    }
    return std::move(json);
}

template<class T>
auto unpack_def_inputs(Comms::Setup_Channel& channel, std::vector<T>& io) -> bool
{
    io.clear();
    uint32_t size = 0;
    if (!channel.unpack_param(size))
    {
        return false;
    }
    io.resize(size);
    for (auto& i: io)
    {
        if (!channel.unpack_param(i.name) ||
                !channel.unpack_param(i.type) ||
                !channel.unpack_param(i.rate))
        {
            return false;
        }
    }
    return true;
}
template<class T>
auto unpack_inputs(Comms::Setup_Channel& channel, std::vector<T>& io) -> bool
{
    io.clear();
    uint32_t size = 0;
    if (!channel.unpack_param(size))
    {
        return false;
    }
    io.resize(size);
    std::string stream_path;
    for (auto& i: io)
    {
        if (!channel.unpack_param(i.name) ||
                !channel.unpack_param(i.type) ||
                !channel.unpack_param(i.rate) ||
                !channel.unpack_param(stream_path))
        {
            return false;
        }
        i.stream_path = q::Path(stream_path);
    }
    return true;
}

template<class T>
auto unpack_outputs(Comms::Setup_Channel& channel, std::vector<T>& io) -> bool
{
    io.clear();
    uint32_t size = 0;
    if (!channel.unpack_param(size))
    {
        return false;
    }
    io.resize(size);
    for (auto& o: io)
    {
        if (!channel.unpack_param(o.name) ||
            !channel.unpack_param(o.type) ||
            !channel.unpack_param(o.rate))
        {
            return false;
        }
    }
    return true;
}

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

static auto unpack_node_def_data(Comms::Setup_Channel& channel, node::gs::Node_Def& node_def) -> bool
{
    bool ok = channel.unpack_param(node_def.name);
    ok &= channel.unpack_param(node_def.type);
    ok &= unpack_def_inputs(channel, node_def.inputs);
    ok &= unpack_outputs(channel, node_def.outputs);
    ok &= channel.unpack_param(node_def.default_init_params);
    return ok;
}

auto Comms::unpack_node_data(Comms::Setup_Channel& channel, node::gs::Node& node) -> bool
{
    bool ok = channel.unpack_param(node.type);
    ok &= unpack_inputs(channel, node.inputs);
    ok &= unpack_outputs(channel, node.outputs);
    ok &= channel.unpack_param(node.init_params);
    ok &= channel.unpack_param(node.config);

    if (ok)
    {
        for (auto& os: node.outputs)
        {
            auto stream = m_hal.m_streams.find_by_name(node.name + "/" + os.name);
            os.stream = stream;
        }
    }

    return ok;
}

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

void Comms::handle_clock()
{
    uint64_t us;
    if (m_setup_channel.unpack_all(us))
    {
        m_hal.m_remote_clock.set_epoch(Manual_Clock::time_point(std::chrono::microseconds(us)));
    }
    else
    {
        QLOGE("Error in enumerating clock");
    }
}

void Comms::handle_multi_config()
{
    bool success = false;
    bool ok = m_setup_channel.begin_unpack() &&
              m_setup_channel.unpack_param(success);
    if (!ok)
    {
        QLOGE("Failed to unpack multi config");
        return;
    }

    QLOGI("Multi config received");

    if (success)
    {
        rapidjson::Document configj;
        if (!m_setup_channel.unpack_param(configj))
        {
            QLOGE("Failed to unpack config json");
        }

        config::Multi config;
        autojsoncxx::error::ErrorStack result;
        if (!autojsoncxx::from_value(config, configj, result))
        {
            std::ostringstream ss;
            ss << result;
            QLOGE("Req Id: {} - Cannot deserialize multi config: {}", ss.str());
            return;
        }
        m_hal.m_configs.multi = config;
    }
    m_hal.multi_config_refreshed_signal.execute();

    m_setup_channel.end_unpack();
}


void Comms::handle_enumerate_node_defs()
{
    uint32_t node_count = 0;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(node_count))
    {
        QLOGE("Error in enumerating node defs");
        return;
    }

    QLOGI("Received defs for {} nodes", node_count);

    for (uint32_t i = 0; i < node_count; i++)
    {
        auto def = std::make_shared<node::gs::Node_Def>();
        if (!unpack_node_def_data(m_setup_channel, *def))
        {
            QLOGE("\t\tBad node");
            return;
        }
        QLOGI("\tNode: {}", def->name);
        m_hal.m_node_defs.add(std::move(def));
    }

    m_setup_channel.end_unpack();
}

auto create_stream_from_type(stream::Type type) -> std::shared_ptr<stream::gs::Stream>
{
    switch (type)
    {
        case stream::gs::Acceleration::TYPE:             return std::make_shared<stream::gs::Acceleration>();
//        case stream::gs::ENU_Acceleration::TYPE:         return std::make_shared<stream::gs::ENU_Acceleration>();
//        case stream::gs::ECEF_Acceleration::TYPE:        return std::make_shared<stream::gs::ECEF_Acceleration>();
        case stream::gs::ADC::TYPE:                      return std::make_shared<stream::gs::ADC>();
        case stream::gs::Angular_Velocity::TYPE:         return std::make_shared<stream::gs::Angular_Velocity>();
//        case stream::gs::ENU_Angular_Velocity::TYPE:     return std::make_shared<stream::gs::ENU_Angular_Velocity>();
//        case stream::gs::ECEF_Angular_Velocity::TYPE:    return std::make_shared<stream::gs::ECEF_Angular_Velocity>();
        case stream::gs::Battery_State::TYPE:            return std::make_shared<stream::gs::Battery_State>();
        case stream::gs::Current::TYPE:                  return std::make_shared<stream::gs::Current>();
        case stream::gs::Distance::TYPE:                 return std::make_shared<stream::gs::Distance>();
//        case stream::gs::ENU_Distance::TYPE:             return std::make_shared<stream::gs::ENU_Distance>();
//        case stream::gs::ECEF_Distance::TYPE:            return std::make_shared<stream::gs::ECEF_Distance>();
        case stream::gs::Float::TYPE:                    return std::make_shared<stream::gs::Float>();
        case stream::gs::Bool::TYPE:                     return std::make_shared<stream::gs::Bool>();
        case stream::gs::Force::TYPE:                    return std::make_shared<stream::gs::Force>();
//        case stream::gs::ENU_Force::TYPE:                return std::make_shared<stream::gs::ENU_Force>();
//        case stream::gs::ECEF_Force::TYPE:               return std::make_shared<stream::gs::ECEF_Force>();
        case stream::gs::Frame::TYPE:                    return std::make_shared<stream::gs::Frame>();
        case stream::gs::GPS_Info::TYPE:                 return std::make_shared<stream::gs::GPS_Info>();
        case stream::gs::Linear_Acceleration::TYPE:      return std::make_shared<stream::gs::Linear_Acceleration>();
//        case stream::gs::ENU_Linear_Acceleration::TYPE:  return std::make_shared<stream::gs::ENU_Linear_Acceleration>();
//        case stream::gs::ECEF_Linear_Acceleration::TYPE: return std::make_shared<stream::gs::ECEF_Linear_Acceleration>();
        case stream::gs::ECEF_Position::TYPE:            return std::make_shared<stream::gs::ECEF_Position>();
        case stream::gs::Magnetic_Field::TYPE:           return std::make_shared<stream::gs::ECEF_Magnetic_Field>();
//        case stream::gs::ENU_Magnetic_Field::TYPE:       return std::make_shared<stream::gs::ENU_Magnetic_Field>();
//        case stream::gs::ECEF_Magnetic_Field::TYPE:      return std::make_shared<stream::gs::ECEF_Magnetic_Field>();
        case stream::gs::Pressure::TYPE:                 return std::make_shared<stream::gs::Pressure>();
        case stream::gs::PWM::TYPE:                      return std::make_shared<stream::gs::PWM>();
        case stream::gs::Temperature::TYPE:              return std::make_shared<stream::gs::Temperature>();
        case stream::gs::Throttle::TYPE:                 return std::make_shared<stream::gs::Throttle>();
        case stream::gs::Torque::TYPE:                   return std::make_shared<stream::gs::Torque>();
//        case stream::gs::ENU_Torque::TYPE:               return std::make_shared<stream::gs::ENU_Torque>();
//        case stream::gs::ECEF_Torque::TYPE:              return std::make_shared<stream::gs::ECEF_Torque>();
        case stream::gs::Velocity::TYPE:                 return std::make_shared<stream::gs::Velocity>();
//        case stream::gs::ENU_Velocity::TYPE:             return std::make_shared<stream::gs::ENU_Velocity>();
//        case stream::gs::ECEF_Velocity::TYPE:            return std::make_shared<stream::gs::ECEF_Velocity>();
        case stream::gs::Voltage::TYPE:                  return std::make_shared<stream::gs::Voltage>();
        case stream::gs::Video::TYPE:                    return std::make_shared<stream::gs::Video>();
        case stream::gs::Multi_Commands::TYPE:              return std::make_shared<stream::gs::Multi_Commands>();
        case stream::gs::Multi_State::TYPE:              return std::make_shared<stream::gs::Multi_State>();
        case stream::gs::Proximity::TYPE:                return std::make_shared<stream::gs::Proximity>();
    }

    QASSERT(0);
    return std::shared_ptr<stream::gs::Stream>();
}

auto Comms::publish_outputs(node::gs::Node_ptr node) -> bool
{
    for (auto& os: node->outputs)
    {
        auto stream = create_stream_from_type(os.type);
        if (!stream)
        {
            QLOGE("\t\tCannot create output stream '{}/{}', type {}", node->name, os.name, os.type);
            return false;
        }
        os.stream = stream;

        stream->node = node;
        stream->name = node->name + "/" + os.name;
        stream->rate = os.rate;
        m_hal.m_streams.add(stream);
    }
    return true;
}

auto Comms::unpublish_outputs(node::gs::Node_ptr node) -> bool
{
    for (auto& os: node->outputs)
    {
        auto stream = m_hal.m_streams.find_by_name(node->name + "/" + os.name);
        m_hal.m_streams.remove(stream);
    }
    return true;
}

auto Comms::link_inputs(node::gs::Node_ptr node) -> bool
{
    for (auto& i: node->inputs)
    {
        if (i.stream_path.empty())
        {
            i.stream.reset();
            continue;
        }
        if (i.stream_path.size() != 2)
        {
            QLOGE("Wrong value for input stream '{}', node '{}'", i.stream_path, node->name);
            return false;
        }
        std::string stream_name = i.stream_path.get_as<std::string>();
        auto stream = m_hal.m_streams.find_by_name(stream_name);
        if (!stream)
        {
            QLOGE("Cannot find input stream '{}', node '{}'", i.stream_path, node->name);
            return false;
        }
        i.stream = stream;
    }
    return true;
}

void Comms::handle_enumerate_nodes()
{
    uint32_t node_count = 0;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(node_count))
    {
        QLOGE("Error in unpacking enumerate nodes message");
        return;
    }

    QLOGI("Received {} nodes", node_count);

    std::vector<silk::node::gs::Node_ptr> nodes;
    for (uint32_t i = 0; i < node_count; i++)
    {
        auto node = std::make_shared<node::gs::Node>();
        bool ok = m_setup_channel.unpack_param(node->name);
        ok &= unpack_node_data(m_setup_channel, *node);
        if (!ok)
        {
            QLOGE("\t\tBad node");
            return;
        }
        QLOGI("\tNode: {}", node->name);
        nodes.push_back(node);
    }

    //first register the streams, so that when I add the nodes they cna find their inputs
    for (auto& n: nodes)
    {
        if (!publish_outputs(n))
        {
            return;
        }
    }
    //now link the input. This will search in the previously added streams
    for (auto& n: nodes)
    {
        if (!link_inputs(n))
        {
            return;
        }
    }
    //now finally add the nodes
    //the order doesn't matter as nodes only depend on streams, not other nodes.
    for (auto& n: nodes)
    {
        m_hal.m_nodes.add(n);
    }

    //emit the changed signal so the UI can link nodes together
    for (auto& n: nodes)
    {
        n->changed_signal.execute();
    }

    m_setup_channel.end_unpack();
}


void Comms::handle_node_message()
{
    std::string name;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(name))
    {
        QLOGE("Failed to unpack node message");
        return;
    }

    auto node = m_hal.get_nodes().find_by_name(name);
    if (!node)
    {
        QLOGE("Cannot find node '{}'", name);
        return;
    }

//    QLOGI("Node '{}' - message received", name);

    rapidjson::Document json;
    if (!m_setup_channel.unpack_param(json))
    {
        QLOGE("Node '{}' - failed to unpack message", name);
    }

    node->message_received_signal.execute(json);
}

void Comms::handle_node_config()
{
    std::string name;
    rapidjson::Document json;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(name) ||
        !m_setup_channel.unpack_param(json))
    {
        QLOGE("Failed to unpack node config");
        return;
    }

    auto node = m_hal.get_nodes().find_by_name(name);
    if (!node)
    {
        QLOGE("Cannot find node '{}'", name);
        return;
    }
    node->config.SetObject();
    jsonutil::clone_value(node->config, json, node->config.GetAllocator());
    node->changed_signal.execute();
}


void Comms::handle_get_node_data()
{
    std::string name;
    bool ok = m_setup_channel.begin_unpack() &&
              m_setup_channel.unpack_param(name);
    if (!ok)
    {
        QLOGE("Failed to unpack node data");
        return;
    }

    auto node = m_hal.get_nodes().find_by_name(name);
    if (!node)
    {
        QLOGE("Cannot find node '{}'", name);
        return;
    }

    QLOGI("Node '{}' - config received", name);

    if (!unpack_node_data(m_setup_channel, *node))
    {
        QLOGE("Node '{}' - failed to unpack config", name);
    }
    link_inputs(node);

    node->changed_signal.execute();
}

void Comms::handle_node_input_stream_path()
{
    std::string name;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(name))
    {
        QLOGE("Failed to unpack node input stream path");
        return;
    }

    auto node = m_hal.get_nodes().find_by_name(name);
    if (!node)
    {
        QLOGE("Cannot find node '{}'", name);
        return;
    }

    QLOGI("Node '{}' - message received", name);

    if (!unpack_node_data(m_setup_channel, *node))
    {
        QLOGE("Node '{}' - failed to unpack config", name);
    }
    link_inputs(node);

    node->changed_signal.execute();
}

void Comms::handle_add_node()
{
    std::string name;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(name))
    {
        QLOGE("Failed to unpack add node request");
        return;
    }

    auto node = std::make_shared<node::gs::Node>();
    bool ok = unpack_node_data(m_setup_channel, *node);
    if (!ok)
    {
        return;
    }
    if (!link_inputs(node))
    {
        return;
    }

    node->name = name;

    publish_outputs(node);
    link_inputs(node);

    m_hal.m_nodes.add(node);
    node->changed_signal.execute();
}

void Comms::handle_remove_node()
{
    std::string name;
    bool ok = m_setup_channel.begin_unpack() &&
              m_setup_channel.unpack_param(name);
    if (!ok)
    {
        QLOGE("Failed to unpack remove node request");
        return;
    }

    auto node = m_hal.get_nodes().find_by_name(name);
    if (!node)
    {
        QLOGE("Cannot find node '{}'", name);
        return;
    }

    unpublish_outputs(node);
    m_hal.m_nodes.remove(node);

    request_all_node_configs();
}

void Comms::handle_streams_telemetry_active()
{
    bool is_active = false;
    bool ok = m_setup_channel.begin_unpack() &&
              m_setup_channel.unpack_param(is_active);
    if (!ok)
    {
        QLOGE("Failed to unpack remove node request");
        return;
    }
}

template<class Stream>
auto unpack_stream_samples(Comms::Telemetry_Channel& channel, uint32_t sample_count, silk::stream::gs::Stream& _stream) -> bool
{
    if (_stream.get_type() == Stream::TYPE)
    {
        auto& stream = static_cast<Stream&>(_stream);
        typename Stream::Sample sample;
        for (uint32_t i = 0; i < sample_count; i++)
        {
            if (!channel.unpack_param(sample))
            {
                QLOGE("Error unpacking samples!!!");
                return false;
            }
            stream.samples.push_back(sample);
        }
        stream.samples_available_signal.execute(stream.samples);
        stream.samples.clear();
        return true;
    }
    return false;
}

void Comms::handle_stream_data()
{
    std::string stream_name;
    uint32_t sample_count = 0;
    bool ok = m_telemetry_channel.begin_unpack() &&
              m_telemetry_channel.unpack_param(stream_name) &&
              m_telemetry_channel.unpack_param(sample_count);
    if (!ok)
    {
        QLOGE("Failed to unpack stream telemetry");
        return;
    }
    auto stream = m_hal.get_streams().find_by_name(stream_name);
    if (!stream)
    {
        QLOGW("Cannot find stream '{}'", stream_name);
        return;
    }

    using namespace silk::stream;

    if (!unpack_stream_samples<gs::Acceleration>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::ADC>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Angular_Velocity>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Battery_State>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Current>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Distance>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Float>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Bool>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Force>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Frame>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::GPS_Info>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Linear_Acceleration>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::ECEF_Position>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Magnetic_Field>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Pressure>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::PWM>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Temperature>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Throttle>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Torque>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Velocity>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Voltage>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Multi_Commands>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Multi_State>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Proximity>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<gs::Video>(m_telemetry_channel, sample_count, *stream))
    {
        QASSERT(0);
        QLOGE("Cannot unpack stream '{}'", stream_name);
        return;
    }
}

//void Comms::handle_frame_data()
//{
//    std::string stream_name;
//    stream::gs::Video::Sample sample;
//    bool ok = m_video_channel.begin_unpack() &&
//              m_video_channel.unpack_param(stream_name) &&
//              m_video_channel.unpack_param(sample);
//    if (!ok)
//    {
//        QLOGE("Failed to unpack video stream");
//        return;
//    }
//    auto _stream = m_hal.get_streams().find_by_name(stream_name);
//    if (!_stream || _stream->get_type() != stream::gs::Video::TYPE)
//    {
//        QLOGW("Cannot find stream '{}'", stream_name);
//        return;
//    }
//    auto& stream = static_cast<stream::gs::Video&>(*_stream);

//    stream.samples.push_back(std::move(sample));
//    stream.samples_available_signal.execute(stream.samples);
//    stream.samples.clear();
// }

void Comms::handle_multi_state()
{
    auto& channel = m_pilot_channel;

    stream::IMulti_State::Sample sample;
    if (!channel.unpack_all(sample))
    {
        QLOGE("Error in unpacking multi state");
        return;
    }

    m_multi_state_samples.push_back(sample);
}
void Comms::handle_video()
{
    auto& channel = m_video_channel;

    stream::IVideo::Sample sample;
    if (!channel.unpack_all(sample))
    {
        QLOGE("Error in unpacking video");
        return;
    }

    m_video_samples.push_back(sample);
}

auto Comms::get_video_samples() const -> std::vector<stream::IVideo::Sample> const&
{
    return m_video_samples;
}
auto Comms::get_multi_state_samples() const -> std::vector<stream::IMulti_State::Sample> const&
{
    return m_multi_state_samples;
}
void Comms::send_multi_commands_value(stream::IMulti_Commands::Value const& value)
{
    m_pilot_channel.pack_all(silk::comms::Pilot_Message::MULTI_COMMANDS, value);
    m_pilot_channel.try_sending(*m_rcp);
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

void Comms::process()
{
    if (!is_connected())
    {
        return;
    }

    m_multi_state_samples.clear();
    m_video_samples.clear();

    while (auto msg = m_pilot_channel.get_next_message(*m_rcp))
    {
        switch (msg.get())
        {
        case comms::Pilot_Message::MULTI_STATE : handle_multi_state(); break;
        default: break;
        }
    }
    while (auto msg = m_video_channel.get_next_message(*m_rcp))
    {
        switch (msg.get())
        {
        case comms::Video_Message::FRAME_DATA : handle_video(); break;
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
            case comms::Telemetry_Message::STREAM_DATA : handle_stream_data(); break;
            default: break;
            }
        }
    }

    while (auto msg = m_setup_channel.get_next_message(*m_rcp))
    {
        switch (msg.get())
        {
        case comms::Setup_Message::MULTI_CONFIG: handle_multi_config(); break;

        case comms::Setup_Message::CLOCK: handle_clock(); break;

        case comms::Setup_Message::ENUMERATE_NODE_DEFS: handle_enumerate_node_defs(); break;
        case comms::Setup_Message::ENUMERATE_NODES: handle_enumerate_nodes(); break;
        case comms::Setup_Message::GET_NODE_DATA: handle_get_node_data(); break;

        case comms::Setup_Message::ADD_NODE: handle_add_node(); break;
        case comms::Setup_Message::REMOVE_NODE: handle_remove_node(); break;
        case comms::Setup_Message::NODE_CONFIG: handle_node_config(); break;
        case comms::Setup_Message::NODE_MESSAGE: handle_node_message(); break;
        case comms::Setup_Message::NODE_INPUT_STREAM_PATH: handle_node_input_stream_path(); break;

        case comms::Setup_Message::STREAM_TELEMETRY_ACTIVE: handle_streams_telemetry_active(); break;

        default: break;
        }
    }
    //    QLOGI("*********** LOOP: {}", xxx);

    if (m_rcp->is_connected())
    {
        if (!m_did_request_data)
        {
            m_did_request_data = true;
            request_data();
        }
    }
    else
    {
        m_did_request_data = false;
    }

    m_setup_channel.send(*m_rcp);
    m_telemetry_channel.try_sending(*m_rcp);
}

