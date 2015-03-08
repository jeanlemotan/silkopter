#pragma once

#include "common/node/IProcessor.h"

namespace silk
{
namespace node
{

class ILPF : public IProcessor
{
    DEFINE_RTTI_CLASS(ILPF, IProcessor);
public:
    virtual ~ILPF() {}
};
DECLARE_CLASS_PTR(ILPF);


}
}

