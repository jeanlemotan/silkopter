#pragma once

#include "ISymbol.h"
#include "attributes/IAttribute_Container.h"

namespace ts
{

class IValue;

class IType : virtual public ISymbol, virtual public IAttribute_Container
{
public:
    virtual ~IType() = default;

    virtual auto clone(std::string const& name) const -> std::unique_ptr<IType> = 0;

    virtual auto get_default_value() const -> std::shared_ptr<const IValue> = 0;

    virtual auto create_value() const -> std::unique_ptr<IValue> = 0;
};

}
