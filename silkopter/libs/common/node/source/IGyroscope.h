#pragma once

#include "ISource.h"
#include "common/node/stream/IAngular_Velocity.h"

namespace silk
{
namespace node
{
namespace source
{


class IGyroscope : public ISource
{
public:
    virtual ~IGyroscope() {}

    struct Config
    {
        math::vec3f bias;
        math::vec3f scale = math::vec3f(1);
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_stream() const -> stream::IAngular_Velocity const& = 0;
};

DECLARE_CLASS_PTR(IGyroscope);

}
}
}
