#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class IProcessor : public INode
{
    DEFINE_RTTI_CLASS(IProcessor, INode);
public:
    virtual ~IProcessor() {}
};
DECLARE_CLASS_PTR(IProcessor);


}
}

