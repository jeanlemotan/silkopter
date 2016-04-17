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

    virtual std::shared_ptr<IType const> get_type() const = 0;
    virtual std::unique_ptr<IValue> evaluate() const = 0;
};

}
