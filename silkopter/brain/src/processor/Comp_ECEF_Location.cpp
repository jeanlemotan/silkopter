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

auto Comp_ECEF_Location::get_stream_inputs() const -> std::vector<Stream_Input>
{
    std::vector<Stream_Input> inputs =
    {{
        { stream::IECEF_Location::TYPE, m_init_params->rate, "Location" },
        { stream::ILinear_Acceleration::TYPE, m_init_params->rate, "Linear Acceleration (ecef)" },
        { stream::IPressure::TYPE, m_init_params->rate, "Pressure" }
    }};
    return inputs;
}
auto Comp_ECEF_Location::get_stream_outputs() const -> std::vector<Stream_Output>
{
    std::vector<Stream_Output> outputs =
    {{
        { stream::IECEF_Location::TYPE, "Location", m_location_output_stream },
        { stream::IENU_Frame::TYPE, "ENU Frame", m_enu_frame_output_stream },
    }};
    return outputs;
}

void Comp_ECEF_Location::process()
{
    QLOG_TOPIC("comp_location::process");


    m_location_output_stream->samples.clear();

    m_accumulator.process([this](
                          size_t idx,
                          stream::IECEF_Location::Sample const& loc_sample,
                          stream::ILinear_Acceleration::Sample const& la_sample,
                          stream::IPressure::Sample const& p_sample)
    {
        auto& sample = m_location_output_stream->last_sample;
        sample.dt = m_dt;
        sample.sample_idx++;
        sample.value = loc_sample.value;

        m_location_output_stream->samples.push_back(sample);
    });
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
    m_accumulator.clear_streams();

    auto location_stream = m_hal.get_streams().find_by_name<stream::IECEF_Location>(sz.input_streams.ecef_location);
    auto linear_acceleration_stream = m_hal.get_streams().find_by_name<stream::ILinear_Acceleration>(sz.input_streams.ecef_linear_acceleration);
    auto pressure_stream = m_hal.get_streams().find_by_name<stream::IPressure>(sz.input_streams.pressure);

    auto rate = location_stream ? location_stream->get_rate() : 0u;
    if (rate != m_location_output_stream->rate)
    {
        m_config->input_streams.ecef_location.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.ecef_location, m_location_output_stream->rate, rate);
    }
    else
    {
        m_accumulator.set_stream<0>(location_stream);
    }

    rate = linear_acceleration_stream ? linear_acceleration_stream->get_rate() : 0u;
    if (rate != m_location_output_stream->rate)
    {
        m_config->input_streams.ecef_linear_acceleration.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.ecef_linear_acceleration, m_location_output_stream->rate, rate);
    }
    else
    {
        m_accumulator.set_stream<1>(linear_acceleration_stream);
    }

    rate = pressure_stream ? pressure_stream->get_rate() : 0u;
    if (rate != m_location_output_stream->rate)
    {
        m_config->input_streams.pressure.clear();
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.input_streams.pressure, m_location_output_stream->rate, rate);
    }
    else
    {
        m_accumulator.set_stream<2>(pressure_stream);
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

auto Comp_ECEF_Location::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}

}
}
