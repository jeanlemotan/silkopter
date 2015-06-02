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
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_stream_inputs() const -> std::vector<Stream_Input>;
    auto get_stream_outputs() const -> std::vector<Stream_Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    sz::Vec3_Generator::Init_Params m_init_params;
    sz::Vec3_Generator::Config m_config;

    std::weak_ptr<stream::IFloat> m_x_modulation_stream;
    std::weak_ptr<stream::IFloat> m_y_modulation_stream;
    std::weak_ptr<stream::IFloat> m_z_modulation_stream;

    std::vector<stream::IFloat::Sample> m_x_modulation_samples;
    std::vector<stream::IFloat::Sample> m_y_modulation_samples;
    std::vector<stream::IFloat::Sample> m_z_modulation_samples;

    typedef Basic_Output_Stream<Stream_t> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};


template<class Stream_t>
Vec3_Generator<Stream_t>::Vec3_Generator(HAL& hal)
    : m_hal(hal)
{
    autojsoncxx::to_document(m_init_params, m_init_paramsj);
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
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
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
auto Vec3_Generator<Stream_t>::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
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

    auto x_modulation_stream = m_hal.get_streams().template find_by_name<stream::IFloat>(sz.input_streams.x_modulation);
    if (x_modulation_stream && x_modulation_stream->get_rate() != m_output_stream->get_rate())
    {
        QLOGW("Bad x modulation stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.x_modulation, m_output_stream->get_rate(), x_modulation_stream->get_rate());
        m_x_modulation_stream.reset();
    }
    else
    {
        m_x_modulation_stream = x_modulation_stream;
    }

    auto y_modulation_stream = m_hal.get_streams().template find_by_name<stream::IFloat>(sz.input_streams.y_modulation);
    if (y_modulation_stream && y_modulation_stream->get_rate() != m_output_stream->get_rate())
    {
        QLOGW("Bad y modulation stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.y_modulation, m_output_stream->get_rate(), y_modulation_stream->get_rate());
        m_y_modulation_stream.reset();
    }
    else
    {
        m_y_modulation_stream = y_modulation_stream;
    }

    auto z_modulation_stream = m_hal.get_streams().template find_by_name<stream::IFloat>(sz.input_streams.z_modulation);
    if (z_modulation_stream && z_modulation_stream->get_rate() != m_output_stream->get_rate())
    {
        QLOGW("Bad z modulation stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.z_modulation, m_output_stream->get_rate(), z_modulation_stream->get_rate());
        m_z_modulation_stream.reset();
    }
    else
    {
        m_z_modulation_stream = z_modulation_stream;
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
auto Vec3_Generator<Stream_t>::get_stream_inputs() const -> std::vector<Stream_Input>
{
    std::vector<Stream_Input> inputs =
    {{
        { stream::IFloat::TYPE, m_init_params.rate, "X Modulation" },
        { stream::IFloat::TYPE, m_init_params.rate, "Y Modulation" },
        { stream::IFloat::TYPE, m_init_params.rate, "Z Modulation" }
    }};
    return inputs;
}
template<class Stream_t>
auto Vec3_Generator<Stream_t>::get_stream_outputs() const -> std::vector<Stream_Output>
{
    std::vector<Stream_Output> outputs(1);
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

    auto x_modulation_stream = m_x_modulation_stream.lock();
    auto y_modulation_stream = m_y_modulation_stream.lock();
    auto z_modulation_stream = m_z_modulation_stream.lock();

    auto count = m_output_stream->compute_samples_needed();
    if (count == 0)
    {
        return;
    }

    if (x_modulation_stream)
    {
        auto const& samples = x_modulation_stream->get_samples();
        m_x_modulation_samples.reserve(m_x_modulation_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_x_modulation_samples));
        count = std::min(count, m_x_modulation_samples.size());
    }
    if (y_modulation_stream)
    {
        auto const& samples = y_modulation_stream->get_samples();
        m_y_modulation_samples.reserve(m_y_modulation_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_y_modulation_samples));
        count = std::min(count, m_y_modulation_samples.size());
    }
    if (z_modulation_stream)
    {
        auto const& samples = z_modulation_stream->get_samples();
        m_z_modulation_samples.reserve(m_z_modulation_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_z_modulation_samples));
        count = std::min(count, m_z_modulation_samples.size());
    }

    if (count == 0)
    {
        return;
    }

    for (size_t i = 0; i < count; i++)
    {
        math::vec3f value = m_config.value;

        if (m_x_modulation_samples.size() > i)
        {
            value.x += m_x_modulation_samples[i].value;
        }
        if (m_y_modulation_samples.size() > i)
        {
            value.y += m_y_modulation_samples[i].value;
        }
        if (m_z_modulation_samples.size() > i)
        {
            value.z += m_z_modulation_samples[i].value;
        }

        m_output_stream->push_sample(value, true);
    }

    //consume samples
    if (m_x_modulation_samples.size() >= count)
    {
        m_x_modulation_samples.erase(m_x_modulation_samples.begin(), m_x_modulation_samples.begin() + count);
    }
    if (m_y_modulation_samples.size() >= count)
    {
        m_y_modulation_samples.erase(m_y_modulation_samples.begin(), m_y_modulation_samples.begin() + count);
    }
    if (m_z_modulation_samples.size() >= count)
    {
        m_z_modulation_samples.erase(m_z_modulation_samples.begin(), m_z_modulation_samples.begin() + count);
    }
}



}
}
