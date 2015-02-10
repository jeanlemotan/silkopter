#pragma once

#include "common/sensors/Sensor_Samples.h"

namespace silk
{
namespace sensor
{

class IADC : q::util::Noncopyable
{
public:
    virtual ~IADC() {}

    virtual auto get_name() const -> q::String const& = 0;

    struct Config
    {
        float bias;
        float scale = 1;
    };

//    virtual void set_config(Config const& config) = 0;
//    virtual auto get_config() const -> Config const& = 0;

    virtual auto get_samples() const -> std::vector<ADC_Sample> const& = 0;
};

DECLARE_CLASS_PTR(IADC);

}
}
