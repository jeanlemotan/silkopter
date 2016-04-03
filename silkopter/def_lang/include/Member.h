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

    virtual auto get_value() const -> std::shared_ptr<const IValue> = 0;
    virtual auto get_value() -> std::shared_ptr<IValue> = 0;
};

}
