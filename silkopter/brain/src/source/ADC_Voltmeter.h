#pragma once

#include "common/node/stream/IADC_Value.h"
#include "common/node/source/IVoltmeter.h"

namespace silk
{
namespace node
{
namespace source
{

class ADC_Voltmeter : public IVoltmeter
{
public:
    ADC_Voltmeter();

    struct Init_Params
    {

    };

    auto init(stream::IADC_Value* stream, Init_Params const& params) -> bool;

    auto get_stream() const -> stream::IVoltage const&;

    void process();

private:
    stream::IADC_Value* m_source_stream = nullptr;

//    struct Stream : public node::IVoltage_Stream
//    {
//        auto get_samples() const -> std::vector<Sample> const&;
//        std::vector<Sample> samples;
//        uint32_t sample_idx = 0;
//    } m_stream;
};


DECLARE_CLASS_PTR(ADC_Voltmeter);

}
}
}
