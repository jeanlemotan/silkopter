#include "stdafx.h"
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

        request_nodes();

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

auto Comms::get_remote_clock() const -> Manual_Clock const&
{
    return m_remote_clock;
}

void Comms::reset()
{
    m_hal.m_sinks.remove_all();
    m_hal.m_sources.remove_all();
    m_hal.m_processors.remove_all();
    m_hal.m_streams.remove_all();
}

void Comms::request_nodes()
{
    m_setup_channel.pack_all(comms::Setup_Message::ENUMERATE_NODE_DEFS, ++m_last_req_id);
    m_setup_channel.pack_all(comms::Setup_Message::ENUMERATE_NODES, ++m_last_req_id);
}


//void Comms::send_telemetry_streams()
//{
//    for (auto const& stream: m_telemetry_streams)
//    {
//        QASSERT(stream);
//        if (send_telemetry_stream<node::stream::IAcceleration>(*stream) ||
//            send_telemetry_stream<node::stream::IAngular_Velocity>(*stream) ||
//            send_telemetry_stream<node::stream::IMagnetic_Field>(*stream) ||
//            send_telemetry_stream<node::stream::IPressure>(*stream) ||
//            send_telemetry_stream<node::stream::IBattery_State>(*stream) ||
//            send_telemetry_stream<node::stream::ILinear_Acceleration>(*stream) ||
//            send_telemetry_stream<node::stream::ICardinal_Points>(*stream) ||
//            send_telemetry_stream<node::stream::ICurrent>(*stream) ||
//            send_telemetry_stream<node::stream::IVoltage>(*stream) ||
//            send_telemetry_stream<node::stream::IDistance>(*stream) ||
//            send_telemetry_stream<node::stream::ILocation>(*stream) ||
//            send_telemetry_stream<node::stream::IPWM_Value>(*stream) ||
//            send_telemetry_stream<node::stream::IReference_Frame>(*stream) ||
//            send_telemetry_stream<node::stream::ITemperature>(*stream) ||
//            send_telemetry_stream<node::stream::IADC_Value>(*stream)
////          send_telemetry_stream<node::stream::IVideo>(*stream)
//                )
//        {
//            ;//nothing
//        }
//        else
//        {
//            QLOGW("Unrecognized stream type: {} / {}", stream->get_name(), q::rtti::get_class_name(*stream));
//        }
//    }
//}


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
            !channel.unpack_param(i.class_id) ||
            !channel.unpack_param(i.stream))
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
    for (auto& i: io)
    {
        if (!channel.unpack_param(i.name) ||
            !channel.unpack_param(i.class_id) ||
            !channel.unpack_param(i.rate))
        {
            return false;
        }
    }
    return true;
}

static auto unpack_source_data(Comms::Setup_Channel& channel, node::source::Source& node) -> bool
{
    bool ok = channel.unpack_param(node.class_id);
    ok &= unpack_outputs(channel, node.outputs);
    ok &= unpack_json(channel, node.init_params);
    ok &= unpack_json(channel, node.config);
    return ok;
}
static auto unpack_sink_data(Comms::Setup_Channel& channel, node::sink::Sink& node) -> bool
{
    bool ok = channel.unpack_param(node.class_id);
    ok &= unpack_inputs(channel, node.inputs);
    ok &= unpack_json(channel, node.init_params);
    ok &= unpack_json(channel, node.config);
    return ok;
}
static auto unpack_processor_data(Comms::Setup_Channel& channel, node::processor::Processor& node) -> bool
{
    bool ok = channel.unpack_param(node.class_id);
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
//        i.stream = hal.get_streams().find_by_name<node::stream::GS_IStream>(name);
//        if (!i.stream)
//        {
//            QLOGE("Cannot find stream name {}", i.name);
//            return false;
//        }
//    }
//    return true;
//}

static void pack_json(Comms::Setup_Channel& channel, rapidjson::Document const& json)
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json.Accept(writer);
    std::string str(buffer.GetString(), buffer.GetSize());
    channel.pack_param(str);
}


