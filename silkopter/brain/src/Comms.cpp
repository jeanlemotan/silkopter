#include "BrainStdAfx.h"
#include "Comms.h"
#include "utils/Timed_Scope.h"
#include "utils/Json_Util.h"

#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IADC.h"
#include "common/node/stream/IFloat.h"
#include "common/node/stream/IBool.h"
#include "common/node/stream/IBattery_State.h"
#include "common/node/stream/ICurrent.h"
#include "common/node/stream/IDistance.h"
#include "common/node/stream/IPosition.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/IPressure.h"
#include "common/node/stream/IPWM.h"
#include "common/node/stream/IFrame.h"
#include "common/node/stream/IGPS_Info.h"
#include "common/node/stream/ITemperature.h"
#include "common/node/stream/IVideo.h"
#include "common/node/stream/IForce.h"
#include "common/node/stream/IVelocity.h"
#include "common/node/stream/IThrottle.h"
#include "common/node/stream/ITorque.h"
#include "common/node/stream/IVoltage.h"

#include "common/node/IPilot.h"

#include "sz_math.hpp"
#include "sz_Comms_Source.hpp"
#include "sz_Multi_Config.hpp"

using namespace silk;
using namespace boost::asio;

constexpr uint8_t SETUP_CHANNEL = 10;
constexpr uint8_t INPUT_CHANNEL = 15;
constexpr uint8_t TELEMETRY_CHANNEL = 20;
constexpr uint8_t VIDEO_CHANNEL = 4;

constexpr q::Clock::duration RUDP_PERIOD = std::chrono::milliseconds(30);

Comms::Comms(boost::asio::io_service& io_service, HAL& hal)
    : m_io_service(io_service)
    , m_hal(hal)
    , m_socket(io_service)
    , m_rudp(m_socket)
    , m_setup_channel(m_rudp, SETUP_CHANNEL)
    , m_input_channel(m_rudp, INPUT_CHANNEL)
    , m_telemetry_channel(m_rudp, TELEMETRY_CHANNEL)
    , m_video_channel(m_rudp, VIDEO_CHANNEL)
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
        params.max_receive_time = std::chrono::milliseconds(500);
        m_rudp.set_receive_params(TELEMETRY_CHANNEL, params);
    }

    m_source.reset(new Source(*this));
    m_commands_stream.reset(new Commands);

    m_config.reset(new sz::Comms::Source::Config);
    m_init_params.reset(new sz::Comms::Source::Init_Params);
}

auto Comms::start(uint16_t send_port, uint16_t receive_port) -> bool
{
    try
    {
        m_socket.open(receive_port);
        m_socket.start_listening();
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
    return m_socket.get_send_endpoint().address();
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

auto Comms::send_video_stream(Stream_Telemetry_Data& ts, node::stream::IStream const& _stream) -> bool
{
    if (_stream.get_type() != node::stream::IVideo::TYPE)
    {
        return false;
    }

    auto const& stream = static_cast<node::stream::IVideo const&>(_stream);
    auto const& samples = stream.get_samples();

    Sample_Data data;

    for (auto const& s: samples)
    {
        m_video_channel.begin_pack(comms::Video_Message::FRAME_DATA);
        m_video_channel.pack_param(ts.stream_name);
        m_video_channel.pack_param(s.value.type);
        m_video_channel.pack_param(s.value.resolution);

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

        m_video_channel.pack_param(data);
        m_video_channel.pack_param(static_cast<uint32_t>(s.value.data.size()));
        m_video_channel.pack_data(s.value.data.data(), s.value.data.size());
        m_video_channel.end_pack();
    }

    return true;
}

template<class Stream> auto Comms::gather_telemetry_stream(Stream_Telemetry_Data& ts, node::stream::IStream const& _stream) -> bool
{
    if (_stream.get_type() == Stream::TYPE)
    {
        auto const& stream = static_cast<Stream const&>(_stream);
        auto const& samples = stream.get_samples();

        if (ts.sample_count < 1000000)
        {
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
        }
        else
        {
            QLOGW("Too many samples accumulated in the telemetry buffer for stream {}: {}", ts.stream_name, ts.sample_count);
        }
        return true;
    }
    return false;
}

void Comms::gather_telemetry_data()
{
    //first we gather samples and we send them at 30Hz. This improves bandwidth by reducing header overhead and allowing for better compression
    //Except for video which is always sent directly as it comes
    for (auto& ts: m_stream_telemetry_data)
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
                gather_telemetry_stream<node::stream::ICurrent>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IVoltage>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IDistance>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IECEF_Position>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IECEF_Velocity>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IPWM>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IFrame>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IGPS_Info>(ts, *stream) ||
                gather_telemetry_stream<node::stream::ITemperature>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IADC>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IFloat>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IBool>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IForce>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IVelocity>(ts, *stream) ||
                gather_telemetry_stream<node::stream::IThrottle>(ts, *stream) ||
                gather_telemetry_stream<node::stream::ITorque>(ts, *stream) ||
                send_video_stream(ts, *stream)
                )
            {
                ;//nothing
            }
            else
            {
                QLOGE("Unrecognized stream type: {} / {}", ts.stream_name, static_cast<int>(stream->get_type()));
            }
        }
    }


    //pack HAL telemetry
    if (m_hal_telemetry_data.is_enabled)
    {
        HAL::Telemetry_Data const& telemetry_data = m_hal.get_telemetry_data();

        m_hal_telemetry_data.sample_count++;
        size_t off = m_hal_telemetry_data.data.size();

        auto dt = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(telemetry_data.total_duration).count());
        util::detail::set_value(m_hal_telemetry_data.data, dt, off);
        util::detail::set_value(m_hal_telemetry_data.data, telemetry_data.rate, off);
        util::detail::set_value(m_hal_telemetry_data.data, static_cast<uint32_t>(telemetry_data.nodes.size()), off);

        for (auto const& nt: telemetry_data.nodes)
        {
            auto const& node_name = nt.first;
            auto const& node_telemetry_data = nt.second;

            util::detail::set_value(m_hal_telemetry_data.data, node_name, off);
            auto dt = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(node_telemetry_data.process_duration).count());
            util::detail::set_value(m_hal_telemetry_data.data, dt, off);
            util::detail::set_value(m_hal_telemetry_data.data, node_telemetry_data.process_percentage, off);
        }
    }
}

