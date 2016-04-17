#pragma once

#include <memory>
#include "ITemplate_Argument.h"
#include "IInitializer.h"

namespace ts
{

class IValue;

class ILiteral : public virtual ITemplate_Argument, public virtual IInitializer
{
public:
    virtual ~ILiteral() = default;

    virtual IValue const& get_value() const = 0;
};

}
