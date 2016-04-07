#pragma once

#include "IType.h"

namespace ts
{

class ITemplate_Argument;

class ITemplated_Type : virtual public IType
{
public:
    virtual ~ITemplated_Type() = default;

    virtual auto init(std::vector<std::shared_ptr<ITemplate_Argument>> const& arguments) -> bool = 0;
};

}
