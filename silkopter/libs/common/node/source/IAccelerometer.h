#pragma once

#include "ISource.h"
#include "common/node/stream/IAcceleration.h"

namespace silk
{
namespace node
{
namespace source
{

class IAccelerometer : public ISource
{
public:
    virtual ~IAccelerometer() {}

    struct Config
    {
        math::vec3f bias;
        math::vec3f scale = math::vec3f(1);
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_stream() -> stream::IAcceleration& = 0;
};

DECLARE_CLASS_PTR(IAccelerometer);


}
}
}
