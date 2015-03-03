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

auto HAL::get_source_defs()  -> Registry<node::source::GS_ISource>&
{
    return m_source_defs;
}
auto HAL::get_sink_defs()    -> Registry<node::sink::GS_ISink>&
{
    return m_sink_defs;
}
auto HAL::get_processor_defs()  -> Registry<node::processor::GS_IProcessor>&
{
    return m_processor_defs;
}

auto HAL::get_sources()  -> Registry<node::source::GS_ISource>&
{
    return m_sources;
}
auto HAL::get_sinks()    -> Registry<node::sink::GS_ISink>&
{
    return m_sinks;
}
auto HAL::get_processors()  -> Registry<node::processor::GS_IProcessor>&
{
    return m_processors;
}
auto HAL::get_streams()  -> Registry<node::stream::GS_IStream>&
{
    return m_streams;
}

}

