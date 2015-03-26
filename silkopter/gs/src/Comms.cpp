#include "Comms.h"



using namespace silk;
using namespace boost::asio;

constexpr uint8_t SETUP_CHANNEL = 10;
constexpr uint8_t INPUT_CHANNEL = 15;
constexpr uint8_t TELEMETRY_CHANNEL = 20;


Comms::Comms(boost::asio::io_service& io_service, HAL& hal)
    : m_hal(hal)
    , m_io_service(io_service)
    , m_socket(io_service)
    , m_rudp(m_socket)
    , m_setup_channel(m_rudp, SETUP_CHANNEL)
    , m_input_channel(m_rudp, INPUT_CHANNEL)
    , m_telemetry_channel(m_rudp, TELEMETRY_CHANNEL)
{
    {
        util::RUDP::Send_Params params;
        params.is_compressed = true;
        params.is_reliable = false;
        params.importance = 64;
        m_rudp.set_send_params(TELEMETRY_CHANNEL, params);
    }

    {
        util::RUDP::Send_Params params;
        params.mtu = 100;
        params.is_compressed = true;
        params.is_reliable = true;
        params.importance = 126;
        m_rudp.set_send_params(SETUP_CHANNEL, params);
    }
    {
        util::RUDP::Send_Params params;
        params.mtu = 100;
        params.is_compressed = true;
        params.is_reliable = false;
        params.cancel_on_new_data = true;
        params.importance = 127;
        m_rudp.set_send_params(INPUT_CHANNEL, params);
    }

    {
        util::RUDP::Receive_Params params;
        params.max_receive_time = std::chrono::seconds(999999);
        m_rudp.set_receive_params(SETUP_CHANNEL, params);
    }
    {
        util::RUDP::Receive_Params params;
        params.max_receive_time = std::chrono::seconds(200);
        m_rudp.set_receive_params(INPUT_CHANNEL, params);
    }

    {
        util::RUDP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(50);
        m_rudp.set_receive_params(TELEMETRY_CHANNEL, params);
    }

}

auto Comms::start(boost::asio::ip::address const& address, uint16_t send_port, uint16_t receive_port) -> bool
{
    try
    {
        m_socket.open(ip::udp::v4());
        m_socket.set_option(ip::udp::socket::reuse_address(true));
        m_socket.set_option(socket_base::send_buffer_size(65536));
        m_socket.bind(ip::udp::endpoint(ip::udp::v4(), receive_port));

        m_rudp.set_send_endpoint(ip::udp::endpoint(address, send_port));

        m_rudp.start();

        request_data();

        QLOGI("Started sending on port {} and receiving on port {}", send_port, receive_port);
    }
    catch(...)
    {
        m_socket.close();
        QLOGW("Connect failed");
        return false;
    }

    reset();

    return true;
}

void Comms::disconnect()
{
    reset();
    m_socket.close();
}

auto Comms::is_connected() const -> bool
{
    return m_socket.is_open();
}

auto Comms::get_remote_address() const -> boost::asio::ip::address
{
    return m_remote_endpoint.address();
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
    m_setup_channel.pack_all(comms::Setup_Message::CLOCK, ++m_last_req_id);
    m_setup_channel.pack_all(comms::Setup_Message::ENUMERATE_NODE_DEFS, ++m_last_req_id);
    m_setup_channel.pack_all(comms::Setup_Message::ENUMERATE_NODES, ++m_last_req_id);
}

void Comms::request_all_node_configs()
{
    auto nodes = m_hal.get_nodes().get_all();
    for (auto const& n: nodes)
    {
        m_setup_channel.pack_all(comms::Setup_Message::NODE_CONFIG, ++m_last_req_id, n->name);
    }
}


auto parse_json(std::string const& str) -> std::unique_ptr<rapidjson::Document>
{
    std::unique_ptr<rapidjson::Document> json(new rapidjson::Document);
    json->SetObject();
    if (!str.empty() && json->Parse(str.c_str()).HasParseError())
    {
        //            QLOGE("Failed to parse config: {}:{}", req_id, name, node->config.GetParseError(), node->config.GetErrorOffset());
        return nullptr;
    }
    return std::move(json);
}

