#pragma once

#include "HAL.h"
#include "common/node/IGenerator.h"
#include "generator/Oscillator.h"

#include "Basic_Output_Stream.h"

#include "hal.def.h"

namespace silk
{
namespace node
{

template<class Stream_t>
class Scalar_Generator : public IGenerator
{
public:
    Scalar_Generator(HAL& hal);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    ts::Result<void> start(q::Clock::time_point tp) override;

    ts::Result<void> set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();

    HAL& m_hal;

    std::shared_ptr<hal::Scalar_Generator_Descriptor> m_descriptor;
    std::shared_ptr<hal::Scalar_Generator_Config> m_config;

    std::weak_ptr<stream::IFloat> m_modulation_stream;
    q::Path m_modulation_stream_path;

    typedef Basic_Output_Stream<Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class Stream_t>
Scalar_Generator<Stream_t>::Scalar_Generator(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Scalar_Generator_Descriptor)
    , m_config(new hal::Scalar_Generator_Config)
{
    m_output_stream = std::make_shared<Output_Stream>();
}

template<class Stream_t>
ts::Result<void> Scalar_Generator<Stream_t>::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("scalar_generator::init");

    auto specialized = dynamic_cast<hal::Scalar_Generator_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

template<class Stream_t>
ts::Result<void> Scalar_Generator<Stream_t>::init()
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return ts::success;
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

template<class Stream_t>
ts::Result<void> Scalar_Generator<Stream_t>::start(q::Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

template<class Stream_t>
ts::Result<void> Scalar_Generator<Stream_t>::set_input_stream_path(size_t idx, q::Path const& path)
{
    auto modulation_stream = m_hal.get_stream_registry().template find_by_name<stream::IFloat>(path.get_as<std::string>());
    if (modulation_stream && modulation_stream->get_rate() != m_output_stream->get_rate())
    {
        m_modulation_stream.reset();
        m_modulation_stream_path.clear();
        return make_error("Bad modulation stream '{}'. Expected rate {}Hz, got {}Hz", path, m_output_stream->get_rate(), modulation_stream->get_rate());
    }
    else
    {
        m_modulation_stream = modulation_stream;
        m_modulation_stream_path = path;
    }
    return ts::success;
}

template<class Stream_t>
ts::Result<void> Scalar_Generator<Stream_t>::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("scalar_generator::set_config");

    auto specialized = dynamic_cast<hal::Scalar_Generator_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}

//template<class Stream_t>
//auto Scalar_Generator<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}
template<class Stream_t>
auto Scalar_Generator<Stream_t>::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IFloat::TYPE, m_descriptor->get_rate(), "Modulation", m_modulation_stream_path }
    }};
    return inputs;
}
template<class Stream_t>
auto Scalar_Generator<Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "Output";
    outputs[0].stream = m_output_stream;
    return outputs;
}

template<class Stream_t>
void Scalar_Generator<Stream_t>::process()
{
    QLOG_TOPIC("scalar_generator::process");

    m_output_stream->clear();

    auto modulation_stream = m_modulation_stream.lock();
    if (modulation_stream)
    {
        auto const& samples = modulation_stream->get_samples();
        for (auto const& s: samples)
        {
            if (s.is_healthy)
            {
                m_output_stream->push_sample(m_config->get_value() + s.value, true);
            }
            else
            {
                m_output_stream->push_last_sample(false);
            }
        }
    }
    else
    {
        auto samples_needed = m_output_stream->compute_samples_needed();
        while (samples_needed > 0)
        {
            m_output_stream->push_sample(m_config->get_value(), true);
            samples_needed--;
        }
    }
}



}
}
