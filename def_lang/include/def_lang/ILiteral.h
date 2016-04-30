#pragma once

#include <memory>
#include "ITemplate_Argument.h"
#include "IInitializer.h"

namespace ts
{

class IValue;

class ILiteral : public virtual ITemplate_Argument
{
public:
    virtual ~ILiteral() = default;

    virtual std::shared_ptr<const IValue> get_value() const = 0;
};

}
