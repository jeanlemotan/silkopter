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

    virtual auto get_member_def() const -> std::shared_ptr<const IMember_Def> = 0;

    virtual auto get_value() const -> std::shared_ptr<const IValue> = 0;
    virtual auto get_value() -> std::shared_ptr<IValue> = 0;
};

}
