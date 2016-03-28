#pragma once

#include "INode.h"

namespace ast
{

class IMember_Container : virtual public INode
{
public:
    virtual ~IMember_Container() = default;
};

}
