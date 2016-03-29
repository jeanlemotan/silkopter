#pragma once

#include "types/IType.h"
#include "values/IValue.h"

namespace ts
{

class ILiteral
{
public:
    virtual ~ILiteral() = default;

    virtual auto get_type() const -> std::shared_ptr<IType> = 0;
    virtual auto get_value() const -> std::shared_ptr<const IValue> = 0;
};

}
