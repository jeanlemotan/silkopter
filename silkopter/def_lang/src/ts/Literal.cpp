#include "Literal.h"

#include "values/IValue.h"

namespace ts
{

Literal::Literal(std::unique_ptr<const IValue> value)
    : m_value(std::move(m_value))
{
}

auto Literal::get_template_instantiation_string() const -> std::string
{
    if (std::shared_ptr<const ITemplate_Argument> ta = std::dynamic_pointer_cast<const ITemplate_Argument>(m_value))
    {
        return ta->get_template_instantiation_string();
    }
    return std::string();
}

auto Literal::get_value() const -> std::shared_ptr<const IValue>
{
    return m_value;
}


}
