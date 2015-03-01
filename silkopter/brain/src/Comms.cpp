#include "BrainStdAfx.h"
#include "Comms.h"
#include "utils/Timed_Scope.h"

#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IADC_Value.h"
#include "common/node/stream/IBattery_State.h"
#include "common/node/stream/ICardinal_Points.h"
#include "common/node/stream/ICurrent.h"
#include "common/node/stream/IDistance.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/ILocation.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/IPressure.h"
#include "common/node/stream/IPWM_Value.h"
#include "common/node/stream/IReference_Frame.h"
#include "common/node/stream/ITemperature.h"
#include "common/node/stream/IVideo.h"
#include "common/node/stream/IVoltage.h"

#include "common/node/processor/IMultirotor.h"


using namespace silk;
using namespace boost::asio;

constexpr uint8_t SETUP_CHANNEL = 10;
constexpr uint8_t INPUT_CHANNEL = 15;
constexpr uint8_t TELEMETRY_CHANNEL = 20;
constexpr uint8_t VIDEO_CHANNEL = 4;
constexpr uint8_t SIMULATOR_CHANNEL = 30;

constexpr q::Clock::duration RUDP_PERIOD = std::chrono::milliseconds(30);

Comms::Comms(boost::asio::io_service& io_service, HAL& hal)
    : m_io_service(io_service)
    , m_hal(hal)
    , m_socket(io_service)
    , m_rudp(m_socket)
    , m_setup_channel(m_rudp, SETUP_CHANNEL)
    , m_input_channel(m_rudp, INPUT_CHANNEL)
    , m_telemetry_channel(m_rudp, TELEMETRY_CHANNEL)
{
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
        params.importance = 127;
        m_rudp.set_send_params(INPUT_CHANNEL, params);
    }

    {
        util::RUDP::Send_Params params;
        params.mtu = 100;
        params.is_compressed = true;
        params.is_reliable = true;
        params.importance = 127;
        m_rudp.set_send_params(SIMULATOR_CHANNEL, params);
    }

    {
        util::RUDP::Send_Params params;
        params.mtu = 8192;
        params.is_compressed = true;
        params.is_reliable = false;
        params.importance = 0;
        params.cancel_on_new_data = true;
        params.cancel_after = std::chrono::milliseconds(200);
        m_rudp.set_send_params(TELEMETRY_CHANNEL, params);
    }

    {
        util::RUDP::Send_Params params;
        params.mtu = 16000;
        params.is_compressed = false;
        params.is_reliable = false;
        params.importance = 10;
//        params.cancel_on_new_data = true;
        params.cancel_after = std::chrono::milliseconds(100);
        m_rudp.set_send_params(VIDEO_CHANNEL, params);
    }

    {
        util::RUDP::Receive_Params params;
        params.max_receive_time = std::chrono::seconds(999999);
        m_rudp.set_receive_params(SETUP_CHANNEL, params);
    }
    {
        util::RUDP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(100);
        m_rudp.set_receive_params(INPUT_CHANNEL, params);
    }

    {
        util::RUDP::Receive_Params params;
        params.max_receive_time = std::chrono::seconds(999999);
        m_rudp.set_receive_params(SIMULATOR_CHANNEL, params);
    }

    {
        util::RUDP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(500);
        m_rudp.set_receive_params(TELEMETRY_CHANNEL, params);
    }
}

auto Comms::start(uint16_t send_port, uint16_t receive_port) -> bool
{
    try
    {
        m_socket.open(ip::udp::v4());
        m_socket.set_option(ip::udp::socket::reuse_address(true));
        //m_socket.set_option(socket_base::send_buffer_size(65536));
        m_socket.bind(ip::udp::endpoint(ip::udp::v4(), receive_port));
        //m_rudp.set_send_endpoint(ip::udp::endpoint(ip::address::from_string("192.168.1.37"), send_port));

        m_rudp.start();
    }
    catch(std::exception e)
    {
        QLOGW("Cannot start comms on ports s:{} r:{}", send_port, receive_port);
        return false;
    }

    m_send_port = send_port;
    m_receive_port = receive_port;

    m_is_connected = true;
    QLOGI("Started sending on ports s:{} r:{}", send_port, receive_port);

    return true;
}

auto Comms::is_connected() const -> bool
{
    return m_is_connected;
}

