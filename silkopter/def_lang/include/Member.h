#pragma once

#include "ISymbol.h"

namespace ts
{

class Member_Def;
class IValue;

class Member final
{
public:
    virtual auto get_member_def() const -> std::shared_ptr<const Member_Def> = 0;

    virtual auto get_value() const -> IValue const& = 0;
    virtual auto get_value() -> IValue& = 0;
};

}
