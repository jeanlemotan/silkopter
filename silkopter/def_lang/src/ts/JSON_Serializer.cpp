#include "JSON_Serializer.h"

namespace ts
{

Result<void> JSON_Serializer::begin_object(std::string const& name, size_t reserve)
{
    if (name.empty())
    {
        return Error("Cannot add an unnamed object");
    }
    m_objects_started++;

    m_json += std::string(m_ident, '\t');
    m_json += "\"";
    m_json += name;
    m_json += "\"= {\n";
    m_ident++;
    return success;
}

Result<void> JSON_Serializer::end_object()
{
    if (m_objects_started == 0)
    {
        return Error("No object started");
    }
    TS_ASSERT(m_ident > 0);
    m_ident--;
    m_json += std::string(m_ident, '\t');
    m_json += "}\n";

    m_objects_started--;
    return success;
}

Result<void> JSON_Serializer::add_member(std::string const& name, Value const& value)
{
    if (name.empty())
    {
        return Error("Cannot add an unnamed member");
    }
    if (m_objects_started == 0)
    {
        return Error("No object started");
    }
    m_json += std::string(m_ident, '\t');
    m_json += "\"";
    m_json += name;
    m_json += "\"=";
    m_json += value.to_string();
    m_json += "\n";
    return success;
}

Result<void> JSON_Serializer::begin_array(size_t reserve)
{
    m_arrays_started++;

    m_json += std::string(m_ident, '\t');
    m_json += "[\n";
    m_ident++;
    return success;
}

Result<void> JSON_Serializer::end_array()
{
    if (m_arrays_started == 0)
    {
        return Error("No array started");
    }
    TS_ASSERT(m_ident > 0);
    m_ident--;
    m_json += std::string(m_ident, '\t');
    m_json += "]\n";

    m_arrays_started--;
    return success;
}

Result<void> JSON_Serializer::add_element(Value const& value)
{
    if (m_arrays_started == 0)
    {
        return Error("No array started");
    }
    m_json += std::string(m_ident, '\t');
    m_json += value.to_string();
    m_json += "\n";
    return success;
}

std::string JSON_Serializer::to_string() const
{
    return m_json;
}


}
