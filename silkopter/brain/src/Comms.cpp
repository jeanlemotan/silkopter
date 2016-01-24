#include "BrainStdAfx.h"
#include "Comms.h"
#include "utils/Timed_Scope.h"
#include "utils/Json_Util.h"

#include "common/stream/IAcceleration.h"
#include "common/stream/IAngular_Velocity.h"
#include "common/stream/IADC.h"
#include "common/stream/IFloat.h"
#include "common/stream/IBool.h"
#include "common/stream/IBattery_State.h"
#include "common/stream/ICurrent.h"
#include "common/stream/IDistance.h"
#include "common/stream/IPosition.h"
#include "common/stream/ILinear_Acceleration.h"
#include "common/stream/IMagnetic_Field.h"
#include "common/stream/IPressure.h"
#include "common/stream/IPWM.h"
#include "common/stream/IFrame.h"
#include "common/stream/IGPS_Info.h"
#include "common/stream/ITemperature.h"
#include "common/stream/IVideo.h"
#include "common/stream/IForce.h"
#include "common/stream/IVelocity.h"
#include "common/stream/IThrottle.h"
#include "common/stream/ITorque.h"
#include "common/stream/IVoltage.h"
#include "common/stream/IProximity.h"
#include "common/stream/IMulti_State.h"
#include "common/stream/IMulti_Commands.h"

#include "common/node/IBrain.h"


#include "utils/RCP.h"
#include "utils/RCP_UDP_Socket.h"
#include "utils/RCP_RFMON_Socket.h"
#include "utils/Channel.h"


#include "sz_math.hpp"
//#include "sz_Comms_Source.hpp"
#include "sz_Multi_Config.hpp"

using namespace silk;
using namespace boost::asio;

constexpr uint8_t SETUP_CHANNEL = 10;
constexpr uint8_t PILOT_CHANNEL = 15;
constexpr uint8_t VIDEO_CHANNEL = 16;
constexpr uint8_t TELEMETRY_CHANNEL = 20;

constexpr q::Clock::duration RCP_PERIOD = std::chrono::milliseconds(30);

struct Comms::Channels
{
    typedef util::Channel<comms::Setup_Message, uint32_t> Setup;
    typedef util::Channel<comms::Pilot_Message, uint32_t> Pilot;
    typedef util::Channel<comms::Video_Message, uint32_t> Video;
    typedef util::Channel<comms::Telemetry_Message, uint32_t> Telemetry;

    Channels()
        : setup(SETUP_CHANNEL)
        , pilot(PILOT_CHANNEL)
        , telemetry(TELEMETRY_CHANNEL)
        , video(VIDEO_CHANNEL)
    {}

    Setup setup;
    Pilot pilot;
    Telemetry telemetry;
    Video video;
};

Comms::Comms(HAL& hal)
    : m_hal(hal)
    , m_channels(new Channels())
{
}

auto Comms::start_udp(uint16_t send_port, uint16_t receive_port) -> bool
{
    try
    {
        auto s = new util::RCP_UDP_Socket();
        m_socket.reset(s);
        m_rcp.reset(new util::RCP());

        util::RCP::Socket_Handle handle = m_rcp->add_socket(m_socket.get());
        if (handle >= 0)
        {
            m_rcp->set_internal_socket_handle(handle);
            m_rcp->set_socket_handle(SETUP_CHANNEL, handle);
            m_rcp->set_socket_handle(PILOT_CHANNEL, handle);
            m_rcp->set_socket_handle(VIDEO_CHANNEL, handle);
            m_rcp->set_socket_handle(TELEMETRY_CHANNEL, handle);

            s->open(send_port, receive_port);
            s->start_listening();
            s->set_send_endpoint(ip::udp::endpoint(ip::address::from_string("127.0.0.1"), send_port));
            m_is_connected = true;
        }
    }
    catch(std::exception e)
    {
        m_is_connected = false;
        return false;
    }

    if (!m_is_connected)
    {
        m_socket.reset();
        m_rcp.reset();
        QLOGW("Cannot start comms on ports s:{} r:{}", send_port, receive_port);
        return false;
    }

    QLOGI("Started sending on ports s:{} r:{}", send_port, receive_port);

    configure_channels();

    return true;
}

