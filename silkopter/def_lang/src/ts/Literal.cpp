#include "impl/Literal.h"
#include "IValue.h"

namespace ts
{

Literal::Literal(std::unique_ptr<const IValue> value)
    : m_value(std::move(value))
{
}

Literal::~Literal()
{

}

auto Literal::get_template_instantiation_string() const -> std::string
{
    if (ITemplate_Argument const* ta = dynamic_cast<ITemplate_Argument const*>(m_value.get()))
    {
        return ta->get_template_instantiation_string();
    }
    return std::string();
}

auto Literal::get_value() const -> IValue const&
{
    return *m_value;
}

auto Literal::get_initializer_value() const -> IValue const*
{
    return m_value.get();
}

}
