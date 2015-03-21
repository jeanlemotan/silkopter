#include "BrainStdAfx.h"
#include "Comp_ECEF_Location.h"

#include "sz_math.hpp"
#include "sz_Comp_ECEF_Location.hpp"

namespace silk
{
namespace node
{

Comp_ECEF_Location::Comp_ECEF_Location(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Comp_ECEF_Location::Init_Params())
    , m_config(new sz::Comp_ECEF_Location::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto Comp_ECEF_Location::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("comp_location::init");

    sz::Comp_ECEF_Location::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_Location data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}
auto Comp_ECEF_Location::init() -> bool
{
    m_location_output_stream = std::make_shared<ECEF_Location_Stream>();
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_location_output_stream->rate = m_init_params->rate;
    m_dt = std::chrono::microseconds(1000000 / m_location_output_stream->rate);
    return true;
}

auto Comp_ECEF_Location::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(3);
    inputs[0].type = stream::IECEF_Location::TYPE;
    inputs[0].rate = m_location_output_stream ? m_location_output_stream->rate : 0;
    inputs[0].name = "Location";
    inputs[1].type = stream::ILinear_Acceleration::TYPE;
    inputs[1].rate = m_location_output_stream ? m_location_output_stream->rate : 0;
    inputs[1].name = "Linear Acceleration (ecef)";
    inputs[2].type = stream::IPressure::TYPE;
    inputs[2].rate = m_location_output_stream ? m_location_output_stream->rate : 0;
    inputs[2].name = "Pressure";
    return inputs;
}
auto Comp_ECEF_Location::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(2);
    outputs[0].type = stream::IECEF_Location::TYPE;
    outputs[0].name = "Location";
    outputs[0].stream = m_location_output_stream;
    outputs[1].type = stream::IENU_Frame::TYPE;
    outputs[1].name = "ENU Frame";
    outputs[1].stream = m_enu_frame_output_stream;
    return outputs;
}

void Comp_ECEF_Location::process()
{
    QLOG_TOPIC("comp_location::process");

    m_location_output_stream->samples.clear();

    auto location_stream = m_location_stream.lock();
    auto linear_acceleration_stream = m_linear_acceleration_stream.lock();
    auto pressure_stream = m_pressure_stream.lock();
    if (!location_stream ||
        !linear_acceleration_stream ||
        !pressure_stream)
    {
        return;
    }

    //accumulate the input streams
    {
        auto const& samples = location_stream->get_samples();
        m_location_samples.reserve(m_location_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_location_samples));
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
                         m_location_samples.size(),
                         m_linear_acceleration_samples.size()),
                         m_pressure_samples.size());
    if (count == 0)
    {
        return;
    }

    m_location_output_stream->samples.resize(count);

    for (size_t i = 0; i < count; i++)
    {
        m_location_output_stream->last_sample.dt = m_dt;
        m_location_output_stream->last_sample.sample_idx++;

        m_location_output_stream->last_sample.value = m_location_samples[i].value;

        m_location_output_stream->samples[i] = m_location_output_stream->last_sample;
    }


    //consume processed samples
    m_location_samples.erase(m_location_samples.begin(), m_location_samples.begin() + count);
    m_linear_acceleration_samples.erase(m_linear_acceleration_samples.begin(), m_linear_acceleration_samples.begin() + count);
    m_pressure_samples.erase(m_pressure_samples.begin(), m_pressure_samples.begin() + count);
}


auto Comp_ECEF_Location::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("comp_location::set_config");

    sz::Comp_ECEF_Location::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Comp_Location config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    auto location_stream = m_hal.get_streams().find_by_name<stream::IECEF_Location>(sz.inputs.ecef_location);
    auto linear_acceleration_stream = m_hal.get_streams().find_by_name<stream::ILinear_Acceleration>(sz.inputs.ecef_linear_acceleration);
    auto pressure_stream = m_hal.get_streams().find_by_name<stream::IPressure>(sz.inputs.pressure);

    auto rate = location_stream ? location_stream->get_rate() : 0u;
    if (rate != m_location_output_stream->rate)
    {
        m_config->inputs.ecef_location.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.ecef_location, m_location_output_stream->rate, rate);
        m_location_stream.reset();
    }
    else
    {
        m_location_stream = location_stream;
    }

    rate = linear_acceleration_stream ? linear_acceleration_stream->get_rate() : 0u;
    if (rate != m_location_output_stream->rate)
    {
        m_config->inputs.ecef_linear_acceleration.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.ecef_linear_acceleration, m_location_output_stream->rate, rate);
        m_linear_acceleration_stream.reset();
    }
    else
    {
        m_linear_acceleration_stream = linear_acceleration_stream;
    }

    rate = pressure_stream ? pressure_stream->get_rate() : 0u;
    if (rate != m_location_output_stream->rate)
    {
        m_config->inputs.pressure.clear();
        QLOGE("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.pressure, m_location_output_stream->rate, rate);
        m_pressure_stream.reset();
    }
    else
    {
        m_pressure_stream = pressure_stream;
    }

    return true;
}
auto Comp_ECEF_Location::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Comp_ECEF_Location::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}

}
}
