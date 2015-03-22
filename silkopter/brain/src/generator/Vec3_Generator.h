#pragma once

#include "HAL.h"
#include "common/node/processor/IGenerator.h"
#include "generator/Factor_Generator.h"

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

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    sz::Vec3_Generator::Init_Params m_init_params;
    sz::Vec3_Generator::Config m_config;

    std::weak_ptr<stream::IFactor> m_x_factor_stream;
    std::weak_ptr<stream::IFactor> m_y_factor_stream;
    std::weak_ptr<stream::IFactor> m_z_factor_stream;

    std::vector<stream::IFactor::Sample> m_x_factor_samples;
    std::vector<stream::IFactor::Sample> m_y_factor_samples;
    std::vector<stream::IFactor::Sample> m_z_factor_samples;


    q::Clock::time_point m_last_tp;
    q::Clock::duration m_dt;

    struct Stream : public Stream_t
    {
        auto get_samples() const -> std::vector<typename Stream_t::Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        std::vector<typename Stream_t::Sample> samples;
        uint32_t sample_idx = 0;
    };
    mutable std::shared_ptr<Stream> m_output_stream;
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
    m_output_stream = std::make_shared<Stream>();
    if (m_init_params.rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params.rate);
        return false;
    }
    m_output_stream->rate = m_init_params.rate;
    m_last_tp = q::Clock::now();
    m_dt = std::chrono::microseconds(1000000 / m_init_params.rate);
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

    auto x_factor_stream = m_hal.get_streams().template find_by_name<stream::IFactor>(sz.inputs.x_factor);
    if (x_factor_stream && x_factor_stream->get_rate() != m_output_stream->rate)
    {
        QLOGW("Bad x factor stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.x_factor, m_output_stream->rate, x_factor_stream->get_rate());
        m_x_factor_stream.reset();
    }
    else
    {
        m_x_factor_stream = x_factor_stream;
    }

    auto y_factor_stream = m_hal.get_streams().template find_by_name<stream::IFactor>(sz.inputs.y_factor);
    if (y_factor_stream && y_factor_stream->get_rate() != m_output_stream->rate)
    {
        QLOGW("Bad y factor stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.y_factor, m_output_stream->rate, y_factor_stream->get_rate());
        m_y_factor_stream.reset();
    }
    else
    {
        m_y_factor_stream = y_factor_stream;
    }

    auto z_factor_stream = m_hal.get_streams().template find_by_name<stream::IFactor>(sz.inputs.z_factor);
    if (z_factor_stream && z_factor_stream->get_rate() != m_output_stream->rate)
    {
        QLOGW("Bad z factor stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.z_factor, m_output_stream->rate, z_factor_stream->get_rate());
        m_z_factor_stream.reset();
    }
    else
    {
        m_z_factor_stream = z_factor_stream;
    }

    m_config = sz;

    return true;
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
    std::vector<Input> inputs(3);
    inputs[0].type = stream::IFactor::TYPE;
    inputs[0].rate = m_init_params.rate;
    inputs[0].name = "X Factor";
    inputs[1].type = stream::IFactor::TYPE;
    inputs[1].rate = m_init_params.rate;
    inputs[1].name = "Y Factor";
    inputs[2].type = stream::IFactor::TYPE;
    inputs[2].rate = m_init_params.rate;
    inputs[2].name = "Z Factor";
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

    m_output_stream->samples.clear();

    auto x_factor_stream = m_x_factor_stream.lock();
    auto y_factor_stream = m_y_factor_stream.lock();
    auto z_factor_stream = m_z_factor_stream.lock();

    auto now = q::Clock::now();
    auto dt = now - m_last_tp;
    if (dt < m_dt)
    {
        return;
    }
    size_t count = dt / m_dt;

    if (x_factor_stream)
    {
        auto const& samples = x_factor_stream->get_samples();
        m_x_factor_samples.reserve(m_x_factor_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_x_factor_samples));
        count = std::min(count, m_x_factor_samples.size());
    }
    if (y_factor_stream)
    {
        auto const& samples = y_factor_stream->get_samples();
        m_y_factor_samples.reserve(m_y_factor_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_y_factor_samples));
        count = std::min(count, m_y_factor_samples.size());
    }
    if (z_factor_stream)
    {
        auto const& samples = z_factor_stream->get_samples();
        m_z_factor_samples.reserve(m_z_factor_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_z_factor_samples));
        count = std::min(count, m_z_factor_samples.size());
    }

    if (count == 0)
    {
        return;
    }


    auto tp = m_last_tp + m_dt;
    m_last_tp = now;

    m_output_stream->samples.resize(count);
    for (size_t i = 0; i < count; i++)
    {
        typename Stream::Sample vs;
        vs.dt = m_dt;
        vs.tp = tp;
        vs.sample_idx = ++m_output_stream->sample_idx;
        vs.value = m_config.value;

        if (m_x_factor_samples.size() > i)
        {
            vs.value.x += m_x_factor_samples[i].value;
        }
        if (m_y_factor_samples.size() > i)
        {
            vs.value.y += m_y_factor_samples[i].value;
        }
        if (m_z_factor_samples.size() > i)
        {
            vs.value.z += m_z_factor_samples[i].value;
        }

        m_output_stream->samples[i] = vs;
        tp += m_dt;
        dt -= m_dt;
    }

    //reminder for next process
    m_last_tp -= dt;

    //consume samples
    if (m_x_factor_samples.size() >= count)
    {
        m_x_factor_samples.erase(m_x_factor_samples.begin(), m_x_factor_samples.begin() + count);
    }
    if (m_y_factor_samples.size() >= count)
    {
        m_y_factor_samples.erase(m_y_factor_samples.begin(), m_y_factor_samples.begin() + count);
    }
    if (m_z_factor_samples.size() >= count)
    {
        m_z_factor_samples.erase(m_z_factor_samples.begin(), m_z_factor_samples.begin() + count);
    }
}



}
}
