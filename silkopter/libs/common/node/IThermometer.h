#pragma once

#include "Node_Data.h"

namespace silk
{
namespace node
{

class IThermometer : q::util::Noncopyable
{
public:
    typedef float                   Temperature;
    typedef Sample<Temperature>     Sample;
    typedef TP_Sample<Temperature>  TP_Sample;

    virtual ~IThermometer() {}

    virtual auto get_name() const -> q::String const& = 0;

    struct Config
    {
        float bias;
        float scale = 1;
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};

DECLARE_CLASS_PTR(IThermometer);

}
}
