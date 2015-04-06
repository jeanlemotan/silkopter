#pragma once

#include "HAL.h"
#include "common/node/IResampler.h"
#include "utils/Butterworth.h"
#include <deque>

#include "sz_math.hpp"
#include "sz_Resampler.hpp"

namespace silk
{
namespace node
{

template<class Stream_t>
class Resampler : public IResampler
{
public:
    static const int MAX_POLES = 8;

    Resampler(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;
    void downsample();
    void upsample();


    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    sz::Resampler::Init_Params m_init_params;
    sz::Resampler::Config m_config;

    std::weak_ptr<Stream_t> m_input_stream;

    q::Clock::duration m_input_stream_dt;
    q::Clock::duration m_dt = q::Clock::duration(0);
    q::Clock::duration m_input_accumulated_dt = q::Clock::duration(0);
    q::Clock::duration m_processed_dt = q::Clock::duration(0);

    std::deque<typename Stream_t::Sample> m_input_samples;

    util::Butterworth<typename Stream_t::Value> m_dsp;

    struct Stream : public Stream_t
    {
        auto get_samples() const -> std::vector<typename Stream_t::Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        std::vector<typename Stream_t::Sample> samples;
        uint32_t sample_idx = 0;
    };
    mutable std::shared_ptr<Stream> m_output_stream;
};


template<class Stream_t>
Resampler<Stream_t>::Resampler(HAL& hal)
    : m_hal(hal)
{
    autojsoncxx::to_document(m_init_params, m_init_paramsj);
}

template<class Stream_t>
auto Resampler<Stream_t>::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("resampler::init");

    sz::Resampler::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Resampler data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    m_init_params = sz;
    return init();
}
template<class Stream_t>
auto Resampler<Stream_t>::init() -> bool
{
    m_output_stream = std::make_shared<Stream>();
    if (m_init_params.rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params.rate);
        return false;
    }
    m_output_stream->rate = m_init_params.rate;

    m_dt = std::chrono::microseconds(1000000 / m_output_stream->rate);

    return true;
}

template<class Stream_t>
auto Resampler<Stream_t>::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}

template<class Stream_t>
auto Resampler<Stream_t>::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("resampler::set_config");

    sz::Resampler::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Resampler config data: {}", ss.str());
        return false;
    }

    auto input_stream = m_hal.get_streams().template find_by_name<Stream_t>(sz.inputs.input);
    auto input_rate = input_stream ? input_stream->get_rate() : 0u;
    if (input_rate == 0)
    {
        QLOGW("Bad input stream '{}' @ {}Hz", sz.inputs.input, input_rate);
        m_input_stream.reset();
        m_input_stream_dt = std::chrono::microseconds(0);
    }
    else
    {
        m_input_stream = input_stream;
        m_input_stream_dt = std::chrono::microseconds(1000000 / input_rate);
    }

    auto output_rate = m_output_stream->rate;

    uint32_t filter_rate = math::max(output_rate, input_rate);
    double max_cutoff = math::min(output_rate / 2.0, input_rate / 2.0);
    sz.cutoff_frequency = sz.cutoff_frequency > 0 ? sz.cutoff_frequency : max_cutoff;
    if (sz.cutoff_frequency > max_cutoff)
    {
        QLOGE("Cutoff frequency of {}Hz s too big for the resampler. Max cutoff is {}Hz.", sz.cutoff_frequency, max_cutoff);
        return false;
    }
    sz.poles = math::max<uint32_t>(sz.poles, 2);
    if (!m_dsp.setup(sz.poles, filter_rate, sz.cutoff_frequency))
    {
        QLOGE("Cannot setup dsp filter.");
        return false;
    }
    m_dsp.reset();

    m_config = sz;

    return true;
}
template<class Stream_t>
auto Resampler<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}
template<class Stream_t>
auto Resampler<Stream_t>::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_config, json);
    return std::move(json);
}

template<class Stream_t>
auto Resampler<Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(1);
    inputs[0].type = Stream_t::TYPE;
    inputs[0].rate = 0; //any sample rate
    inputs[0].name = "Input";
    return inputs;
}
template<class Stream_t>
auto Resampler<Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].type = Stream_t::TYPE;
    outputs[0].name = "Output";
    outputs[0].stream = m_output_stream;
    return outputs;
}
template<class Stream_t>
void Resampler<Stream_t>::process()
{
    QLOG_TOPIC("resampler::process");

    m_output_stream->samples.clear();

    auto input_stream = m_input_stream.lock();
    if (!input_stream || m_config.cutoff_frequency == 0)
    {
        return;
    }

    //first accumulate input samples
    auto const& is = input_stream->get_samples();

    if (m_output_stream->rate <= input_stream->get_rate())
    {
        for (auto const& s: is)
        {
            m_input_accumulated_dt += s.dt;
            m_input_samples.push_back(s);
            if (s.is_healthy)
            {
                m_dsp.process(m_input_samples.back().value);
            }
        }
        downsample();
    }
    else if (m_output_stream->rate > input_stream->get_rate())
    {
        for (auto const& s: is)
        {
            m_input_accumulated_dt += s.dt;
            m_input_samples.push_back(s);
        }
        upsample();
    }
}

template<class Stream_t>
void Resampler<Stream_t>::downsample()
{
    m_output_stream->samples.reserve(m_input_samples.size() * (m_dt / m_input_stream_dt));

    auto tp = q::Clock::now() - m_input_accumulated_dt;
    tp += m_dt;

    typename Stream_t::Sample s;
    s.dt = m_dt;
    while (m_input_accumulated_dt >= m_dt)
    {
        s.value = m_input_samples.front().value;
        s.sample_idx = ++m_output_stream->sample_idx;
        s.tp = tp;
        tp += m_dt;

        m_output_stream->samples.push_back(s);

        m_input_accumulated_dt -= m_dt;

        m_processed_dt += m_dt;
        while (m_processed_dt >= m_input_stream_dt)
        {
            m_processed_dt -= m_input_stream_dt;
            m_input_samples.pop_front();
            if (m_input_samples.empty())
            {
                m_input_accumulated_dt = q::Clock::duration(0);
                break;
            }
        }
    }

}
template<class Stream_t>
void Resampler<Stream_t>::upsample()
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

    m_output_stream->samples.reserve(m_input_samples.size() * (m_dt / m_input_stream_dt));

    auto tp = q::Clock::now() - m_input_accumulated_dt;
    tp += m_dt;

    typename Stream_t::Sample s;
    s.dt = m_dt;
    while (m_input_accumulated_dt >= m_dt)
    {
        s.value = m_input_samples.front().value;
        s.sample_idx = ++m_output_stream->sample_idx;
        s.tp = tp;
        tp += m_dt;

        if (s.is_healthy)
        {
            m_dsp.process(s.value);
        }

        m_output_stream->samples.push_back(s);

        m_input_accumulated_dt -= m_dt;

        m_processed_dt += m_dt;
        while (m_processed_dt >= m_input_stream_dt)
        {
            m_processed_dt -= m_input_stream_dt;
            m_input_samples.pop_front();
            if (m_input_samples.empty())
            {
                m_input_accumulated_dt = q::Clock::duration(0);
                break;
            }
        }
    }
}


}
}
