#include "Attribute.h"

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

auto Attribute::get_type() const -> Type
{
    return m_type;
}
auto Attribute::get_name() const -> std::string const&
{
    return m_name;
}

auto Attribute::get_as_bool() const -> bool
{
    return boost::get<bool>(m_value);
}
auto Attribute::get_as_integral() const -> int64_t
{
    return boost::get<int64_t>(m_value);
}
auto Attribute::get_as_float() const -> float
{
    return boost::get<float>(m_value);
}
auto Attribute::get_as_double() const -> double
{
    return boost::get<double>(m_value);
}
auto Attribute::get_as_string() const -> std::string const&
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
    template<typename T>
    std::string operator()(T v) const
    {
        return std::to_string(v);
    }
};

auto Attribute::to_string() const -> std::string
{
    return m_name + ":" + boost::apply_visitor(to_string_visitor(), m_value);
}

}