auto Comms::start_rfmon(std::string const& interface, uint8_t id) -> bool
{
    try
    {
        auto s = new util::RCP_RFMON_Socket(interface, id);
        m_socket.reset(s);
        m_rcp.reset(new util::RCP);

        util::RCP::Socket_Handle handle = m_rcp->add_socket(m_socket.get());
        if (handle >= 0)
        {
            m_rcp->set_internal_socket_handle(handle);
            m_rcp->set_socket_handle(SETUP_CHANNEL, handle);
            m_rcp->set_socket_handle(PILOT_CHANNEL, handle);
            m_rcp->set_socket_handle(VIDEO_CHANNEL, handle);
            m_rcp->set_socket_handle(TELEMETRY_CHANNEL, handle);

            m_is_connected = s->start();
        }
    }
    catch(std::exception e)
    {
        m_is_connected = false;
    }

    if (!m_is_connected)
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

void Comms::configure_channels()
{
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
        params.importance = 10;
        params.cancel_after = std::chrono::milliseconds(150);
        m_rcp->set_send_params(TELEMETRY_CHANNEL, params);
    }
    {
        util::RCP::Send_Params params;
        params.is_compressed = true;
        params.is_reliable = true;
        params.importance = 100;
        params.cancel_previous_data = true;
        params.cancel_after = std::chrono::milliseconds(150);
        m_rcp->set_send_params(PILOT_CHANNEL, params);
    }
    {
        util::RCP::Send_Params params;
        params.is_compressed = false;
        params.is_reliable = true;
        params.importance = 100;
        params.cancel_after = std::chrono::milliseconds(150);
        m_rcp->set_send_params(VIDEO_CHANNEL, params);
    }

    {
        util::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::seconds(999999);
        m_rcp->set_receive_params(SETUP_CHANNEL, params);
    }
    {
        util::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(100);
        m_rcp->set_receive_params(PILOT_CHANNEL, params);
    }
    {
        util::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::milliseconds(500);
        m_rcp->set_receive_params(TELEMETRY_CHANNEL, params);
    }
}

auto Comms::is_connected() const -> bool
{
    return m_is_connected;
}

auto Comms::get_remote_clock() const -> Manual_Clock const&
{
    return m_remote_clock;
}

//auto Comms::send_video_stream(Stream_Telemetry_Data& ts, stream::IStream const& _stream) -> bool
//{
//    if (_stream.get_type() != stream::IVideo::TYPE)
//    {
//        return false;
//    }

//    auto const& stream = static_cast<stream::IVideo const&>(_stream);
//    auto const& samples = stream.get_samples();

//    for (auto const& s: samples)
//    {
//        m_channels->video.begin_pack(comms::Video_Message::FRAME_DATA);
//        m_channels->video.pack_param(ts.stream_name);
//        m_channels->video.pack_param(s);
//        m_channels->video.end_pack();
//        m_channels->video.try_sending(*m_rcp);
//    }
//    return true;
//}

