#pragma once

#include "def_lang/IExpression.h"


namespace ts
{

class ILiteral;

class Literal_Expression final : public virtual IExpression
{
public:

    Literal_Expression(std::shared_ptr<const ILiteral> literal);
    ~Literal_Expression();

    std::shared_ptr<IValue> evaluate() const override;

private:
    std::shared_ptr<const ILiteral> m_literal;
};

}
