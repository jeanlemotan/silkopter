#pragma once

#include "ISink.h"
#include "common/node/stream/IPWM_Value.h"

namespace silk
{
namespace node
{

namespace sink
{


class IPWM : public ISink
{
public:
    virtual ~IPWM() {}

    virtual auto get_input_stream() -> stream::IPWM_Value& = 0;
};

}
}
}