template<class Stream> auto Comms::gather_telemetry_stream(Stream_Telemetry_Data& ts, stream::IStream const& _stream) -> bool
{
    if (_stream.get_type() == Stream::TYPE)
    {
        auto const& stream = static_cast<Stream const&>(_stream);
        auto const& samples = stream.get_samples();

        if (ts.sample_count < 1000000)
        {
            ts.sample_count += static_cast<uint32_t>(samples.size());
            size_t off = ts.data.size();

            for (auto const& s: samples)
            {
                util::serialization::serialize(ts.data, s, off);
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
    for (auto& ts: m_stream_telemetry_data)
    {
        auto stream = ts.stream.lock();
        if (stream)
        {
            if (gather_telemetry_stream<stream::IAcceleration>(ts, *stream) ||
                gather_telemetry_stream<stream::IAngular_Velocity>(ts, *stream) ||
                gather_telemetry_stream<stream::IMagnetic_Field>(ts, *stream) ||
                gather_telemetry_stream<stream::IPressure>(ts, *stream) ||
                gather_telemetry_stream<stream::IBattery_State>(ts, *stream) ||
                gather_telemetry_stream<stream::ILinear_Acceleration>(ts, *stream) ||
                gather_telemetry_stream<stream::ICurrent>(ts, *stream) ||
                gather_telemetry_stream<stream::IVoltage>(ts, *stream) ||
                gather_telemetry_stream<stream::IDistance>(ts, *stream) ||
                gather_telemetry_stream<stream::IECEF_Position>(ts, *stream) ||
                gather_telemetry_stream<stream::IECEF_Velocity>(ts, *stream) ||
                gather_telemetry_stream<stream::IPWM>(ts, *stream) ||
                gather_telemetry_stream<stream::IFrame>(ts, *stream) ||
                gather_telemetry_stream<stream::IGPS_Info>(ts, *stream) ||
                gather_telemetry_stream<stream::ITemperature>(ts, *stream) ||
                gather_telemetry_stream<stream::IADC>(ts, *stream) ||
                gather_telemetry_stream<stream::IFloat>(ts, *stream) ||
                gather_telemetry_stream<stream::IBool>(ts, *stream) ||
                gather_telemetry_stream<stream::IForce>(ts, *stream) ||
                gather_telemetry_stream<stream::IVelocity>(ts, *stream) ||
                gather_telemetry_stream<stream::IThrottle>(ts, *stream) ||
                gather_telemetry_stream<stream::ITorque>(ts, *stream) ||
                gather_telemetry_stream<stream::IMulti_Commands>(ts, *stream) ||
                gather_telemetry_stream<stream::IMulti_State>(ts, *stream) ||
                gather_telemetry_stream<stream::IProximity>(ts, *stream) ||
                gather_telemetry_stream<stream::IVideo>(ts, *stream))
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
        util::serialization::serialize(m_hal_telemetry_data.data, dt, off);
        util::serialization::serialize(m_hal_telemetry_data.data, telemetry_data.rate, off);
        util::serialization::serialize(m_hal_telemetry_data.data, static_cast<uint32_t>(telemetry_data.nodes.size()), off);

        for (auto const& nt: telemetry_data.nodes)
        {
            auto const& node_name = nt.first;
            auto const& node_telemetry_data = nt.second;

            util::serialization::serialize(m_hal_telemetry_data.data, node_name, off);
            auto dt = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(node_telemetry_data.process_duration).count());
            util::serialization::serialize(m_hal_telemetry_data.data, dt, off);
            util::serialization::serialize(m_hal_telemetry_data.data, node_telemetry_data.process_percentage, off);
        }
    }
}

void Comms::pack_telemetry_data()
{
    for (auto& ts: m_stream_telemetry_data)
    {
        if (!ts.data.empty() && ts.sample_count > 0)
        {
            m_channels->telemetry.begin_pack(comms::Telemetry_Message::STREAM_DATA);
            m_channels->telemetry.pack_param(ts.stream_name);
            m_channels->telemetry.pack_param(ts.sample_count);
            m_channels->telemetry.pack_data(ts.data.data(), ts.data.size());
            m_channels->telemetry.end_pack();
        }
        ts.data.clear();
        ts.sample_count = 0;
    }

    if (m_hal_telemetry_data.is_enabled)
    {
        auto& t = m_hal_telemetry_data;
        if (!t.data.empty() && t.sample_count > 0)
        {
            m_channels->telemetry.begin_pack(comms::Telemetry_Message::STREAM_DATA);
            m_channels->telemetry.pack_param(t.sample_count);
            m_channels->telemetry.pack_data(t.data.data(), t.data.size());
            m_channels->telemetry.end_pack();
        }
        t.data.clear();
        t.sample_count = 0;
    }
}

template<class T>
void pack_outputs(Comms::Channels::Setup& channel, std::vector<T> const& io)
{
    channel.pack_param(static_cast<uint32_t>(io.size()));
    for (auto const& i: io)
    {
        channel.pack_param(i.name);
        channel.pack_param(i.stream->get_type());
        channel.pack_param(i.stream->get_rate());
    }
}

template<class T>
void pack_def_inputs(Comms::Channels::Setup& channel, std::vector<T> const& io)
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
void pack_inputs(Comms::Channels::Setup& channel, std::vector<T> const& io)
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
//            QLOGE("Failed to parse config: {}:{}", name, node->config.GetParseError(), node->config.GetErrorOffset());
        return nullptr;
    }
    return std::move(json);
}

static void pack_node_def_data(Comms::Channels::Setup& channel, node::INode const& node)
{
    pack_def_inputs(channel, node.get_inputs());
    pack_outputs(channel, node.get_outputs());
    channel.pack_param(node.get_init_params());
}

static void pack_node_data(Comms::Channels::Setup& channel, node::INode const& node)
{
    channel.pack_param(node.get_type());
    pack_inputs(channel, node.get_inputs());
    pack_outputs(channel, node.get_outputs());
    channel.pack_param(node.get_init_params());
    channel.pack_param(node.get_config());
}




void Comms::handle_clock()
{
    auto& channel = m_channels->setup;
    QLOGI("Req clock");

    auto tp = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(q::Clock::now().time_since_epoch()).count());
    channel.pack_all(comms::Setup_Message::CLOCK, tp);
}

