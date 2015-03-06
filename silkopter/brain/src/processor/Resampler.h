#pragma once

#include "HAL.h"
#include "common/node/processor/IProcessor.h"
#include "utils/Butterworth.h"
#include <deque>

#include "sz_math.hpp"
#include "sz_Resampler.hpp"

namespace silk
{
namespace node
{
namespace processor
{


template<class Stream_t>
class Resampler : public IProcessor
{
public:
    static const int MAX_POLES = 8;

    Resampler(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;
    void downsample();
    void upsample();


    HAL& m_hal;

    sz::Resampler::Init_Params m_init_params;
    sz::Resampler::Config m_config;

    std::weak_ptr<Stream_t> m_input_stream;

    q::Clock::duration m_input_dt;
    q::Clock::duration m_dt = q::Clock::duration(0);
    q::Clock::duration m_input_accumulated_dt = q::Clock::duration(0);
    q::Clock::duration m_processed_dt = q::Clock::duration(0);

    std::deque<typename Stream_t::Sample> m_input_samples;

    util::Butterworth<MAX_POLES, Stream_t::FILTER_CHANNELS> m_dsp;

    struct Stream : public Stream_t
    {
        auto get_samples() const -> std::vector<typename Stream_t::Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        std::vector<typename Stream_t::Sample> samples;
        uint32_t sample_idx = 0;
    };
    mutable std::shared_ptr<Stream> m_stream;
};


template<class Stream_t>
Resampler<Stream_t>::Resampler(HAL& hal)
    : m_hal(hal)
{
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
    m_init_params = sz;
    return init();
}
template<class Stream_t>
auto Resampler<Stream_t>::init() -> bool
{
    m_stream = std::make_shared<Stream>();

    m_stream->rate = math::max<uint32_t>(m_config.outputs.output.rate, 1);
    m_dt = std::chrono::microseconds(1000000 / m_stream->rate);

    return true;
}

template<class Stream_t>
auto Resampler<Stream_t>::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_init_params, json);
    return std::move(json);
}

template<class Stream_t>
auto Resampler<Stream_t>::set_config(rapidjson::Value const& json) -> bool
{
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
    if (!input_stream || input_stream->get_rate() == 0)
    {
        QLOGE("No input input stream specified");
        return false;
    }

    auto input_rate = input_stream->get_rate();

    uint32_t filter_rate = math::max(m_stream->rate, input_rate);
    double max_cutoff = math::min(m_stream->rate / 2.0, input_rate / 2.0);
    sz.cutoff_frequency = sz.cutoff_frequency > 0 ? sz.cutoff_frequency : max_cutoff;
    if (sz.cutoff_frequency > max_cutoff)
    {
        QLOGE("Cutoff frequency of {}Hz s too big for the resampler. Max cutoff is {}Hz.", sz.cutoff_frequency, max_cutoff);
        return false;
    }
    sz.poles = math::clamp<uint32_t>(sz.poles, 1, MAX_POLES);
    if (!m_dsp.setup(sz.poles, filter_rate, sz.cutoff_frequency))
    {
        QLOGE("Cannot setup dsp filter.");
        return false;
    }


    m_input_stream = input_stream;
    m_input_dt = std::chrono::microseconds(1000000 / input_rate);
    m_stream->rate = input_stream->get_rate();

    m_config = sz;
    return true;
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
    inputs[0].class_id = q::rtti::get_class_id<Stream_t>();
    inputs[0].name = "Input";
    inputs[0].stream_name = m_config.inputs.input;
    return inputs;
}
template<class Stream_t>
auto Resampler<Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].class_id = q::rtti::get_class_id<Stream_t>();
    outputs[0].name = "Output";
    outputs[0].stream = m_stream;
    return outputs;
}
template<class Stream_t>
void Resampler<Stream_t>::process()
{
    m_stream->samples.clear();

    auto input_stream = m_input_stream.lock();
    if (!input_stream)
    {
        return;
    }

    //first accumulate input samples
    auto const& is = input_stream->get_samples();

    if (m_stream->rate == input_stream->get_rate())
    {
        m_stream->samples.reserve(is.size());
        std::copy(is.begin(), is.end(), std::back_inserter(m_stream->samples));
    }
    else if (m_stream->rate < input_stream->get_rate())
    {
        std::array<double, Stream_t::FILTER_CHANNELS> channels;
        for (auto const& s: is)
        {
            m_input_accumulated_dt += s.dt;
            m_input_samples.push_back(s);
            if (Stream_t::get_channels_from_value(channels, m_input_samples.back().value))
            {
                m_dsp.process(channels.data());
                Stream_t::get_value_from_channels(m_input_samples.back().value, channels);
            }
        }
        downsample();
    }
    else if (m_stream->rate > input_stream->get_rate())
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
    m_stream->samples.reserve(m_input_samples.size() * (m_dt / m_input_dt));

    while (m_input_accumulated_dt >= m_dt)
    {
        typename Stream_t::Sample s;
        s.value = m_input_samples.front().value;
        s.sample_idx = ++m_stream->sample_idx;
        s.dt = m_dt;

        m_stream->samples.push_back(s);

        m_input_accumulated_dt -= m_dt;

        m_processed_dt += m_dt;
        while (m_processed_dt >= m_input_dt)
        {
            m_processed_dt -= m_input_dt;
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

    m_stream->samples.reserve(m_input_samples.size() * (m_dt / m_input_dt));

    std::array<double, Stream_t::FILTER_CHANNELS> channels;
    while (m_input_accumulated_dt >= m_dt)
    {
        typename Stream_t::Sample s;
        s.value = m_input_samples.front().value;
        s.sample_idx = ++m_stream->sample_idx;
        s.dt = m_dt;

        if (Stream_t::get_channels_from_value(channels, s.value))
        {
            m_dsp.process(channels.data());
            Stream_t::get_value_from_channels(s.value, channels);
        }

        m_stream->samples.push_back(s);

        m_input_accumulated_dt -= m_dt;

        m_processed_dt += m_dt;
        while (m_processed_dt >= m_input_dt)
        {
            m_processed_dt -= m_input_dt;
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
}
