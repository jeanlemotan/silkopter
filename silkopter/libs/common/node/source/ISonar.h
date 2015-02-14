#pragma once

#include "ISource.h"
#include "common/node/stream/IDistance.h"

namespace silk
{
namespace node
{
namespace source
{


class ISonar : public ISource
{
public:
    virtual ~ISonar() {}

    struct Config
    {
        float bias;
        float scale = 1;
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_stream() -> stream::IDistance& = 0;
};

DECLARE_CLASS_PTR(ISonar);


}
}
}
