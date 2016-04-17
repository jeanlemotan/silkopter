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

    virtual IValue const& get_value() const = 0;
};

}