static auto unpack_json(Comms::Setup_Channel& channel, rapidjson::Document& json) -> bool
{
    std::string str;
    if (!channel.unpack_param(str))
    {
        return false;
    }
    json.SetObject();
    if (!str.empty() && json.Parse(str.c_str()).HasParseError())
    {
        QLOGE("Failed to parse json: {}:{}", json.GetParseError(), json.GetErrorOffset());
        return false;
    }
    return true;
}
static void pack_json(Comms::Setup_Channel& channel, rapidjson::Document const& json)
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json.Accept(writer);
    std::string str(buffer.GetString(), buffer.GetSize());
    channel.pack_param(str);
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

static auto unpack_node_def_data(Comms::Setup_Channel& channel, node::Node_Def& node_def) -> bool
{
    bool ok = channel.unpack_param(node_def.name);
    ok &= channel.unpack_param(node_def.type);
    ok &= unpack_inputs(channel, node_def.inputs);
    ok &= unpack_outputs(channel, node_def.outputs);
    ok &= unpack_json(channel, node_def.default_init_params);
    ok &= unpack_json(channel, node_def.default_config);
    return ok;
}

static auto unpack_node_data(Comms::Setup_Channel& channel, node::Node& node) -> bool
{
    bool ok = channel.unpack_param(node.type);
    ok &= unpack_inputs(channel, node.inputs);
    ok &= unpack_outputs(channel, node.outputs);
    ok &= unpack_json(channel, node.init_params);
    ok &= unpack_json(channel, node.config);
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
//        i.stream = hal.get_streams().find_by_name<node::stream::Stream>(name);
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
    uint32_t req_id = 0;
    if (m_setup_channel.begin_unpack() &&
        m_setup_channel.unpack_param(req_id) &&
        m_setup_channel.unpack_param(us))
    {
        m_setup_channel.end_unpack();

        m_hal.m_remote_clock.set_epoch(Manual_Clock::time_point(std::chrono::microseconds(us)));
    }
    else
    {
        QLOGE("Error in enumerating node defs");
    }
}


void Comms::handle_enumerate_node_defs()
{
    uint32_t req_id = 0;
    if (m_setup_channel.begin_unpack() &&
            m_setup_channel.unpack_param(req_id))
    {
        uint32_t node_count = 0;

        if (!m_setup_channel.unpack_param(node_count))
        {
            QLOGE("Error in unpacking enumerate node defs message");
            return;
        }

        QLOGI("Req Id: {}, Received defs for {} nodes", req_id, node_count);

        for (uint32_t i = 0; i < node_count; i++)
        {
            auto def = std::make_shared<node::Node_Def>();
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
    else
    {
        QLOGE("Error in enumerating node defs");
    }

    m_hal.node_defs_refreshed_signal.execute();
}

auto create_stream_from_type(node::stream::Type type) -> std::shared_ptr<node::stream::Stream>
{
    switch (type)
    {
        case node::stream::IAcceleration::TYPE:             return std::make_shared<node::stream::Acceleration>();
//        case node::stream::IENU_Acceleration::TYPE:         return std::make_shared<node::stream::ENU_Acceleration>();
//        case node::stream::IECEF_Acceleration::TYPE:        return std::make_shared<node::stream::ECEF_Acceleration>();
        case node::stream::IADC::TYPE:                      return std::make_shared<node::stream::ADC>();
        case node::stream::IAngular_Velocity::TYPE:         return std::make_shared<node::stream::Angular_Velocity>();
//        case node::stream::IENU_Angular_Velocity::TYPE:     return std::make_shared<node::stream::ENU_Angular_Velocity>();
//        case node::stream::IECEF_Angular_Velocity::TYPE:    return std::make_shared<node::stream::ECEF_Angular_Velocity>();
        case node::stream::IBattery_State::TYPE:            return std::make_shared<node::stream::Battery_State>();
        case node::stream::ICommands::TYPE:                 return std::make_shared<node::stream::Commands>();
        case node::stream::ICurrent::TYPE:                  return std::make_shared<node::stream::Current>();
        case node::stream::IDistance::TYPE:                 return std::make_shared<node::stream::Distance>();
//        case node::stream::IENU_Distance::TYPE:             return std::make_shared<node::stream::ENU_Distance>();
//        case node::stream::IECEF_Distance::TYPE:            return std::make_shared<node::stream::ECEF_Distance>();
        case node::stream::IFactor::TYPE:                   return std::make_shared<node::stream::Factor>();
        case node::stream::IForce::TYPE:                    return std::make_shared<node::stream::Force>();
//        case node::stream::IENU_Force::TYPE:                return std::make_shared<node::stream::ENU_Force>();
//        case node::stream::IECEF_Force::TYPE:               return std::make_shared<node::stream::ECEF_Force>();
        case node::stream::IFrame::TYPE:                    return std::make_shared<node::stream::Frame>();
        case node::stream::ILinear_Acceleration::TYPE:      return std::make_shared<node::stream::Linear_Acceleration>();
//        case node::stream::IENU_Linear_Acceleration::TYPE:  return std::make_shared<node::stream::ENU_Linear_Acceleration>();
//        case node::stream::IECEF_Linear_Acceleration::TYPE: return std::make_shared<node::stream::ECEF_Linear_Acceleration>();
        case node::stream::IECEF_Location::TYPE:            return std::make_shared<node::stream::ECEF_Location>();
        case node::stream::IMagnetic_Field::TYPE:           return std::make_shared<node::stream::ECEF_Magnetic_Field>();
//        case node::stream::IENU_Magnetic_Field::TYPE:       return std::make_shared<node::stream::ENU_Magnetic_Field>();
//        case node::stream::IECEF_Magnetic_Field::TYPE:      return std::make_shared<node::stream::ECEF_Magnetic_Field>();
        case node::stream::IPressure::TYPE:                 return std::make_shared<node::stream::Pressure>();
        case node::stream::IPWM::TYPE:                      return std::make_shared<node::stream::PWM>();
        case node::stream::ITemperature::TYPE:              return std::make_shared<node::stream::Temperature>();
        case node::stream::IThrottle::TYPE:                 return std::make_shared<node::stream::Throttle>();
        case node::stream::ITorque::TYPE:                   return std::make_shared<node::stream::Torque>();
//        case node::stream::IENU_Torque::TYPE:               return std::make_shared<node::stream::ENU_Torque>();
//        case node::stream::IECEF_Torque::TYPE:              return std::make_shared<node::stream::ECEF_Torque>();
        case node::stream::IVelocity::TYPE:                 return std::make_shared<node::stream::Velocity>();
//        case node::stream::IENU_Velocity::TYPE:             return std::make_shared<node::stream::ENU_Velocity>();
//        case node::stream::IECEF_Velocity::TYPE:            return std::make_shared<node::stream::ECEF_Velocity>();
        case node::stream::IVoltage::TYPE:                  return std::make_shared<node::stream::Voltage>();
        case node::stream::IVideo::TYPE:                    return std::make_shared<node::stream::Video>();
    }

    QASSERT(0);
    return std::shared_ptr<node::stream::Stream>();
}

auto Comms::publish_output_streams(node::Node_ptr node) -> bool
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
        stream->type = os.type;
        stream->rate = os.rate;
        m_hal.m_streams.add(stream);
    }
    return true;
}

