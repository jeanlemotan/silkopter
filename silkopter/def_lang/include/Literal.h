#pragma once

#include "ILiteral.h"

namespace ts
{

class Literal final : public virtual ILiteral
{
public:
    Literal(std::unique_ptr<const IValue> value);

    auto get_template_instantiation_string() const -> std::string;

    auto get_value() const -> std::shared_ptr<const IValue> override;

private:
    std::shared_ptr<const IValue> m_value;
};

}
