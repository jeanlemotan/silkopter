#include "BrainStdAfx.h"
#include "Pressure_Velocity.h"

#include "hal.def.h"
//#include "sz_Pressure_Velocity.hpp"

namespace silk
{
namespace node
{

Pressure_Velocity::Pressure_Velocity(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Pressure_Velocity_Descriptor())
    , m_config(new hal::Pressure_Velocity_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

ts::Result<void> Pressure_Velocity::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("Pressure_Velocity::init");

    auto specialized = dynamic_cast<hal::Pressure_Velocity_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}
ts::Result<void> Pressure_Velocity::init()
{
    m_output_stream->set_rate(m_descriptor->get_rate());
    return ts::success;
}

ts::Result<void> Pressure_Velocity::start(q::Clock::time_point tp)
{
    m_output_stream->set_tp(tp);
    return ts::success;
}

auto Pressure_Velocity::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IPressure::TYPE, m_descriptor->get_rate(), "pressure", m_accumulator.get_stream_path(0) }
    }};
    return inputs;
}
auto Pressure_Velocity::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {{
        { "velocity", m_output_stream },
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

ts::Result<void> Pressure_Velocity::set_input_stream_path(size_t idx, std::string const& path)
{
    return m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_hal);
}

ts::Result<void> Pressure_Velocity::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("Pressure_Velocity::set_config");

    auto specialized = dynamic_cast<hal::Pressure_Velocity_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
auto Pressure_Velocity::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto Pressure_Velocity::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}
//auto Pressure_Velocity::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}


}
}
