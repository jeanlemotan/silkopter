#pragma once

#include "def_lang/ILiteral.h"

namespace ts
{

class IDeclaration_Scope;

class Literal final : public virtual ILiteral
{
public:
    Literal(IDeclaration_Scope const& scope, bool value);
    Literal(IDeclaration_Scope const& scope, int64_t value);
    Literal(IDeclaration_Scope const& scope, float value);
    Literal(IDeclaration_Scope const& scope, double value);
    Literal(IDeclaration_Scope const& scope, std::string const& value);
    Literal(std::shared_ptr<const IValue> value);
    ~Literal();

    std::string get_template_instantiation_string() const;

    std::shared_ptr<const IValue> get_value() const override;

private:
    std::shared_ptr<const IValue> m_value;
};

}
