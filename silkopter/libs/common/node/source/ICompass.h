#pragma once

#include "ISource.h"
#include "common/node/stream/IMagnetic_Field.h"

namespace silk
{
namespace node
{
namespace source
{

class ICompass : public ISource
{
public:
    virtual ~ICompass() {}

    struct Config
    {
        math::vec3f bias;
        math::vec3f scale = math::vec3f(1);
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_stream() -> stream::IMagnetic_Field& = 0;
};


}
}
}
