#include "def_lang/ast/Attribute.h"

namespace ts
{
namespace ast
{

Attribute::Attribute(std::string const& name, bool value)
    : m_type(Type::BOOL)
    , m_name(name)
    , m_value(value)
{
}
Attribute::Attribute(std::string const& name, int64_t value)
    : m_type(Type::INTEGRAL)
    , m_name(name)
    , m_value(value)
{
}
Attribute::Attribute(std::string const& name, float value)
    : m_type(Type::FLOAT)
    , m_name(name)
    , m_value(value)
{
}
Attribute::Attribute(std::string const& name, double value)
    : m_type(Type::DOUBLE)
    , m_name(name)
    , m_value(value)
{
}
Attribute::Attribute(std::string const& name, std::string const& value)
    : m_type(Type::STRING)
    , m_name(name)
    , m_value(value)
{
}

Attribute::Attribute(std::string const& name, char const* value)
    : m_type(Type::STRING)
    , m_name(name)
    , m_value(std::string(value))
{
}

Attribute::Type Attribute::get_type() const
{
    return m_type;
}
std::string const& Attribute::get_name() const
{
    return m_name;
}

bool Attribute::get_as_bool() const
{
    return boost::get<bool>(m_value);
}
int64_t Attribute::get_as_integral() const
{
    return boost::get<int64_t>(m_value);
}
float Attribute::get_as_float() const
{
    return boost::get<float>(m_value);
}
double Attribute::get_as_double() const
{
    return boost::get<double>(m_value);
}
std::string const& Attribute::get_as_string() const
{
    return boost::get<std::string>(m_value);
}

class to_string_visitor : public boost::static_visitor<std::string>
{
public:
    std::string operator()(bool v) const
    {
        return v ? "true" : "false";
    }
    std::string operator()(std::string const& v) const
    {
        return v;
    }
    std::string operator()(float v) const
    {
        return std::to_string(v);
    }
    template<typename T>
    std::string operator()(T v) const
    {
        return std::to_string(v);
    }
};

static const char* type_to_string(Attribute::Type type)
{
#define CASE(X) case Attribute::Type::X: return #X; break;
    switch (type)
    {
    CASE(STRING);
    CASE(BOOL);
    CASE(INTEGRAL);
    CASE(FLOAT);
    CASE(DOUBLE);
    default: TS_ASSERT(false); return "<unknown>"; break;
    }
#undef CASE
}

std::string Attribute::to_string() const
{
    return m_name + ":" + type_to_string(m_type) + ":" + boost::apply_visitor(to_string_visitor(), m_value);
}


class serialize_visitor : public boost::static_visitor<serialization::Value>
{
public:
    template<typename T>
    serialization::Value operator()(T v) const
    {
        return serialization::Value(v);
    }
};

Result<serialization::Value> Attribute::serialize() const
{
    ts::serialization::Value sz_value(ts::serialization::Value::Type::OBJECT);

    sz_value.add_object_member("type", ts::serialization::Value(static_cast<uint8_t>(m_type)));
    sz_value.add_object_member("value", boost::apply_visitor(serialize_visitor(), m_value));

    return sz_value;
}


}
}
