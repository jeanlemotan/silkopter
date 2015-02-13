#pragma once

#include "ISource.h"
#include "common/node/stream/ILocation.h"

namespace silk
{
namespace node
{
namespace source
{

class IGPS : public ISource
{
public:
    virtual ~IGPS() {}

    struct Config
    {
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_stream() const -> stream::ILocation const& = 0;
};

DECLARE_CLASS_PTR(IGPS);

}
}
}
