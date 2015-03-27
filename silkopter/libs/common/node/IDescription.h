#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class IDescription : public INode_Base<Type::DESCRIPTION>
{
public:
};
DECLARE_CLASS_PTR(IDescription);


}
}

