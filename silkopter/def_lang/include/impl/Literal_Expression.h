#pragma once

#include "IExpression.h"


namespace ts
{

class ILiteral;

class Literal_Expression final : public virtual IExpression
{
public:

    Literal_Expression(std::unique_ptr<const ILiteral> literal);
    ~Literal_Expression();

    std::unique_ptr<IValue> evaluate() const override;

private:
    std::unique_ptr<const ILiteral> m_literal;
};

}
