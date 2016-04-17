#pragma once

#include <memory>
#include "ISymbol.h"

namespace ts
{

class IMember_Def;
class IValue;

class IMember
{
public:
    virtual ~IMember() = default;

    virtual std::shared_ptr<IMember_Def const> get_member_def() const = 0;

    virtual IValue const& get_value() const = 0;
    virtual IValue& get_value() = 0;
};

}
