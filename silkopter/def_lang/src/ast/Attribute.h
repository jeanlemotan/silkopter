#pragma once

#include <boost/variant.hpp>

namespace ast
{

class Attribute
{
public:

    enum class Type
    {
        STRING,
        BOOL,
        INTEGRAL,
        FLOAT,
        DOUBLE
    };

    Attribute(std::string const& name, bool value);
    Attribute(std::string const& name, int64_t value);
    Attribute(std::string const& name, float value);
    Attribute(std::string const& name, double value);
    Attribute(std::string const& name, std::string const& value);
    Attribute(std::string const& name, char const* value);

    auto get_type() const -> Type;
    auto get_name() const -> std::string const&;

    auto get_as_bool() const -> bool;
    auto get_as_integral() const -> int64_t;
    auto get_as_float() const -> float;
    auto get_as_double() const -> double;
    auto get_as_string() const -> std::string const&;

    auto to_string() const -> std::string;

private:
    Type m_type;
    std::string m_name;
    boost::variant<bool, int64_t, float, double, std::string> m_value;
};


}
