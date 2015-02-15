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

    struct Config
    {
        float min = 0;
        float max = 1;
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

//    virtual void set_value(float value) = 0;
    virtual auto get_stream() -> stream::IPWM_Value& = 0;
};

}
}
}