auto Comms::link_input_streams(node::Node_ptr node) -> bool
{
    for (auto& i: node->inputs)
    {
        //find the connection in the config
        q::Path input_path("Inputs/" + i.name);
        auto* input_streamj = jsonutil::find_value(node->config, input_path);
        if (!input_streamj || !input_streamj->IsString())
        {
            QLOGE("Cannot find config for input stream '{}'", input_path);
            return false;
        }
        q::Path stream_path(input_streamj->GetString());
        if (stream_path.empty())
        {
            i.stream.reset();
            continue;
        }
        if (stream_path.size() != 2)
        {
            QLOGE("Wrong value for input stream '{}' - {}", input_path, stream_path);
            return false;
        }
        std::string stream_name = stream_path.get_as<std::string>();
        auto stream = m_hal.m_streams.find_by_name(stream_name);
        if (!stream)
        {
            QLOGE("Cannot find input stream '{}' - {}", input_path, stream_path);
            return false;
        }
        i.stream = stream;
    }
    return true;
}

void Comms::handle_enumerate_nodes()
{
    uint32_t req_id = 0;
    if (m_setup_channel.begin_unpack() &&
            m_setup_channel.unpack_param(req_id))
    {
        uint32_t node_count = 0;
        if (!m_setup_channel.unpack_param(node_count))
        {
            QLOGE("Error in unpacking enumerate nodes message");
            return;
        }

        QLOGI("Req Id: {}, Received {} nodes", req_id, node_count);

        for (uint32_t i = 0; i < node_count; i++)
        {
            auto node = std::make_shared<node::Node>();
            bool ok = m_setup_channel.unpack_param(node->name);
            ok &= unpack_node_data(m_setup_channel, *node);
            if (!ok)
            {
                QLOGE("\t\tBad node");
                return;
            }
            QLOGI("\tNode: {}", node->name);
            m_hal.m_nodes.add(node);
        }

        auto nodes = m_hal.get_nodes().get_all();
        for (auto& n: nodes)
        {
            if (!publish_output_streams(n))
            {
                return;
            }
        }
        for (auto& n: nodes)
        {
            if (!link_input_streams(n))
            {
                return;
            }
        }

        m_setup_channel.end_unpack();
    }
    else
    {
        QLOGE("Error in enumerating nodes");
    }

    m_hal.nodes_refreshed_signal.execute();
}


