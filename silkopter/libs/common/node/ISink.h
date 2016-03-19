#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class ISink : public Node_Base<Type::SINK>
{
public:
    //sinks don't have any outputs
    virtual auto get_outputs() const -> std::vector<Output> final { return std::vector<Output>(); }
};


}
}

