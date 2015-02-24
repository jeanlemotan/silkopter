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

auto HAL::get_sources()  -> Registry<node::source::ISource>&
{
    return m_sources;
}
auto HAL::get_sinks()    -> Registry<node::sink::ISink>&
{
    return m_sinks;
}
auto HAL::get_processors()  -> Registry<node::processor::IProcessor>&
{
    return m_processors;
}
auto HAL::get_streams()  -> Registry<node::stream::IStream>&
{
    return m_streams;
}


}

