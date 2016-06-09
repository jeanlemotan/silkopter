#include "BrainStdAfx.h"
#include "Pressure_Velocity.h"

#include "uav.def.h"
//#include "sz_Pressure_Velocity.hpp"

namespace silk
{
namespace node
{

Pressure_Velocity::Pressure_Velocity(UAV& uav)
    : m_uav(uav)
    , m_descriptor(new Pressure_Velocity_Descriptor())
    , m_config(new Pressure_Velocity_Config())
{
    m_output_stream = std::make_shared<Output_Stream>();
}

auto Pressure_Velocity::init(std::shared_ptr<Node_Descriptor_Base> descriptor) -> bool
{
    QLOG_TOPIC("Pressure_Velocity::init");

    auto specialized = std::dynamic_pointer_cast<Pressure_Velocity_Descriptor>(descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }

    *m_descriptor = *specialized;

    return init();
}
auto Pressure_Velocity::init() -> bool
{
    m_output_stream->set_rate(m_descriptor->get_rate());

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
        { stream::IPressure::TYPE, m_descriptor->get_rate(), "Pressure", m_accumulator.get_stream_path(0) }
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
    m_accumulator.set_stream_path(idx, path, m_output_stream->get_rate(), m_uav);
}

auto Pressure_Velocity::set_config(std::shared_ptr<Node_Config_Base> config) -> bool
{
    QLOG_TOPIC("Pressure_Velocity::set_config");

    auto specialized = std::dynamic_pointer_cast<Pressure_Velocity_Config>(config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }

    *m_config = *specialized;

    return true;
}
auto Pressure_Velocity::get_config() const -> std::shared_ptr<Node_Config_Base>
{
    return m_config;
}

auto Pressure_Velocity::get_descriptor() const -> std::shared_ptr<Node_Descriptor_Base>
{
    return m_descriptor;
}
auto Pressure_Velocity::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