void Comms::handle_enumerate_node_defs()
{
    uint32_t req_id = 0;
    if (m_setup_channel.begin_unpack() &&
        m_setup_channel.unpack_param(req_id))
    {
        uint32_t source_count = 0;
        uint32_t sink_count = 0;
        uint32_t processor_count = 0;

        bool ok = m_setup_channel.unpack_param(source_count);
        ok &= m_setup_channel.unpack_param(sink_count);
        ok &= m_setup_channel.unpack_param(processor_count);
        if (!ok)
        {
            QLOGE("Error in unpacking enumerate node defs message");
            return;
        }

        QLOGI("Req Id: {}, Received defs for {} sources, {} sinks and {} processors", req_id, source_count, sink_count, processor_count);

        for (uint32_t i = 0; i < source_count; i++)
        {
            auto def = std::make_shared<node::source::Source_Def>();
            bool ok = m_setup_channel.unpack_param(def->name);
            ok &= m_setup_channel.unpack_param(def->class_id);
            ok &= m_setup_channel.unpack_param(def->default_init_params);
            ok &= m_setup_channel.unpack_param(def->default_config);
            if (!ok)
            {
                QLOGE("\t\tBad source");
                return;
            }
            QLOGI("\tSource: {}", def->name);
            m_hal.m_source_defs.add(std::move(def));
        }

        for (uint32_t i = 0; i < sink_count; i++)
        {
            auto def = std::make_shared<node::sink::Sink_Def>();
            bool ok = m_setup_channel.unpack_param(def->name);
            ok &= m_setup_channel.unpack_param(def->class_id);
            ok &= m_setup_channel.unpack_param(def->default_init_params);
            ok &= m_setup_channel.unpack_param(def->default_config);
            if (!ok)
            {
                QLOGE("\t\tBad sink");
                return;
            }
            QLOGI("\tSink: {}", def->name);
            m_hal.m_sink_defs.add(std::move(def));
        }

        for (uint32_t i = 0; i < processor_count; i++)
        {
            auto def = std::make_shared<node::processor::Processor_Def>();
            bool ok = m_setup_channel.unpack_param(def->name);
            ok &= m_setup_channel.unpack_param(def->class_id);
            ok &= m_setup_channel.unpack_param(def->default_init_params);
            ok &= m_setup_channel.unpack_param(def->default_config);
            if (!ok)
            {
                QLOGE("\t\tBad processor");
                return;
            }
            QLOGI("\tProcessor: {}", def->name);
            m_hal.m_processor_defs.add(std::move(def));
        }

        m_setup_channel.end_unpack();
    }
    else
    {
        QLOGE("Error in enumerating node defs");
    }

    m_hal.node_defs_refreshed_signal.execute();
}

//auto create_stream_from_class_id(q::rtti::class_id class_id) -> std::shared_ptr<node::stream::GS_IStream>
//{
//    if (class_id == q::rtti::get_class_id<node::stream::IAcceleration>())
//    {
//        return std::make_shared<node::stream::Acceleration>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::IAngular_Velocity>())
//    {
//        return std::make_shared<node::stream::Angular_Velocity>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::IMagnetic_Field>())
//    {
//        return std::make_shared<node::stream::Magnetic_Field>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::IPressure>())
//    {
//        return std::make_shared<node::stream::Pressure>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::IBattery_State>())
//    {
//        return std::make_shared<node::stream::Battery_State>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::ILinear_Acceleration>())
//    {
//        return std::make_shared<node::stream::Linear_Acceleration>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::ICardinal_Points>())
//    {
//        return std::make_shared<node::stream::Cardinal_Points>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::ICurrent>())
//    {
//        return std::make_shared<node::stream::Current>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::IVoltage>())
//    {
//        return std::make_shared<node::stream::Voltage>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::IDistance>())
//    {
//        return std::make_shared<node::stream::Distance>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::ILocation>())
//    {
//        return std::make_shared<node::stream::Location>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::IPWM_Value>())
//    {
//        return std::make_shared<node::stream::PWM_Value>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::IReference_Frame>())
//    {
//        return std::make_shared<node::stream::Reference_Frame>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::ITemperature>())
//    {
//        return std::make_shared<node::stream::Temperature>();
//    }
//    if (class_id == q::rtti::get_class_id<node::stream::IADC_Value>())
//    {
//        return std::make_shared<node::stream::ADC_Value>();
//    }
////  create_stream_from_rtti<node::stream::Video>(type);

