#include "BrainStdAfx.h"
#include "Comms.h"
#include "utils/Timed_Scope.h"
#include "utils/Json_Helpers.h"

#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IADC_Value.h"
#include "common/node/stream/IBattery_State.h"
#include "common/node/stream/ICardinal_Points.h"
#include "common/node/stream/ICurrent.h"
#include "common/node/stream/IDistance.h"
#include "common/node/stream/IECEF_Location.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/IPressure.h"
#include "common/node/stream/IPWM_Value.h"
#include "common/node/stream/ILocal_Frame.h"
#include "common/node/stream/IENU_Frame.h"
#include "common/node/stream/ITemperature.h"
#include "common/node/stream/IVideo.h"
#include "common/node/stream/IVoltage.h"

#include "common/node/processor/IPilot.h"

#include "sz_math.hpp"
#include "sz_Comms_Source.hpp"

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
    , m_comms_start_tp(q::Clock::now())
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

    m_source.reset(new Source(*this));
    m_commands_stream.reset(new Commands);

    m_config.reset(new sz::Comms::Source::Config);
    m_init_params.reset(new sz::Comms::Source::Init_Params);
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
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

#pragma pack(push, 1)
struct Sample_Data
{
    uint64_t dt : 24; //10us
    uint64_t tp : 40; //1us
    uint16_t sample_idx : 15;
    uint16_t is_healthy : 1;
};
#pragma pack(pop)

template<class Stream> auto Comms::gather_telemetry_stream(Telemetry_Stream& ts, node::stream::IStream const& _stream) -> bool
{
    if (q::rtti::is_of_type<Stream>(_stream))
    {
        auto const& stream = static_cast<Stream const&>(_stream);
        auto const& samples = stream.get_samples();

        ts.sample_count += static_cast<uint32_t>(samples.size());
        size_t off = ts.data.size();

        Sample_Data data;
        for (auto const& s: samples)
        {
            util::detail::set_value(ts.data, s.value, off);

            auto dt = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(s.dt).count()) >> 3;
            if (dt >= (1 << 24))
            {
                QLOGE("Sample dt is too big!!! {} > {}", dt, 1 << 24);
                dt = (1 << 24) - 1;
            }
            auto tp = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(s.tp - m_comms_start_tp).count());
            if (tp >= (uint64_t(1) << 40))
            {
                QLOGE("Sample tp is too big!!! {} > {}", tp, uint64_t(1) << 40);
                tp = (uint64_t(1) << 40) - 1;
            }

            data.sample_idx = s.sample_idx;
            data.is_healthy = s.is_healthy;
            data.dt = dt;
            data.tp = tp;
            util::detail::set_value(ts.data, data, off);
        }
        return true;
    }
    return false;
}

void Comms::gather_telemetry_streams()
{
    for (auto& ts: m_telemetry_streams)
    {
        auto stream = ts.stream.lock();
        if (stream)
        {
            if (gather_telemetry_stream<node::stream::IAcceleration>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IAngular_Velocity>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IMagnetic_Field>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IPressure>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IBattery_State>(ts, *stream) ||
                gather_telemetry_stream<node::stream::ILinear_Acceleration>(ts, *stream) ||
                gather_telemetry_stream<node::stream::ICardinal_Points>(ts, *stream) ||
                gather_telemetry_stream<node::stream::ICurrent>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IVoltage>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IDistance>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IECEF_Location>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IPWM_Value>(ts, *stream) ||
                gather_telemetry_stream<node::stream::ILocal_Frame>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IENU_Frame>(ts, *stream) ||
                gather_telemetry_stream<node::stream::ITemperature>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IADC_Value>(ts, *stream)
                //          send_telemetry_stream<node::stream::IVideo>(sd, *stream)
                )
            {
                ;//nothing
            }
            else
            {
                QLOGW("Unrecognized stream type: {} / {}", ts.stream_name, q::rtti::get_class_id(*stream));
            }
        }
    }
}

void Comms::pack_telemetry_streams()
{
    for (auto& ts: m_telemetry_streams)
    {
        if (!ts.data.empty() && ts.sample_count > 0)
        {
            m_telemetry_channel.begin_pack(comms::Telemetry_Message::STREAM_DATA);
            m_telemetry_channel.pack_param(ts.stream_name);
            m_telemetry_channel.pack_param(ts.sample_count);
            m_telemetry_channel.pack_data(ts.data.data(), ts.data.size());
            m_telemetry_channel.end_pack();
        }
        ts.data.clear();
        ts.sample_count = 0;
    }
}

