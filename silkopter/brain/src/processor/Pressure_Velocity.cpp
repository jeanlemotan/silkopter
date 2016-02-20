#include "BrainStdAfx.h"
#include "Pressure_Velocity.h"

#include "sz_math.hpp"
#include "sz_Pressure_Velocity.hpp"

namespace silk
{
namespace node
{

Pressure_Velocity::Pressure_Velocity(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Pressure_Velocity::Init_Params())
    , m_config(new sz::Pressure_Velocity::Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto Pressure_Velocity::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("Pressure_Velocity::init");

    sz::Pressure_Velocity::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Pressure_Velocity data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto Pressure_Velocity::init() -> bool
{
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }

    m_output_stream->set_rate(m_init_params->rate);

    return true;
}

auto Pressure_Velocity::start(q::Clock::time_point tp) -> bool
{
    m_output_stream->set_tp(tp);
    return true;
}

auto Pressure_Velocity::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IPressure::TYPE, m_init_params->rate, "Pressure", m_accumulator.get_stream_path(0) }
    }};
    return inputs;
}
auto Pressure_Velocity::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
        { "Velocity (enu)", m_output_stream },
    }};
    return outputs;
}

void Pressure_Velocity::process()
{
    QLOG_TOPIC("Pressure_Velocity::process");

    m_output_stream->clear();

    q::Seconds dt(m_output_stream->get_dt());
    float dts = dt.count();

    m_accumulator.process([this, dts](stream::IPressure::Sample const& sample)
    {
        if (sample.is_healthy)
        {
            float alt = static_cast<float>((1.0 - std::pow((sample.value / 101.325), 0.190284)) * 44307.69396);
            float speed = (m_last_altitude) ? ((alt - *m_last_altitude) / dts) : 0.0;
            m_last_altitude = alt;

            math::vec3f enu_velocity(0.f, 0.f, speed);
            m_output_stream->push_sample(enu_velocity, true);
        }
        else
        {
            m_output_stream->push_last_sample(false);
        }
    });
}

void Pressure_Velocity::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

auto Pressure_Velocity::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("Pressure_Velocity::set_config");

    sz::Pressure_Velocity::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Pressure_Velocity config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}
auto Pressure_Velocity::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Pressure_Velocity::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}
auto Pressure_Velocity::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
