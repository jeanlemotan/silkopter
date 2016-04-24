#pragma once

#include "Result.h"
#include "IType.h"

namespace ts
{

class ITemplate_Argument;

class ITemplated_Type : virtual public IType
{
public:
    virtual ~ITemplated_Type() = default;

    virtual Result<void> init(std::vector<std::shared_ptr<const ITemplate_Argument>> const& arguments) = 0;
};

}
