#include "expression/Literal_Expression.h"
#include "values/IValue.h"
#include "ILiteral.h"

namespace ts
{

Literal_Expression::Literal_Expression(std::unique_ptr<const ILiteral> literal)
    : m_literal(std::move(literal))
{

}

Literal_Expression::~Literal_Expression()
{

}

auto Literal_Expression::evaluate() const -> std::unique_ptr<IValue>
{
    return m_literal->get_value()->clone();
}

}
