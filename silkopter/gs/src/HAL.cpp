#include "stdafx.h"
#include "HAL.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/stringbuffer.h"


/////////////////////////////////////////////////////////////////////////////////////

namespace silk
{

///////////////////////////////////////////////////////////////

HAL::HAL()
{
    QLOG_TOPIC("hal");
}

HAL::~HAL()
{
}

auto HAL::get_source_defs() const  -> Registry<node::source::Source_Def> const&
{
    return m_source_defs;
}
auto HAL::get_sink_defs() const    -> Registry<node::sink::Sink_Def> const&
{
    return m_sink_defs;
}
auto HAL::get_processor_defs() const  -> Registry<node::processor::Processor_Def> const&
{
    return m_processor_defs;
}

auto HAL::get_sources() const  -> Registry<node::source::Source> const&
{
    return m_sources;
}
auto HAL::get_sinks() const    -> Registry<node::sink::Sink> const&
{
    return m_sinks;
}
auto HAL::get_processors() const  -> Registry<node::processor::Processor> const&
{
    return m_processors;
}
auto HAL::get_streams() const  -> Registry<node::stream::GS_IStream> const&
{
    return m_streams;
}

void HAL::add_source(std::string const& def_name,
                     std::string const& name,
                     rapidjson::Document&& init_params,
                     Add_Source_Callback callback)
{
    QASSERT(callback);
    if (!callback)
    {
        return;
    }
    Add_Queue_Item item;
    item.def_name = def_name;
    item.name = name;
    item.init_params = std::move(init_params);
    item.source_callback = callback;
    m_add_queue.push_back(std::move(item));
}

void HAL::add_sink(std::string const& def_name,
                     std::string const& name,
                     rapidjson::Document&& init_params,
                     Add_Sink_Callback callback)
{
    QASSERT(callback);
    if (!callback)
    {
        return;
    }
    Add_Queue_Item item;
    item.def_name = def_name;
    item.name = name;
    item.init_params = std::move(init_params);
    item.sink_callback = callback;
    m_add_queue.push_back(std::move(item));
}

void HAL::add_processor(std::string const& def_name,
                     std::string const& name,
                     rapidjson::Document&& init_params,
                     Add_Processor_Callback callback)
{
    QASSERT(callback);
    if (!callback)
    {
        return;
    }
    Add_Queue_Item item;
    item.def_name = def_name;
    item.name = name;
    item.init_params = std::move(init_params);
    item.processor_callback = callback;
    m_add_queue.push_back(std::move(item));
}

void HAL::connect_input(node::processor::Processor_ptr node, std::string const& input_name, std::string const& stream_name, Connect_Callback callback)
{
    QASSERT(callback);
    if (!callback)
    {
        return;
    }

    auto document = jsonutil::clone_value(node->config);

    q::Path path("inputs");
    path += input_name;

    auto* value = jsonutil::find_value(document, path);
    if (!value)
    {
        callback(Result::FAILED);
        return;
    }

    value->SetString(stream_name.c_str(), document.GetAllocator());

    Connect_Queue_Item item;
    item.message = silk::comms::Setup_Message::PROCESSOR_CONFIG;
    item.name = node->name;
    item.config = std::move(document);
    item.callback = callback;
    m_connect_queue.push_back(std::move(item));
}



}

