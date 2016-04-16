#pragma once

#include "ISymbol.h"

namespace ts
{

class IMember_Def;
class IValue;

class IMember
{
public:
    virtual ~IMember() = default;

    virtual auto get_member_def() const -> IMember_Def const& = 0;

    virtual auto get_value() const -> IValue const& = 0;
    virtual auto get_value() -> IValue& = 0;
};

}
