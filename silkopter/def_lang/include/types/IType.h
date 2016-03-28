#pragma once

#include "INode.h"

namespace ast
{

class IType : virtual public INode
{
public:
    virtual ~IType() = default;

    virtual std::unique_ptr<IValue> create_value();
};

}