void Comms::pack_telemetry_data()
{
    for (auto& ts: m_stream_telemetry_data)
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

    if (m_hal_telemetry_data.is_enabled)
    {
        auto& t = m_hal_telemetry_data;
        if (!t.data.empty() && t.sample_count > 0)
        {
            m_telemetry_channel.begin_pack(comms::Telemetry_Message::STREAM_DATA);
            m_telemetry_channel.pack_param(t.sample_count);
            m_telemetry_channel.pack_data(t.data.data(), t.data.size());
            m_telemetry_channel.end_pack();
        }
        t.data.clear();
        t.sample_count = 0;
    }
}

template<class T>
void pack_outputs(Comms::Setup_Channel& channel, std::vector<T> const& io)
{
    channel.pack_param(static_cast<uint32_t>(io.size()));
    for (auto const& i: io)
    {
        channel.pack_param(i.name);
        channel.pack_param(i.type);
        channel.pack_param(i.stream ? i.stream->get_rate() : 0);
    }
}

template<class T>
void pack_def_inputs(Comms::Setup_Channel& channel, std::vector<T> const& io)
{
    channel.pack_param(static_cast<uint32_t>(io.size()));
    for (auto const& i: io)
    {
        channel.pack_param(i.name);
        channel.pack_param(i.type);
        channel.pack_param(i.rate);
    }
}
template<class T>
void pack_inputs(Comms::Setup_Channel& channel, std::vector<T> const& io)
{
    channel.pack_param(static_cast<uint32_t>(io.size()));
    for (auto const& i: io)
    {
        channel.pack_param(i.name);
        channel.pack_param(i.type);
        channel.pack_param(i.rate);
        channel.pack_param(i.stream_path.template get_as<std::string>());
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

static void pack_node_def_data(Comms::Setup_Channel& channel, node::INode const& node)
{
    pack_def_inputs(channel, node.get_inputs());
    pack_outputs(channel, node.get_outputs());
    channel.pack_param(node.get_init_params());
    channel.pack_param(node.get_config());
}

static void pack_node_data(Comms::Setup_Channel& channel, node::INode const& node)
{
    channel.pack_param(node.get_type());
    pack_inputs(channel, node.get_inputs());
    pack_outputs(channel, node.get_outputs());
    channel.pack_param(node.get_init_params());
    channel.pack_param(node.get_config());
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

void Comms::handle_multi_config()
{
    m_setup_channel.begin_unpack();
    uint32_t req_id = 0;
    if (!m_setup_channel.unpack_param(req_id))
    {
        QLOGE("Error in unpacking multi config rquest");
        return;
    }

    QLOGI("Req Id: {} - multi config", req_id);

    rapidjson::Document configj;
    if (m_setup_channel.unpack_param(configj))
    {
        config::Multi config;
        autojsoncxx::error::ErrorStack result;
        if (!autojsoncxx::from_value(config, configj, result))
        {
            std::ostringstream ss;
            ss << result;
            QLOGE("Req Id: {} - Cannot deserialize multi config: {}", ss.str());
            return;
        }
        if (m_hal.set_multi_config(config))
        {
            m_hal.save_settings();
        }
    }

    m_setup_channel.begin_pack(comms::Setup_Message::MULTI_CONFIG);
    m_setup_channel.pack_param(req_id);

    auto config = m_hal.get_multi_config();
    if (config)
    {
        m_setup_channel.pack_param(true);
        configj.SetObject();
        autojsoncxx::to_document(*config, configj);
        m_setup_channel.pack_param(configj);
    }
    else
    {
        m_setup_channel.pack_param(false);
    }

    m_setup_channel.end_pack();
}


void Comms::handle_enumerate_node_defs()
{
    //first disable all telemetry because the GS doesn't yet have all the streams
    m_stream_telemetry_data.clear();
    m_hal_telemetry_data.is_enabled = false;

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
            m_setup_channel.pack_param(n.node->get_type());
            pack_node_def_data(m_setup_channel, *n.node);
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
    //first disable all telemetry because the GS doesn't yet have all the streams
    m_stream_telemetry_data.clear();
    m_hal_telemetry_data.is_enabled = false;

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

void Comms::handle_get_node_data()
{
    m_setup_channel.begin_unpack();
    uint32_t req_id = 0;
    std::string name;
    if (!m_setup_channel.unpack_param(req_id) ||
        !m_setup_channel.unpack_param(name))
    {
        QLOGE("Error in unpacking node data request");
        return;
    }

    QLOGI("Req Id: {} - get node data", req_id);
    auto node = m_hal.get_nodes().find_by_name<node::INode>(name);
    if (!node)
    {
        QLOGE("Req Id: {} - cannot find node '{}'", req_id, name);
        return;
    }

    m_setup_channel.begin_pack(comms::Setup_Message::GET_NODE_DATA);
    m_setup_channel.pack_param(req_id);
    m_setup_channel.pack_param(name);
    pack_node_data(m_setup_channel, *node);
    m_setup_channel.end_pack();
}

void Comms::handle_node_config()
{
    m_setup_channel.begin_unpack();
    uint32_t req_id = 0;
    std::string name;
    if (!m_setup_channel.unpack_param(req_id) ||
        !m_setup_channel.unpack_param(name))
    {
        QLOGE("Error in unpacking config request");
        return;
    }

    QLOGI("Req Id: {} - node config", req_id);
    auto node = m_hal.get_nodes().find_by_name<node::INode>(name);
    if (!node)
    {
        QLOGE("Req Id: {} - cannot find node '{}'", req_id, name);
        return;
    }

    rapidjson::Document config;
    if (m_setup_channel.unpack_param(config))
    {
        node->set_config(config);
    }
    m_hal.save_settings();

    m_setup_channel.begin_pack(comms::Setup_Message::NODE_CONFIG);
    m_setup_channel.pack_param(req_id);
    m_setup_channel.pack_param(name);
    m_setup_channel.pack_param(node->get_config());
    m_setup_channel.end_pack();
}

void Comms::handle_node_message()
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

    QLOGI("Req Id: {} - node message", req_id);
    auto node = m_hal.get_nodes().find_by_name<node::INode>(name);
    if (!node)
    {
        QLOGE("Req Id: {} - cannot find node '{}'", req_id, name);
        return;
    }

    rapidjson::Document message;
    if (!m_setup_channel.unpack_param(message))
    {
        QLOGE("Req Id: {} - cannot unpack node '{}' message", req_id, name);
        return;
    }
    auto response = node->send_message(message);

    m_setup_channel.begin_pack(comms::Setup_Message::NODE_MESSAGE);
    m_setup_channel.pack_param(req_id);
    m_setup_channel.pack_param(name);
    m_setup_channel.pack_param(response);
    m_setup_channel.end_pack();
}

void Comms::handle_node_input_stream_path()
{
    m_setup_channel.begin_unpack();
    uint32_t req_id = 0;
    std::string name;
    uint32_t input_idx = 0;
    std::string path;
    if (!m_setup_channel.unpack_param(req_id) ||
        !m_setup_channel.unpack_param(name) ||
        !m_setup_channel.unpack_param(input_idx) ||
        !m_setup_channel.unpack_param(path))
    {
        QLOGE("Error in unpacking input stream path request");
        return;
    }

    QLOGI("Req Id: {} - node input stream path", req_id);
    auto node = m_hal.get_nodes().find_by_name<node::INode>(name);
    if (!node)
    {
        QLOGE("Req Id: {} - cannot find node '{}'", req_id, name);
        return;
    }

    node->set_input_stream_path(input_idx, q::Path(path));

    m_hal.save_settings();

    m_setup_channel.begin_pack(comms::Setup_Message::NODE_INPUT_STREAM_PATH);
    m_setup_channel.pack_param(req_id);
    m_setup_channel.pack_param(name);
    pack_node_data(m_setup_channel, *node);
    m_setup_channel.end_pack();
}

void Comms::handle_node_output_calibration_data()
{
    m_setup_channel.begin_unpack();
    uint32_t req_id = 0;
    std::string name;
    uint32_t output_idx = 0;
    std::string path;
    if (!m_setup_channel.unpack_param(req_id) ||
        !m_setup_channel.unpack_param(name) ||
        !m_setup_channel.unpack_param(output_idx))
    {
        QLOGE("Error in unpacking input stream path request");
        return;
    }

    QLOGI("Req Id: {} - node input stream path", req_id);
    auto node = m_hal.get_nodes().find_by_name<node::INode>(name);
    if (!node)
    {
        QLOGE("Req Id: {} - cannot find node '{}'", req_id, name);
        return;
    }

    auto outputs = node->get_outputs();
    if (output_idx >= outputs.size())
    {
        QLOGE("Req Id: {} - output stream idx {} is out of range", req_id, output_idx);
        return;
    }
    auto stream = outputs[output_idx].stream;
    if (!stream)
    {
        QLOGE("Req Id: {} - output stream idx {} is null", req_id, output_idx);
        return;
    }

    std::vector<uint8_t> data;
    if (!m_setup_channel.unpack_remaining_data(data) ||
        !stream->deserialize_calibration_data(data))
    {
        QLOGE("Req Id: {} - cannot deserialize calibration data", req_id);
        return;
    }

    m_hal.save_settings();

    m_setup_channel.begin_pack(comms::Setup_Message::NODE_OUTPUT_CALIBRATION_DATA);
    m_setup_channel.pack_param(req_id);
    m_setup_channel.pack_param(name);
    pack_node_data(m_setup_channel, *node);
    m_setup_channel.end_pack();
}

void Comms::handle_add_node()
{
    uint32_t req_id = 0;
    std::string def_name, name;
    rapidjson::Document init_paramsj;
    rapidjson::Document configj;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(req_id) ||
        !m_setup_channel.unpack_param(def_name) ||
        !m_setup_channel.unpack_param(name) ||
        !m_setup_channel.unpack_param(init_paramsj) ||
        !m_setup_channel.unpack_param(configj))
    {
        QLOGE("Error in unpacking add node request");
        return;
    }

    QLOGI("Req Id: {} - add node", req_id);
    QLOGI("\tAdd node {} of type {}", name, def_name);

    auto node = m_hal.create_node(def_name, name, std::move(init_paramsj));
    if (!node)
    {
        m_setup_channel.end_pack();
        return;
    }
    node->set_config(configj);
    m_hal.save_settings();

    //reply
    m_setup_channel.begin_pack(comms::Setup_Message::ADD_NODE);
    m_setup_channel.pack_param(req_id);
    m_setup_channel.pack_param(name);
    pack_node_data(m_setup_channel, *node);
    m_setup_channel.end_pack();
}

void Comms::handle_remove_node()
{
    uint32_t req_id = 0;
    std::string name;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(req_id) ||
        !m_setup_channel.unpack_param(name))
    {
        QLOGE("Error in unpacking remove node request");
        return;
    }

    QLOGI("Req Id: {} - remove node {}", req_id, name);

    auto node = m_hal.get_nodes().find_by_name<node::INode>(name);
    if (!node)
    {
        QLOGE("Req Id: {} - cannot find node '{}'", req_id, name);
        return;
    }

    m_hal.get_nodes().remove(node);
    m_hal.save_settings();

    //reply
    m_setup_channel.begin_pack(comms::Setup_Message::REMOVE_NODE);
    m_setup_channel.pack_param(req_id);
    m_setup_channel.pack_param(name);
    m_setup_channel.end_pack();
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

    //remove the stream from the telemetry list (it's added again below if needed)
    m_stream_telemetry_data.erase(std::remove_if(m_stream_telemetry_data.begin(), m_stream_telemetry_data.end(), [&stream_name](Stream_Telemetry_Data const& ts)
    {
        return ts.stream_name == stream_name;
    }), m_stream_telemetry_data.end());


    m_setup_channel.begin_pack(comms::Setup_Message::STREAM_TELEMETRY_ACTIVE);
    m_setup_channel.pack_param(req_id);

    if (is_active)
    {
        auto stream = m_hal.get_streams().find_by_name<node::stream::IStream>(stream_name);
        if (stream)
        {
            //add the stream to the telemetry list
            Stream_Telemetry_Data ts;
            ts.stream_name = stream_name;
            ts.stream = stream;
            m_stream_telemetry_data.push_back(ts);

            m_setup_channel.pack_param(true);
        }
        else
        {
            m_setup_channel.pack_param(false);
            QLOGE("Req Id: {} - cannot find stream '{}' for telemetry", req_id, stream_name);
        }
    }
    else
    {
        m_setup_channel.pack_param(false);
    }

    m_setup_channel.end_pack();
}

void Comms::handle_hal_telemetry_active()
{
    uint32_t req_id = 0;
    if (!m_setup_channel.begin_unpack() ||
        !m_setup_channel.unpack_param(req_id))
    {
        QLOGE("Error in unpacking stream telemetry");
        return;
    }
    m_setup_channel.end_unpack();

    bool is_active = false;
    if (m_setup_channel.unpack_param(is_active))
    {
        QLOGI("Req Id: {} - hal telemetry: {}", req_id, is_active ? "ON" : "OFF");
        m_hal_telemetry_data.is_enabled = is_active;
    }

    //respond
    m_setup_channel.begin_pack(comms::Setup_Message::HAL_TELEMETRY_ACTIVE);
    m_setup_channel.pack_param(req_id);
    m_setup_channel.pack_param(m_hal_telemetry_data.is_enabled);
    m_setup_channel.end_pack();
}

void Comms::handle_simulator_stop_motion()
{

}

void Comms::handle_simulator_reset()
{

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

        case comms::Setup_Message::MULTI_CONFIG: handle_multi_config(); break;

        case comms::Setup_Message::ENUMERATE_NODE_DEFS: handle_enumerate_node_defs(); break;
        case comms::Setup_Message::ENUMERATE_NODES: handle_enumerate_nodes(); break;
        case comms::Setup_Message::GET_NODE_DATA: handle_get_node_data(); break;

        case comms::Setup_Message::ADD_NODE: handle_add_node(); break;
        case comms::Setup_Message::REMOVE_NODE: handle_remove_node(); break;
        case comms::Setup_Message::NODE_CONFIG: handle_node_config(); break;
        case comms::Setup_Message::NODE_MESSAGE: handle_node_message(); break;
        case comms::Setup_Message::NODE_INPUT_STREAM_PATH: handle_node_input_stream_path(); break;
        case comms::Setup_Message::NODE_OUTPUT_CALIBRATION_DATA: handle_node_output_calibration_data(); break;

        case comms::Setup_Message::STREAM_TELEMETRY_ACTIVE: handle_streams_telemetry_active(); break;
        case comms::Setup_Message::HAL_TELEMETRY_ACTIVE: handle_hal_telemetry_active(); break;

        default: QLOGE("Received unrecognised setup message: {}", static_cast<int>(msg.get())); break;
        }
    }

    gather_telemetry_data();

    {
        if (m_socket.get_send_endpoint().address().is_unspecified() && !m_socket.get_last_receive_endpoint().address().is_unspecified())
        {
            auto endpoint = m_socket.get_last_receive_endpoint();
            endpoint.port(m_send_port);
            m_socket.set_send_endpoint(endpoint);
            m_rudp.reconnect();
        }
    }

    m_rudp.process();

    auto now = q::Clock::now();
    if (now - m_last_rudp_tp >= RUDP_PERIOD)
    {
        m_last_rudp_tp = now;

        pack_telemetry_data();

        m_setup_channel.send();
        m_input_channel.send();
        m_telemetry_channel.try_sending();
    }

    m_video_channel.try_sending();

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
    *m_comms.m_init_params = sz;
    m_comms.m_commands_stream->rate = 50;
    return true;
}
auto Comms::Source::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_comms.m_init_params, json);
    return std::move(json);
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
    outputs[0].type = node::stream::ICommands::TYPE;
    outputs[0].name = "Multirotor Input";
    outputs[0].stream = m_comms.m_commands_stream;
    return outputs;
}

void Comms::Source::process()
{

}
