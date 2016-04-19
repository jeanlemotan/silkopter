#include "impl/Literal_Expression.h"
#include "IValue.h"
#include "ILiteral.h"

namespace ts
{

Literal_Expression::Literal_Expression(std::shared_ptr<const ILiteral> literal)
    : m_literal(std::move(literal))
{

}

Literal_Expression::~Literal_Expression()
{

}

std::shared_ptr<IValue> Literal_Expression::evaluate() const
{
    return m_literal->get_value().clone();
}

}
