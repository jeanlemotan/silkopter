#pragma once

#include "UAV.h"
#include "common/node/IGenerator.h"
#include "generator/Oscillator.h"

//#include "sz_math.hpp"
//#include "sz_Vec3_Generator.hpp"

namespace silk
{
namespace node
{

template<class Stream_t>
class Vec3_Generator : public IGenerator
{
public:
    Vec3_Generator(UAV& uav);

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

    std::shared_ptr<uav::Vec3_Generator_Descriptor> m_descriptor;
    std::shared_ptr<uav::Vec3_Generator_Config> m_config;

    std::array<q::Path, 3> m_modulation_stream_paths;
    std::array<std::weak_ptr<stream::IFloat>, 3> m_modulation_streams;
    std::array<std::vector<stream::IFloat::Sample>, 3> m_modulation_samples;

    typedef Basic_Output_Stream<Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class Stream_t>
Vec3_Generator<Stream_t>::Vec3_Generator(UAV& uav)
    : m_uav(uav)
{
    m_output_stream = std::make_shared<Output_Stream>();
}

template<class Stream_t>
auto Vec3_Generator<Stream_t>::init(std::shared_ptr<uav::INode_Descriptor> descriptor) -> bool
{
    QLOG_TOPIC("vec3_generator::init");

    auto specialized = std::dynamic_pointer_cast<uav::Vec3_Generator_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;

    return init();
}

template<class Stream_t>
auto Vec3_Generator<Stream_t>::init() -> bool
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return true;
}

template<class Stream_t>
auto Vec3_Generator<Stream_t>::get_descriptor() const -> std::shared_ptr<uav::INode_Descriptor>
{
    return m_descriptor;
}

template<class Stream_t>
auto Vec3_Generator<Stream_t>::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

template<class Stream_t>
void Vec3_Generator<Stream_t>::set_input_stream_path(size_t idx, q::Path const& path)
{
    if (idx >= 3)
    {
        return;
    }

    char name[3] = { 'x', 'y', 'z' };

    auto stream = m_uav.get_streams().template find_by_name<stream::IFloat>(path.get_as<std::string>());
    if (stream && stream->get_rate() != m_output_stream->get_rate())
    {
        QLOGW("Bad {} modulation stream '{}'. Expected rate {}Hz, got {}Hz", name[idx], path, m_output_stream->get_rate(), stream->get_rate());
        m_modulation_streams[idx].reset();
        m_modulation_stream_paths[idx].clear();
    }
    else
    {
        m_modulation_streams[idx] = stream;
        m_modulation_stream_paths[idx] = path;
    }
}

template<class Stream_t>
auto Vec3_Generator<Stream_t>::set_config(std::shared_ptr<uav::INode_Config> config) -> bool
{
    QLOG_TOPIC("vec3_generator::set_config");

    auto specialized = std::dynamic_pointer_cast<uav::Vec3_Generator_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    return true;
}
//template<class Stream_t>
//auto Vec3_Generator<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}
template<class Stream_t>
auto Vec3_Generator<Stream_t>::get_config() const -> std::shared_ptr<uav::INode_Config>
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