auto Comms::get_remote_address() const -> boost::asio::ip::address
{
    return m_rudp.get_send_endpoint().address();
}

auto Comms::get_remote_clock() const -> Manual_Clock const&
{
    return m_remote_clock;
}

void Comms::set_setup_message_callback(comms::Setup_Message message, Setup_Channel_Callback callback)
{
    m_setup_channel_callbacks[static_cast<size_t>(message)] = callback;
}

void Comms::set_input_message_callback(comms::Input_Message message, Input_Channel_Callback callback)
{
    m_input_channel_callbacks[static_cast<size_t>(message)] = callback;
}

template<class Stream> auto Comms::send_telemetry_stream(node::stream::IStream const& _stream) -> bool
{
    if (q::rtti::is_of_type<Stream>(_stream))
    {
        auto const& stream = static_cast<Stream const&>(_stream);
        auto const& samples = stream.get_samples();
        m_telemetry_channel.begin_pack(q::rtti::get_class_id(stream));
        m_telemetry_channel.pack_param(static_cast<uint32_t>(samples.size()));
        for (auto const& s: samples)
        {
            m_telemetry_channel.pack_param(s.value);
            m_telemetry_channel.pack_param(s.sample_idx);
            m_telemetry_channel.pack_param(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(s.dt).count()));
            m_telemetry_channel.pack_param(s.is_healthy);
        }
        m_telemetry_channel.end_pack();
        return true;
    }
    return false;
}

void Comms::send_telemetry_streams()
{
    for (auto const& stream: m_telemetry_streams)
    {
        QASSERT(stream);
        if (send_telemetry_stream<node::stream::IAcceleration>(*stream) ||
            send_telemetry_stream<node::stream::IAngular_Velocity>(*stream) ||
            send_telemetry_stream<node::stream::IMagnetic_Field>(*stream) ||
            send_telemetry_stream<node::stream::IPressure>(*stream) ||
            send_telemetry_stream<node::stream::IBattery_State>(*stream) ||
            send_telemetry_stream<node::stream::ILinear_Acceleration>(*stream) ||
            send_telemetry_stream<node::stream::ICardinal_Points>(*stream) ||
            send_telemetry_stream<node::stream::ICurrent>(*stream) ||
            send_telemetry_stream<node::stream::IVoltage>(*stream) ||
            send_telemetry_stream<node::stream::IDistance>(*stream) ||
            send_telemetry_stream<node::stream::ILocation>(*stream) ||
            send_telemetry_stream<node::stream::IPWM_Value>(*stream) ||
            send_telemetry_stream<node::stream::IReference_Frame>(*stream) ||
            send_telemetry_stream<node::stream::ITemperature>(*stream) ||
            send_telemetry_stream<node::stream::IADC_Value>(*stream)
//          send_telemetry_stream<node::stream::IVideo>(*stream)
                )
        {
            ;//nothing
        }
        else
        {
            QLOGW("Unrecognized stream type: {} / {}", stream->get_name(), q::rtti::get_class_name(*stream));
        }
    }
}

void Comms::handle_enumerate_node_factory()
{
    auto pack_json = [this](rapidjson::Document const& json)
    {
        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        json.Accept(writer);
        std::string str(buffer.GetString(), buffer.GetSize());
        m_setup_channel.pack_param(str);
    };


    uint32_t req_id = 0;
    if (m_setup_channel.unpack(req_id))
    {
        QLOGI("Req Id: {} - enumerate node facrory", req_id);
        auto buses = m_hal.get_bus_factory().get_all();
        auto source = m_hal.get_source_factory().get_all();
        auto sinks = m_hal.get_sink_factory().get_all();
        auto processors = m_hal.get_processor_factory().get_all();

        m_setup_channel.begin_pack(comms::Setup_Message::ENUMERATE_NODE_FACTORY);
        m_setup_channel.pack_param(req_id);
        m_setup_channel.pack_param(static_cast<uint32_t>(buses.size()));
        m_setup_channel.pack_param(static_cast<uint32_t>(source.size()));
        m_setup_channel.pack_param(static_cast<uint32_t>(sinks.size()));
        m_setup_channel.pack_param(static_cast<uint32_t>(processors.size()));

        for (auto const& n: buses)
        {
            m_setup_channel.pack_param(n.name);
            pack_json(n.init_params);
            pack_json(n.config);
        }
        for (auto const& n: source)
        {
            m_setup_channel.pack_param(n.name);
            pack_json(n.init_params);
            pack_json(n.config);
        }
        for (auto const& n: sinks)
        {
            m_setup_channel.pack_param(n.name);
            pack_json(n.init_params);
            pack_json(n.config);
        }
        for (auto const& n: processors)
        {
            m_setup_channel.pack_param(n.name);
            pack_json(n.init_params);
            pack_json(n.config);
        }

        m_setup_channel.end_pack();
    }
    else
    {
        QLOGE("Error in enumerating node factory");
    }
}

