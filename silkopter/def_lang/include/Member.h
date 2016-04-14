#pragma once

#include "ISymbol.h"

namespace ts
{

class Member_Def;
class IValue;

class Member final
{
public:
    auto get_member_def() const -> std::shared_ptr<const Member_Def>;

    auto get_value() const -> IValue const&;
    auto get_value() -> IValue&;
};

}