void Comms::handle_node_message()
{
    uint32_t req_id = 0;
    std::string name;
    bool ok = m_setup_channel.begin_unpack() &&
              m_setup_channel.unpack_param(req_id) &&
              m_setup_channel.unpack_param(name);
    if (!ok)
    {
        QLOGE("Failed to unpack node message");
        return;
    }

    auto it = std::find_if(begin(m_hal.m_send_node_message_queue), end(m_hal.m_send_node_message_queue), [&](HAL::Send_Node_Message_Queue_Item const& i) { return i.req_id == req_id; });
    QASSERT(it != m_hal.m_send_node_message_queue.end());
    if (it == m_hal.m_send_node_message_queue.end())
    {
        QLOGE("Cannot find node message request {}", req_id);
        return;
    }

    auto node = m_hal.get_nodes().find_by_name(name);
    if (!node)
    {
        it->callback(HAL::Result::FAILED, rapidjson::Document());
        QLOGE("Req Id: {}, cannot find node '{}'", req_id, name);
        return;
    }

    QLOGI("Req Id: {}, node '{}' - message received", req_id, name);

    rapidjson::Document json;
    if (!unpack_json(m_setup_channel, json))
    {
        it->callback(HAL::Result::FAILED, rapidjson::Document());
        QLOGE("Req Id: {}, node '{}' - failed to unpack message", req_id, name);
    }

    it->callback(HAL::Result::OK, std::move(json));
}

void Comms::handle_node_data()
{
    uint32_t req_id = 0;
    std::string name;
    bool ok = m_setup_channel.begin_unpack() &&
              m_setup_channel.unpack_param(req_id) &&
              m_setup_channel.unpack_param(name);
    if (!ok)
    {
        QLOGE("Failed to unpack node data");
        return;
    }

    auto node = m_hal.get_nodes().find_by_name(name);
    if (!node)
    {
        QLOGE("Req Id: {}, cannot find node '{}'", req_id, name);
        return;
    }

    QLOGI("Req Id: {}, node '{}' - config received", req_id, name);

    if (!unpack_node_data(m_setup_channel, *node))
    {
        QLOGE("Req Id: {}, node '{}' - failed to unpack config", req_id, name);
    }
    link_input_streams(node);

    node->changed_signal.execute(*node);
}

void Comms::handle_add_node()
{
    uint32_t req_id = 0;
    if (!m_setup_channel.begin_unpack() || !m_setup_channel.unpack_param(req_id))
    {
        QLOGE("Failed to unpack add node request");
        return;
    }
    auto it = std::find_if(begin(m_hal.m_add_queue), end(m_hal.m_add_queue), [&](HAL::Add_Queue_Item const& i) { return i.req_id == req_id; });
    QASSERT(it != m_hal.m_add_queue.end());
    if (it == m_hal.m_add_queue.end())
    {
        QLOGE("Cannot find add node request {}", req_id);
        return;
    }
    auto node = std::make_shared<node::Node>();
    bool ok = unpack_node_data(m_setup_channel, *node);
    if (!ok)
    {
        it->callback(HAL::Result::FAILED, node::Node_ptr());
        return;
    }
    if (!link_input_streams(node))
    {
        it->callback(HAL::Result::FAILED, node::Node_ptr());
        return;
    }

    node->name = it->name;
    m_hal.m_nodes.add(node);

    publish_output_streams(node);
    link_input_streams(node);

    it->callback(HAL::Result::OK, node);
}