void Comms::handle_enumerate_nodes()
{
    auto pack_json = [this](rapidjson::Document const& json)
    {
        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        json.Accept(writer);
        std::string str(buffer.GetString(), buffer.GetSize());
        m_setup_channel.pack_param(str);
    };

    uint32_t req_id = 0;
    if (m_setup_channel.unpack(req_id))
    {
        QLOGI("Req Id: {} - enumerate nodes", req_id);
        auto const& buses = m_hal.get_buses().get_all();
        auto const& sources = m_hal.get_sources().get_all();
        auto const& sinks = m_hal.get_sinks().get_all();
        auto const& streams = m_hal.get_streams().get_all();
        auto const& processors = m_hal.get_processors().get_all();

        m_setup_channel.begin_pack(comms::Setup_Message::ENUMERATE_NODES);
        m_setup_channel.pack_param(req_id);
        m_setup_channel.pack_param(static_cast<uint32_t>(buses.size()));
        m_setup_channel.pack_param(static_cast<uint32_t>(streams.size()));
        m_setup_channel.pack_param(static_cast<uint32_t>(sources.size()));
        m_setup_channel.pack_param(static_cast<uint32_t>(sinks.size()));
        m_setup_channel.pack_param(static_cast<uint32_t>(processors.size()));

        for (auto const& n: buses)
        {
            m_setup_channel.pack_param(n->get_name());
            m_setup_channel.pack_param(n->get_type());
            pack_json(n->get_init_params());
            pack_json(n->get_config());
        }
        for (auto const& n: streams)
        {
            m_setup_channel.pack_param(n->get_name());
            m_setup_channel.pack_param(n->get_type());
            m_setup_channel.pack_param(n->get_rate());
        }
        for (auto const& n: sources)
        {
            m_setup_channel.pack_param(n->get_name());
            pack_json(n->get_init_params());
            pack_json(n->get_config());
            m_setup_channel.pack_param(static_cast<uint32_t>(n->get_output_stream_count()));
            for (size_t i = 0; i < n->get_output_stream_count(); i++)
            {
                m_setup_channel.pack_param(n->get_output_stream(i).get_name());
            }
        }
        for (auto const& n: sinks)
        {
            m_setup_channel.pack_param(n->get_name());
            pack_json(n->get_init_params());
            pack_json(n->get_config());
            m_setup_channel.pack_param(static_cast<uint32_t>(n->get_input_stream_count()));
            for (size_t i = 0; i < n->get_input_stream_count(); i++)
            {
                m_setup_channel.pack_param(n->get_input_stream(i).get_name());
            }
        }
        for (auto const& n: processors)
        {
            m_setup_channel.pack_param(n->get_name());
            pack_json(n->get_init_params());
            pack_json(n->get_config());
            m_setup_channel.pack_param(static_cast<uint32_t>(n->get_input_stream_count()));
            m_setup_channel.pack_param(static_cast<uint32_t>(n->get_output_stream_count()));
            for (size_t i = 0; i < n->get_input_stream_count(); i++)
            {
                m_setup_channel.pack_param(n->get_input_stream(i).get_name());
            }
            for (size_t i = 0; i < n->get_output_stream_count(); i++)
            {
                m_setup_channel.pack_param(n->get_output_stream(i).get_name());
            }
        }

        m_setup_channel.end_pack();
    }
    else
    {
        QLOGE("Error in enumerating nodes");
    }
}

