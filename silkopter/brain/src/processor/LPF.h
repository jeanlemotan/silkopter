#pragma once

#include "HAL.h"
#include "common/node/processor/IFilter.h"

namespace silk
{
namespace node
{
namespace processor
{

class LPF_Butterworth_Base : q::util::Noncopyable
{
public:
    struct Init_Params
    {
        std::string name;
        std::string source_stream;
        uint32_t poles = 1;
        uint32_t cutoff_frequency = 0;
    };
};

template<class Stream_t>
class LPF_Butterworth : public IFilter<Stream_t>, public LPF_Butterworth_Base
{
public:
    typedef IFilter<Stream_t> Base;

    LPF_Butterworth(HAL& hal)
        : m_hal(hal)
    {
    }

    auto init(Init_Params const& params) -> bool
    {
        return true;
    }

    auto get_input_stream() -> Stream_t&
    {

    }
    auto get_output_stream() -> Stream_t&
    {

    }

    void process()
    {

    }

private:
    auto init() -> bool
    {
        return true;
    }

    HAL& m_hal;
    Init_Params m_params;

    struct Stream : public Stream_t
    {
        auto get_samples() const -> std::vector<typename Stream_t::Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return source_stream->get_rate(); }

        Stream_t* source_stream = nullptr;
        std::vector<typename Stream_t::Sample> samples;
        uint32_t sample_idx = 0;
    } m_stream;
};


DECLARE_CLASS_PTR(ADC_Voltmeter);

}
}
}
