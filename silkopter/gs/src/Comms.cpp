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

        QLOGI("Started sending on port {} and receiving on port {}", send_port, receive_port);
    }
    catch(...)
    {
        m_socket.close();
        QLOGW("Connect failed");
        return false;
    }

    return true;
}

void Comms::disconnect()
{
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

void Comms::handle_enumerate_sources()
{
    uint32_t req_id = 0;
    uint32_t count = 0;
    std::string name;
    if (m_setup_channel.begin_unpack() &&
        m_setup_channel.unpack_param(req_id) &&
        m_setup_channel.unpack_param(count))
    {
        QLOGI("Req Id: {} - enumerate sources", req_id);

        for (uint32_t i = 0; i < count; i++)
        {
            auto source = q::make_unique<node::source::Source>();
            uint32_t ocount = 0;
            m_setup_channel.unpack_param(source->name);
            m_setup_channel.unpack_param(ocount);
            for (uint32_t j = 0; j < ocount; j++)
            {
                m_setup_channel.unpack_param(name);
                auto* stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
                if (!stream)
                {
                    QLOGE("Req Id: {}, source '{}' - Cannot find stream '{}'", req_id, source->name, name);
                    return;
                }
                source->output_streams.push_back(stream);
            }
            m_hal.get_sources().add(std::move(source));
        }

        m_setup_channel.end_unpack();
    }
    else
    {
        QLOGE("Error in enumerating sources");
    }
}

void Comms::handle_enumerate_sinks()
{
    uint32_t req_id = 0;
    uint32_t count = 0;
    std::string name;
    if (m_setup_channel.begin_unpack() &&
        m_setup_channel.unpack_param(req_id) &&
        m_setup_channel.unpack_param(count))
    {
        QLOGI("Req Id: {} - enumerate sinks", req_id);

        for (uint32_t i = 0; i < count; i++)
        {
            auto sink = q::make_unique<node::sink::Sink>();
            uint32_t icount = 0;
            m_setup_channel.unpack_param(sink->name);
            m_setup_channel.unpack_param(icount);
            for (uint32_t j = 0; j < icount; j++)
            {
                m_setup_channel.unpack_param(name);
                auto* stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
                if (!stream)
                {
                    QLOGE("Req Id: {}, sink '{}' - Cannot find stream '{}'", req_id, sink->name, name);
                    return;
                }
                sink->input_streams.push_back(stream);
            }
            m_hal.get_sinks().add(std::move(sink));
        }

        m_setup_channel.end_unpack();
    }
    else
    {
        QLOGE("Error in enumerating sinks");
    }
}

void Comms::handle_enumerate_processors()
{
    uint32_t req_id = 0;
    uint32_t count = 0;
    std::string name;
    if (m_setup_channel.begin_unpack() &&
        m_setup_channel.unpack_param(req_id) &&
        m_setup_channel.unpack_param(count))
    {
        QLOGI("Req Id: {} - enumerate processors", req_id);

        for (uint32_t i = 0; i < count; i++)
        {
            auto processor = q::make_unique<node::processor::Processor>();
            uint32_t icount = 0;
            m_setup_channel.unpack_param(processor->name);
            m_setup_channel.unpack_param(icount);
            for (uint32_t j = 0; j < icount; j++)
            {
                m_setup_channel.unpack_param(name);
                auto* stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
                if (!stream)
                {
                    QLOGE("Req Id: {}, processor '{}' - Cannot find stream '{}'", req_id, processor->name, name);
                    return;
                }
                processor->input_streams.push_back(stream);
            }

            uint32_t ocount = 0;
            m_setup_channel.unpack_param(processor->name);
            m_setup_channel.unpack_param(ocount);
            for (uint32_t j = 0; j < ocount; j++)
            {
                m_setup_channel.unpack_param(name);
                auto* stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
                if (!stream)
                {
                    QLOGE("Req Id: {}, processor '{}' - Cannot find stream '{}'", req_id, processor->name, name);
                    return;
                }
                processor->output_streams.push_back(stream);
            }

            m_hal.get_processors().add(std::move(processor));
        }

        m_setup_channel.end_unpack();
    }
    else
    {
        QLOGE("Error in enumerating processors");
    }
}

template<class T>
auto create_stream_from_rtti(std::string const& rtti, std::string const& name, uint32_t rate) -> std::unique_ptr<silk::node::stream::IStream>
{
    if (q::rtti::get_class_name<T>() == rtti)
    {
        auto s = q::make_unique<T>();
        s->name = name;
        s->rate = rate;
        return std::move(s);
    }
    return std::unique_ptr<silk::node::stream::IStream>();
}

void Comms::handle_enumerate_streams()
{
    uint32_t req_id = 0;
    uint32_t count = 0;
    std::string type;
    std::string name;
    uint32_t rate = 0;
    if (m_setup_channel.begin_unpack() &&
        m_setup_channel.unpack_param(req_id) &&
        m_setup_channel.unpack_param(count))
    {
        QLOGI("Req Id: {} - enumerate streams", req_id);

        for (uint32_t i = 0; i < count; i++)
        {
            m_setup_channel.unpack_param(type);
            m_setup_channel.unpack_param(name);
            m_setup_channel.unpack_param(rate);

            auto stream = create_stream_from_rtti<node::stream::Acceleration>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::Angular_Velocity>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::Magnetic_Field>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::Pressure>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::Battery_State>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::Linear_Acceleration>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::Cardinal_Points>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::Current>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::Voltage>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::Distance>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::Location>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::PWM_Value>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::Reference_Frame>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::Temperature>(type, name, rate);
            stream = stream ? std::move(stream) : create_stream_from_rtti<node::stream::ADC_Value>(type, name, rate);
    //      stream = stream ? stream : create_stream_from_rtti<node::stream::Video>(type);

            if (!stream)
            {
                QLOGE("Req Id: {}, stream '{}' type '{}' - Cannot create stream", req_id, name, type);
                return;
            }

            m_hal.get_streams().add(std::move(stream));
        }

        m_setup_channel.end_unpack();
    }
    else
    {
        QLOGE("Error in enumerating streams");
    }
}

//template<class Registry, class Node_Base>
//void Comms::handle_node_config(comms::Setup_Message message, Registry const& registry)
//{
//    m_setup_channel.begin_unpack();
//    uint32_t req_id = 0;
//    std::string name;
//    if (!m_setup_channel.unpack_param(req_id) ||
//        !m_setup_channel.unpack_param(name))
//    {
//        QLOGE("Error in unpacking config rquest");
//        return;
//    }

//    QLOGI("Req Id: {} - node config", req_id);
//    auto* node = registry.template find_by_name<Node_Base>(name);
//    if (!node)
//    {
//        QLOGE("Req Id: {} - cannot find node '{}'", req_id, name);
//        return;
//    }

//    std::string config_str;
//    m_setup_channel.unpack_param<std::string>(config_str);

//    if (!config_str.empty())
//    {
//        rapidjson::Document config;
//        if (config.Parse(config_str.c_str()).HasParseError())
//        {
//            QLOGE("Req Id: {} - failed to parse config for '{}': {}:{}", req_id, name, config.GetParseError(), config.GetErrorOffset());
//            return;
//        }
//        node->set_config(config);
//    }

//    {
//        config_str.clear();
//        auto config = node->get_config();
//        if (config)
//        {
//            rapidjson::StringBuffer s;
//            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
//            config->Accept(writer);    // Accept() traverses the DOM and generates Handler events.
//            config_str = s.GetString();
//        }
//        m_setup_channel.pack(message, req_id, name, config_str);
//    }
//}

//void Comms::handle_source_config()
//{
//    handle_node_config<decltype(m_hal.get_sources()), node::source::ISource>(
//                comms::Setup_Message::SOURCE_CONFIG, m_hal.get_sources());
//}
//void Comms::handle_sink_config()
//{
//    handle_node_config<decltype(m_hal.get_sinks()), node::sink::ISink>(
//                comms::Setup_Message::SINK_CONFIG, m_hal.get_sinks());
//}
//void Comms::handle_processor_config()
//{
//    handle_node_config<decltype(m_hal.get_processors()), node::processor::IProcessor>(
//                comms::Setup_Message::PROCESSOR_CONFIG, m_hal.get_processors());
//}
//void Comms::handle_stream_config()
//{
//    handle_node_config<decltype(m_hal.get_streams()), node::stream::IStream>(
//                comms::Setup_Message::STREAM_CONFIG, m_hal.get_streams());
//}

//void Comms::handle_telemetry_streams()
//{
//    uint32_t req_id = 0;
//    if (!m_setup_channel.begin_unpack() ||
//        !m_setup_channel.unpack_param(req_id))
//    {
//        QLOGE("Error in unpacking telemetry streams");
//        return;
//    }
//    QLOGI("Req Id: {} - telemetry streams", req_id);

//    uint32_t size = 0;
//    if (m_setup_channel.unpack_param(size))
//    {
//        m_telemetry_streams.clear();
//        m_telemetry_streams.reserve(size);

//        std::string name;
//        for (uint32_t i = 0; i < size; i++)
//        {
//            if (!m_setup_channel.unpack_param(name))
//            {
//                m_telemetry_streams.clear();
//                QLOGE("Req Id: {} - error in unpacking telemetry streams", req_id);
//                return;
//            }
//            auto* stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
//            if (stream)
//            {
//                m_telemetry_streams.push_back(stream);
//            }
//            else
//            {
//                QLOGW("Req Id: {} - cannot find stream '{}' for telemetry", req_id, name);
//            }
//        }
//    }

//    m_setup_channel.end_unpack();

//    m_setup_channel.begin_pack(comms::Setup_Message::TELEMETRY_STREAMS);
//    m_setup_channel.pack_param(req_id);
//    m_setup_channel.pack_param(static_cast<uint32_t>(m_telemetry_streams.size()));
//    for (auto const& s: m_telemetry_streams)
//    {
//        QASSERT(s);
//        m_setup_channel.pack_param(s->get_name());
//    }
//    m_setup_channel.end_pack();
//}

//void Comms::handle_multirotor_mode()
//{
//    uint32_t req_id = 0;
//    if (!m_setup_channel.begin_unpack() ||
//        !m_setup_channel.unpack_param(req_id))
//    {
//        QLOGE("Error in unpacking multirotor input request");
//        return;
//    }
//    QLOGI("Req Id: {} - multirotor input request", req_id);

//    std::string name;
//    if (!m_setup_channel.unpack_param(name))
//    {
//        QLOGE("Req Id: {} - Error in unpacking multirotor input request", req_id);
//        m_setup_channel.pack(comms::Setup_Message::MULTIROTOR_MODE, req_id);
//        return;
//    }

//    auto multirotor = m_hal.get_processors().find_by_name<node::processor::IMultirotor>(name);
//    if (!multirotor)
//    {
//        QLOGE("Req Id: {} - Cannot find multirotor '{}'", req_id, name);
//        m_setup_channel.pack(comms::Setup_Message::MULTIROTOR_MODE, req_id);
//        return;
//    }

//    node::processor::IMultirotor::Mode mode;
//    if (m_setup_channel.unpack_param(mode))
//    {
//        multirotor->set_mode(mode);
//    }

//    m_setup_channel.pack(comms::Setup_Message::MULTIROTOR_MODE, req_id, multirotor->get_input());
//}

//void Comms::handle_multirotor_input_request()
//{
//    uint32_t req_id = 0;
//    if (!m_setup_channel.begin_unpack() ||
//        !m_setup_channel.unpack_param(req_id))
//    {
//        QLOGE("Error in unpacking multirotor input request");
//        return;
//    }
//    QLOGI("Req Id: {} - multirotor input request", req_id);

//    std::string name;
//    if (!m_setup_channel.unpack_param(name))
//    {
//        QLOGE("Req Id: {} - Error in unpacking multirotor input request", req_id);
//        m_setup_channel.pack(comms::Setup_Message::MULTIROTOR_INPUT_REQUEST, req_id);
//        return;
//    }

//    auto multirotor = m_hal.get_processors().find_by_name<node::processor::IMultirotor>(name);
//    if (!multirotor)
//    {
//        QLOGE("Req Id: {} - Cannot find multirotor '{}'", req_id, name);
//        m_setup_channel.pack(comms::Setup_Message::MULTIROTOR_INPUT_REQUEST, req_id);
//        return;
//    }

//    m_setup_channel.pack(comms::Setup_Message::MULTIROTOR_INPUT_REQUEST, req_id, multirotor->get_input());
//}

//void Comms::handle_multirotor_input()
//{
//    uint32_t req_id = 0;
//    if (!m_input_channel.begin_unpack() ||
//        !m_input_channel.unpack_param(req_id))
//    {
//        QLOGE("Error in unpacking multirotor input request");
//        return;
//    }
//    QLOGI("Req Id: {} - multirotor input request", req_id);

//    std::string name;
//    if (!m_input_channel.unpack_param(name))
//    {
//        QLOGE("Req Id: {} - Error in unpacking multirotor input request", req_id);
//        return;
//    }

//    auto multirotor = m_hal.get_processors().find_by_name<node::processor::IMultirotor>(name);
//    if (!multirotor)
//    {
//        QLOGE("Req Id: {} - Cannot find multirotor '{}'", req_id, name);
//        return;
//    }

//    node::processor::IMultirotor::Input input;
//    if (m_input_channel.unpack_param(input))
//    {
//        multirotor->set_input(input);
//    }
//    else
//    {
//        QLOGE("Req Id: {} - Cannot unpack input for multirotor '{}'", req_id, name);
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
        case comms::Setup_Message::ENUMERATE_SOURCES: handle_enumerate_sources(); break;
        case comms::Setup_Message::ENUMERATE_SINKS: handle_enumerate_sinks(); break;
        case comms::Setup_Message::ENUMERATE_STREAMS: handle_enumerate_streams(); break;
        case comms::Setup_Message::ENUMERATE_PROCESSORS: handle_enumerate_processors(); break;

//        case comms::Setup_Message::SOURCE_CONFIG: handle_source_config(); break;
//        case comms::Setup_Message::SINK_CONFIG: handle_sink_config(); break;
//        case comms::Setup_Message::PROCESSOR_CONFIG: handle_processor_config(); break;
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
