#pragma once

#include "HAL.h"
#include "common/node/processor/IFilter.h"
#include "DspFilters/Butterworth.h"
#include <deque>

#include "sz_math.hpp"
#include "sz_hal_nodes.hpp"

namespace silk
{
namespace node
{
namespace processor
{


template<class Stream_t>
class Resampler : public IFilter<Stream_t>
{
public:
    typedef IFilter<Stream_t> Base;

    struct Init_Params
    {
        std::string name;
        Stream_t* source_stream = nullptr;
        uint32_t output_rate = 0;
    };

    Resampler(HAL& hal)
        : m_hal(hal)
    {
    }

    auto init(rapidjson::Value const& json) -> bool
    {
        sz::Resampler sz;
        autojsoncxx::error::ErrorStack result;
        if (!autojsoncxx::from_value(sz, json, result))
        {
            std::ostringstream ss;
            ss << result;
            QLOGE("Cannot deserialize Resampler data: {}", ss.str());
            return false;
        }
        Init_Params params;
        params.name = sz.name;
        params.source_stream = m_hal.get_streams().template find_by_name<Stream_t>(sz.source_stream);
        params.output_rate = sz.output_rate;

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
        QASSERT(m_stream.source_stream);
        return *m_stream.source_stream;
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

        //first accumulate input samples
        auto const& is = get_input_stream(0).get_samples();

        if (m_stream.rate == m_params.source_stream->get_rate())
        {
            m_stream.samples.reserve(is.size());
            std::copy(is.begin(), is.end(), std::back_inserter(m_stream.samples));
        }
        else
        {
            if (m_stream.rate < m_params.source_stream->get_rate())
            {
                auto** channels = m_channels;
                for (auto const& s: is)
                {
                    m_input_dt += s.dt;
                    m_input_samples.push_back(s);
                    Stream_t::setup_channels(channels, m_input_samples.back().value);
                    m_dsp.process(1, channels);
                }
                downsample();
            }
            else if (m_stream.rate > m_params.source_stream->get_rate())
            {
                for (auto const& s: is)
                {
                    m_input_dt += s.dt;
                    m_input_samples.push_back(s);
                }
                upsample();
            }
        }
    }

    void downsample()
    {
        m_stream.samples.reserve(m_input_samples.size() * (m_dt / m_source_dt));

        while (m_input_dt >= m_dt)
        {
            typename Stream_t::Sample s;
            s.value = m_input_samples.front().value;
            s.sample_idx = ++m_stream.sample_idx;
            s.dt = m_dt;

            m_stream.samples.push_back(s);

            m_input_dt -= m_dt;

            m_processed_dt += m_dt;
            while (m_processed_dt >= m_source_dt)
            {
                m_processed_dt -= m_source_dt;
                m_input_samples.pop_front();
                if (m_input_samples.empty())
                {
                    m_input_dt = q::Clock::duration(0);
                    break;
                }
            }
        }

    }
    void upsample()
    {
        if (m_input_samples.empty())
        {
            return;
        }

//        if (!m_has_started)
//        {
//            m_last_input_time_point = m_input_samples.front().time_point - m_dt;
//            m_has_started = true;
//        }

        m_stream.samples.reserve(m_input_samples.size() * (m_dt / m_source_dt));

        auto** channels = m_channels;
        while (m_input_dt >= m_dt)
        {
            typename Stream_t::Sample s;
            s.value = m_input_samples.front().value;
            s.sample_idx = ++m_stream.sample_idx;
            s.dt = m_dt;

            Stream_t::setup_channels(channels, s.value);
            m_dsp.process(1, channels);

            m_stream.samples.push_back(s);

            m_input_dt -= m_dt;

            m_processed_dt += m_dt;
            while (m_processed_dt >= m_source_dt)
            {
                m_processed_dt -= m_source_dt;
                m_input_samples.pop_front();
                if (m_input_samples.empty())
                {
                    m_input_dt = q::Clock::duration(0);
                    break;
                }
            }
        }
    }

private:
    auto init() -> bool
    {
        m_stream.source_stream = m_params.source_stream;
        if (!m_stream.source_stream)
        {
            QLOGE("No source specified");
            return false;
        }

        m_params.output_rate = math::max<uint32_t>(m_params.output_rate, 1);
        m_stream.rate = m_params.output_rate;
        m_dt = std::chrono::microseconds(1000000 / m_params.output_rate);
        m_source_dt = std::chrono::microseconds(1000000 / m_stream.source_stream->get_rate());

        if (m_stream.rate < m_params.source_stream->get_rate())
        {
            m_dsp.setup(1, m_stream.source_stream->get_rate(), m_params.output_rate / 2);
        }
        else if (m_stream.rate > m_params.source_stream->get_rate())
        {
            m_dsp.setup(1, m_params.output_rate, m_stream.source_stream->get_rate() / 2);
        }

        return true;
    }

    HAL& m_hal;
    Init_Params m_params;

    q::Clock::duration m_source_dt;
    q::Clock::duration m_dt = q::Clock::duration(0);
    q::Clock::duration m_input_dt = q::Clock::duration(0);
    q::Clock::duration m_processed_dt = q::Clock::duration(0);

    std::deque<typename Stream_t::Sample> m_input_samples;

    Dsp::SimpleFilter <Dsp::Butterworth::LowPass<1>, Stream_t::FILTER_CHANNELS> m_dsp;
    typename Stream_t::FILTER_CHANNEL_TYPE* m_channels[Stream_t::FILTER_CHANNELS] = { nullptr };

    struct Stream : public Stream_t
    {
        auto get_samples() const -> std::vector<typename Stream_t::Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        uint32_t rate = 0;
        Stream_t* source_stream = nullptr;
        std::vector<typename Stream_t::Sample> samples;
        uint32_t sample_idx = 0;
        std::string name;
    } m_stream;
};


}
}
}