template<class T>
void pack_outputs(Comms::Setup_Channel& channel, std::vector<T> const& io)
{
    channel.pack_param(static_cast<uint32_t>(io.size()));
    for (auto const& i: io)
    {
        channel.pack_param(i.name);
        channel.pack_param(i.class_id);
        channel.pack_param(i.stream ? i.stream->get_rate() : 0);
    }
}

template<class T>
void pack_inputs(Comms::Setup_Channel& channel, std::vector<T> const& io)
{
    channel.pack_param(static_cast<uint32_t>(io.size()));
    for (auto const& i: io)
    {
        channel.pack_param(i.name);
        channel.pack_param(i.class_id);
        channel.pack_param(i.rate);
    }
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

static void pack_node_data(Comms::Setup_Channel& channel, node::INode const& node)
{
    channel.pack_param(q::rtti::get_class_id(node));
    pack_inputs(channel, node.get_inputs());
    pack_outputs(channel, node.get_outputs());
    pack_json(channel, node.get_init_params());
    pack_json(channel, node.get_config());
}

void Comms::handle_clock()
{
    uint32_t req_id = 0;
    if (m_setup_channel.unpack_all(req_id))
    {
        QLOGI("Req Id: {} - clock", req_id);

        auto tp = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(q::Clock::now() - m_comms_start_tp).count());
        m_setup_channel.pack_all(comms::Setup_Message::CLOCK, req_id, tp);
    }
}

