#pragma once

#include "ISource.h"
#include "common/node/stream/IVoltage.h"

namespace silk
{
namespace node
{
namespace source
{


class IVoltmeter : public ISource
{
public:
    virtual ~IVoltmeter() {}

    struct Config
    {
        float bias;
        float scale = 1;
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_stream() -> stream::IVoltage& = 0;
};

DECLARE_CLASS_PTR(IVoltmeter);

}
}
}
