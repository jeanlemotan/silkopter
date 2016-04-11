#include "attributes/Decimals_Attribute.h"
#include "IInitializer.h"
#include "values/All_INumeric_Values.h"

namespace ts
{

Decimals_Attribute::Decimals_Attribute(size_t decimals)
    : m_decimals(decimals)
{

}
Decimals_Attribute::~Decimals_Attribute()
{

}
auto Decimals_Attribute::get_name() const -> std::string
{
    return "decimals";
}
auto Decimals_Attribute::get_decimals() const -> size_t
{
    return m_decimals;
}

}
