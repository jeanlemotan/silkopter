#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class ISource : public INode_Base<INode::Type::SOURCE>
{
public:
    //sources don't have any inputs
    virtual auto get_inputs() const -> std::vector<Input> final { return std::vector<Input>(); }
};
DECLARE_CLASS_PTR(ISource);


}
}

