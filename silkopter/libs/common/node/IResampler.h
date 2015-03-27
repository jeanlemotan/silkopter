#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class IResampler : public INode_Base<Type::RESAMPLER>
{
public:
};
DECLARE_CLASS_PTR(IResampler);


}
}

