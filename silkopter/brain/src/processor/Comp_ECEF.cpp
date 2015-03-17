#include "BrainStdAfx.h"
#include "Comp_ECEF.h"

#include "sz_math.hpp"
#include "sz_Comp_ECEF.hpp"

namespace silk
{
namespace node
{

Comp_ECEF::Comp_ECEF(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Comp_ECEF::Init_Params())
    , m_config(new sz::Comp_ECEF::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto Comp_ECEF::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("comp_ecef::init");

    sz::Comp_ECEF::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_ECEF data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}
auto Comp_ECEF::init() -> bool
{
    m_output_stream = std::make_shared<Stream>();
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_stream->rate = m_init_params->rate;
    m_dt = std::chrono::microseconds(1000000 / m_output_stream->rate);
    return true;
}

auto Comp_ECEF::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(3);
    inputs[0].class_id = q::rtti::get_class_id<stream::IECEF>();
    inputs[0].rate = m_output_stream ? m_output_stream->rate : 0;
    inputs[0].name = "ECEF";
    inputs[1].class_id = q::rtti::get_class_id<stream::ILinear_Acceleration>();
    inputs[1].rate = m_output_stream ? m_output_stream->rate : 0;
    inputs[1].name = "Linear Acceleration";
    inputs[2].class_id = q::rtti::get_class_id<stream::IPressure>();
    inputs[2].rate = m_output_stream ? m_output_stream->rate : 0;
    inputs[2].name = "Pressure";
    return inputs;
}
auto Comp_ECEF::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(1);
    outputs[0].class_id = q::rtti::get_class_id<stream::IECEF>();
    outputs[0].name = "ECEF";
    outputs[0].stream = m_output_stream;
    return outputs;
}

void Comp_ECEF::process()
{
    QLOG_TOPIC("comp_ecef::process");

    m_output_stream->samples.clear();

    auto ecef_stream = m_ecef_stream.lock();
    auto linear_acceleration_stream = m_linear_acceleration_stream.lock();
    auto pressure_stream = m_pressure_stream.lock();
    if (!ecef_stream ||
        !linear_acceleration_stream ||
        !pressure_stream)
    {
        return;
    }

    //accumulate the input streams
    {
        auto const& samples = ecef_stream->get_samples();
        m_ecef_samples.reserve(m_ecef_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_ecef_samples));
    }
    {
        auto const& samples = linear_acceleration_stream->get_samples();
        m_linear_acceleration_samples.reserve(m_linear_acceleration_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_linear_acceleration_samples));
    }
    {
        auto const& samples = pressure_stream->get_samples();
        m_pressure_samples.reserve(m_pressure_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_pressure_samples));
    }

    //TODO add some protecton for severely out-of-sync streams

    size_t count = std::min(std::min(
                         m_ecef_samples.size(),
                         m_linear_acceleration_samples.size()),
                         m_pressure_samples.size());
    if (count == 0)
    {
        return;
    }

    m_output_stream->samples.resize(count);

    for (size_t i = 0; i < count; i++)
    {
        m_output_stream->last_sample.dt = m_dt;
        m_output_stream->last_sample.sample_idx++;

        m_output_stream->last_sample.value = m_ecef_samples[i].value;

        m_output_stream->samples[i] = m_output_stream->last_sample;
    }


    //consume processed samples
    m_ecef_samples.erase(m_ecef_samples.begin(), m_ecef_samples.begin() + count);
    m_linear_acceleration_samples.erase(m_linear_acceleration_samples.begin(), m_linear_acceleration_samples.begin() + count);
    m_pressure_samples.erase(m_pressure_samples.begin(), m_pressure_samples.begin() + count);
}


auto Comp_ECEF::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("comp_ecef::set_config");

    sz::Comp_ECEF::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_ECEF config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    auto ecef_stream = m_hal.get_streams().find_by_name<stream::IECEF>(sz.inputs.ecef);
    auto linear_acceleration_stream = m_hal.get_streams().find_by_name<stream::ILinear_Acceleration>(sz.inputs.linear_acceleration);
    auto pressure_stream = m_hal.get_streams().find_by_name<stream::IPressure>(sz.inputs.pressure);

    auto rate = ecef_stream ? ecef_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.ecef.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.ecef, m_output_stream->rate, rate);
        m_ecef_stream.reset();
    }
    else
    {
        m_ecef_stream = ecef_stream;
    }

    rate = linear_acceleration_stream ? linear_acceleration_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.linear_acceleration.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.linear_acceleration, m_output_stream->rate, rate);
        m_linear_acceleration_stream.reset();
    }
    else
    {
        m_linear_acceleration_stream = linear_acceleration_stream;
    }

    rate = pressure_stream ? pressure_stream->get_rate() : 0u;
    if (rate != m_output_stream->rate)
    {
        m_config->inputs.pressure.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.pressure, m_output_stream->rate, rate);
        m_pressure_stream.reset();
    }
    else
    {
        m_pressure_stream = pressure_stream;
    }

    return true;
}
auto Comp_ECEF::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Comp_ECEF::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}

}
}