void Comms::handle_remove_node()
{
    uint32_t req_id = 0;
    std::string name;
    bool ok = m_setup_channel.begin_unpack() &&
              m_setup_channel.unpack_param(req_id) &&
              m_setup_channel.unpack_param(name);
    if (!ok)
    {
        QLOGE("Failed to unpack remove node request");
        return;
    }
    auto it = std::find_if(begin(m_hal.m_remove_queue), end(m_hal.m_remove_queue), [&](HAL::Remove_Queue_Item const& i) { return i.req_id == req_id; });
    QASSERT(it != m_hal.m_remove_queue.end());
    if (it == m_hal.m_remove_queue.end())
    {
        QLOGE("Cannot find remove node request {}", req_id);
        return;
    }
    auto node = m_hal.get_nodes().find_by_name(name);
    if (!node)
    {
        it->callback(HAL::Result::FAILED);
        QLOGE("Req Id: {}, cannot find node '{}'", req_id, name);
        return;
    }

    QLOGI("Req Id: {}, node '{}' - config received", req_id, name);

    if (!unpack_node_data(m_setup_channel, *node))
    {
        it->callback(HAL::Result::FAILED);
        QLOGE("Req Id: {}, node '{}' - failed to unpack config", req_id, name);
        return;
    }
    m_hal.m_nodes.remove(node);
    it->callback(HAL::Result::OK);

    request_all_node_configs();
}

void Comms::handle_streams_telemetry_active()
{
    bool is_active = false;
    uint32_t req_id = 0;
    bool ok = m_setup_channel.begin_unpack() &&
              m_setup_channel.unpack_param(req_id) &&
              m_setup_channel.unpack_param(is_active);
    if (!ok)
    {
        QLOGE("Failed to unpack remove node request");
        return;
    }
    auto it = std::find_if(begin(m_hal.m_stream_telemetry_queue), end(m_hal.m_stream_telemetry_queue),
                           [&](HAL::Stream_Telemetry_Queue_Item const& i) { return i.req_id == req_id; });
    QASSERT(it != m_hal.m_stream_telemetry_queue.end());
    if (it == m_hal.m_stream_telemetry_queue.end())
    {
        QLOGE("Cannot find telemetry node request {}", req_id);
        return;
    }
    it->callback(is_active ? HAL::Result::OK : HAL::Result::FAILED);
    m_hal.m_stream_telemetry_queue.erase(it);
}

#pragma pack(push, 1)
struct Sample_Data
{
    uint64_t dt : 24; //10us
    uint64_t tp : 40; //1us
    uint16_t sample_idx : 15;
    uint16_t is_healthy : 1;
};
#pragma pack(pop)

template<class IStream, class Stream>
auto unpack_stream_samples(Comms::Telemetry_Channel& channel, uint32_t sample_count, silk::node::stream::Stream& _stream) -> bool
{
    if (_stream.type == IStream::TYPE)
    {
        auto& stream = static_cast<Stream&>(_stream);
        typename Stream::Sample sample;
        Sample_Data data;
        for (uint32_t i = 0; i < sample_count; i++)
        {
            uint32_t dt = 0;
            bool ok = channel.unpack_param(sample.value);
            ok &= channel.unpack_param(data);
            if (!ok)
            {
                QLOGE("Error unpacking samples!!!");
                return false;
            }

            sample.sample_idx = data.sample_idx;
            sample.dt = std::chrono::microseconds(data.dt << 3);
            sample.tp = Manual_Clock::time_point(std::chrono::microseconds(data.tp));
            sample.is_healthy = data.is_healthy ? true : false;
            stream.samples.push_back(sample);
        }
        stream.samples_available_signal.execute(stream);
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
        QLOGE("Cannot find stream '{}'", stream_name);
        return;
    }

    using namespace silk::node::stream;

    if (!unpack_stream_samples<IAcceleration, Acceleration>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IADC, ADC>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IAngular_Velocity, Angular_Velocity>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IBattery_State, Battery_State>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<ICommands, Commands>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<ICurrent, Current>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IDistance, Distance>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IFactor, Factor>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IForce, Force>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IFrame, Frame>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<ILinear_Acceleration, Linear_Acceleration>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IECEF_Location, ECEF_Location>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IMagnetic_Field, Magnetic_Field>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IPressure, Pressure>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IPWM, PWM>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<ITemperature, Temperature>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IThrottle, Throttle>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<ITorque, Torque>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IVelocity, Velocity>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IVoltage, Voltage>(m_telemetry_channel, sample_count, *stream) &&
        !unpack_stream_samples<IVideo, Video>(m_telemetry_channel, sample_count, *stream))
    {
        QASSERT(0);
        QLOGE("Cannot unpack stream '{}'", stream_name);
        return;
    }
}

