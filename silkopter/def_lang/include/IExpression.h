#pragma once

#include <memory>
#include "IInitializer.h"

namespace ts
{

class IType;
class IValue;

class IExpression: public virtual IInitializer
{
public:
    virtual ~IExpression() = default;

    virtual auto get_type() const -> IType const& = 0;
    virtual auto evaluate() const -> std::unique_ptr<IValue> = 0;
};

}