void Comms::handle_enumerate_node_defs()
{
    uint32_t req_id = 0;
    if (m_setup_channel.unpack_all(req_id))
    {
        QLOGI("Req Id: {} - enumerate node factory", req_id);
        auto nodes = m_hal.get_node_factory().create_all();

        m_setup_channel.begin_pack(comms::Setup_Message::ENUMERATE_NODE_DEFS);
        m_setup_channel.pack_param(req_id);
        m_setup_channel.pack_param(static_cast<uint32_t>(nodes.size()));

        for (auto const& n: nodes)
        {
            m_setup_channel.pack_param(n.name);
            m_setup_channel.pack_param(q::rtti::get_class_id(*n.node));
            pack_inputs(m_setup_channel, n.node->get_inputs());
            pack_outputs(m_setup_channel, n.node->get_outputs());
            pack_json(m_setup_channel, n.node->get_init_params());
            pack_json(m_setup_channel, n.node->get_config());
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
    uint32_t req_id = 0;
    if (m_setup_channel.unpack_all(req_id))
    {
        QLOGI("Req Id: {} - enumerate nodes", req_id);
        auto const& nodes = m_hal.get_nodes().get_all();

        m_setup_channel.begin_pack(comms::Setup_Message::ENUMERATE_NODES);
        m_setup_channel.pack_param(req_id);
        m_setup_channel.pack_param(static_cast<uint32_t>(nodes.size()));

        for (auto const& n: nodes)
        {
            m_setup_channel.pack_param(n.name);
            pack_node_data(m_setup_channel, *n.node);
        }

        m_setup_channel.end_pack();
    }
    else
    {
        QLOGE("Error in enumerating nodes");
    }
}

void Comms::handle_node_config()
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

    m_setup_channel.begin_pack(comms::Setup_Message::NODE_DATA);
    m_setup_channel.pack_param(req_id);
    m_setup_channel.pack_param(name);

    QLOGI("Req Id: {} - node config", req_id);
    auto node = m_hal.get_nodes().find_by_name<node::INode>(name);
    if (!node)
    {
        QLOGE("Req Id: {} - cannot find node '{}'", req_id, name);
        return;
    }

    rapidjson::Document config;
    if (unpack_json(m_setup_channel, config))
    {
        node->set_config(config);
    }

    pack_node_data(m_setup_channel, *node);
    m_setup_channel.end_pack();

    m_hal.save_settings();
}

void Comms::handle_add_node()
{
    uint32_t req_id = 0;
    if (m_setup_channel.begin_unpack() &&
        m_setup_channel.unpack_param(req_id))
    {
        QLOGI("Req Id: {} - add node", req_id);
        std::string def_name, name;
        rapidjson::Document init_params;

        m_setup_channel.begin_pack(comms::Setup_Message::ADD_NODE);
        m_setup_channel.pack_param(req_id);

        bool ok = m_setup_channel.unpack_param(def_name);
        ok &= m_setup_channel.unpack_param(name);
        ok &= unpack_json(m_setup_channel, init_params);
        QLOGI("\tAdd node {} of type {}", name, def_name);
        if (!ok)
        {
            QLOGE("\t\tBad node");
            m_setup_channel.end_pack();
            return;
        }

        auto node = m_hal.create_node(def_name, name, std::move(init_params));
        if (!node)
        {
            m_setup_channel.end_pack();
            return;
        }
        m_hal.save_settings();

        //reply
        pack_node_data(m_setup_channel, *node);
        m_setup_channel.end_pack();
    }
    else
    {
        QLOGE("Error in adding node");
    }
}

void Comms::handle_streams_telemetry_active()
{
    uint32_t req_id = 0;
    std::string stream_name;
    bool is_active = false;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(req_id) ||
        !m_setup_channel.unpack_param(stream_name) ||
        !m_setup_channel.unpack_param(is_active))
    {
        QLOGE("Error in unpacking stream telemetry");
        return;
    }
    m_setup_channel.end_unpack();

    QLOGI("Req Id: {} - stream '{}' telemetry: {}", req_id, stream_name, is_active ? "ON" : "OFF");

    m_setup_channel.begin_pack(comms::Setup_Message::STREAM_TELEMETRY_ACTIVE);
    m_setup_channel.pack_param(req_id);

    auto stream = m_hal.get_streams().find_by_name<node::stream::IStream>(stream_name);
    if (stream)
    {
        Telemetry_Stream ts;
        ts.stream_name = stream_name;
        ts.stream = stream;
        m_telemetry_streams.push_back(ts);

        m_setup_channel.pack_param(true);
    }
    else
    {
        m_setup_channel.pack_param(false);
        QLOGE("Req Id: {} - cannot find stream '{}' for telemetry", req_id, stream_name);
    }
    m_setup_channel.end_pack();
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
        //case comms::Input_Message::MULTIROTOR_INPUT: handle_multirotor_input(); break;

        default: QLOGE("Received unrecognised input message: {}", static_cast<int>(msg.get())); break;
        }
    }


    while (auto msg = m_setup_channel.get_next_message())
    {
        switch (msg.get())
        {
        case comms::Setup_Message::CLOCK: handle_clock(); break;
        case comms::Setup_Message::ENUMERATE_NODE_DEFS: handle_enumerate_node_defs(); break;
        case comms::Setup_Message::ENUMERATE_NODES: handle_enumerate_nodes(); break;

        case comms::Setup_Message::ADD_NODE: handle_add_node(); break;
        case comms::Setup_Message::NODE_CONFIG: handle_node_config(); break;

        case comms::Setup_Message::STREAM_TELEMETRY_ACTIVE: handle_streams_telemetry_active(); break;

        default: QLOGE("Received unrecognised setup message: {}", static_cast<int>(msg.get())); break;
        }
    }

    gather_telemetry_streams();

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
    if (now - m_last_rudp_tp >= RUDP_PERIOD)
    {
        m_last_rudp_tp = now;

        pack_telemetry_streams();

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



auto Comms::Source::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("comms::source::init");

    sz::Comms::Source::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comms::Source data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_comms.m_init_paramsj, init_params, m_comms.m_init_paramsj.GetAllocator());
    *m_comms.m_init_params = sz;
    m_comms.m_commands_stream->rate = 50;
    return true;
}
auto Comms::Source::get_init_params() const -> rapidjson::Document const&
{
    return m_comms.m_init_paramsj;
}
auto Comms::Source::set_config(rapidjson::Value const& json) -> bool
{
    sz::Comms::Source::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comms::Source config data: {}", ss.str());
        return false;
    }

    *m_comms.m_config = sz;
    return true;
}
auto Comms::Source::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_comms.m_config, json);
    return std::move(json);
}
auto Comms::Source::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].class_id = q::rtti::get_class_id<node::stream::ICommands>();
    outputs[0].name = "Multirotor Input";
    outputs[0].stream = m_comms.m_commands_stream;
    return outputs;
}

void Comms::Source::process()
{

}
