#pragma once

#include <boost/variant.hpp>
#include "def_lang/Serialization.h"
#include "def_lang/Result.h"

namespace ts
{
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

    Type get_type() const;
    std::string const& get_name() const;

    bool get_as_bool() const;
    int64_t get_as_integral() const;
    float get_as_float() const;
    double get_as_double() const;
    std::string const& get_as_string() const;

    std::string to_string() const;

    Result<serialization::Value> serialize() const;

private:
    Type m_type;
    std::string m_name;
    boost::variant<bool, int64_t, float, double, std::string> m_value;
};


}
}