void Comms::handle_multi_config()
{
    auto& channel = m_channels->setup;
    channel.begin_unpack();

    QLOGI("Req multi config");

    rapidjson::Document configj;
    if (channel.unpack_param(configj))
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

    channel.begin_pack(comms::Setup_Message::MULTI_CONFIG);

    auto config = m_hal.get_multi_config();
    if (config)
    {
        channel.pack_param(true);
        configj.SetObject();
        autojsoncxx::to_document(*config, configj);
        channel.pack_param(configj);
    }
    else
    {
        channel.pack_param(false);
    }

    channel.end_pack();
}


void Comms::handle_enumerate_node_defs()
{
    auto& channel = m_channels->setup;

    //first disable all telemetry because the GS doesn't yet have all the streams
    m_stream_telemetry_data.clear();
    m_hal_telemetry_data.is_enabled = false;

    QLOGI("Enumerate node factory");
    auto nodes = m_hal.get_node_factory().create_all();

    channel.begin_pack(comms::Setup_Message::ENUMERATE_NODE_DEFS);
    channel.pack_param(static_cast<uint32_t>(nodes.size()));

    for (auto const& n: nodes)
    {
        channel.pack_param(n.name);
        channel.pack_param(n.node->get_type());
        pack_node_def_data(channel, *n.node);
    }

    channel.end_pack();
}

void Comms::handle_enumerate_nodes()
{
    auto& channel = m_channels->setup;

    //first disable all telemetry because the GS doesn't yet have all the streams
    m_stream_telemetry_data.clear();
    m_hal_telemetry_data.is_enabled = false;

    QLOGI("Enumerate nodes");
    auto const& nodes = m_hal.get_nodes().get_all();

    channel.begin_pack(comms::Setup_Message::ENUMERATE_NODES);
    channel.pack_param(static_cast<uint32_t>(nodes.size()));

    for (auto const& n: nodes)
    {
        channel.pack_param(n.name);
        pack_node_data(channel, *n.node);
    }

    channel.end_pack();
}

void Comms::handle_get_node_data()
{
    auto& channel = m_channels->setup;

    channel.begin_unpack();
    std::string name;
    if (!channel.unpack_param(name))
    {
        QLOGE("Error in unpacking node data request");
        return;
    }

    QLOGI("Get node data");
    auto node = m_hal.get_nodes().find_by_name<node::INode>(name);
    if (!node)
    {
        QLOGE("Cannot find node '{}'", name);
        return;
    }

    channel.begin_pack(comms::Setup_Message::GET_NODE_DATA);
    channel.pack_param(name);
    pack_node_data(m_channels->setup, *node);
    channel.end_pack();
}

void Comms::handle_node_config()
{
    auto& channel = m_channels->setup;

    channel.begin_unpack();
    std::string name;
    if (!channel.unpack_param(name))
    {
        QLOGE("Error in unpacking config request");
        return;
    }

    QLOGI("Node config");
    auto node = m_hal.get_nodes().find_by_name<node::INode>(name);
    if (!node)
    {
        QLOGE("Cannot find node '{}'", name);
        return;
    }

    rapidjson::Document config;
    if (channel.unpack_param(config))
    {
        node->set_config(config);
    }
    m_hal.save_settings();

    channel.begin_pack(comms::Setup_Message::NODE_CONFIG);
    channel.pack_param(name);
    channel.pack_param(node->get_config());
    channel.end_pack();
}

