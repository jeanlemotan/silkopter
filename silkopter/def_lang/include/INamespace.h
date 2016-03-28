#pragma once

#include "INode.h"

namespace ast
{

class INamespace : virtual public INode
{
public:
    virtual ~INamespace() = default;
};

}
