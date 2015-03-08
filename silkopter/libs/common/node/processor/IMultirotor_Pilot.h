#pragma once

#include "common/node/IProcessor.h"

namespace silk
{
namespace node
{

class IMultirotor_Pilot : public IProcessor
{
    DEFINE_RTTI_CLASS(IMultirotor_Pilot, IProcessor);
public:
    virtual ~IMultirotor_Pilot() {}

};



}
}