template<class Registry, class Node_Base>
void Comms::handle_node_config(comms::Setup_Message message, Registry const& registry)
{
    m_setup_channel.begin_unpack();
    uint32_t req_id = 0;
    std::string name;
    if (!m_setup_channel.unpack_param(req_id) ||
        !m_setup_channel.unpack_param(name))
    {
        QLOGE("Error in unpacking config rquest");
        return;
    }

    QLOGI("Req Id: {} - node config", req_id);
    auto* node = registry.template find_by_name<Node_Base>(name);
    if (!node)
    {
        QLOGE("Req Id: {} - cannot find node '{}'", req_id, name);
        return;
    }

    std::string config_str;
    m_setup_channel.unpack_param<std::string>(config_str);

    if (!config_str.empty())
    {
        rapidjson::Document config;
        if (config.Parse(config_str.c_str()).HasParseError())
        {
            QLOGE("Req Id: {} - failed to parse config for '{}': {}:{}", req_id, name, config.GetParseError(), config.GetErrorOffset());
            return;
        }
        node->set_config(config);
    }

    {
        config_str.clear();
        auto config = node->get_config();
        rapidjson::StringBuffer s;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
        config.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
        config_str = s.GetString();
        m_setup_channel.pack(message, req_id, name, config_str);
    }
}

void Comms::handle_source_config()
{
    handle_node_config<decltype(m_hal.get_sources()), node::source::ISource>(
                comms::Setup_Message::SOURCE_CONFIG, m_hal.get_sources());
}
void Comms::handle_sink_config()
{
    handle_node_config<decltype(m_hal.get_sinks()), node::sink::ISink>(
                comms::Setup_Message::SINK_CONFIG, m_hal.get_sinks());
}
void Comms::handle_processor_config()
{
    handle_node_config<decltype(m_hal.get_processors()), node::processor::IProcessor>(
                comms::Setup_Message::PROCESSOR_CONFIG, m_hal.get_processors());
}
//void Comms::handle_stream_config()
//{
//    handle_node_config<decltype(m_hal.get_streams()), node::stream::IStream>(
//                comms::Setup_Message::STREAM_CONFIG, m_hal.get_streams());
//}

void Comms::handle_telemetry_streams()
{
    uint32_t req_id = 0;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(req_id))
    {
        QLOGE("Error in unpacking telemetry streams");
        return;
    }
    QLOGI("Req Id: {} - telemetry streams", req_id);

    uint32_t size = 0;
    if (m_setup_channel.unpack_param(size))
    {
        m_telemetry_streams.clear();
        m_telemetry_streams.reserve(size);

        std::string name;
        for (uint32_t i = 0; i < size; i++)
        {
            if (!m_setup_channel.unpack_param(name))
            {
                m_telemetry_streams.clear();
                QLOGE("Req Id: {} - error in unpacking telemetry streams", req_id);
                return;
            }
            auto* stream = m_hal.get_streams().find_by_name<node::stream::IStream>(name);
            if (stream)
            {
                m_telemetry_streams.push_back(stream);
            }
            else
            {
                QLOGW("Req Id: {} - cannot find stream '{}' for telemetry", req_id, name);
            }
        }
    }

    m_setup_channel.end_unpack();

    m_setup_channel.begin_pack(comms::Setup_Message::TELEMETRY_STREAMS);
    m_setup_channel.pack_param(req_id);
    m_setup_channel.pack_param(static_cast<uint32_t>(m_telemetry_streams.size()));
    for (auto const& s: m_telemetry_streams)
    {
        QASSERT(s);
        m_setup_channel.pack_param(s->get_name());
    }
    m_setup_channel.end_pack();
}

void Comms::handle_multirotor_mode()
{
    uint32_t req_id = 0;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(req_id))
    {
        QLOGE("Error in unpacking multirotor input request");
        return;
    }
    QLOGI("Req Id: {} - multirotor input request", req_id);

    std::string name;
    if (!m_setup_channel.unpack_param(name))
    {
        QLOGE("Req Id: {} - Error in unpacking multirotor input request", req_id);
        m_setup_channel.pack(comms::Setup_Message::MULTIROTOR_MODE, req_id);
        return;
    }

    auto multirotor = m_hal.get_processors().find_by_name<node::processor::IMultirotor>(name);
    if (!multirotor)
    {
        QLOGE("Req Id: {} - Cannot find multirotor '{}'", req_id, name);
        m_setup_channel.pack(comms::Setup_Message::MULTIROTOR_MODE, req_id);
        return;
    }

    node::processor::IMultirotor::Mode mode;
    if (m_setup_channel.unpack_param(mode))
    {
        multirotor->set_mode(mode);
    }

    m_setup_channel.pack(comms::Setup_Message::MULTIROTOR_MODE, req_id, multirotor->get_input());
}

