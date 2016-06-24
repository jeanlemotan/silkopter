//#include "HAL.h"
//#include "Comms.h"
//#include "utils/Json_Util.h"
//#include "utils/Timed_Scope.h"

//#include "sz_math.hpp"
//#include "sz_Multi_Config.hpp"

//#include "rapidjson/document.h"     // rapidjson's DOM-style API
//#include "rapidjson/prettywriter.h" // for stringify JSON
//#include "rapidjson/stringbuffer.h"


///////////////////////////////////////////////////////////////////////////////////////

//namespace silk
//{

/////////////////////////////////////////////////////////////////

//HAL::HAL()
//{
//    QLOG_TOPIC("hal");
//}

//HAL::~HAL()
//{
//}

//void HAL::init(Comms& comms)
//{
//    m_comms = &comms;
//}

//auto HAL::get_remote_clock() const -> Manual_Clock const&
//{
//    return m_remote_clock;
//}

//auto HAL::get_node_defs() const  -> Registry<node::gs::Node_Def> const&
//{
//    return m_node_defs;
//}
//auto HAL::get_nodes() const  -> Registry<node::gs::Node> const&
//{
//    return m_nodes;
//}
//auto HAL::get_streams() const  -> Registry<stream::gs::Stream> const&
//{
//    return m_streams;
//}

//auto HAL::get_multi_config() const -> boost::optional<config::Multi>
//{
//    return m_configs.multi;
//}
//void HAL::set_multi_config(config::Multi const& config)
//{
//    rapidjson::Document configj;
//    configj.SetObject();
//    autojsoncxx::to_document(config, configj);

//    auto& channel = m_comms->get_setup_channel();
//    channel.begin_pack(comms::Setup_Message::MULTI_CONFIG);
//    channel.pack_param(configj);
//    channel.end_pack();
//}

//void HAL::add_node(std::string const& def_name,
//                     std::string const& name,
//                     rapidjson::Document const& init_params)
//{
//    auto& channel = m_comms->get_setup_channel();
//    channel.begin_pack(comms::Setup_Message::ADD_NODE);
//    channel.pack_param(def_name);
//    channel.pack_param(name);
//    channel.pack_param(init_params);
//    channel.end_pack();
//}

//void HAL::remove_node(node::gs::Node_ptr node)
//{
//    auto& channel = m_comms->get_setup_channel();
//    channel.begin_pack(comms::Setup_Message::REMOVE_NODE);
//    channel.pack_param(node->name);
//    channel.end_pack();
//}

//void HAL::set_node_input_stream_path(node::gs::Node_ptr node, std::string const& input_name, q::Path const& stream_path)
//{
//    auto it = std::find_if(node->inputs.begin(), node->inputs.end(), [input_name](node::gs::Node::Input const& is) { return is.name == input_name; });
//    if (it == node->inputs.end())
//    {
//        QLOGE("Cannot find input stream '{}' for node '{}'", input_name, node->name);
//        return;
//    }

//    uint32_t input_idx = static_cast<uint32_t>(std::distance(node->inputs.begin(), it));

//    auto& channel = m_comms->get_setup_channel();
//    channel.begin_pack(comms::Setup_Message::NODE_INPUT_STREAM_PATH);
//    channel.pack_param(node->name);
//    channel.pack_param(input_idx);
//    channel.pack_param(stream_path.get_as<std::string>());
//    channel.end_pack();
//}

//void HAL::set_node_config(node::gs::Node_ptr node, rapidjson::Document const& config)
//{
//    auto& channel = m_comms->get_setup_channel();
//    channel.begin_pack(comms::Setup_Message::NODE_CONFIG);
//    channel.pack_param(node->name);
//    channel.pack_param(config);
//    channel.end_pack();
//}

//void HAL::send_node_message(node::gs::Node_ptr node, rapidjson::Document const& json)
//{
//    auto& channel = m_comms->get_setup_channel();
//    channel.begin_pack(comms::Setup_Message::NODE_MESSAGE);
//    channel.pack_param(node->name);
//    channel.pack_param(json);
//    channel.end_pack();
//}

//void HAL::set_stream_telemetry_active(std::string const& stream_name, bool active)
//{
//    auto stream = m_streams.find_by_name(stream_name);
//    if (!stream)
//    {
//        QLOGW("Cannot find stream '{}'", stream_name);
//        return;
//    }
//    if (stream->telemetry_active_req == 0 && !active)
//    {
//        QLOGW("Trying to disable stream '{}' but it's already disabled", stream_name);
//        return;
//    }

//    stream->telemetry_active_req += active ? 1 : -1;

//    //if the status will not change, it's ok. No need to do the request
//    bool new_active = stream->telemetry_active_req > 0;
//    if (new_active == stream->is_telemetry_active)
//    {
//        return;
//    }

//    auto& channel = m_comms->get_setup_channel();
//    channel.begin_pack(comms::Setup_Message::STREAM_TELEMETRY_ACTIVE);
//    channel.pack_param(stream_name);
//    channel.pack_param(new_active);
//    channel.end_pack();
//}




//}

