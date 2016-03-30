#pragma once

#include "ISymbol.h"

namespace ts
{

class IType;
class IValue;

class IMember_Def : virtual public ISymbol
{
public:
    virtual ~IMember_Def() = default;

    virtual auto get_type() const -> std::shared_ptr<IType> = 0;
    virtual auto get_default_value() const -> std::shared_ptr<const IValue> = 0;
};

}
