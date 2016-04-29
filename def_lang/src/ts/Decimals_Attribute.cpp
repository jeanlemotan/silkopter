#include "def_lang/impl/Decimals_Attribute.h"

namespace ts
{

Decimals_Attribute::Decimals_Attribute(size_t decimals)
    : m_decimals(decimals)
{

}
Decimals_Attribute::~Decimals_Attribute()
{

}
std::string Decimals_Attribute::get_name() const
{
    return "decimals";
}
size_t Decimals_Attribute::get_decimals() const
{
    return m_decimals;
}

}
