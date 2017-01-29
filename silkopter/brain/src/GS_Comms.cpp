#include "BrainStdAfx.h"
#include "GS_Comms.h"
#include "utils/Timed_Scope.h"

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
#include "common/stream/IMultirotor_State.h"
#include "common/stream/IMultirotor_Commands.h"

#include "common/node/IBrain.h"

#include "utils/comms/RCP.h"
#include "utils/comms/UDP_Socket.h"
#include "utils/comms/Channel.h"

#include "hal.def.h"
#include "gs_comms.def.h"
#include "messages.def.h"
#include "def_lang/JSON_Serializer.h"

#include <boost/asio.hpp>

namespace silk
{

constexpr uint8_t SETUP_CHANNEL = 10;
constexpr uint8_t TELEMETRY_CHANNEL = 11;

constexpr Clock::duration RCP_PERIOD = std::chrono::milliseconds(30);

GS_Comms::GS_Comms(HAL& hal)
    : m_hal(hal)
    , m_telemetry_channel(TELEMETRY_CHANNEL)
{
}

auto GS_Comms::start_udp(uint16_t send_port, uint16_t receive_port) -> bool
{
    try
    {
        auto s = new util::comms::UDP_Socket();
        m_socket.reset(s);
        m_rcp.reset(new util::comms::RCP());

        util::comms::RCP::Socket_Handle handle = m_rcp->add_socket(m_socket.get());
        if (handle >= 0)
        {
            m_rcp->set_internal_socket_handle(handle);
            m_rcp->set_socket_handle(SETUP_CHANNEL, handle);
            m_rcp->set_socket_handle(TELEMETRY_CHANNEL, handle);

            s->open(send_port, receive_port);
            s->start_listening();
//            s->set_send_endpoint(boost::asio::ip::address::from_string("127.0.0.1"), send_port);
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

void GS_Comms::configure_channels()
{
    {
        util::comms::RCP::Send_Params params;
        params.is_compressed = true;
        params.is_reliable = true;
        params.importance = 100;
        m_rcp->set_send_params(SETUP_CHANNEL, params);
    }
    {
        util::comms::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::seconds(999999);
        m_rcp->set_receive_params(SETUP_CHANNEL, params);
    }

    {
        util::comms::RCP::Send_Params params;
        params.is_compressed = false;
        params.is_reliable = true;
        params.importance = 90;
        m_rcp->set_send_params(TELEMETRY_CHANNEL, params);
    }
    {
        util::comms::RCP::Receive_Params params;
        params.max_receive_time = std::chrono::seconds(10);
        m_rcp->set_receive_params(TELEMETRY_CHANNEL, params);
    }}

auto GS_Comms::is_connected() const -> bool
{
    return m_is_connected;
}

template<class Stream> auto GS_Comms::gather_telemetry_stream(Stream_Telemetry_Data& ts, stream::IStream const& _stream) -> bool
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
            QLOGW("Too many samples accumulated in the telemetry buffer for stream {}: {}", ts.stream_path, ts.sample_count);
        }
        return true;
    }
    return false;
}

void GS_Comms::gather_telemetry_data()
{
    //first we gather samples and we send them at 30Hz. This improves bandwidth by reducing header overhead and allowing for better compression
    for (auto& ts: m_stream_telemetry_data)
    {
        auto stream = ts.stream.lock();
        if (stream)
        {
            if (gather_telemetry_stream<stream::IAcceleration>(ts, *stream) ||
                gather_telemetry_stream<stream::IENU_Acceleration>(ts, *stream) ||
                gather_telemetry_stream<stream::IECEF_Acceleration>(ts, *stream) ||

                gather_telemetry_stream<stream::IAngular_Velocity>(ts, *stream) ||
                gather_telemetry_stream<stream::IENU_Angular_Velocity>(ts, *stream) ||
                gather_telemetry_stream<stream::IECEF_Angular_Velocity>(ts, *stream) ||

                gather_telemetry_stream<stream::IMagnetic_Field>(ts, *stream) ||
                gather_telemetry_stream<stream::IENU_Magnetic_Field>(ts, *stream) ||
                gather_telemetry_stream<stream::IECEF_Magnetic_Field>(ts, *stream) ||

                gather_telemetry_stream<stream::ILinear_Acceleration>(ts, *stream) ||
                gather_telemetry_stream<stream::IENU_Linear_Acceleration>(ts, *stream) ||
                gather_telemetry_stream<stream::IECEF_Linear_Acceleration>(ts, *stream) ||

                gather_telemetry_stream<stream::IECEF_Position>(ts, *stream) ||

                gather_telemetry_stream<stream::IVelocity>(ts, *stream) ||
                gather_telemetry_stream<stream::IENU_Velocity>(ts, *stream) ||
                gather_telemetry_stream<stream::IECEF_Velocity>(ts, *stream) ||

                gather_telemetry_stream<stream::IGimbal_Frame>(ts, *stream) ||
                gather_telemetry_stream<stream::IFrame>(ts, *stream) ||
                gather_telemetry_stream<stream::IENU_Frame>(ts, *stream) ||

                gather_telemetry_stream<stream::IForce>(ts, *stream) ||
                gather_telemetry_stream<stream::IENU_Force>(ts, *stream) ||
                gather_telemetry_stream<stream::IECEF_Force>(ts, *stream) ||

                gather_telemetry_stream<stream::ITorque>(ts, *stream) ||
                gather_telemetry_stream<stream::IENU_Torque>(ts, *stream) ||
                gather_telemetry_stream<stream::IECEF_Torque>(ts, *stream) ||

                gather_telemetry_stream<stream::IPressure>(ts, *stream) ||
                gather_telemetry_stream<stream::IBattery_State>(ts, *stream) ||
                gather_telemetry_stream<stream::ICurrent>(ts, *stream) ||
                gather_telemetry_stream<stream::IVoltage>(ts, *stream) ||
                gather_telemetry_stream<stream::IDistance>(ts, *stream) ||
                gather_telemetry_stream<stream::IPWM>(ts, *stream) ||
                gather_telemetry_stream<stream::IGPS_Info>(ts, *stream) ||
                gather_telemetry_stream<stream::ITemperature>(ts, *stream) ||
                gather_telemetry_stream<stream::IADC>(ts, *stream) ||
                gather_telemetry_stream<stream::IFloat>(ts, *stream) ||
                gather_telemetry_stream<stream::IBool>(ts, *stream) ||
                gather_telemetry_stream<stream::IThrottle>(ts, *stream) ||
                gather_telemetry_stream<stream::IMultirotor_Commands>(ts, *stream) ||
                gather_telemetry_stream<stream::IMultirotor_State>(ts, *stream) ||
                gather_telemetry_stream<stream::IProximity>(ts, *stream) ||
                gather_telemetry_stream<stream::IVideo>(ts, *stream))
            {
                ;//nothing
            }
            else
            {
                QLOGE("Unrecognized stream type: {} / {}:{}", ts.stream_path, static_cast<int>(stream->get_type().get_semantic()), static_cast<int>(stream->get_type().get_space()));
            }
        }
    }


    //pack UAV telemetry
    HAL::Telemetry_Data const& telemetry_data = m_hal.get_telemetry_data();
    if (m_internal_telemetry_data.is_enabled && m_internal_telemetry_data.version != telemetry_data.version)
    {
        m_internal_telemetry_data.version = telemetry_data.version;
        m_internal_telemetry_data.sample_count++;
        size_t off = m_internal_telemetry_data.data.size();

        auto dt = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(telemetry_data.total_duration).count());
        util::serialization::serialize(m_internal_telemetry_data.data, dt, off);

        dt = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(telemetry_data.max_total_duration).count());
        util::serialization::serialize(m_internal_telemetry_data.data, dt, off);

