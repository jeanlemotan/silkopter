#pragma once

#include "UAV.h"
#include "common/node/ILPF.h"
#include "utils/Butterworth.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"

#include "uav.def.h"

namespace silk
{
namespace node
{

template<class Stream_t>
class LPF : public ILPF
{
public:
    LPF(UAV& uav);

    bool init(std::shared_ptr<uav::INode_Descriptor> descriptor) override;
    std::shared_ptr<uav::INode_Descriptor> get_descriptor() const override;

    bool set_config(std::shared_ptr<uav::INode_Config> config) override;
    std::shared_ptr<uav::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

    std::shared_ptr<uav::LPF_Descriptor> m_descriptor;
    std::shared_ptr<uav::LPF_Config> m_config;

    Sample_Accumulator<Stream_t> m_accumulator;

    util::Butterworth<typename Stream_t::Value> m_dsp;

    typedef Basic_Output_Stream<Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class Stream_t>
LPF<Stream_t>::LPF(UAV& uav)
    : m_uav(uav)
{
    m_output_stream = std::make_shared<Output_Stream>();
}

template<class Stream_t>
auto LPF<Stream_t>::init(std::shared_ptr<uav::INode_Descriptor> descriptor) -> bool
{
    QLOG_TOPIC("lpf::init");

    auto specialized = std::dynamic_pointer_cast<uav::LPF_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;

    return init();
}

template<class Stream_t>
auto LPF<Stream_t>::init() -> bool
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return true;
}

template<class Stream_t>
auto LPF<Stream_t>::get_descriptor() const -> std::shared_ptr<uav::INode_Descriptor>
{
    return m_descriptor;
}

template<class Stream_t>
void LPF<Stream_t>::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_uav);
}

template<class Stream_t>
auto LPF<Stream_t>::set_config(std::shared_ptr<uav::INode_Config> config) -> bool
{
    QLOG_TOPIC("lpf::config");

    auto specialized = std::dynamic_pointer_cast<uav::LPF_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
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
        QLOGE("Cannot setup dsp filter.");
        return false;
    }
    m_dsp.reset();

    return true;
}
//template<class Stream_t>
//auto LPF<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}
template<class Stream_t>
auto LPF<Stream_t>::get_config() const -> std::shared_ptr<uav::INode_Config>
{
    return m_config;
}

template<class Stream_t>
auto LPF<Stream_t>::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
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
