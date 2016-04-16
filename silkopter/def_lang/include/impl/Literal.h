#pragma once

#include "ILiteral.h"

namespace ts
{

class Literal final : public virtual ILiteral
{
public:
    Literal(std::unique_ptr<const IValue> value);
    ~Literal();

    auto get_template_instantiation_string() const -> std::string;

    auto get_value() const -> IValue const& override;
    auto get_initializer_value() const -> IValue const* override;

private:
    std::unique_ptr<const IValue> m_value;
};

}
