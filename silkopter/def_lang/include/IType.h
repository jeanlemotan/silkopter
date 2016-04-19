#pragma once

#include "ISymbol.h"
#include "IAttribute_Container.h"
#include "ITemplate_Argument.h"

namespace ts
{

class IValue;

class IType : virtual public ISymbol, virtual public IAttribute_Container, virtual public ITemplate_Argument
{
public:
    virtual ~IType() = default;

    virtual std::shared_ptr<IType> clone(std::string const& name) const = 0;

    virtual std::shared_ptr<IValue> create_value() const = 0;
};

}
