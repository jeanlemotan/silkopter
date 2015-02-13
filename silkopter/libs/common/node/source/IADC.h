#pragma once

#include "ISource.h"
#include "common/node/stream/IADC_Value.h"

namespace silk
{
namespace node
{
namespace source
{


class IADC : public ISource
{
public:
    virtual ~IADC() {}

    struct Config
    {
        float bias;
        float scale = 1;
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_stream() const -> stream::IADC_Value const& = 0;
};

DECLARE_CLASS_PTR(IADC);


}
}
}
