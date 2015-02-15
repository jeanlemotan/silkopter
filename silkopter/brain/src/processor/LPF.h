#pragma once

#include "HAL.h"
#include "common/node/processor/IFilter.h"
#include "DspFilters/Butterworth.h"

namespace silk
{
namespace node
{
namespace processor
{


template<class Stream_t>
class LPF : public IFilter<Stream_t>
{
public:
    typedef IFilter<Stream_t> Base;
    static const int MAX_POLES = 8;

    struct Init_Params
    {
        std::string name;
        Stream_t* source_stream = nullptr;
        uint32_t poles = 1;
        uint32_t cutoff_frequency = 0;
    };

    LPF(HAL& hal)
        : m_hal(hal)
    {
    }

    auto init(rapidjson::Value const& json) -> bool
    {

    }
    auto init(Init_Params const& params) -> bool
    {
        m_params = params;
        m_stream.source_stream = m_params.source_stream;

        if (!init())
        {
            return false;
        }

        if (!m_params.name.empty())
        {
            m_stream.name = q::util::format2<std::string>("{}/stream", m_params.name);
            if (!m_hal.get_streams().add(m_stream))
            {
                return false;
            }
        }

        return true;
    }

    auto get_input_stream() -> Stream_t&
    {
        QASSERT(m_stream.source_stream);
        return *m_stream.source_stream;
    }
    auto get_output_stream() -> Stream_t&
    {
        return m_stream;
    }
    auto get_name() const -> std::string const&
    {
        return m_params.name;
    }

    void process()
    {
        m_stream.samples.clear();
        auto const& s = get_input_stream().get_samples();
        m_stream.samples.resize(s.size());

        double* channels = m_channels;
        std::transform(s.begin(), s.end(), m_stream.samples.begin(), [this, channels](typename Stream_t::Sample const& sample)
        {
           typename Stream_t::Sample vs;
           vs.dt = sample.dt;
           vs.sample_idx = sample.sample_idx;

           Stream_t::get_channels(channels, sample.value);
           m_dsp.process(1, &channels);
           Stream_t::get_value(vs.value, channels);

           return vs;
        });
    }

private:
    auto init() -> bool
    {
        m_params.poles = math::clamp<uint32_t>(m_params.poles, 1, MAX_POLES);

        if (!m_stream.source_stream)
        {
            QLOGE("No source specified");
            return false;
        }

        m_dsp.setup(m_params.poles, m_stream.source_stream->get_rate(), m_params.cutoff_frequency);

        return true;
    }

    HAL& m_hal;
    Init_Params m_params;

    Dsp::SimpleFilter <Dsp::Butterworth::LowPass<MAX_POLES>, Stream_t::FILTER_CHANNELS> m_dsp;
    double m_channels[Stream_t::FILTER_CHANNELS];


    struct Stream : public Stream_t
    {
        auto get_samples() const -> std::vector<typename Stream_t::Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return source_stream->get_rate(); }
        auto get_name() const -> std::string const& { return name; }

        Stream_t* source_stream = nullptr;
        std::vector<typename Stream_t::Sample> samples;
        uint32_t sample_idx = 0;
        std::string name;
    } m_stream;
};


}
}
}
