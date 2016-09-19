#include "def_lang/impl/Literal_Attribute.h"

namespace ts
{

Literal_Attribute::Literal_Attribute(std::string const& name, std::shared_ptr<IValue> value)
    : m_name(name)
    , m_value(std::move(value))
{
}
Literal_Attribute::~Literal_Attribute()
{
}
std::string Literal_Attribute::get_name() const
{
    return m_name;
}
IValue const& Literal_Attribute::get_value() const
{
    return *m_value;
}

}
