#include "def_lang/impl/Max_Attribute.h"

namespace ts
{

Max_Attribute::Max_Attribute(std::shared_ptr<IValue> value)
    : m_value(std::move(value))
{
}
Max_Attribute::~Max_Attribute()
{
}
std::string Max_Attribute::get_name() const
{
    return "max";
}
IValue const& Max_Attribute::get_max_value() const
{
    return *m_value;
}

}
