#pragma once

#include "ISymbol.h"

namespace ts
{

class IType;
class IValue;

class IConstant : virtual public ISymbol
{
public:
    virtual auto get_type() const -> std::shared_ptr<IType> = 0;
    virtual auto get_value() const -> std::shared_ptr<const IValue> = 0;
};

}
