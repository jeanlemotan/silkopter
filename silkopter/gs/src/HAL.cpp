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
auto HAL::get_sink_defs() const    -> Registry<node::sink::Sink> const&
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

void HAL::add_source(node::source::Source_ptr node, Add_Source_Callback callback)
{
    QASSERT(callback && node && node->def);
    if (!callback || !node || !node->def)
    {
        return;
    }
    Add_Source_Queue_Item item;
    item.node = node;
    item.callback = callback;
    m_add_source_queue.push_back(item);
}

void HAL::add_processor(node::processor::Processor_ptr node, Add_Processor_Callback callback)
{
    QASSERT(callback && node && node->def);
    if (!callback || !node || !node->def)
    {
        return;
    }
    Add_Processor_Queue_Item item;
    item.node = node;
    item.callback = callback;
    m_add_processor_queue.push_back(item);
}


}