        util::serialization::serialize(m_internal_telemetry_data.data, static_cast<uint32_t>(telemetry_data.nodes.size()), off);

        for (auto const& nt: telemetry_data.nodes)
        {
            auto const& node_name = nt.first;
            auto const& node_telemetry_data = nt.second;

            util::serialization::serialize(m_internal_telemetry_data.data, node_name, off);

            auto dt = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(node_telemetry_data.process_duration).count());
            util::serialization::serialize(m_internal_telemetry_data.data, dt, off);

            dt = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(node_telemetry_data.max_process_duration).count());
            util::serialization::serialize(m_internal_telemetry_data.data, dt, off);
        }
    }
}

void GS_Comms::pack_telemetry_data()
{
    for (auto& ts: m_stream_telemetry_data)
    {
        if (!ts.data.empty() && ts.sample_count > 0)
        {
            m_telemetry_channel.begin_pack();
            m_telemetry_channel.pack_param(ts.stream_path);
            m_telemetry_channel.pack_param(ts.stream_type);
            m_telemetry_channel.pack_param(ts.sample_count);
            m_telemetry_channel.pack_data(ts.data.data(), ts.data.size());
            m_telemetry_channel.end_pack();
        }
        ts.data.clear();
        ts.sample_count = 0;
    }

    if (m_internal_telemetry_data.is_enabled)
    {
        auto& t = m_internal_telemetry_data;
        if (!t.data.empty() && t.sample_count > 0)
        {
            m_telemetry_channel.begin_pack();
            m_telemetry_channel.pack_param(std::string("#hal"));
            m_telemetry_channel.pack_param(t.sample_count);
            m_telemetry_channel.pack_data(t.data.data(), t.data.size());
            m_telemetry_channel.end_pack();
        }
        t.data.clear();
        t.sample_count = 0;
    }
}

