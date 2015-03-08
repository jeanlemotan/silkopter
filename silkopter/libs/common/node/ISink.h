#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class ISink : public INode
{
    DEFINE_RTTI_CLASS(ISink, INode);
public:
    virtual ~ISink() {}

    //sinks don't have any outputs
    virtual auto get_outputs() const -> std::vector<Output> final { return std::vector<Output>(); }
};
DECLARE_CLASS_PTR(ISink);


}
}