void Comms::send_hal_requests()
{
#ifdef NDEBUG
    static const std::chrono::seconds REQUEST_TIMEOUT(3);
#else
    static const std::chrono::seconds REQUEST_TIMEOUT(300);
#endif

    auto now = q::Clock::now();

    for (auto it = m_hal.m_add_queue.begin(); it != m_hal.m_add_queue.end();)
    {
        auto& req = *it;
        if (!req.was_sent)
        {
            req.was_sent = true;
            req.sent_time_point = now;
            req.req_id = ++m_last_req_id;
            m_setup_channel.begin_pack(comms::Setup_Message::ADD_NODE);
            m_setup_channel.pack_param(req.req_id);
            m_setup_channel.pack_param(req.def_name);
            m_setup_channel.pack_param(req.name);
            pack_json(m_setup_channel, req.init_params);
            m_setup_channel.end_pack();
        }
        if (now - req.sent_time_point > REQUEST_TIMEOUT)
        {
            req.callback(HAL::Result::TIMEOUT, node::Node_ptr());
            m_hal.m_add_queue.erase(it);
        }
        else
        {
            ++it;
        }
    }


    for (auto it = m_hal.m_set_config_queue.begin(); it != m_hal.m_set_config_queue.end();)
    {
        auto& req = *it;
        if (!req.was_sent)
        {
            req.was_sent = true;
            req.sent_time_point = now;
            req.req_id = ++m_last_req_id;
            m_setup_channel.begin_pack(req.message);
            m_setup_channel.pack_param(req.req_id);
            m_setup_channel.pack_param(req.name);
            pack_json(m_setup_channel, req.config);
            m_setup_channel.end_pack();
        }
        if (now - req.sent_time_point > REQUEST_TIMEOUT)
        {
            m_hal.m_set_config_queue.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = m_hal.m_stream_telemetry_queue.begin(); it != m_hal.m_stream_telemetry_queue.end();)
    {
        auto& req = *it;
        if (!req.was_sent)
        {
            req.was_sent = true;
            req.sent_time_point = now;
            req.req_id = ++m_last_req_id;
            m_setup_channel.begin_pack(comms::Setup_Message::STREAM_TELEMETRY_ACTIVE);
            m_setup_channel.pack_param(req.req_id);
            m_setup_channel.pack_param(req.stream_name);
            m_setup_channel.pack_param(req.is_active);
            m_setup_channel.end_pack();
        }
        if (now - req.sent_time_point > REQUEST_TIMEOUT)
        {
            req.callback(HAL::Result::TIMEOUT);
            m_hal.m_stream_telemetry_queue.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto it = m_hal.m_send_node_message_queue.begin(); it != m_hal.m_send_node_message_queue.end();)
    {
        auto& req = *it;
        if (!req.was_sent)
        {
            req.was_sent = true;
            req.sent_time_point = now;
            req.req_id = ++m_last_req_id;
            m_setup_channel.begin_pack(req.message);
            m_setup_channel.pack_param(req.req_id);
            m_setup_channel.pack_param(req.name);
            pack_json(m_setup_channel, req.json);
            m_setup_channel.end_pack();
        }
        if (now - req.sent_time_point > REQUEST_TIMEOUT)
        {
            req.callback(HAL::Result::TIMEOUT, rapidjson::Document());
            m_hal.m_send_node_message_queue.erase(it);
        }
        else
        {
            ++it;
        }
    }
}


void Comms::process()
{
    if (!is_connected())
    {
        return;
    }

    send_hal_requests();

    while (auto msg = m_telemetry_channel.get_next_message())
    {
        switch (msg.get())
        {
            case comms::Telemetry_Message::STREAM_DATA : handle_stream_data(); break;
        }
    }

    while (auto msg = m_setup_channel.get_next_message())
    {
        switch (msg.get())
        {
            case comms::Setup_Message::CLOCK: handle_clock(); break;

            case comms::Setup_Message::ENUMERATE_NODE_DEFS: handle_enumerate_node_defs(); break;
            case comms::Setup_Message::ENUMERATE_NODES: handle_enumerate_nodes(); break;

            case comms::Setup_Message::NODE_MESSAGE: handle_node_message(); break;
            case comms::Setup_Message::NODE_DATA: handle_node_data(); break;
            case comms::Setup_Message::ADD_NODE: handle_add_node(); break;
            case comms::Setup_Message::REMOVE_NODE: handle_remove_node(); break;

            case comms::Setup_Message::STREAM_TELEMETRY_ACTIVE: handle_streams_telemetry_active(); break;
        }
    }
    //    QLOGI("*********** LOOP: {}", xxx);

    m_rudp.process();
    m_setup_channel.send();
    m_telemetry_channel.try_sending();
    m_input_channel.try_sending();
}

auto Comms::get_rudp() -> util::RUDP&
{
    return m_rudp;
}