template<typename T>
void GS_Comms::serialize_and_send(size_t channel_idx, T const& message)
{
    TIMED_FUNCTION();
    if (m_rcp->is_connected())
    {
        ts::sz::Value sz_value = silk::gs_comms::serialize(message);
        ts::sz::to_json(m_json_buffer, sz_value, false);
        if (!m_rcp->send(channel_idx, m_json_buffer.data(), m_json_buffer.size()))
        {
            QLOGE("Failed to send message");
            return;
        }
    }
}

std::string const& GS_Comms::decode_json(std::string const& json_base64)
{
    m_base64_buffer.resize(q::util::compute_base64_max_decoded_size(json_base64.size()));
    auto last_it = q::util::decode_base64(json_base64.data(), json_base64.data() + json_base64.size(), m_base64_buffer.begin());
    m_base64_buffer.erase(last_it, m_base64_buffer.end());
    return m_base64_buffer;
}
std::string const& GS_Comms::encode_json(std::string const& json)
{
    m_base64_buffer.resize(q::util::compute_base64_encoded_size(json.size()));
    q::util::encode_base64(reinterpret_cast<uint8_t const*>(json.data()), json.size(), &m_base64_buffer[0]);

    {
        std::string buf;
        buf.resize(q::util::compute_base64_max_decoded_size(m_base64_buffer.size()));
        auto last_it = q::util::decode_base64(m_base64_buffer.data(), m_base64_buffer.data() + m_base64_buffer.size(), buf.begin());
        buf.erase(last_it, buf.end());
        QASSERT(json == buf);
    }

    return m_base64_buffer;
}


boost::variant<gs_comms::setup::Node_Data, gs_comms::setup::Error> GS_Comms::get_node_data(std::string const& name, node::INode const& node)
{
    TIMED_FUNCTION();
    gs_comms::setup::Node_Data node_data;

    node_data.set_name(name);
    node_data.set_type(static_cast<uint8_t>(node.get_type()));

    for (node::INode::Input const& input: node.get_inputs())
    {
        gs_comms::setup::Node_Data::Input node_data_input;
        node_data_input.set_name(input.name);
        node_data_input.set_space(static_cast<uint8_t>(input.type.get_space()));
        node_data_input.set_semantic(static_cast<uint8_t>(input.type.get_semantic()));
        node_data_input.set_rate(input.rate);
        node_data_input.set_stream_path(input.stream_path);
        node_data.get_inputs().push_back(std::move(node_data_input));
    }
    for (node::INode::Output const& output: node.get_outputs())
    {
        gs_comms::setup::Node_Data::Output node_data_output;
        node_data_output.set_name(output.name);
        node_data_output.set_space(static_cast<uint8_t>(output.stream->get_type().get_space()));
        node_data_output.set_semantic(static_cast<uint8_t>(output.stream->get_type().get_semantic()));
        node_data_output.set_rate(output.stream->get_rate());
        node_data.get_outputs().push_back(std::move(node_data_output));
    }

    ts::sz::Value sz_value = hal::serialize(hal::Poly<const hal::INode_Descriptor>(node.get_descriptor()));
    node_data.set_descriptor_data(encode_json(ts::sz::to_json(m_json_buffer, sz_value, false)));

    sz_value = hal::serialize(hal::Poly<const hal::INode_Config>(node.get_config()));
    node_data.set_config_data(encode_json((ts::sz::to_json(m_json_buffer, sz_value, false))));

    return std::move(node_data);
}

