#include "BrainStdAfx.h"
#include "Simulator.h"

#include "sz_math.hpp"
#include "sz_Simulator.hpp"

namespace silk
{
namespace node
{

Simulator::Simulator(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Simulator::Init_Params())
    , m_config(new sz::Simulator::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto Simulator::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("simulator::init");

    sz::Simulator::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Simulator data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}
auto Simulator::init() -> bool
{
    m_angular_velocity_stream = std::make_shared<Angular_Velocity>();
    m_acceleration_stream = std::make_shared<Acceleration>();
    m_magnetic_field_stream = std::make_shared<Magnetic_Field>();
    m_pressure_stream = std::make_shared<Pressure>();
    m_temperature_stream = std::make_shared<Temperature>();
    m_distance_stream = std::make_shared<Distance>();
    m_ecef_location_stream = std::make_shared<ECEF_Location>();
    if (m_init_params->angular_velocity_rate == 0)
    {
        QLOGE("Bad angular velocity rate: {}Hz", m_init_params->angular_velocity_rate);
        return false;
    }
    if (m_init_params->acceleration_rate == 0)
    {
        QLOGE("Bad acceleration rate: {}Hz", m_init_params->acceleration_rate);
        return false;
    }
    if (m_init_params->magnetic_field_rate == 0)
    {
        QLOGE("Bad magnetic field rate: {}Hz", m_init_params->magnetic_field_rate);
        return false;
    }
    if (m_init_params->pressure_rate == 0)
    {
        QLOGE("Bad pressure rate: {}Hz", m_init_params->pressure_rate);
        return false;
    }
    if (m_init_params->temperature_rate == 0)
    {
        QLOGE("Bad temperature rate: {}Hz", m_init_params->temperature_rate);
        return false;
    }
    if (m_init_params->distance_rate == 0)
    {
        QLOGE("Bad distance rate: {}Hz", m_init_params->distance_rate);
        return false;
    }
    if (m_init_params->location_rate == 0)
    {
        QLOGE("Bad location rate: {}Hz", m_init_params->location_rate);
        return false;
    }
    if (m_init_params->motor_count == 0)
    {
        QLOGE("Bad motor count: {}", m_init_params->motor_count);
        return false;
    }
    if (!m_world.init_world(1000))
    {
        return false;
    }

    m_last_tp = q::Clock::now();

    m_angular_velocity_stream->rate = m_init_params->angular_velocity_rate;
    m_angular_velocity_stream->last_sample.dt = std::chrono::microseconds(1000000 / m_angular_velocity_stream->rate);
    m_angular_velocity_stream->last_sample.tp = m_last_tp;

    m_acceleration_stream->rate = m_init_params->acceleration_rate;
    m_acceleration_stream->last_sample.dt = std::chrono::microseconds(1000000 / m_acceleration_stream->rate);
    m_acceleration_stream->last_sample.tp = m_last_tp;

    m_magnetic_field_stream->rate = m_init_params->magnetic_field_rate;
    m_magnetic_field_stream->last_sample.dt = std::chrono::microseconds(1000000 / m_magnetic_field_stream->rate);
    m_magnetic_field_stream->last_sample.tp = m_last_tp;

    m_pressure_stream->rate = m_init_params->pressure_rate;
    m_pressure_stream->last_sample.dt = std::chrono::microseconds(1000000 / m_pressure_stream->rate);
    m_pressure_stream->last_sample.tp = m_last_tp;

    m_temperature_stream->rate = m_init_params->temperature_rate;
    m_temperature_stream->last_sample.dt = std::chrono::microseconds(1000000 / m_temperature_stream->rate);
    m_temperature_stream->last_sample.tp = m_last_tp;

    m_distance_stream->rate = m_init_params->distance_rate;
    m_distance_stream->last_sample.dt = std::chrono::microseconds(1000000 / m_distance_stream->rate);
    m_distance_stream->last_sample.tp = m_last_tp;

    m_ecef_location_stream->rate = m_init_params->location_rate;
    m_ecef_location_stream->last_sample.dt = std::chrono::microseconds(1000000 / m_ecef_location_stream->rate);
    m_ecef_location_stream->last_sample.tp = m_last_tp;


    m_input_pwm_streams.resize(m_init_params->motor_count);
    m_config->motors.resize(m_init_params->motor_count);
    m_config->inputs.pwm.resize(m_init_params->motor_count);

    return true;
}

auto Simulator::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(m_input_pwm_streams.size());
    for (size_t i = 0; i < m_input_pwm_streams.size(); i++)
    {
        inputs[i].type = stream::IPWM::TYPE;
        inputs[i].rate = m_init_params->pwm_rate;
        inputs[i].name = q::util::format2<std::string>("PWM/[{}]", i);
    }
    return inputs;
}
auto Simulator::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(7);
    outputs[0].type = stream::IAngular_Velocity::TYPE;
    outputs[0].name = "Angular Velocity";
    outputs[0].stream = m_angular_velocity_stream;
    outputs[1].type = stream::IAcceleration::TYPE;
    outputs[1].name = "Acceleration";
    outputs[1].stream = m_acceleration_stream;
    outputs[2].type = stream::IMagnetic_Field::TYPE;
    outputs[2].name = "Magnetic Field";
    outputs[2].stream = m_magnetic_field_stream;
    outputs[3].type = stream::IPressure::TYPE;
    outputs[3].name = "Pressure";
    outputs[3].stream = m_pressure_stream;
    outputs[4].type = stream::ITemperature::TYPE;
    outputs[4].name = "Temperature";
    outputs[4].stream = m_temperature_stream;
    outputs[5].type = stream::IDistance::TYPE;
    outputs[5].name = "Sonar Distance";
    outputs[5].stream = m_distance_stream;
    outputs[6].type = stream::IECEF_Location::TYPE;
    outputs[6].name = "Location (ecef)";
    outputs[6].stream = m_ecef_location_stream;
    return outputs;
}

