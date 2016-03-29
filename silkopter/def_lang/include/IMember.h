#pragma once

#include "ISymbol.h"
#include "types/IType.h"
#include "values/IValue.h"
#include "attributes/IAttribute_Container.h"

namespace ts
{

class IMember : virtual public ISymbol, virtual public IAttribute_Container
{
public:
    virtual ~IMember() = default;

    virtual auto get_type() const -> std::shared_ptr<IType> = 0;
    virtual auto get_default_value() const -> std::shared_ptr<const IValue> = 0;

    virtual auto get_value() const -> std::shared_ptr<const IValue> = 0;
    virtual auto get_value() -> std::shared_ptr<IValue> = 0;
};

}