template<class Format_String, typename... Params>
gs_comms::setup::Error GS_Comms::make_error_response(uint32_t req_id, Format_String const& fmt, Params&&... params)
{
    gs_comms::setup::Error error;
    error.set_req_id(req_id);
    error.set_message(q::util::format<std::string>(fmt, std::forward<Params>(params)...));
    //QLOGE("Comms error: {}", error.get_message());
    return error;
}

void GS_Comms::handle_req(gs_comms::setup::Get_AST_Req const& req)
{
    TIMED_FUNCTION();
    QLOGI("Get_AST_Req {}", req.get_req_id());

    gs_comms::setup::Brain_Res response = gs_comms::setup::Get_AST_Res();
    gs_comms::setup::Get_AST_Res& res = boost::get<gs_comms::setup::Get_AST_Res>(response);

    res.set_req_id(req.get_req_id());
    res.set_data(encode_json(hal::get_ast_json()));

    serialize_and_send(SETUP_CHANNEL, response);
}

void GS_Comms::handle_req(gs_comms::setup::Set_Clock_Req const& req)
{
    TIMED_FUNCTION();
    QLOGI("Set_Clock_Req {}", req.get_req_id());

    gs_comms::setup::Brain_Res response;

    int64_t time_t_data = req.get_time();
#ifdef RASPBERRY_PI
    time_t time_s = time_t_data / 1000;
    if (stime(&time_s) != 0)
    {
        response = make_error_response(req.get_req_id(), "Failed to set time: {}", strerror(errno));
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }
#endif

    char mbstr[256] = {0};
    std::time_t t = std::time(nullptr);
    if (!std::strftime(mbstr, 100, "%e-%m-%Y-%H-%M-%S", std::localtime(&t)))
    {
        strcpy(mbstr, "<cannot format>");
    }
    QLOGI("Clock set, current time is: {}", mbstr);

    gs_comms::setup::Set_Clock_Res res;
    res.set_req_id(req.get_req_id());
    res.set_time(static_cast<uint64_t>(t) * 1000);
    response = res;

    serialize_and_send(SETUP_CHANNEL, response);
}

void GS_Comms::handle_req(gs_comms::setup::Set_UAV_Descriptor_Req const& req)
{
    TIMED_FUNCTION();
    QLOGI("Set_UAV_Descriptor_Req {}", req.get_req_id());

    gs_comms::setup::Brain_Res response;

    std::string const& json = decode_json(req.get_data());
    auto json_result = ts::sz::from_json(json);
    if (json_result != ts::success)
    {
        response = make_error_response(req.get_req_id(), "Cannot deserialize uav descriptor data: {}", json_result.error().what());
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }
    hal::Poly<const hal::IUAV_Descriptor> uav_descriptor;
    auto deserialize_result = hal::deserialize(uav_descriptor, json_result.payload());
    if (deserialize_result != ts::success)
    {
        response = make_error_response(req.get_req_id(), "Cannot deserialize uav descriptor json: {}", deserialize_result.error().what());
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }

    {
        auto result = m_hal.set_uav_descriptor(uav_descriptor.get_shared_ptr());
        if (result != ts::success)
        {
            response = make_error_response(req.get_req_id(), "Cannot set uav descriptor: {}", result.error().what());
            serialize_and_send(SETUP_CHANNEL, response);
            return;
        }
    }

    m_hal.save_settings();

    ts::sz::Value sz_value = hal::serialize(hal::Poly<const hal::IUAV_Descriptor>(m_hal.get_uav_descriptor()));

    response = gs_comms::setup::Set_UAV_Descriptor_Res();
    gs_comms::setup::Set_UAV_Descriptor_Res& res = boost::get<gs_comms::setup::Set_UAV_Descriptor_Res>(response);
    res.set_req_id(req.get_req_id());
    res.set_data(encode_json(ts::sz::to_json(m_json_buffer, sz_value, false)));

    serialize_and_send(SETUP_CHANNEL, response);
}

