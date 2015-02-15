#pragma once

#include "ISource.h"
#include "common/node/stream/ITemperature.h"

namespace silk
{
namespace node
{
namespace source
{


class IThermometer : public ISource
{
public:
    virtual ~IThermometer() {}

    struct Config
    {
        float bias;
        float scale = 1;
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_stream() -> stream::ITemperature& = 0;
};


}
}
}
