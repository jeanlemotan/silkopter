#pragma once

#include "HAL.h"
#include "common/node/stream/IADC_Value.h"
#include "common/node/stream/IVoltage.h"
#include "common/node/processor/ITransform.h"

namespace silk
{
namespace node
{
namespace processor
{

class ADC_Voltmeter : public ITransform<stream::IADC_Value, stream::IVoltage>
{
public:
    typedef ITransform<stream::IADC_Value, stream::IVoltage> Base;

    ADC_Voltmeter(HAL& hal);

    struct Init_Params
    {
        std::string name;
        std::string source_stream;
    };

    auto init(Init_Params const& params) -> bool;

    auto get_input_stream() -> stream::IADC_Value&;
    auto get_output_stream() -> stream::IVoltage&;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;
    Init_Params m_params;

    struct Stream : public stream::IVoltage
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return source_stream->get_rate(); }

        stream::IADC_Value* source_stream = nullptr;
        std::vector<Sample> samples;
        uint32_t sample_idx = 0;
    } m_stream;
};


DECLARE_CLASS_PTR(ADC_Voltmeter);

}
}
}