void GS_Comms::handle_req(gs_comms::setup::Get_UAV_Descriptor_Req const& req)
{
    TIMED_FUNCTION();
    QLOGI("Get_UAV_Descriptor_Req {}", req.get_req_id());

    gs_comms::setup::Brain_Res response = gs_comms::setup::Get_UAV_Descriptor_Res();
    gs_comms::setup::Get_UAV_Descriptor_Res& res = boost::get<gs_comms::setup::Get_UAV_Descriptor_Res>(response);

    ts::sz::Value sz_value = hal::serialize(hal::Poly<const hal::IUAV_Descriptor>(m_hal.get_uav_descriptor()));
    res.set_req_id(req.get_req_id());
    res.set_data(encode_json(ts::sz::to_json(m_json_buffer, sz_value, false)));

    serialize_and_send(SETUP_CHANNEL, response);
}

void GS_Comms::handle_req(gs_comms::setup::Get_Node_Defs_Req const& req)
{
    TIMED_FUNCTION();
    QLOGI("Get_Node_Defs_Req {}", req.get_req_id());

    gs_comms::setup::Brain_Res response = gs_comms::setup::Get_Node_Defs_Res();
    gs_comms::setup::Get_Node_Defs_Res& res = boost::get<gs_comms::setup::Get_Node_Defs_Res>(response);

//    //first disable all telemetry because the GS doesn't yet have all the streams
//    m_stream_telemetry_data.clear();
//    m_internal_telemetry_data.is_enabled = false;

    std::vector<HAL::Node_Factory::Info> nodes = m_hal.get_node_factory().create_all();

    res.set_req_id(req.get_req_id());

    res.get_node_def_datas().resize(nodes.size());
    for (size_t nidx = 0; nidx < nodes.size(); nidx++)
    {
        HAL::Node_Factory::Info const& n = nodes[nidx];
        gs_comms::setup::Node_Def_Data& node_data = res.get_node_def_datas()[nidx];

        node_data.set_name(n.name);
        node_data.set_type(static_cast<uint8_t>(n.ptr->get_type()));

        std::vector<node::INode::Input> inputs = n.ptr->get_inputs();
        std::vector<gs_comms::setup::Node_Def_Data::Input>& node_data_inputs = node_data.get_inputs();
        node_data_inputs.resize(inputs.size());
        for (size_t i = 0; i < inputs.size(); i++)
        {
            node::INode::Input const& input = inputs[i];
            gs_comms::setup::Node_Def_Data::Input& node_data_input = node_data_inputs[i];

            node_data_input.set_name(input.name);
            node_data_input.set_space(static_cast<uint8_t>(input.type.get_space()));
            node_data_input.set_semantic(static_cast<uint8_t>(input.type.get_semantic()));
        }

        std::vector<node::INode::Output> outputs = n.ptr->get_outputs();
        std::vector<gs_comms::setup::Node_Def_Data::Output>& node_data_outputs = node_data.get_outputs();
        node_data_outputs.resize(outputs.size());
        for (size_t i = 0; i < outputs.size(); i++)
        {
            node::INode::Output const& output = outputs[i];
            gs_comms::setup::Node_Def_Data::Output& node_data_output = node_data_outputs[i];

            node_data_output.set_name(output.name);
            node_data_output.set_space(static_cast<uint8_t>(output.stream->get_type().get_space()));
            node_data_output.set_semantic(static_cast<uint8_t>(output.stream->get_type().get_semantic()));
        }

        std::shared_ptr<const hal::INode_Descriptor> descriptor = n.ptr->get_descriptor();
        ts::sz::Value sz_value = hal::serialize(hal::Poly<const hal::INode_Descriptor>(descriptor));
        node_data.set_descriptor_data(encode_json(ts::sz::to_json(m_json_buffer, sz_value, false)));
    }

    serialize_and_send(SETUP_CHANNEL, response);
}

void GS_Comms::handle_req(gs_comms::setup::Remove_Node_Req const& req)
{
    TIMED_FUNCTION();
    QLOGI("Remove_Node_Req {}", req.get_req_id());

    gs_comms::setup::Brain_Res response;

    std::shared_ptr<node::INode> node = m_hal.get_node_registry().find_by_name<node::INode>(req.get_name());
    if (!node)
    {
        response = make_error_response(req.get_req_id(), "Cannot find node '{}'", req.get_name());
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }

    m_hal.remove_node(node);
    m_hal.save_settings();

    gs_comms::setup::Remove_Node_Res res;
    res.set_req_id(req.get_req_id());
    response = res;
    serialize_and_send(SETUP_CHANNEL, response);
}