void Comms::handle_multirotor_input_request()
{
    uint32_t req_id = 0;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(req_id))
    {
        QLOGE("Error in unpacking multirotor input request");
        return;
    }
    QLOGI("Req Id: {} - multirotor input request", req_id);

    std::string name;
    if (!m_setup_channel.unpack_param(name))
    {
        QLOGE("Req Id: {} - Error in unpacking multirotor input request", req_id);
        m_setup_channel.pack(comms::Setup_Message::MULTIROTOR_INPUT_REQUEST, req_id);
        return;
    }

    auto multirotor = m_hal.get_processors().find_by_name<node::processor::IMultirotor>(name);
    if (!multirotor)
    {
        QLOGE("Req Id: {} - Cannot find multirotor '{}'", req_id, name);
        m_setup_channel.pack(comms::Setup_Message::MULTIROTOR_INPUT_REQUEST, req_id);
        return;
    }

    m_setup_channel.pack(comms::Setup_Message::MULTIROTOR_INPUT_REQUEST, req_id, multirotor->get_input());
}

void Comms::handle_multirotor_input()
{
    uint32_t req_id = 0;
    if (!m_input_channel.begin_unpack() ||
        !m_input_channel.unpack_param(req_id))
    {
        QLOGE("Error in unpacking multirotor input request");
        return;
    }
    QLOGI("Req Id: {} - multirotor input request", req_id);

    std::string name;
    if (!m_input_channel.unpack_param(name))
    {
        QLOGE("Req Id: {} - Error in unpacking multirotor input request", req_id);
        return;
    }

    auto multirotor = m_hal.get_processors().find_by_name<node::processor::IMultirotor>(name);
    if (!multirotor)
    {
        QLOGE("Req Id: {} - Cannot find multirotor '{}'", req_id, name);
        return;
    }

    node::processor::IMultirotor::Input input;
    if (m_input_channel.unpack_param(input))
    {
        multirotor->set_input(input);
    }
    else
    {
        QLOGE("Req Id: {} - Cannot unpack input for multirotor '{}'", req_id, name);
    }
}


void Comms::process()
{
    if (!is_connected())
    {
        return;
    }

    while (auto msg = m_input_channel.get_next_message())
    {
        switch (msg.get())
        {
        case comms::Input_Message::MULTIROTOR_INPUT: handle_multirotor_input(); break;

        default: QLOGE("Received unrecognised input message: {}", static_cast<int>(msg.get())); break;
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

        case comms::Setup_Message::TELEMETRY_STREAMS: handle_telemetry_streams(); break;

        case comms::Setup_Message::MULTIROTOR_MODE: handle_multirotor_mode(); break;
        case comms::Setup_Message::MULTIROTOR_INPUT_REQUEST: handle_multirotor_input_request(); break;

        default: QLOGE("Received unrecognised setup message: {}", static_cast<int>(msg.get())); break;
        }
    }

    send_telemetry_streams();

    {
        if (m_rudp.get_send_endpoint().address().is_unspecified() && !m_rudp.get_last_receive_endpoint().address().is_unspecified())
        {
            auto endpoint = m_rudp.get_last_receive_endpoint();
            endpoint.port(m_send_port);
            m_rudp.set_send_endpoint(endpoint);
        }
    }

    m_rudp.process();

    auto now = q::Clock::now();
    if (now - m_last_rudp_time_stamp >= RUDP_PERIOD)
    {
        m_last_rudp_time_stamp = now;

        m_setup_channel.send();
        m_input_channel.send();
        m_telemetry_channel.try_sending();
    }

//    static std::vector<uint8_t> buf;
//    if (buf.empty())
//    {
//        buf.resize(16000000);
//        std::generate(buf.begin(), buf.end(), [](){ return rand() % 50;});
//    }

//    while(true)
//    {
//        m_rudp.send(12, buf.data(), buf.size());

//        m_rudp.process();
//        //m_channel.send(COMMS_CHANNEL);
//        static int xxx = 0;
//        LOG_INFO("{}", xxx);
//        xxx++;
//        boost::this_thread::sleep_for(boost::chrono::milliseconds(30));
//    }
}

auto Comms::get_rudp() -> util::RUDP&
{
    return m_rudp;
}

