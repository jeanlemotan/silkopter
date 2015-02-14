#pragma once

#include "HAL.h"
#include "common/node/stream/IADC_Value.h"
#include "common/node/source/IAmmeter.h"

namespace silk
{
namespace node
{
namespace source
{

class ADC_Ammeter : public IAmmeter
{
public:
    ADC_Ammeter(HAL& hal);

    struct Init_Params
    {
        q::String name;
        q::String source_stream;
    };

    auto init(Init_Params const& params) -> bool;

    auto get_stream() -> stream::ICurrent&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;
    stream::IADC_Value* m_source_stream = nullptr;

    struct Stream : public stream::ICurrent
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        std::vector<Sample> samples;
        uint32_t sample_idx = 0;
    } m_stream;
};


DECLARE_CLASS_PTR(ADC_Ammeter);

}
}
}
