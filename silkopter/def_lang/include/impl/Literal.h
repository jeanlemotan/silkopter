#pragma once

#include "ILiteral.h"

namespace ts
{

class Literal final : public virtual ILiteral
{
public:
    Literal(std::unique_ptr<const IValue> value);
    ~Literal();

    std::string get_template_instantiation_string() const;

    IValue const& get_value() const override;
    IValue const* get_initializer_value() const override;

private:
    std::unique_ptr<const IValue> m_value;
};

}
