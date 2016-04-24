#include "ast/Attribute.h"

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
        return std::to_string(v) + "f";
    }
    template<typename T>
    std::string operator()(T v) const
    {
        return std::to_string(v);
    }
};

std::string Attribute::to_string() const
{
    return m_name + ":" + boost::apply_visitor(to_string_visitor(), m_value);
}

}