void GS_Comms::handle_req(gs_comms::setup::Add_Node_Req const& req)
{
    TIMED_FUNCTION();
    QLOGI("Add_Node_Req {}", req.get_req_id());

    gs_comms::setup::Brain_Res response;

    std::string const& json = decode_json(req.get_descriptor_data());
    auto json_result = ts::sz::from_json(json);
    if (json_result != ts::success)
    {
        response = make_error_response(req.get_req_id(), "Cannot deserialize node {} descriptor data: {}", req.get_def_name(), json_result.error().what());
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }
    hal::Poly<const hal::INode_Descriptor> descriptor;
    auto deserialize_result = hal::deserialize(descriptor, json_result.payload());
    if (deserialize_result != ts::success)
    {
        response = make_error_response(req.get_req_id(), "Cannot deserialize node {} descriptor json: {}", req.get_def_name(), deserialize_result.error().what());
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }

    auto create_node_result = m_hal.create_node(req.get_def_name(), req.get_name(), *descriptor);
    if (create_node_result != ts::success)
    {
        response = make_error_response(req.get_req_id(), "Cannot create node {}: {}", req.get_def_name(), create_node_result.error().what());
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }
    m_hal.save_settings();

    gs_comms::setup::Add_Node_Res res;
    res.set_req_id(req.get_req_id());

    boost::variant<gs_comms::setup::Node_Data, gs_comms::setup::Error> result = get_node_data(req.get_name(), *create_node_result.payload());
    if (auto* error = boost::get<gs_comms::setup::Error>(&result))
    {
        response = std::move(*error);
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }

    res.set_node_data(std::move(boost::get<gs_comms::setup::Node_Data>(result)));
    response = std::move(res);
    serialize_and_send(SETUP_CHANNEL, response);
}

void GS_Comms::handle_req(gs_comms::setup::Get_Nodes_Req const& req)
{
    TIMED_FUNCTION();
    QLOGI("Get_Nodes_Req {}", req.get_req_id());

    gs_comms::setup::Brain_Res response;

    gs_comms::setup::Get_Nodes_Res res;
    res.set_req_id(req.get_req_id());

    std::string const& name = req.get_name();
    if (name.empty())
    {
        std::vector<HAL::Node_Registry::Item> const& nodes = m_hal.get_node_registry().get_all();

        for (HAL::Node_Registry::Item const& item: nodes)
        {
            boost::variant<gs_comms::setup::Node_Data, gs_comms::setup::Error> result = get_node_data(item.name, *item.ptr);
            if (auto* error = boost::get<gs_comms::setup::Error>(&result))
            {
                response = std::move(*error);
                serialize_and_send(SETUP_CHANNEL, response);
                return;
            }
            res.get_node_datas().push_back(std::move(boost::get<gs_comms::setup::Node_Data>(result)));
        }
    }
    else
    {
        std::shared_ptr<node::INode> node = m_hal.get_node_registry().find_by_name<node::INode>(name);
        if (!node)
        {
            response = make_error_response(req.get_req_id(), "Cannot find node '{}'", name);
            serialize_and_send(SETUP_CHANNEL, response);
            return;
        }

        boost::variant<gs_comms::setup::Node_Data, gs_comms::setup::Error> result = get_node_data(name, *node);
        if (auto* error = boost::get<gs_comms::setup::Error>(&result))
        {
            response = std::move(*error);
            serialize_and_send(SETUP_CHANNEL, response);
            return;
        }
        res.get_node_datas().push_back(std::move(boost::get<gs_comms::setup::Node_Data>(result)));
    }

    response = std::move(res);
    serialize_and_send(SETUP_CHANNEL, response);
}

