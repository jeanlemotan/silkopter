#pragma once

#include "Node_Data.h"

namespace silk
{
namespace node
{

class ISonar : q::util::Noncopyable
{
public:
    typedef float                   Distance;
    typedef Sample<Distance>        Sample;
    typedef TP_Sample<Distance>     TP_Sample;

    virtual ~ISonar() {}

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

DECLARE_CLASS_PTR(ISonar);

}
}
