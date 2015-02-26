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
            auto source = std::make_shared<node::source::Source>();
            uint32_t ocount = 0;
            m_setup_channel.unpack_param(source->name);
            m_setup_channel.unpack_param(ocount);
            for (uint32_t j = 0; j < ocount; j++)
            {
                m_setup_channel.unpack_param(name);
                auto stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
                if (!stream)
                {
                    QLOGE("Req Id: {}, source '{}' - Cannot find stream '{}'", req_id, source->name, name);
                    return;
                }
                source->output_streams.push_back(stream);
            }
            m_hal.get_sources().add(source);

            //send request to get config
            m_setup_channel.pack(comms::Setup_Message::SOURCE_CONFIG, ++m_last_req_id, source->name);
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
            auto sink = std::make_shared<node::sink::Sink>();
            uint32_t icount = 0;
            m_setup_channel.unpack_param(sink->name);
            m_setup_channel.unpack_param(icount);
            for (uint32_t j = 0; j < icount; j++)
            {
                m_setup_channel.unpack_param(name);
                auto stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
                if (!stream)
                {
                    QLOGE("Req Id: {}, sink '{}' - Cannot find stream '{}'", req_id, sink->name, name);
                    return;
                }
                sink->input_streams.push_back(stream);
            }
            m_hal.get_sinks().add(sink);

            //send request to get config
            m_setup_channel.pack(comms::Setup_Message::SINK_CONFIG, ++m_last_req_id, sink->name);
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
            auto processor = std::make_shared<node::processor::Processor>();
            uint32_t icount = 0;
            m_setup_channel.unpack_param(processor->name);
            m_setup_channel.unpack_param(icount);
            for (uint32_t j = 0; j < icount; j++)
            {
                m_setup_channel.unpack_param(name);
                auto stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
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
                auto stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
                if (!stream)
                {
                    QLOGE("Req Id: {}, processor '{}' - Cannot find stream '{}'", req_id, processor->name, name);
                    return;
                }
                processor->output_streams.push_back(stream);
            }

            m_hal.get_processors().add(processor);

            //send request to get config
            m_setup_channel.pack(comms::Setup_Message::PROCESSOR_CONFIG, ++m_last_req_id, processor->name);
        }

        m_setup_channel.end_unpack();
    }
    else
    {
        QLOGE("Error in enumerating processors");
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
            stream = stream ? stream : create_stream_from_rtti<node::stream::Angular_Velocity>(type, name, rate);
            stream = stream ? stream : create_stream_from_rtti<node::stream::Magnetic_Field>(type, name, rate);
            stream = stream ? stream : create_stream_from_rtti<node::stream::Pressure>(type, name, rate);
            stream = stream ? stream : create_stream_from_rtti<node::stream::Battery_State>(type, name, rate);
            stream = stream ? stream : create_stream_from_rtti<node::stream::Linear_Acceleration>(type, name, rate);
            stream = stream ? stream : create_stream_from_rtti<node::stream::Cardinal_Points>(type, name, rate);
            stream = stream ? stream : create_stream_from_rtti<node::stream::Current>(type, name, rate);
            stream = stream ? stream : create_stream_from_rtti<node::stream::Voltage>(type, name, rate);
            stream = stream ? stream : create_stream_from_rtti<node::stream::Distance>(type, name, rate);
            stream = stream ? stream : create_stream_from_rtti<node::stream::Location>(type, name, rate);
            stream = stream ? stream : create_stream_from_rtti<node::stream::PWM_Value>(type, name, rate);
            stream = stream ? stream : create_stream_from_rtti<node::stream::Reference_Frame>(type, name, rate);
            stream = stream ? stream : create_stream_from_rtti<node::stream::Temperature>(type, name, rate);
            stream = stream ? stream : create_stream_from_rtti<node::stream::ADC_Value>(type, name, rate);
    //      stream = stream ? stream : create_stream_from_rtti<node::stream::Video>(type);

            if (!stream)
            {
                QLOGE("Req Id: {}, stream '{}' type '{}' - Cannot create stream", req_id, name, type);
                return;
            }

            m_hal.get_streams().add(stream);

            //send request to get config
            m_setup_channel.pack(comms::Setup_Message::STREAM_CONFIG, ++m_last_req_id, name);
        }

        m_setup_channel.end_unpack();
    }
    else
    {
        QLOGE("Error in enumerating streams");
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
    if (!!ok)
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

    if (node->config.Parse(config_str.c_str()).HasParseError())
    {
        QLOGE("Req Id: {} node '{}' - failed to parse config: {}:{}", req_id, name, node->config.GetParseError(), node->config.GetErrorOffset());
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
    if (!!ok)
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

    if (node->config.Parse(config_str.c_str()).HasParseError())
    {
        QLOGE("Req Id: {} node '{}' - failed to parse config: {}:{}", req_id, name, node->config.GetParseError(), node->config.GetErrorOffset());
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
    if (!!ok)
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

    if (node->config.Parse(config_str.c_str()).HasParseError())
    {
        QLOGE("Req Id: {} node '{}' - failed to parse config: {}:{}", req_id, name, node->config.GetParseError(), node->config.GetErrorOffset());
        return;
    }
}

void Comms::handle_stream_config()
{
    uint32_t req_id = 0;
    std::string name;
    std::string config_str;
    bool ok = m_setup_channel.begin_unpack() &&
                m_setup_channel.unpack_param(req_id) &&
                m_setup_channel.unpack_param(name) &&
                m_setup_channel.unpack_param(config_str);
    if (!!ok)
    {
        QLOGE("Failed to unpack node config");
        return;
    }

    auto node = m_hal.get_streams().find_by_name<node::stream::Stream_Common>(name);
    if (!node)
    {
        QLOGE("Req Id: {}, cannot find node '{}'", req_id, name);
        return;
    }

    QLOGI("Req Id: {}, node '{}' - config received", req_id, name);

    if (node->config.Parse(config_str.c_str()).HasParseError())
    {
        QLOGE("Req Id: {} node '{}' - failed to parse config: {}:{}", req_id, name, node->config.GetParseError(), node->config.GetErrorOffset());
        return;
    }
}


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

        case comms::Setup_Message::SOURCE_CONFIG: handle_source_config(); break;
        case comms::Setup_Message::SINK_CONFIG: handle_sink_config(); break;
        case comms::Setup_Message::PROCESSOR_CONFIG: handle_processor_config(); break;
        case comms::Setup_Message::STREAM_CONFIG: handle_stream_config(); break;

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