//    return std::shared_ptr<node::stream::GS_IStream>();
//}

void Comms::handle_enumerate_nodes()
{
    uint32_t req_id = 0;
    if (m_setup_channel.begin_unpack() &&
        m_setup_channel.unpack_param(req_id))
    {
        uint32_t source_count = 0;
        uint32_t sink_count = 0;
        uint32_t processor_count = 0;
        std::string def_name;
        std::string init_params;
        std::string config;

        bool ok = m_setup_channel.unpack_param(source_count);
        ok &= m_setup_channel.unpack_param(sink_count);
        ok &= m_setup_channel.unpack_param(processor_count);
        if (!ok)
        {
            QLOGE("Error in unpacking enumerate nodes message");
            return;
        }

        QLOGI("Req Id: {}, Received {} sources, {} sinks, {} processors", req_id, source_count, sink_count, processor_count);

        for (uint32_t i = 0; i < source_count; i++)
        {
            auto node = std::make_shared<node::source::Source>();
            bool ok = m_setup_channel.unpack_param(node->name);
            ok &= unpack_source_data(m_setup_channel, *node);
            if (!ok)
            {
                QLOGE("\t\tBad source");
                return;
            }
            QLOGI("\tSource: {}", node->name);
            m_hal.m_sources.add(node);
        }

        for (uint32_t i = 0; i < sink_count; i++)
        {
            auto node = std::make_shared<node::sink::Sink>();
            bool ok = m_setup_channel.unpack_param(node->name);
            ok &= unpack_sink_data(m_setup_channel, *node);
            if (!ok)
            {
                QLOGE("\t\tBad sink");
                return;
            }
            QLOGI("\tSink: {}", node->name);
            m_hal.m_sinks.add(node);
        }

        for (uint32_t i = 0; i < processor_count; i++)
        {
            auto node = std::make_shared<node::processor::Processor>();
            bool ok = m_setup_channel.unpack_param(node->name);
            ok &= unpack_processor_data(m_setup_channel, *node);
            if (!ok)
            {
                QLOGE("\t\tBad processor");
                return;
            }
            QLOGI("\tProcessor: {}", node->name);
            m_hal.m_processors.add(node);
        }


        m_setup_channel.end_unpack();
    }
    else
    {
        QLOGE("Error in enumerating nodes");
    }

    m_hal.nodes_refreshed_signal.execute();
}


void Comms::handle_source_config()
{
    uint32_t req_id = 0;
    std::string name;
    std::string config_str;
    bool ok = m_setup_channel.begin_unpack() &&
                m_setup_channel.unpack_param(req_id) &&
                m_setup_channel.unpack_param(name) &&
                m_setup_channel.unpack_param(config_str);
    if (!ok)
    {
        QLOGE("Failed to unpack node config");
        return;
    }

    auto node = m_hal.get_sources().find_by_name(name);
    if (!node)
    {
        QLOGE("Req Id: {}, cannot find node '{}'", req_id, name);
        return;
    }

    QLOGI("Req Id: {}, node '{}' - config received", req_id, name);

    node->config.SetObject();
    if (!config_str.empty() &&
            node->config.Parse(config_str.c_str()).HasParseError())
    {
        QLOGE("Req Id: {}, node '{}' - failed to parse config: {}:{}", req_id, name, node->config.GetParseError(), node->config.GetErrorOffset());
        return;
    }
}

