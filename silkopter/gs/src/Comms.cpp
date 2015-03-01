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
    m_hal.get_sinks().remove_all();
    m_hal.get_sources().remove_all();
    m_hal.get_processors().remove_all();
    m_hal.get_streams().remove_all();
}

void Comms::request_nodes()
{
    m_setup_channel.pack(comms::Setup_Message::ENUMERATE_NODE_FACTORY, ++m_last_req_id);
    m_setup_channel.pack(comms::Setup_Message::ENUMERATE_NODES, ++m_last_req_id);
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

void Comms::handle_enumerate_node_factory()
{
    auto parse_json = [](std::string const& str) -> std::unique_ptr<rapidjson::Document>
    {
        std::unique_ptr<rapidjson::Document> json(new rapidjson::Document);
        json->SetObject();
        if (!str.empty() && json->Parse(str.c_str()).HasParseError())
        {
//            QLOGE("Failed to parse config: {}:{}", req_id, name, node->config.GetParseError(), node->config.GetErrorOffset());
            return nullptr;
        }
        return std::move(json);
    };

    uint32_t req_id = 0;
    if (m_setup_channel.begin_unpack() &&
        m_setup_channel.unpack_param(req_id))
    {
        uint32_t bus_count = 0;
        uint32_t source_count = 0;
        uint32_t sink_count = 0;
        uint32_t processor_count = 0;
        std::string name;
        std::string init_params_str;
        std::string config_str;

        bool ok = m_setup_channel.unpack_param(bus_count);
        ok &= m_setup_channel.unpack_param(source_count);
        ok &= m_setup_channel.unpack_param(sink_count);
        ok &= m_setup_channel.unpack_param(processor_count);
        if (!ok)
        {
            QLOGE("Error in unpacking enumerate node factory message");
            return;
        }

        QLOGI("Req Id: {}, Factory has {} buses, {} sources, {} sinks and {} processors", req_id, bus_count, source_count, sink_count, processor_count);

        for (uint32_t i = 0; i < bus_count; i++)
        {
            m_setup_channel.unpack_param(name);
            m_setup_channel.unpack_param(init_params_str);
            m_setup_channel.unpack_param(config_str);
            QLOGI("\tBus: {}, init_params {}, config {}", name, init_params_str, config_str);
            auto init_params = parse_json(init_params_str);
            auto config = parse_json(config_str);
            if (!init_params || !config)
            {
                QLOGE("\t\tBad init_params or config jsons");
                return;
            }
            m_hal.get_bus_factory().register_node(name, std::move(*init_params), std::move(*config));
        }

        for (uint32_t i = 0; i < source_count; i++)
        {
            m_setup_channel.unpack_param(name);
            m_setup_channel.unpack_param(init_params_str);
            m_setup_channel.unpack_param(config_str);
            QLOGI("\tSource: {}, init_params {}, config {}", name, init_params_str, config_str);
            auto init_params = parse_json(init_params_str);
            auto config = parse_json(config_str);
            if (!init_params || !config)
            {
                QLOGE("\t\tBad init_params or config jsons");
                return;
            }
            m_hal.get_source_factory().register_node(name, std::move(*init_params), std::move(*config));
        }

        for (uint32_t i = 0; i < sink_count; i++)
        {
            m_setup_channel.unpack_param(name);
            m_setup_channel.unpack_param(init_params_str);
            m_setup_channel.unpack_param(config_str);
            QLOGI("\tSink: {}, init_params {}, config {}", name, init_params_str, config_str);
            auto init_params = parse_json(init_params_str);
            auto config = parse_json(config_str);
            if (!init_params || !config)
            {
                QLOGE("\t\tBad init_params or config jsons");
                return;
            }
            m_hal.get_sink_factory().register_node(name, std::move(*init_params), std::move(*config));
        }

        for (uint32_t i = 0; i < processor_count; i++)
        {
            m_setup_channel.unpack_param(name);
            m_setup_channel.unpack_param(init_params_str);
            m_setup_channel.unpack_param(config_str);
            QLOGI("\tProcessor: {}, init_params {}, config {}", name, init_params_str, config_str);
            auto init_params = parse_json(init_params_str);
            auto config = parse_json(config_str);
            if (!init_params || !config)
            {
                QLOGE("\t\tBad init_params or config jsons");
                return;
            }
            m_hal.get_processor_factory().register_node(name, std::move(*init_params), std::move(*config));
        }

        m_setup_channel.end_unpack();
    }
    else
    {
        QLOGE("Error in enumerating node factory");
    }
}

template<class T>
auto create_stream_from_rtti(std::string const& rtti, std::string const& name, uint32_t rate) -> std::shared_ptr<silk::node::stream::IStream>
{
    if (q::rtti::get_class_name<T>() == rtti)
    {
        auto s = std::make_shared<T>();
        s->name = name;
        s->rate = rate;
        return std::move(s);
    }
    return std::shared_ptr<silk::node::stream::IStream>();
}

void Comms::handle_enumerate_nodes()
{
    auto parse_json = [](std::string const& str) -> std::unique_ptr<rapidjson::Document>
    {
        std::unique_ptr<rapidjson::Document> json(new rapidjson::Document);
        json->SetObject();
        if (!str.empty() && json->Parse(str.c_str()).HasParseError())
        {
//            QLOGE("Failed to parse config: {}:{}", req_id, name, node->config.GetParseError(), node->config.GetErrorOffset());
            return nullptr;
        }
        return std::move(json);
    };


    uint32_t req_id = 0;
    if (m_setup_channel.begin_unpack() &&
        m_setup_channel.unpack_param(req_id))
    {
        uint32_t bus_count = 0;
        uint32_t source_count = 0;
        uint32_t sink_count = 0;
        uint32_t processor_count = 0;
        uint32_t stream_count = 0;
        std::string name;
        std::string type;
        std::string init_params_str;
        std::string config_str;

        bool ok = m_setup_channel.unpack_param(bus_count);
        ok &= m_setup_channel.unpack_param(source_count);
        ok &= m_setup_channel.unpack_param(sink_count);
        ok &= m_setup_channel.unpack_param(processor_count);
        ok &= m_setup_channel.unpack_param(stream_count);
        if (!ok)
        {
            QLOGE("Error in unpacking enumerate nodes message");
            return;
        }

        QLOGI("Req Id: {}, Factory has {} buses, {} sources, {} sinks, {} processors and {} streams", req_id, bus_count, source_count, sink_count, processor_count, stream_count);

        for (uint32_t i = 0; i < bus_count; i++)
        {
            m_setup_channel.unpack_param(name);
            m_setup_channel.unpack_param(type);
            m_setup_channel.unpack_param(init_params_str);
            m_setup_channel.unpack_param(config_str);
            QLOGI("\tBus: {}, type {}, init_params {}, config {}", name, init_params_str, config_str);
            auto init_params = parse_json(init_params_str);
            auto config = parse_json(config_str);
            if (!init_params || !config)
            {
                QLOGE("\t\tBad init_params or config jsons");
                return;
            }
            auto node = std::make_shared<node::bus::Bus>();
            node->type = type;
            node->name = name;
            node->init_params = std::move(*init_params);
            node->config = std::move(*config);
            m_hal.get_buses().add(node);
        }

        for (uint32_t i = 0; i < stream_count; i++)
        {
            uint32_t rate = 0;
            m_setup_channel.unpack_param(type);
            m_setup_channel.unpack_param(name);
            m_setup_channel.unpack_param(rate);
            QLOGI("\tStream: {}, type {}, rate {}Hz", name, type, rate);

            auto node = create_stream_from_rtti<node::stream::Acceleration>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::Angular_Velocity>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::Magnetic_Field>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::Pressure>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::Battery_State>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::Linear_Acceleration>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::Cardinal_Points>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::Current>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::Voltage>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::Distance>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::Location>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::PWM_Value>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::Reference_Frame>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::Temperature>(type, name, rate);
            node = node ? node : create_stream_from_rtti<node::stream::ADC_Value>(type, name, rate);
    //      node = node ? node : create_stream_from_rtti<node::stream::Video>(type);

            if (!node)
            {
                QLOGE("\tCannot create stream {}, type {}, rate {}Hz", name, type, rate);
                return;
            }

            m_hal.get_streams().add(node);
        }

        for (uint32_t i = 0; i < source_count; i++)
        {
            uint32_t output_count = 0;
            m_setup_channel.unpack_param(name);
            m_setup_channel.unpack_param(type);
            m_setup_channel.unpack_param(init_params_str);
            m_setup_channel.unpack_param(config_str);
            m_setup_channel.unpack_param(output_count);
            QLOGI("\tSource: {}, type {}, init_params {}, config {}", name, init_params_str, config_str);
            auto init_params = parse_json(init_params_str);
            auto config = parse_json(config_str);
            if (!init_params || !config)
            {
                QLOGE("\t\tBad init_params or config jsons");
                return;
            }
            auto node = std::make_shared<node::source::Source>();
            node->name = name;
            node->init_params = std::move(*init_params);
            node->config = std::move(*config);

            for (uint32_t i = 0; i < output_count; i++)
            {
                m_setup_channel.unpack_param(name);
                auto stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
                if (!stream)
                {
                    QLOGE("\t\tCannot find output stream {}", name);
                    return;
                }
                QLOGI("\t\tOutput stream {}", name);
                node->output_streams.push_back(stream);
            }

            m_hal.get_sources().add(node);
        }

        for (uint32_t i = 0; i < sink_count; i++)
        {
            uint32_t input_count = 0;
            m_setup_channel.unpack_param(name);
            m_setup_channel.unpack_param(type);
            m_setup_channel.unpack_param(init_params_str);
            m_setup_channel.unpack_param(config_str);
            m_setup_channel.unpack_param(input_count);
            QLOGI("\tSink: {}, type {}, init_params {}, config {}", name, init_params_str, config_str);
            auto init_params = parse_json(init_params_str);
            auto config = parse_json(config_str);
            if (!init_params || !config)
            {
                QLOGE("\t\tBad init_params or config jsons");
                return;
            }
            auto node = std::make_shared<node::sink::Sink>();
            node->name = name;
            node->init_params = std::move(*init_params);
            node->config = std::move(*config);

            for (uint32_t i = 0; i < input_count; i++)
            {
                m_setup_channel.unpack_param(name);
                auto stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
                if (!stream)
                {
                    QLOGE("\t\tCannot find input stream {}", name);
                    return;
                }
                QLOGI("\t\tInput stream {}", name);
                node->input_streams.push_back(stream);
            }

            m_hal.get_sinks().add(node);
        }

        for (uint32_t i = 0; i < processor_count; i++)
        {
            uint32_t input_count = 0;
            uint32_t output_count = 0;
            m_setup_channel.unpack_param(name);
            m_setup_channel.unpack_param(type);
            m_setup_channel.unpack_param(init_params_str);
            m_setup_channel.unpack_param(config_str);
            m_setup_channel.unpack_param(input_count);
            m_setup_channel.unpack_param(output_count);
            QLOGI("\tProcessor: {}, type {}, init_params {}, config {}", name, init_params_str, config_str);
            auto init_params = parse_json(init_params_str);
            auto config = parse_json(config_str);
            if (!init_params || !config)
            {
                QLOGE("\t\tBad init_params or config jsons");
                return;
            }
            auto node = std::make_shared<node::processor::Processor>();
            node->name = name;
            node->init_params = std::move(*init_params);
            node->config = std::move(*config);

            for (uint32_t i = 0; i < input_count; i++)
            {
                m_setup_channel.unpack_param(name);
                auto stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
                if (!stream)
                {
                    QLOGE("\t\tCannot find input stream {}", name);
                    return;
                }
                QLOGI("\t\tOutput stream {}", name);
                node->input_streams.push_back(stream);
            }
            for (uint32_t i = 0; i < output_count; i++)
            {
                m_setup_channel.unpack_param(name);
                auto stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
                if (!stream)
                {
                    QLOGE("\t\tCannot find output stream {}", name);
                    return;
                }
                QLOGI("\t\tInput stream {}", name);
                node->output_streams.push_back(stream);
            }

           m_hal.get_processors().add(node);
        }


        m_setup_channel.end_unpack();
    }
    else
    {
        QLOGE("Error in enumerating node factory");
    }
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

    auto node = m_hal.get_sources().find_by_name<node::source::Source>(name);
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

    auto node = m_hal.get_sinks().find_by_name<node::sink::Sink>(name);
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

    auto node = m_hal.get_processors().find_by_name<node::processor::Processor>(name);
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


void Comms::process()
{
    if (!is_connected())
    {
        return;
    }

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
        case comms::Setup_Message::ENUMERATE_NODE_FACTORY: handle_enumerate_node_factory(); break;
        case comms::Setup_Message::ENUMERATE_NODES: handle_enumerate_nodes(); break;

        case comms::Setup_Message::SOURCE_CONFIG: handle_source_config(); break;
        case comms::Setup_Message::SINK_CONFIG: handle_sink_config(); break;
        case comms::Setup_Message::PROCESSOR_CONFIG: handle_processor_config(); break;
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
