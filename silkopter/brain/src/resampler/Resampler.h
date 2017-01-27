#pragma once

#include "HAL.h"
#include "common/node/IResampler.h"
#include "utils/Butterworth.h"
#include <deque>

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

#include "hal.def.h"

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

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    ts::Result<void> start(Clock::time_point tp) override;

    ts::Result<void> set_input_stream_path(size_t idx, std::string const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();
    void resample();

    HAL& m_hal;

    std::shared_ptr<hal::Resampler_Descriptor> m_descriptor;
    std::shared_ptr<hal::Resampler_Config> m_config;

    Sample_Accumulator<Stream_t> m_accumulator;

    Clock::duration m_input_stream_dt;
    Clock::duration m_processed_dt = Clock::duration(0);

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
    , m_descriptor(new hal::Resampler_Descriptor)
    , m_config(new hal::Resampler_Config)
{
    m_output_stream = std::make_shared<Output_Stream>();
}

template<class Stream_t>
ts::Result<void> Resampler<Stream_t>::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("resampler::init");

    auto specialized = dynamic_cast<hal::Resampler_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}
template<class Stream_t>
ts::Result<void> Resampler<Stream_t>::init()
{
    m_output_stream->set_rate(m_descriptor->get_output_rate());

    m_input_stream_dt = std::chrono::microseconds(1000000 / m_descriptor->get_input_rate());

    uint32_t input_rate = m_descriptor->get_input_rate();
    uint32_t output_rate = m_descriptor->get_output_rate();
    uint32_t min_rate = math::min(output_rate, input_rate);
    float max_cutoff = min_rate / 2.f - min_rate / 100.f;
    hal::LPF_Config& lpf_config = m_config->get_lpf();
    lpf_config.set_cutoff_frequency(max_cutoff);

    return ts::success;
}

template<class Stream_t>
auto Resampler<Stream_t>::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

template<class Stream_t>
ts::Result<void> Resampler<Stream_t>::set_input_stream_path(size_t idx, std::string const& path)
{
    return m_accumulator.set_stream_path(idx, path, m_descriptor->get_input_rate(), m_hal);
}

template<class Stream_t>
ts::Result<void> Resampler<Stream_t>::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("resampler::set_config");

    auto specialized = dynamic_cast<hal::Resampler_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    uint32_t input_rate = m_descriptor->get_input_rate();
    uint32_t output_rate = m_descriptor->get_output_rate();
    uint32_t min_rate = math::min(output_rate, input_rate);
    uint32_t max_rate = math::max(output_rate, input_rate);
    float max_cutoff = min_rate / 2.f - min_rate / 100.f;

    hal::LPF_Config& lpf_config = m_config->get_lpf();

    if (math::is_zero(lpf_config.get_cutoff_frequency()))
    {
        lpf_config.set_cutoff_frequency(max_cutoff);
    }
    lpf_config.set_cutoff_frequency(math::clamp(lpf_config.get_cutoff_frequency(), 0.1f, max_cutoff));
    if (!m_dsp.setup(lpf_config.get_poles(), max_rate, lpf_config.get_cutoff_frequency()))
    {
        return make_error("Cannot setup dsp filter.");
    }
    m_dsp.reset();

    return ts::success;
}
//template<class Stream_t>
//auto Resampler<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}
template<class Stream_t>
auto Resampler<Stream_t>::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

template<class Stream_t>
ts::Result<void> Resampler<Stream_t>::start(Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

template<class Stream_t>
auto Resampler<Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { Stream_t::TYPE, m_descriptor->get_input_rate(), "input", m_accumulator.get_stream_path(0) }
    }};
    return inputs;
}
template<class Stream_t>
auto Resampler<Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "output";
    outputs[0].stream = m_output_stream;
    return outputs;
}
template<class Stream_t>
void Resampler<Stream_t>::process()
{
    QLOG_TOPIC("resampler::process");

    m_output_stream->clear();

    if (m_config->get_lpf().get_cutoff_frequency() == 0)
    {
        return;
    }

    m_accumulator.process([this](typename Stream_t::Sample const& i_sample)
    {
        m_input_samples.push_back(i_sample);
    });


    resample();
}

template<class Stream_t>
void Resampler<Stream_t>::resample()
{
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
//                m_input_accumulated_dt = Clock::duration(0);
//                break;
//            }
//        }
//    }

    bool is_downsampling = m_descriptor->get_output_rate() <= m_descriptor->get_input_rate();

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

            if (is_downsampling && m_last_input_sample.is_healthy)
            {
                //upsampling, filter at the input stream sample rate
                m_dsp.process(m_input_samples.back().value);
            }
            m_input_samples.pop_front();
        }

        if (m_last_input_sample.is_healthy)
        {
            auto value = m_last_input_sample.value;
            if (!is_downsampling)
            {
                //upsampling, filter at the output stream sample rate
                m_dsp.process(value);
            }

            m_output_stream->push_sample(value, true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    }

}


}
}
