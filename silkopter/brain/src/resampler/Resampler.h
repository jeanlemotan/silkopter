#pragma once

#include "HAL.h"
#include "common/node/IResampler.h"
#include "utils/Butterworth.h"
#include <deque>

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


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
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void set_input_stream_path(size_t idx, q::Path const& path);
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

    Sample_Accumulator<Stream_t> m_accumulator;

    q::Clock::duration m_input_stream_dt;
    q::Clock::duration m_processed_dt = q::Clock::duration(0);

    typename Stream_t::Sample m_last_input_sample;

    std::deque<typename Stream_t::Sample> m_input_samples;

    template<class T, bool>
    struct Butterworth
    {
        auto setup(size_t, float, float) -> bool
        {
            return true;
        }
        void reset(typename T::Value const&) {}
        void reset() {}
        void process(typename T::Value&) {}
    };

    template<class T>
    struct Butterworth<T, true>
    {
        util::Butterworth<typename T::Value> dsp;

        auto setup(size_t order, float rate, float cutoff_frequency) -> bool
        {
            return dsp.setup(order, rate, cutoff_frequency);
        }
        void reset(typename T::Value const& t)
        {
            dsp.reset(t);
        }
        void reset()
        {
            dsp.reset();
        }
        void process(typename T::Value& t)
        {
            dsp.process(t);
        }
    };

    Butterworth<Stream_t, Stream_t::can_be_filtered_t::value> m_dsp;

    typedef Basic_Output_Stream<Stream_t> Output_Stream;
//    struct Stream : public Stream_t
//    {
//        auto get_samples() const -> std::vector<typename Stream_t::Sample> const& { return samples; }
//        auto get_rate() const -> uint32_t { return rate; }

//        uint32_t rate = 0;
//        std::vector<typename Stream_t::Sample> samples;
//    };
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class Stream_t>
Resampler<Stream_t>::Resampler(HAL& hal)
    : m_hal(hal)
{
    m_output_stream = std::make_shared<Output_Stream>();
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
    if (m_init_params.rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params.rate);
        return false;
    }
    if (m_init_params.input_rate == 0)
    {
        QLOGE("Bad input rate: {}Hz", m_init_params.input_rate);
        return false;
    }
    m_output_stream->set_rate(m_init_params.rate);
    m_output_stream->set_tp(q::Clock::now());

    m_input_stream_dt = std::chrono::microseconds(1000000 / m_init_params.input_rate);

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
void Resampler<Stream_t>::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_init_params.input_rate, m_hal);
}

template<class Stream_t>
auto Resampler<Stream_t>::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("resampler::set_config");

    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(m_config, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Resampler config data: {}", ss.str());
        return false;
    }

    auto input_rate = m_init_params.input_rate;
    auto output_rate = m_init_params.rate;

    uint32_t filter_rate = math::max(output_rate, input_rate);
    float max_cutoff = math::min(output_rate / 2.f, input_rate / 2.f);
    m_config.cutoff_frequency = m_config.cutoff_frequency > 0 ? m_config.cutoff_frequency : max_cutoff;
    if (m_config.cutoff_frequency > max_cutoff)
    {
        QLOGE("Cutoff frequency of {}Hz s too big for the resampler. Max cutoff is {}Hz.", m_config.cutoff_frequency, max_cutoff);
        return false;
    }
    m_config.poles = math::max<uint32_t>(m_config.poles, 1);
    if (!m_dsp.setup(m_config.poles, filter_rate, m_config.cutoff_frequency))
    {
        QLOGE("Cannot setup dsp filter.");
        return false;
    }
    m_dsp.reset();

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
    std::vector<Input> inputs =
    {{
        { Stream_t::TYPE, m_init_params.input_rate, "Input", m_accumulator.get_stream_path(0) }
    }};
    return inputs;
}
template<class Stream_t>
auto Resampler<Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "Output";
    outputs[0].stream = m_output_stream;
    return outputs;
}
template<class Stream_t>
void Resampler<Stream_t>::process()
{
    QLOG_TOPIC("resampler::process");

    m_output_stream->clear();

    if (m_config.cutoff_frequency == 0)
    {
        return;
    }

    if (m_init_params.rate <= m_init_params.input_rate)
    {
        m_accumulator.process([this](typename Stream_t::Sample const& i_sample)
        {
            m_input_samples.push_back(i_sample);
            m_dsp.process(m_input_samples.back().value);
        });
        downsample();
    }
    else
    {
        m_accumulator.process([this](typename Stream_t::Sample const& i_sample)
        {
            m_input_samples.push_back(i_sample);
        });

        upsample();
    }
}

template<class Stream_t>
void Resampler<Stream_t>::downsample()
{
    auto dt = m_output_stream->get_dt();
    size_t samples_needed = m_output_stream->compute_samples_needed();
    for (size_t i = 0; i < samples_needed; i++)
    {
        //consume input samples
        m_processed_dt += dt;
        while (!m_input_samples.empty() && m_processed_dt >= m_input_stream_dt)
        {
            m_processed_dt -= m_input_stream_dt;
            m_last_input_sample = m_input_samples.front();
            m_input_samples.pop_front();
        }

        auto const& is = m_last_input_sample;
        if (is.is_healthy)
        {
            m_output_stream->push_sample(is.value, true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    }

    //m_output_stream->samples.reserve(m_input_samples.size() * (m_dt / m_input_stream_dt));

//    auto dt = m_output_stream->get_dt();
//    while (m_input_accumulated_dt >= dt)
//    {
//        auto const& is = m_input_samples.front();

//        m_output_stream->push_sample(is.value, is.is_healthy);

//        m_input_accumulated_dt -= dt;

//        m_processed_dt += dt;
//        while (m_processed_dt >= m_input_stream_dt)
//        {
//            m_processed_dt -= m_input_stream_dt;
//            m_input_samples.pop_front();
//            if (m_input_samples.empty())
//            {
//                m_input_accumulated_dt = q::Clock::duration(0);
//                break;
//            }
//        }
//    }

}
template<class Stream_t>
void Resampler<Stream_t>::upsample()
{
    auto dt = m_output_stream->get_dt();
    size_t samples_needed = m_output_stream->compute_samples_needed();
    for (size_t i = 0; i < samples_needed; i++)
    {
        //consume input samples
        m_processed_dt += dt;
        while (!m_input_samples.empty() && m_processed_dt >= m_input_stream_dt)
        {
            m_processed_dt -= m_input_stream_dt;
            m_last_input_sample = m_input_samples.front();
            m_input_samples.pop_front();
        }


        auto is = m_last_input_sample;
        m_dsp.process(is.value);
        if (is.is_healthy)
        {
            m_output_stream->push_sample(is.value, true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    }
}


}
}
