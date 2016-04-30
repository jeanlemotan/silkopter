#pragma once

#include "def_lang/ILiteral_Initializer.h"

namespace ts
{

class Literal_Initializer : public virtual ILiteral_Initializer
{
public:
    Literal_Initializer(std::shared_ptr<const ILiteral> literal);

    std::shared_ptr<const ILiteral> get_literal() const override;

private:
    std::shared_ptr<const ILiteral> m_literal;
};

}
