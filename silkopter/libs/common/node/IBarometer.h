#pragma once

#include "Node_Data.h"

namespace silk
{
namespace node
{

class IBarometer : q::util::Noncopyable
{
public:
    typedef float                   Pressure;
    typedef Sample<Pressure>        Sample;
    typedef TP_Sample<Pressure>     TP_Sample;

    virtual ~IBarometer() {}

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

DECLARE_CLASS_PTR(IBarometer);

}
}
