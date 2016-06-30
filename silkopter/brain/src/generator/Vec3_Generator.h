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
class Vec3_Generator : public IGenerator
{
public:
    Vec3_Generator(HAL& hal);

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

    std::shared_ptr<hal::Vec3_Generator_Descriptor> m_descriptor;
    std::shared_ptr<hal::Vec3_Generator_Config> m_config;

    std::array<std::string, 3> m_modulation_stream_paths;
    std::array<std::weak_ptr<stream::IFloat>, 3> m_modulation_streams;
    std::array<std::vector<stream::IFloat::Sample>, 3> m_modulation_samples;

    typedef Basic_Output_Stream<Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class Stream_t>
Vec3_Generator<Stream_t>::Vec3_Generator(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Vec3_Generator_Descriptor)
    , m_config(new hal::Vec3_Generator_Config)
{
    m_output_stream = std::make_shared<Output_Stream>();
}

template<class Stream_t>
ts::Result<void> Vec3_Generator<Stream_t>::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("vec3_generator::init");

    auto specialized = dynamic_cast<hal::Vec3_Generator_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

template<class Stream_t>
ts::Result<void> Vec3_Generator<Stream_t>::init()
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return ts::success;
}

template<class Stream_t>
auto Vec3_Generator<Stream_t>::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

template<class Stream_t>
ts::Result<void> Vec3_Generator<Stream_t>::start(q::Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

template<class Stream_t>
ts::Result<void> Vec3_Generator<Stream_t>::set_input_stream_path(size_t idx, std::string const& path)
{
    if (idx >= 3)
    {
        return make_error("Stream index {} is out of bounds", idx);
    }

    char name[3] = { 'x', 'y', 'z' };

    auto stream = m_hal.get_stream_registry().template find_by_name<stream::IFloat>(path);
    if (stream && stream->get_rate() != m_output_stream->get_rate())
    {
        m_modulation_streams[idx].reset();
        m_modulation_stream_paths[idx].clear();
        return make_error("Bad {} modulation stream '{}'. Expected rate {}Hz, got {}Hz", name[idx], path, m_output_stream->get_rate(), stream->get_rate());
    }
    else
    {
        m_modulation_streams[idx] = stream;
        m_modulation_stream_paths[idx] = path;
    }
    return ts::success;
}

template<class Stream_t>
ts::Result<void> Vec3_Generator<Stream_t>::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("vec3_generator::set_config");

    auto specialized = dynamic_cast<hal::Vec3_Generator_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
//template<class Stream_t>
//auto Vec3_Generator<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}
template<class Stream_t>
auto Vec3_Generator<Stream_t>::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

template<class Stream_t>
auto Vec3_Generator<Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IFloat::TYPE, m_descriptor->get_rate(), "X Modulation", m_modulation_stream_paths[0] },
        { stream::IFloat::TYPE, m_descriptor->get_rate(), "Y Modulation", m_modulation_stream_paths[1] },
        { stream::IFloat::TYPE, m_descriptor->get_rate(), "Z Modulation", m_modulation_stream_paths[2] }
    }};
    return inputs;
}
template<class Stream_t>
auto Vec3_Generator<Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].name = "Output";
    outputs[0].stream = m_output_stream;
    return outputs;
}

template<class Stream_t>
void Vec3_Generator<Stream_t>::process()
{
    QLOG_TOPIC("vec3_generator::process");

    m_output_stream->clear();

    auto count = m_output_stream->compute_samples_needed();
    if (count == 0)
    {
        return;
    }

    for (size_t s = 0; s < 3; s++)
    {
        auto stream = m_modulation_streams[s].lock();
        if (stream)
        {
            auto const& samples = stream->get_samples();
            m_modulation_samples[s].reserve(m_modulation_samples[s].size() + samples.size());
            std::copy(samples.begin(), samples.end(), std::back_inserter(m_modulation_samples[s]));
            count = std::min(count, m_modulation_samples[s].size());
        }
    }

    if (count == 0)
    {
        return;
    }

    for (size_t i = 0; i < count; i++)
    {
        math::vec3f value = m_config->get_value();

        if (m_modulation_samples[0].size() > i)
        {
            value.x += m_modulation_samples[0][i].value;
        }
        if (m_modulation_samples[1].size() > i)
        {
            value.y += m_modulation_samples[1][i].value;
        }
        if (m_modulation_samples[2].size() > i)
        {
            value.z += m_modulation_samples[2][i].value;
        }

        m_output_stream->push_sample(value, true);
    }

    //consume samples
    for (auto& samples: m_modulation_samples)
    {
        if (samples.size() >= count)
        {
            samples.erase(samples.begin(), samples.begin() + count);
        }
    }
}



}
}
