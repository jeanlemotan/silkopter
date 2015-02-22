#pragma once

#include "HAL.h"
#include "common/node/processor/IFilter.h"
#include "utils/Butterworth.h"

#include "sz_math.hpp"
#include "sz_hal_nodes.hpp"

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
    static const int MAX_POLES = 8;

    struct Init_Params
    {
        std::string name;
        Stream_t* input_stream = nullptr;
        uint32_t poles = 1;
        double cutoff_frequency = 0;
    };

    LPF(HAL& hal)
        : m_hal(hal)
    {
    }

    auto init(rapidjson::Value const& json) -> bool
    {
        sz::LPF_Init_Params sz;
        autojsoncxx::error::ErrorStack result;
        if (!autojsoncxx::from_value(sz, json, result))
        {
            std::ostringstream ss;
            ss << result;
            QLOGE("Cannot deserialize LPF data: {}", ss.str());
            return false;
        }
        Init_Params params;
        params.name = sz.name;
        params.input_stream = m_hal.get_streams().template find_by_name<Stream_t>(sz.input_stream);
        params.poles = sz.poles;
        params.cutoff_frequency = sz.cutoff_frequency;

        return init(params);
    }
    auto init(Init_Params const& params) -> bool
    {
        m_params = params;

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

    auto get_input_stream_count() const -> size_t
    {
        return 1;
    }
    auto get_input_stream(size_t idx) -> Stream_t&
    {
        QASSERT(idx == 0);
        QASSERT(m_params.input_stream);
        return *m_params.input_stream;
    }
    auto get_output_stream_count() const -> size_t
    {
        return 1;
    }
    auto get_output_stream(size_t idx) -> Stream_t&
    {
        QASSERT(idx == 0);
        return m_stream;
    }
    auto get_name() const -> std::string const&
    {
        return m_params.name;
    }

    void process()
    {
        m_stream.samples.clear();
        auto const& is = get_input_stream(0).get_samples();
        m_stream.samples.reserve(is.size());

        std::array<double, Stream_t::FILTER_CHANNELS> channels;
        for (auto& s: is)
        {
           if (Stream_t::get_channels_from_value(channels, s.value))
           {
               m_dsp.process(channels.data());

               typename Stream_t::Sample vs;
               vs.dt = s.dt;
               vs.sample_idx = s.sample_idx;
               vs.value = s.value;
               Stream_t::get_value_from_channels(vs.value, channels);
               m_stream.samples.push_back(vs);
           }
           else
           {
               m_stream.samples.push_back(s);
           }
        };
    }

private:
    auto init() -> bool
    {
        m_stream.params = &m_params;
        if (!m_params.input_stream)
        {
            QLOGE("{}: No input specified", get_name());
            return false;
        }
        if (m_params.cutoff_frequency > m_params.input_stream->get_rate() / 2)
        {
            QLOGE("{}: Cutoff frequency {}Hz is bigger than the niquist frequency of {}Hz",
                  get_name(),
                  m_params.cutoff_frequency, m_params.input_stream->get_rate() / 2);
            return false;
        }

        m_params.poles = math::clamp<uint32_t>(m_params.poles, 1, MAX_POLES);
        if (!m_dsp.setup(m_params.poles, get_input_stream(0).get_rate(), m_params.cutoff_frequency))
        {
            QLOGE("{}: Cannot setup dsp filter.", get_name());
            return false;
        }

        return true;
    }

    HAL& m_hal;
    Init_Params m_params;

    util::Butterworth<MAX_POLES, Stream_t::FILTER_CHANNELS> m_dsp;

    struct Stream : public Stream_t
    {
        auto get_samples() const -> std::vector<typename Stream_t::Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return params->input_stream->get_rate(); }
        auto get_name() const -> std::string const& { return name; }

        Init_Params* params = nullptr;
        std::vector<typename Stream_t::Sample> samples;
        uint32_t sample_idx = 0;
        std::string name;
    } m_stream;
};


}
}
}
