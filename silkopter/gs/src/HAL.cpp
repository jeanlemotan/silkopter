#include "HAL.h"
#include "Comms.h"
#include "utils/Json_Util.h"
#include "utils/Timed_Scope.h"

#include "sz_math.hpp"
#include "sz_Multi_Config.hpp"

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/stringbuffer.h"


/////////////////////////////////////////////////////////////////////////////////////

namespace silk
{

///////////////////////////////////////////////////////////////

HAL::HAL(Comms& comms)
    : m_comms(comms)
{
    QLOG_TOPIC("hal");
}

HAL::~HAL()
{
}

auto HAL::get_remote_clock() const -> Manual_Clock const&
{
    return m_remote_clock;
}

auto HAL::get_node_defs() const  -> Registry<node::Node_Def> const&
{
    return m_node_defs;
}
auto HAL::get_nodes() const  -> Registry<node::Node> const&
{
    return m_nodes;
}
auto HAL::get_streams() const  -> Registry<node::stream::Stream> const&
{
    return m_streams;
}

auto HAL::get_multi_config() const -> boost::optional<config::Multi>
{
    return m_configs.multi;
}
void HAL::set_multi_config(config::Multi const& config)
{
//    Set_Multi_Config_Queue_Item item;
//    autojsoncxx::to_document(config, item.config);
//    m_multi_set_config_queue.push_back(std::move(item));
}

void HAL::add_node(std::string const& def_name,
                     std::string const& name,
                     rapidjson::Document&& init_params)
{
    auto& channel = m_comms.get_setup_channel();
    channel.begin_pack(comms::Setup_Message::ADD_NODE);
    channel.pack_param(m_comms.get_new_req_id());
    channel.pack_param(def_name);
    channel.pack_param(name);
    channel.pack_param(init_params);
    channel.end_pack();
}

void HAL::remove_node(node::Node_ptr node)
{
    auto& channel = m_comms.get_setup_channel();
    channel.begin_pack(comms::Setup_Message::REMOVE_NODE);
    channel.pack_param(m_comms.get_new_req_id());
    channel.pack_param(node->name);
    channel.end_pack();
}

void HAL::set_node_input_stream_path(node::Node_ptr node, std::string const& input_name, q::Path const& stream_path)
{
    auto it = std::find_if(node->input_streams.begin(), node->input_streams.end(), [input_name](node::Node::Stream_Input const& is) { return is.name == input_name; });
    if (it == node->input_streams.end())
    {
        QLOGE("Cannot find input stream '{}' for node '{}'", input_name, node->name);
        return;
    }

    uint32_t input_idx = static_cast<uint32_t>(std::distance(it, node->input_streams.begin()));

    auto& channel = m_comms.get_setup_channel();
    channel.begin_pack(comms::Setup_Message::REMOVE_NODE);
    channel.pack_param(m_comms.get_new_req_id());
    channel.pack_param(node->name);
    channel.pack_param(input_idx);
    channel.pack_param(stream_path);
    channel.end_pack();
}

void HAL::set_node_config(node::Node_ptr node, rapidjson::Document const& config)
{
    auto& channel = m_comms.get_setup_channel();
    channel.begin_pack(comms::Setup_Message::NODE_CONFIG);
    channel.pack_param(m_comms.get_new_req_id());
    channel.pack_param(node->name);
    channel.pack_param(config);
    channel.end_pack();
}

void HAL::send_node_message(node::Node_ptr node, rapidjson::Document const& json)
{
    auto& channel = m_comms.get_setup_channel();
    channel.begin_pack(comms::Setup_Message::NODE_MESSAGE);
    channel.pack_param(m_comms.get_new_req_id());
    channel.pack_param(node->name);
    channel.pack_param(json);
    channel.end_pack();
}

void HAL::set_stream_telemetry_active(std::string const& stream_name, bool active)
{
    auto stream = m_streams.find_by_name(stream_name);
    if (!stream)
    {
        QLOGE("Cannot find stream '{}'", stream_name);
        return;
    }
    if (stream->telemetry_active_req == 0 && !active)
    {
        QLOGW("Trying to disable stream '{}' but it's already disabled", stream_name);
        return;
    }

    stream->telemetry_active_req += active ? 1 : -1;

    //if the status will not change, it's ok. No need to do the request
    bool new_active = stream->telemetry_active_req > 0;
    if (new_active == stream->is_telemetry_active)
    {
        return;
    }

    auto& channel = m_comms.get_setup_channel();
    channel.begin_pack(comms::Setup_Message::STREAM_TELEMETRY_ACTIVE);
    channel.pack_param(m_comms.get_new_req_id());
    channel.pack_param(stream_name);
    channel.pack_param(new_active);
    channel.end_pack();
}




}

