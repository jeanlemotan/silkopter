#pragma once

#include "common/node/IProcessor.h"

namespace silk
{
namespace node
{

class IResampler : public IProcessor
{
    DEFINE_RTTI_CLASS(IResampler, IProcessor);
public:
    virtual ~IResampler() {}
};
DECLARE_CLASS_PTR(IResampler);


}
}

