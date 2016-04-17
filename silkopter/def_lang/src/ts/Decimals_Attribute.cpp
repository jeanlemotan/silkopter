#include "impl/Decimals_Attribute.h"
#include "IInitializer.h"
#include "impl/All_INumeric_Values.h"

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
