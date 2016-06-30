#pragma once

#include "HAL.h"
#include "common/node/ILPF.h"
#include "utils/Butterworth.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

#include "hal.def.h"

namespace silk
{
namespace node
{

template<class Stream_t>
class LPF : public ILPF
{
public:
    LPF(HAL& hal);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    ts::Result<void> start(q::Clock::time_point tp) override;

    ts::Result<void> set_input_stream_path(size_t idx, std::string const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();

    HAL& m_hal;

    std::shared_ptr<hal::LPF_Descriptor> m_descriptor;
    std::shared_ptr<hal::LPF_Config> m_config;

    Sample_Accumulator<Stream_t> m_accumulator;

    util::Butterworth<typename Stream_t::Value> m_dsp;

    typedef Basic_Output_Stream<Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class Stream_t>
LPF<Stream_t>::LPF(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::LPF_Descriptor)
    , m_config(new hal::LPF_Config)
{
    m_output_stream = std::make_shared<Output_Stream>();
}

template<class Stream_t>
ts::Result<void> LPF<Stream_t>::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("lpf::init");

    auto specialized = dynamic_cast<hal::LPF_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

template<class Stream_t>
ts::Result<void> LPF<Stream_t>::init()
{
    m_output_stream->set_rate(m_descriptor->get_rate());

    float output_rate = static_cast<float>(m_output_stream->get_rate());
    float max_cutoff = output_rate / 2.f - output_rate / 100.f;
    m_config->set_cutoff_frequency(max_cutoff);

    return ts::success;
}

template<class Stream_t>
auto LPF<Stream_t>::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

template<class Stream_t>
ts::Result<void> LPF<Stream_t>::set_input_stream_path(size_t idx, std::string const& path)
{
    return m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

template<class Stream_t>
ts::Result<void> LPF<Stream_t>::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("lpf::config");

    auto specialized = dynamic_cast<hal::LPF_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    float output_rate = static_cast<float>(m_output_stream->get_rate());
    float max_cutoff = output_rate / 2.f - output_rate / 100.f;

    if (math::is_zero(m_config->get_cutoff_frequency()))
    {
        m_config->set_cutoff_frequency(max_cutoff);
    }
    m_config->set_cutoff_frequency(math::clamp(m_config->get_cutoff_frequency(), 0.1f, max_cutoff));
    if (!m_dsp.setup(m_config->get_poles(), output_rate, m_config->get_cutoff_frequency()))
    {
        return make_error("Cannot setup dsp filter.");
    }
    m_dsp.reset();

    return ts::success;
}
//template<class Stream_t>
//auto LPF<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}
template<class Stream_t>
auto LPF<Stream_t>::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

template<class Stream_t>
ts::Result<void> LPF<Stream_t>::start(q::Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

template<class Stream_t>
auto LPF<Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { Stream_t::TYPE, m_descriptor->get_rate(), "Input", m_accumulator.get_stream_path(0) }
    }};
    return inputs;
}
template<class Stream_t>
auto LPF<Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "Output";
    outputs[0].stream = m_output_stream;
    return outputs;
}

template<class Stream_t>
void LPF<Stream_t>::process()
{
    QLOG_TOPIC("lpf::process");

    m_output_stream->clear();

    if (m_config->get_cutoff_frequency() == 0)
    {
        return;
    }

    m_accumulator.process([this](typename Stream_t::Sample const& i_sample)
    {
        if (i_sample.is_healthy)
        {
            auto value = i_sample.value;
            QASSERT(math::is_finite(value));
            m_dsp.process(value);
            QASSERT(math::is_finite(value));
            m_output_stream->push_sample(value, true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    });
}



}
}
