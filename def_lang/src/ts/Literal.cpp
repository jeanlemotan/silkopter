#include "def_lang/impl/Literal.h"
#include "def_lang/IValue.h"

namespace ts
{

Literal::Literal(std::shared_ptr<const IValue> value)
    : m_value(std::move(value))
{
}

Literal::~Literal()
{

}

std::string Literal::get_template_instantiation_string() const
{
    if (ITemplate_Argument const* ta = dynamic_cast<ITemplate_Argument const*>(m_value.get()))
    {
        return ta->get_template_instantiation_string();
    }
    return std::string();
}

IValue const& Literal::get_value() const
{
    return *m_value;
}

IValue const* Literal::get_initializer_value() const
{
    return m_value.get();
}

}