void Comms::handle_sink_config()
{
    uint32_t req_id = 0;
    std::string name;
    std::string config_str;
    bool ok = m_setup_channel.begin_unpack() &&
                m_setup_channel.unpack_param(req_id) &&
                m_setup_channel.unpack_param(name) &&
                m_setup_channel.unpack_param(config_str);
    if (!ok)
    {
        QLOGE("Failed to unpack node config");
        return;
    }

    auto node = m_hal.get_sinks().find_by_name(name);
    if (!node)
    {
        QLOGE("Req Id: {}, cannot find node '{}'", req_id, name);
        return;
    }

    QLOGI("Req Id: {}, node '{}' - config received", req_id, name);

    node->config.SetObject();
    if (!config_str.empty() &&
            node->config.Parse(config_str.c_str()).HasParseError())
    {
        QLOGE("Req Id: {}, node '{}' - failed to parse config: {}:{}", req_id, name, node->config.GetParseError(), node->config.GetErrorOffset());
        return;
    }
}

void Comms::handle_processor_config()
{
    uint32_t req_id = 0;
    std::string name;
    std::string config_str;
    bool ok = m_setup_channel.begin_unpack() &&
                m_setup_channel.unpack_param(req_id) &&
                m_setup_channel.unpack_param(name) &&
                m_setup_channel.unpack_param(config_str);
    if (!ok)
    {
        QLOGE("Failed to unpack node config");
        return;
    }

    auto node = m_hal.get_processors().find_by_name(name);
    if (!node)
    {
        QLOGE("Req Id: {}, cannot find node '{}'", req_id, name);
        return;
    }

    QLOGI("Req Id: {}, node '{}' - config received", req_id, name);

    node->config.SetObject();
    if (!config_str.empty() &&
            node->config.Parse(config_str.c_str()).HasParseError())
    {
        QLOGE("Req Id: {}, node '{}' - failed to parse config: {}:{}", req_id, name, node->config.GetParseError(), node->config.GetErrorOffset());
        return;
    }
}

//void Comms::handle_stream_config()
//{
//    uint32_t req_id = 0;
//    std::string name;
//    std::string config_str;
//    bool ok = m_setup_channel.begin_unpack() &&
//                m_setup_channel.unpack_param(req_id) &&
//                m_setup_channel.unpack_param(name) &&
//                m_setup_channel.unpack_param(config_str);
//    if (!ok)
//    {
//        QLOGE("Failed to unpack node config");
//        return;
//    }

//    auto node = m_hal.get_streams().find_by_name<node::stream::Stream_Common>(name);
//    if (!node)
//    {
//        QLOGE("Req Id: {}, cannot find node '{}'", req_id, name);
//        return;
//    }

//    QLOGI("Req Id: {}, node '{}' - config received", req_id, name);

//    node->config.SetObject();
//    if (!config_str.empty() &&
//            node->config.Parse(config_str.c_str()).HasParseError())
//    {
//        QLOGE("Req Id: {}, node '{}' - failed to parse config: {}:{}", req_id, name, node->config.GetParseError(), node->config.GetErrorOffset());
//        return;
//    }
//}

void Comms::handle_add_source()
{
    uint32_t req_id = 0;
    if (!m_setup_channel.begin_unpack() || !m_setup_channel.unpack_param(req_id))
    {
        QLOGE("Failed to unpack source config");
        return;
    }
    auto it = std::find_if(begin(m_hal.m_add_queue), end(m_hal.m_add_queue), [&](HAL::Add_Queue_Item const& i) { return i.req_id == req_id; });
    QASSERT(it != m_hal.m_add_queue.end());
    if (it == m_hal.m_add_queue.end())
    {
        QLOGE("Cannot find add source request {}", req_id);
        return;
    }
    auto callback = it->source_callback;

    auto node = std::make_shared<node::source::Source>();
    bool ok = unpack_source_data(m_setup_channel, *node);
    if (!ok)
    {
        callback(HAL::Result::FAILED, node::source::Source_ptr());
        return;
    }
    m_hal.m_sources.add(node);
    callback(HAL::Result::OK, node);
}

