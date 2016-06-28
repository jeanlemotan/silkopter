#include "def_lang/impl/Public_Attribute.h"

namespace ts
{

Public_Attribute::Public_Attribute(bool is_public)
    : m_is_public(is_public)
{
}
Public_Attribute::~Public_Attribute()
{
}
std::string Public_Attribute::get_name() const
{
    return "public";
}
bool Public_Attribute::is_public() const
{
    return m_is_public;
}

}
