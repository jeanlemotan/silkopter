#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class ITransformer : public INode_Base<Type::TRANSFORMER>
{
public:
};
DECLARE_CLASS_PTR(ITransformer);


}
}

