#pragma once

#include "ISymbol.h"

namespace ts
{

class IType;
class IValue;

class IConstant : virtual public ISymbol
{
public:
    virtual ~IConstant() = default;

    virtual auto get_value() const -> IValue const& = 0;
};

}