void GS_Comms::handle_req(gs_comms::setup::Set_Node_Input_Stream_Path_Req const& req)
{
    TIMED_FUNCTION();
    QLOGI("Set_Node_Input_Stream_Path_Req {}", req.get_req_id());

    gs_comms::setup::Brain_Res response;
    gs_comms::setup::Set_Node_Input_Stream_Path_Res res;
    res.set_req_id(req.get_req_id());

    std::string const& node_name = req.get_node_name();
    std::string const& input_name = req.get_input_name();

    std::shared_ptr<node::INode> node = m_hal.get_node_registry().find_by_name<node::INode>(node_name);
    if (!node)
    {
        response = make_error_response(req.get_req_id(), "Cannot find node '{}'", node_name);
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }

    std::vector<node::INode::Input> inputs = node->get_inputs();
    for (size_t idx = 0; idx < inputs.size(); idx++)
    {
        const node::INode::Input& input = inputs[idx];
        if (input.name == input_name)
        {
            auto set_input_result = node->set_input_stream_path(idx, req.get_stream_path());
            if (set_input_result != ts::success)
            {
                response = make_error_response(req.get_req_id(), set_input_result.error().what());
                serialize_and_send(SETUP_CHANNEL, response);
                return;
            }
            m_hal.save_settings();

            boost::variant<gs_comms::setup::Node_Data, gs_comms::setup::Error> result = get_node_data(node_name, *node);
            if (auto* error = boost::get<gs_comms::setup::Error>(&result))
            {
                response = std::move(*error);
                serialize_and_send(SETUP_CHANNEL, response);
                return;
            }

            //all good!!!
            res.set_node_data(std::move(boost::get<gs_comms::setup::Node_Data>(result)));
            response = std::move(res);
            serialize_and_send(SETUP_CHANNEL, response);
            return;
        }
    }

    response = make_error_response(req.get_req_id(), "Cannot find node '{}', input '{}'", node_name, input_name);
    serialize_and_send(SETUP_CHANNEL, response);
}

void GS_Comms::handle_req(gs_comms::setup::Set_Stream_Telemetry_Enabled_Req const& req)
{
    TIMED_FUNCTION();
    QLOGI("Set_Stream_Telemetry_Enabled_Req {}", req.get_req_id());

    gs_comms::setup::Brain_Res response;
    gs_comms::setup::Set_Stream_Telemetry_Enabled_Res res;
    res.set_req_id(req.get_req_id());

    std::string const& stream_path = req.get_stream_path();
    bool wants_enabled = req.get_enabled();

    if (stream_path == "#hal")
    {
        m_internal_telemetry_data.is_enabled = wants_enabled;
    }
    else
    {
        std::shared_ptr<stream::IStream> stream = m_hal.get_stream_registry().find_by_name<stream::IStream>(stream_path);
        if (!stream)
        {
            response = make_error_response(req.get_req_id(), "Cannot find stream '{}'", stream_path);
            serialize_and_send(SETUP_CHANNEL, response);
            return;
        }

        auto it = std::find_if(m_stream_telemetry_data.begin(), m_stream_telemetry_data.end(), [&stream_path](Stream_Telemetry_Data const& st) { return st.stream_path == stream_path; });
        bool is_enabled = (it != m_stream_telemetry_data.end());
        if (wants_enabled != is_enabled)
        {
            if (wants_enabled)
            {
                Stream_Telemetry_Data std;
                std.stream_path = stream_path;
                std.stream_type = stream->get_type();
                std.stream = stream;
                m_stream_telemetry_data.push_back(std);
            }
            else
            {
                m_stream_telemetry_data.erase(it);
            }
        }
    }

    //all good!!!
    response = std::move(res);
    serialize_and_send(SETUP_CHANNEL, response);
}

void GS_Comms::handle_req(gs_comms::setup::Set_Node_Config_Req const& req)
{
    TIMED_FUNCTION();
    QLOGI("Set_Node_Config_Req {}", req.get_req_id());

    gs_comms::setup::Brain_Res response;
    gs_comms::setup::Set_Node_Config_Res res;
    res.set_req_id(req.get_req_id());

    std::string const& node_name = req.get_name();

    std::string const& json = decode_json(req.get_config_data());
    auto json_result = ts::sz::from_json(json);
    if (json_result != ts::success)
    {
        response = make_error_response(req.get_req_id(), "Cannot deserialize node '{}'' config data: {}", node_name, json_result.error().what());
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }
    hal::Poly<const hal::INode_Config> config;
    auto deserialize_result = hal::deserialize(config, json_result.payload());
    if (deserialize_result != ts::success)
    {
        response = make_error_response(req.get_req_id(), "Cannot deserialize node '{}'' config json: {}", node_name, deserialize_result.error().what());
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }

    std::shared_ptr<node::INode> node = m_hal.get_node_registry().find_by_name<node::INode>(node_name);
    if (!node)
    {
        response = make_error_response(req.get_req_id(), "Cannot find node '{}'", node_name);
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }

    auto set_config_result = node->set_config(*config);
    if (set_config_result != ts::success)
    {
        response = make_error_response(req.get_req_id(), "Cannot set config for node '{}': {}", node_name, set_config_result.error().what());
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }
    m_hal.save_settings();

    boost::variant<gs_comms::setup::Node_Data, gs_comms::setup::Error> result = get_node_data(node_name, *node);
    if (auto* error = boost::get<gs_comms::setup::Error>(&result))
    {
        response = std::move(*error);
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }

    //all good!!!
    res.set_node_data(std::move(boost::get<gs_comms::setup::Node_Data>(result)));
    response = std::move(res);
    serialize_and_send(SETUP_CHANNEL, response);
}

