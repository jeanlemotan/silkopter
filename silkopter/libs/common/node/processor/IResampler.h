#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class IResampler : public INode_Base<INode::Type::RESAMPLER>
{
public:
};
DECLARE_CLASS_PTR(IResampler);


}
}
