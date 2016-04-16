#include "impl/Max_Attribute.h"
#include "IInitializer.h"
#include "IValue.h"

namespace ts
{

Max_Attribute::Max_Attribute(std::unique_ptr<IValue> value)
    : m_value(std::move(value))
{
}
Max_Attribute::~Max_Attribute()
{
}
auto Max_Attribute::get_name() const -> std::string
{
    return "max";
}
auto Max_Attribute::get_max_value() const -> IValue const&
{
    return *m_value;
}

}
