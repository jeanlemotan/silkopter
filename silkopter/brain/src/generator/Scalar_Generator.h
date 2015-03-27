#pragma once

#include "HAL.h"
#include "common/node/IGenerator.h"
#include "generator/Factor_Generator.h"

#include "sz_math.hpp"
#include "sz_Scalar_Generator.hpp"

namespace silk
{
namespace node
{

template<class Stream_t>
class Scalar_Generator : public IGenerator
{
public:
    Scalar_Generator(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    rapidjson::Document m_init_paramsj;
    sz::Scalar_Generator::Init_Params m_init_params;
    sz::Scalar_Generator::Config m_config;

    std::weak_ptr<stream::IFactor> m_factor_stream;

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
Scalar_Generator<Stream_t>::Scalar_Generator(HAL& hal)
    : m_hal(hal)
{
    autojsoncxx::to_document(m_init_params, m_init_paramsj);
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("scalar_generator::init");

    sz::Scalar_Generator::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Scalar_Generator data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    m_init_params = sz;
    return init();
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::init() -> bool
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
auto Scalar_Generator<Stream_t>::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("scalar_generator::set_config");
    sz::Scalar_Generator::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Scalar_Generator config data: {}", ss.str());
        return false;
    }

    auto factor_stream = m_hal.get_streams().template find_by_name<stream::IFactor>(sz.inputs.factor);

    if (factor_stream && factor_stream->get_rate() != m_output_stream->rate)
    {
        QLOGW("Bad factor stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.factor, m_output_stream->rate, factor_stream->get_rate());
        m_factor_stream.reset();
    }
    else
    {
        m_factor_stream = factor_stream;
    }

    m_config = sz;

    return true;
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}
template<class Stream_t>
auto Scalar_Generator<Stream_t>::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(m_config, json);
    return std::move(json);
}

template<class Stream_t>
auto Scalar_Generator<Stream_t>::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(1);
    inputs[0].type = stream::IFactor::TYPE;
    inputs[0].rate = m_init_params.rate;
    inputs[0].name = "Factor";
    return inputs;
}
template<class Stream_t>
auto Scalar_Generator<Stream_t>::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].type = Stream_t::TYPE;
    outputs[0].name = "Output";
    outputs[0].stream = m_output_stream;
    return outputs;
}

template<class Stream_t>
void Scalar_Generator<Stream_t>::process()
{
    QLOG_TOPIC("scalar_generator::process");

    m_output_stream->samples.clear();

    auto factor_stream = m_factor_stream.lock();
    if (factor_stream)
    {
        auto const& samples = factor_stream->get_samples();
        m_output_stream->samples.reserve(samples.size());
        for (auto const& s: samples)
        {
            typename Stream::Sample vs;
            vs.dt = m_dt;
            vs.tp = s.tp;
            vs.sample_idx = ++m_output_stream->sample_idx;
            vs.value = m_config.value + s.value;
            m_output_stream->samples.push_back(vs);
        }
    }
    else
    {
        auto now = q::Clock::now();
        auto dt = now - m_last_tp;
        if (dt < m_dt)
        {
            return;
        }
        auto tp = m_last_tp + m_dt;
        m_last_tp = now;

        m_output_stream->samples.reserve(dt / m_dt);
        while (dt >= m_dt)
        {
            typename Stream::Sample vs;
            vs.dt = m_dt;
            vs.tp = tp;
            vs.sample_idx = ++m_output_stream->sample_idx;
            vs.value = m_config.value;
            m_output_stream->samples.push_back(vs);
            tp += m_dt;
            dt -= m_dt;
        }
        //reminder for next process
        m_last_tp -= dt;
    }
}



}
}
