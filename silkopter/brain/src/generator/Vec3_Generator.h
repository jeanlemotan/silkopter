#pragma once

#include "HAL.h"
#include "common/node/IGenerator.h"
#include "generator/Oscillator.h"

#include "sz_math.hpp"
#include "sz_Vec3_Generator.hpp"

namespace silk
{
namespace node
{

template<class Stream_t>
class Vec3_Generator : public IGenerator
{
public:
    Vec3_Generator(HAL& hal);

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

    HAL& m_hal;

    sz::Vec3_Generator::Init_Params m_init_params;
    sz::Vec3_Generator::Config m_config;

    std::array<q::Path, 3> m_modulation_stream_paths;
    std::array<std::weak_ptr<stream::IFloat>, 3> m_modulation_streams;
    std::array<std::vector<stream::IFloat::Sample>, 3> m_modulation_samples;

    typedef Basic_Output_Stream<Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class Stream_t>
Vec3_Generator<Stream_t>::Vec3_Generator(HAL& hal)
    : m_hal(hal)
{
}

template<class Stream_t>
auto Vec3_Generator<Stream_t>::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("vec3_generator::init");

    sz::Vec3_Generator::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Vec3_Generator data: {}", ss.str());
        return false;
    }
    m_init_params = sz;
    return init();
}

template<class Stream_t>
auto Vec3_Generator<Stream_t>::init() -> bool
{
    m_output_stream = std::make_shared<Output_Stream>();
    if (m_init_params.rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params.rate);
        return false;
    }
    m_output_stream->set_rate(m_init_params.rate);
    m_output_stream->set_tp(q::Clock::now());
    return true;
}

template<class Stream_t>
auto Vec3_Generator<Stream_t>::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_init_params, json);
    return std::move(json);
}

template<class Stream_t>
void Vec3_Generator<Stream_t>::set_input_stream_path(size_t idx, q::Path const& path)
{
    if (idx >= 3)
    {
        return;
    }

    char name[3] = { 'x', 'y', 'z' };

    auto stream = m_hal.get_streams().template find_by_name<stream::IFloat>(path.get_as<std::string>());
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
auto Vec3_Generator<Stream_t>::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("vec3_generator::set_config");
    sz::Vec3_Generator::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Vec3_Generator config data: {}", ss.str());
        return false;
    }

    m_config = sz;

    return true;
}
template<class Stream_t>
auto Vec3_Generator<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}
template<class Stream_t>
auto Vec3_Generator<Stream_t>::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_config, json);
    return std::move(json);
}

template<class Stream_t>
auto Vec3_Generator<Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IFloat::TYPE, m_init_params.rate, "X Modulation", m_modulation_stream_paths[0] },
        { stream::IFloat::TYPE, m_init_params.rate, "Y Modulation", m_modulation_stream_paths[0] },
        { stream::IFloat::TYPE, m_init_params.rate, "Z Modulation", m_modulation_stream_paths[0] }
    }};
    return inputs;
}
template<class Stream_t>
auto Vec3_Generator<Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].type = Stream_t::TYPE;
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
        math::vec3f value = m_config.value;

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
