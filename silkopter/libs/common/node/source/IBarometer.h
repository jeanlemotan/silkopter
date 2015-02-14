#pragma once

#include "ISource.h"
#include "common/node/stream/IPressure.h"

namespace silk
{
namespace node
{
namespace source
{

class IBarometer : public ISource
{
public:
    virtual ~IBarometer() {}

    struct Config
    {
        float bias;
        float scale = 1;
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_stream() -> stream::IPressure& = 0;
};

DECLARE_CLASS_PTR(IBarometer);

}
}
}
