#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class IGenerator : public INode_Base<Type::GENERATOR>
{
public:
};
DECLARE_CLASS_PTR(IGenerator);


}
}