void Comms::handle_add_processor()
{
    uint32_t req_id = 0;
    if (!m_setup_channel.begin_unpack() || !m_setup_channel.unpack_param(req_id))
    {
        QLOGE("Failed to unpack processor config");
        return;
    }
    auto it = std::find_if(begin(m_hal.m_add_queue), end(m_hal.m_add_queue), [&](HAL::Add_Queue_Item const& i) { return i.req_id == req_id; });
    QASSERT(it != m_hal.m_add_queue.end());
    if (it == m_hal.m_add_queue.end())
    {
        QLOGE("Cannot find add processor request {}", req_id);
        return;
    }
    auto callback = it->processor_callback;

    auto node = std::make_shared<node::processor::Processor>();
    bool ok = unpack_processor_data(m_setup_channel, *node);
    if (!ok)
    {
        callback(HAL::Result::FAILED, node::processor::Processor_ptr());
        return;
    }
    m_hal.m_processors.add(node);
    callback(HAL::Result::OK, node);
}

void Comms::handle_add_sink()
{
    uint32_t req_id = 0;
    if (!m_setup_channel.begin_unpack() || !m_setup_channel.unpack_param(req_id))
    {
        QLOGE("Failed to unpack sink config");
        return;
    }
    auto it = std::find_if(begin(m_hal.m_add_queue), end(m_hal.m_add_queue), [&](HAL::Add_Queue_Item const& i) { return i.req_id == req_id; });
    QASSERT(it != m_hal.m_add_queue.end());
    if (it == m_hal.m_add_queue.end())
    {
        QLOGE("Cannot find add sink request {}", req_id);
        return;
    }
    auto callback = it->sink_callback;

    auto node = std::make_shared<node::sink::Sink>();
    bool ok = unpack_sink_data(m_setup_channel, *node);
    if (!ok)
    {
        callback(HAL::Result::FAILED, node::sink::Sink_ptr());
        return;
    }
    m_hal.m_sinks.add(node);
    callback(HAL::Result::OK, node);
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
            if (req.source_callback)
            {
                m_setup_channel.begin_pack(comms::Setup_Message::ADD_SOURCE);
            }
            else if (req.sink_callback)
            {
                m_setup_channel.begin_pack(comms::Setup_Message::ADD_SINK);
            }
            else if (req.processor_callback)
            {
                m_setup_channel.begin_pack(comms::Setup_Message::ADD_PROCESSOR);
            }
            m_setup_channel.pack_param(req.req_id);
            m_setup_channel.pack_param(req.def_name);
            m_setup_channel.pack_param(req.name);
            pack_json(m_setup_channel, req.init_params);
            m_setup_channel.end_pack();
        }
        if (now - req.sent_time_point > REQUEST_TIMEOUT)
        {
            if (req.source_callback)
            {
                req.source_callback(HAL::Result::TIMEOUT, node::source::Source_ptr());
            }
            else if (req.sink_callback)
            {
                req.sink_callback(HAL::Result::TIMEOUT, node::sink::Sink_ptr());
            }
            else if (req.processor_callback)
            {
                req.processor_callback(HAL::Result::TIMEOUT, node::processor::Processor_ptr());
            }
            m_hal.m_add_queue.erase(it);
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
        }
    }

    while (auto msg = m_setup_channel.get_next_message())
    {
        switch (msg.get())
        {
        case comms::Setup_Message::ENUMERATE_NODE_DEFS: handle_enumerate_node_defs(); break;
        case comms::Setup_Message::ENUMERATE_NODES: handle_enumerate_nodes(); break;

        case comms::Setup_Message::SOURCE_CONFIG: handle_source_config(); break;
        case comms::Setup_Message::SINK_CONFIG: handle_sink_config(); break;
        case comms::Setup_Message::PROCESSOR_CONFIG: handle_processor_config(); break;

        case comms::Setup_Message::ADD_SOURCE: handle_add_source(); break;
        case comms::Setup_Message::ADD_PROCESSOR: handle_add_processor(); break;
        case comms::Setup_Message::ADD_SINK: handle_add_sink(); break;
//        case comms::Setup_Message::STREAM_CONFIG: handle_stream_config(); break;

//        case comms::Setup_Message::TELEMETRY_STREAMS: handle_telemetry_streams(); break;

//        case comms::Setup_Message::MULTIROTOR_MODE: handle_multirotor_mode(); break;
//        case comms::Setup_Message::MULTIROTOR_INPUT_REQUEST: handle_multirotor_input_request(); break;
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