void Comms::handle_node_message()
{
    auto& channel = m_channels->setup;

    channel.begin_unpack();
    std::string name;
    if (!channel.unpack_param(name))
    {
        QLOGE("Error in unpacking config rquest");
        return;
    }

//    QLOGI("Node message");
    auto node = m_hal.get_nodes().find_by_name<node::INode>(name);
    if (!node)
    {
        QLOGE("Cannot find node '{}'", name);
        return;
    }

    rapidjson::Document message;
    if (!channel.unpack_param(message))
    {
        QLOGE("Cannot unpack node '{}' message", name);
        return;
    }
    auto response = node->send_message(message);

    channel.begin_pack(comms::Setup_Message::NODE_MESSAGE);
    channel.pack_param(name);
    channel.pack_param(response);
    channel.end_pack();
}

void Comms::handle_node_input_stream_path()
{
    auto& channel = m_channels->setup;

    channel.begin_unpack();
    std::string name;
    uint32_t input_idx = 0;
    std::string path;
    if (!channel.unpack_param(name) ||
        !channel.unpack_param(input_idx) ||
        !channel.unpack_param(path))
    {
        QLOGE("Error in unpacking input stream path request");
        return;
    }

    QLOGI("Node input stream path");
    auto node = m_hal.get_nodes().find_by_name<node::INode>(name);
    if (!node)
    {
        QLOGE("Cannot find node '{}'", name);
        return;
    }

    node->set_input_stream_path(input_idx, q::Path(path));

    m_hal.save_settings();

    channel.begin_pack(comms::Setup_Message::NODE_INPUT_STREAM_PATH);
    channel.pack_param(name);
    pack_node_data(m_channels->setup, *node);
    channel.end_pack();
}

void Comms::handle_add_node()
{
    auto& channel = m_channels->setup;

    std::string def_name, name;
    rapidjson::Document init_paramsj;
    if (!channel.begin_unpack() ||
        !channel.unpack_param(def_name) ||
        !channel.unpack_param(name) ||
        !channel.unpack_param(init_paramsj))
    {
        QLOGE("Error in unpacking add node request");
        return;
    }

    QLOGI("Add node");
    QLOGI("\tAdd node {} of type {}", name, def_name);

    auto node = m_hal.create_node(def_name, name, std::move(init_paramsj));
    if (!node)
    {
        channel.end_pack();
        return;
    }
    m_hal.save_settings();

    //reply
    channel.begin_pack(comms::Setup_Message::ADD_NODE);
    channel.pack_param(name);
    pack_node_data(m_channels->setup, *node);
    channel.end_pack();
}

void Comms::handle_remove_node()
{
    auto& channel = m_channels->setup;

    std::string name;
    if (!channel.begin_unpack() ||
        !channel.unpack_param(name))
    {
        QLOGE("Error in unpacking remove node request");
        return;
    }

    QLOGI("Remove node {}", name);

    auto node = m_hal.get_nodes().find_by_name<node::INode>(name);
    if (!node)
    {
        QLOGE("Cannot find node '{}'", name);
        return;
    }

    m_hal.get_nodes().remove(node);
    m_hal.save_settings();

    //reply
    channel.begin_pack(comms::Setup_Message::REMOVE_NODE);
    channel.pack_param(name);
    channel.end_pack();
}

void Comms::handle_streams_telemetry_active()
{
    auto& channel = m_channels->setup;

    std::string stream_name;
    bool is_active = false;
    if (!channel.begin_unpack() ||
        !channel.unpack_param(stream_name) ||
        !channel.unpack_param(is_active))
    {
        QLOGE("Error in unpacking stream telemetry");
        return;
    }
    channel.end_unpack();

    QLOGI("Stream '{}' telemetry: {}", stream_name, is_active ? "ON" : "OFF");

    //remove the stream from the telemetry list (it's added again below if needed)
    m_stream_telemetry_data.erase(std::remove_if(m_stream_telemetry_data.begin(), m_stream_telemetry_data.end(), [&stream_name](Stream_Telemetry_Data const& ts)
    {
        return ts.stream_name == stream_name;
    }), m_stream_telemetry_data.end());


    channel.begin_pack(comms::Setup_Message::STREAM_TELEMETRY_ACTIVE);

    if (is_active)
    {
        auto stream = m_hal.get_streams().find_by_name<stream::IStream>(stream_name);
        if (stream)
        {
            //add the stream to the telemetry list
            Stream_Telemetry_Data ts;
            ts.stream_name = stream_name;
            ts.stream = stream;
            m_stream_telemetry_data.push_back(ts);

            channel.pack_param(true);
        }
        else
        {
            channel.pack_param(false);
            QLOGE("Cannot find stream '{}' for telemetry", stream_name);
        }
    }
    else
    {
        channel.pack_param(false);
    }

    channel.end_pack();
}

