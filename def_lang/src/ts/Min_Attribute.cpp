#include "def_lang/impl/Min_Attribute.h"
#include "def_lang/IInitializer.h"
#include "def_lang/IValue.h"

namespace ts
{

Min_Attribute::Min_Attribute(std::shared_ptr<IValue> value)
    : m_value(std::move(value))
{
}
Min_Attribute::~Min_Attribute()
{
}
std::string Min_Attribute::get_name() const
{
    return "min";
}
IValue const& Min_Attribute::get_min_value() const
{
    return *m_value;
}

}
