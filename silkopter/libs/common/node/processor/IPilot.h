#pragma once

#include "common/node/IProcessor.h"

namespace silk
{
namespace node
{

class IPilot : public IProcessor
{
    DEFINE_RTTI_CLASS(IPilot, IProcessor);
public:
    virtual ~IPilot() {}

};



}
}