void Comms::handle_hal_telemetry_active()
{
    auto& channel = m_channels->setup;

    if (!channel.begin_unpack())
    {
        QLOGE("Error in unpacking stream telemetry");
        return;
    }
    channel.end_unpack();

    bool is_active = false;
    if (channel.unpack_param(is_active))
    {
        QLOGI("Hal telemetry: {}", is_active ? "ON" : "OFF");
        m_hal_telemetry_data.is_enabled = is_active;
    }

    //respond
    channel.begin_pack(comms::Setup_Message::HAL_TELEMETRY_ACTIVE);
    channel.pack_param(m_hal_telemetry_data.is_enabled);
    channel.end_pack();
}

auto Comms::get_multi_commands_values() const -> std::vector<stream::IMulti_Commands::Value> const&
{
    return m_multi_commands_values;
}
void Comms::add_multi_state_sample(stream::IMulti_State::Sample const& sample)
{
    m_channels->pilot.pack_all(silk::comms::Pilot_Message::MULTI_STATE, sample);
}
void Comms::add_video_sample(stream::IVideo::Sample const& sample)
{
    m_channels->video.pack_all(silk::comms::Video_Message::FRAME_DATA, sample);
}

void Comms::handle_multi_commands()
{
    auto& channel = m_channels->pilot;

    stream::IMulti_Commands::Value value;
    if (!channel.unpack_all(value))
    {
        QLOGE("Error in unpacking multi commands");
        return;
    }

    m_multi_commands_values.push_back(value);
}

void Comms::process()
{
    if (!is_connected())
    {
        return;
    }

    m_multi_commands_values.clear();

    while (auto msg = m_channels->pilot.get_next_message(*m_rcp))
    {
        switch (msg.get())
        {
        case comms::Pilot_Message::MULTI_COMMANDS: handle_multi_commands(); break;

        default: QLOGE("Received unrecognised pilot message: {}", static_cast<int>(msg.get())); break;
        }
    }


    while (auto msg = m_channels->setup.get_next_message(*m_rcp))
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

        case comms::Setup_Message::STREAM_TELEMETRY_ACTIVE: handle_streams_telemetry_active(); break;
        case comms::Setup_Message::HAL_TELEMETRY_ACTIVE: handle_hal_telemetry_active(); break;

        default: QLOGE("Received unrecognised setup message: {}", static_cast<int>(msg.get())); break;
        }
    }

    gather_telemetry_data();

    auto result = m_socket->process();
    if (result != util::RCP_Socket::Result::OK)
    {
        m_rcp->reconnect();
    }


    m_rcp->process();

    m_channels->video.send(*m_rcp);
    m_channels->pilot.send(*m_rcp);

    auto now = q::Clock::now();
    if (now - m_last_rcp_tp >= RCP_PERIOD)
    {
        m_last_rcp_tp = now;

        pack_telemetry_data();

        m_channels->setup.send(*m_rcp);
        m_channels->telemetry.try_sending(*m_rcp);
    }

//    static std::vector<uint8_t> buf;
//    if (buf.empty())
//    {
//        buf.resize(16000000);
//        std::generate(buf.begin(), buf.end(), [](){ return rand() % 50;});
//    }

//    while(true)
//    {
//        m_rcp.send(12, buf.data(), buf.size());

//        m_rcp.process();
//        //m_channel.send(COMMS_CHANNEL);
//        static int xxx = 0;
//        LOG_INFO("{}", xxx);
//        xxx++;
//        std::this_thread::sleep_for(std::chrono::milliseconds(30));
//    }
}