void GS_Comms::handle_req(gs_comms::setup::Send_Node_Message_Req const& req)
{
    TIMED_FUNCTION();
    QLOGI("Send_Node_Message_Req {}", req.get_req_id());

    gs_comms::setup::Brain_Res response;
    gs_comms::setup::Send_Node_Message_Res res;
    res.set_req_id(req.get_req_id());

    std::string const& node_name = req.get_name();

    std::string const& json = decode_json(req.get_message_data());
    auto json_result = ts::sz::from_json(json);
    if (json_result != ts::success)
    {
        response = make_error_response(req.get_req_id(), "Cannot deserialize node '{}'' config data: {}", node_name, json_result.error().what());
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }
    messages::Poly<messages::INode_Message> message;
    auto deserialize_result = messages::deserialize(message, json_result.payload());
    if (deserialize_result != ts::success)
    {
        response = make_error_response(req.get_req_id(), "Cannot deserialize node '{}'' message json: {}", node_name, deserialize_result.error().what());
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }

    std::shared_ptr<node::INode> node = m_hal.get_node_registry().find_by_name<node::INode>(node_name);
    if (!node)
    {
        response = make_error_response(req.get_req_id(), "Cannot find node '{}'", node_name);
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }

    auto send_message_result = node->send_message(*message);
    if (send_message_result != ts::success)
    {
        response = make_error_response(req.get_req_id(), "Cannot send message to node '{}': {}", node_name, send_message_result.error().what());
        serialize_and_send(SETUP_CHANNEL, response);
        return;
    }

    message = messages::Poly<messages::INode_Message>(send_message_result.payload());

    ts::sz::Value sz_value = messages::serialize(message);
    res.set_message_data(encode_json(ts::sz::to_json(m_json_buffer, sz_value, false)));

    //all good!!!
    response = std::move(res);
    serialize_and_send(SETUP_CHANNEL, response);
}

struct GS_Comms::Dispatch_Req_Visitor : boost::static_visitor<void>
{
    Dispatch_Req_Visitor(GS_Comms& comms) : m_comms(comms) {}
    template <typename T> void operator()(T const& t) const { m_comms.handle_req(t); }
    GS_Comms& m_comms;
};

void GS_Comms::process()
{
    if (!is_connected())
    {
        return;
    }

    Dispatch_Req_Visitor dispatcher(*this);
    while (m_rcp->receive(SETUP_CHANNEL, m_setup_buffer))
    {
        auto parse_result = ts::sz::from_json(m_setup_buffer.data(), m_setup_buffer.size());
        if (parse_result != ts::success)
        {
            QLOGE("Cannot parse setup req: {}", parse_result.error().what());
        }
        else
        {
            silk::gs_comms::setup::Brain_Req req;
            auto result = silk::gs_comms::deserialize(req, parse_result.payload());
            if (result != ts::success)
            {
                QLOGE("Cannot deserialize setup req: {}", result.error().what());
            }
            boost::apply_visitor(dispatcher, req);
        }
        m_setup_buffer.clear();
    }

    gather_telemetry_data();

    auto result = m_socket->process();
    if (result != util::comms::ISocket::Result::OK)
    {
        m_rcp->reconnect();
    }

    m_rcp->process();

    auto now = Clock::now();
    if (now - m_last_rcp_tp >= RCP_PERIOD)
    {
        m_last_rcp_tp = now;

        pack_telemetry_data();
        m_telemetry_channel.send(*m_rcp);
    }
}


}