void Simulator::process()
{
    QLOG_TOPIC("simulator::process");

    auto now = q::Clock::now();
    auto dt = now - m_last_tp;
    m_last_tp = now;

    m_angular_velocity_stream->samples.clear();
    m_acceleration_stream->samples.clear();
    m_magnetic_field_stream->samples.clear();
    m_pressure_stream->samples.clear();
    m_temperature_stream->samples.clear();
    m_distance_stream->samples.clear();
    m_ecef_location_stream->samples.clear();

    m_world.process(dt, [this](World& world, q::Clock::duration world_dt)
    {
        {
            auto& stream = *m_angular_velocity_stream;
            stream.accumulated_dt += world_dt;
            while (stream.accumulated_dt >= stream.last_sample.dt)
            {
                stream.accumulated_dt -= stream.last_sample.dt;
                stream.last_sample.tp += stream.last_sample.dt;
                stream.last_sample.sample_idx++;
                stream.last_sample.value = world.get_uav_angular_velocity();
                stream.samples.push_back(stream.last_sample);
            }
        }
        {
            auto& stream = *m_acceleration_stream;
            stream.accumulated_dt += world_dt;
            while (stream.accumulated_dt >= stream.last_sample.dt)
            {
                stream.accumulated_dt -= stream.last_sample.dt;
                stream.last_sample.tp += stream.last_sample.dt;
                stream.last_sample.sample_idx++;
                stream.last_sample.value = world.get_uav_acceleration();
                stream.samples.push_back(stream.last_sample);
            }
        }
    });
}

auto Simulator::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("simulator::set_config");

    sz::Simulator::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Simulator config data: {}", ss.str());
        return false;
    }

    World::UAV_Config uav_config;
    uav_config.mass = sz.uav.mass;
    uav_config.radius = sz.uav.radius;
    uav_config.height = sz.uav.height;
    uav_config.motors.resize(sz.motors.size());
    for (size_t i = 0; i < uav_config.motors.size(); i++)
    {
        uav_config.motors[i].position = sz.motors[i].position;
        uav_config.motors[i].clockwise = sz.motors[i].clockwise;
        uav_config.motors[i].max_thrust = sz.motors[i].max_thrust;
        uav_config.motors[i].max_rpm = sz.motors[i].max_rpm;
        uav_config.motors[i].acceleration = sz.motors[i].acceleration;
        uav_config.motors[i].deceleration = sz.motors[i].deceleration;
    }
    if (!m_world.init_uav(uav_config))
    {
        return false;
    }

    *m_config = sz;

    return true;
}
auto Simulator::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Simulator::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}


}
}
