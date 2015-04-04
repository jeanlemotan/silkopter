#include "BrainStdAfx.h"
#include "Multi_Simulator.h"

#if !defined RASPBERRY_PI

#include "sz_math.hpp"
#include "sz_Multi_Simulator.hpp"

namespace silk
{
namespace node
{

Multi_Simulator::Multi_Simulator(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Multi_Simulator::Init_Params())
    , m_config(new sz::Multi_Simulator::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
}

auto Multi_Simulator::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("multi_simulator::init");

    sz::Multi_Simulator::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Multi_Simulator data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}
auto Multi_Simulator::init() -> bool
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

    auto multi_config = m_hal.get_multi_config();
    if (!multi_config)
    {
        QLOGE("No multi config found");
        return false;
    }

    if (!m_simulation.init(1000))
    {
        return false;
    }

    m_last_tp = q::Clock::now();

    m_input_pwm_streams.resize(multi_config->motors.size());
    m_config->inputs.pwm.resize(multi_config->motors.size());

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



    return true;
}

auto Multi_Simulator::get_inputs() const -> std::vector<Input>
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
auto Multi_Simulator::get_outputs() const -> std::vector<Output>
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

void Multi_Simulator::process()
{
    QLOG_TOPIC("multi_simulator::process");

    auto now = q::Clock::now();
    auto dt = now - m_last_tp;
    m_last_tp = now;

    for (size_t i = 0; i < m_input_pwm_streams.size(); i++)
    {
        auto pwm = m_input_pwm_streams[i].lock();
        if (pwm)
        {
            auto const& samples = pwm->get_samples();
            if (!samples.empty())
            {
                m_simulation.set_motor_throttle(i, samples.back().value);
            }
        }
    }

    m_angular_velocity_stream->samples.clear();
    m_acceleration_stream->samples.clear();
    m_magnetic_field_stream->samples.clear();
    m_pressure_stream->samples.clear();
    m_temperature_stream->samples.clear();
    m_distance_stream->samples.clear();
    m_ecef_location_stream->samples.clear();

    m_simulation.process(dt, [this](Multi_Simulation& simulation, q::Clock::duration simulation_dt)
    {
        auto const& uav_state = simulation.get_uav_state();
        {
            auto& stream = *m_angular_velocity_stream;
            stream.accumulated_dt += simulation_dt;
            while (stream.accumulated_dt >= stream.last_sample.dt)
            {
                stream.accumulated_dt -= stream.last_sample.dt;
                stream.last_sample.tp += stream.last_sample.dt;
                stream.last_sample.sample_idx++;
                stream.last_sample.value = uav_state.angular_velocity;
                stream.samples.push_back(stream.last_sample);
            }
        }
        {
            auto& stream = *m_acceleration_stream;
            stream.accumulated_dt += simulation_dt;
            while (stream.accumulated_dt >= stream.last_sample.dt)
            {
                stream.accumulated_dt -= stream.last_sample.dt;
                stream.last_sample.tp += stream.last_sample.dt;
                stream.last_sample.sample_idx++;
                stream.last_sample.value = uav_state.acceleration;
                stream.samples.push_back(stream.last_sample);
            }
        }
    });
}

auto Multi_Simulator::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("multi_simulator::set_config");

    sz::Multi_Simulator::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Multi_Simulator config data: {}", ss.str());
        return false;
    }

//    Simulation::UAV_Config uav_config;
//    uav_config.mass = sz.uav.mass;
//    uav_config.radius = sz.uav.radius;
//    uav_config.height = sz.uav.height;
//    uav_config.motors.resize(sz.motors.size());
//    for (size_t i = 0; i < uav_config.motors.size(); i++)
//    {
//        uav_config.motors[i].position = sz.motors[i].position;
//        uav_config.motors[i].clockwise = sz.motors[i].clockwise;
//        uav_config.motors[i].max_thrust = sz.motors[i].max_thrust;
//        uav_config.motors[i].max_rpm = sz.motors[i].max_rpm;
//        uav_config.motors[i].acceleration = sz.motors[i].acceleration;
//        uav_config.motors[i].deceleration = sz.motors[i].deceleration;
//    }

    auto multi_config = m_hal.get_multi_config();
    if (!multi_config)
    {
        QLOGE("No multi config found");
        return false;
    }

    if (!m_simulation.init_uav(*multi_config))
    {
        return false;
    }

    m_simulation.set_gravity_enabled(sz.gravity_enabled);
    m_simulation.set_ground_enabled(sz.ground_enabled);
    m_simulation.set_simulation_enabled(sz.simulation_enabled);

    *m_config = sz;

    for (size_t i = 0; i < sz.inputs.pwm.size(); i++)
    {
        auto input_stream = m_hal.get_streams().find_by_name<stream::IPWM>(sz.inputs.pwm[i]);
        auto rate = input_stream ? input_stream->get_rate() : 0u;
        if (rate != m_init_params->pwm_rate)
        {
            m_config->inputs.pwm[i].clear();
            QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", sz.inputs.pwm[i], m_init_params->pwm_rate, rate);
            m_input_pwm_streams[i].reset();
        }
        else
        {
            m_input_pwm_streams[i] = input_stream;
        }
    }


    return true;
}
auto Multi_Simulator::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Multi_Simulator::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}
auto Multi_Simulator::send_message(rapidjson::Value const& json) -> rapidjson::Document
{
    rapidjson::Document response;

    auto* messagej = jsonutil::find_value(json, std::string("message"));
    if (!messagej)
    {
        jsonutil::add_value(response, std::string("error"), rapidjson::Value("Message not found"), response.GetAllocator());
    }
    else if (!messagej->IsString())
    {
        jsonutil::add_value(response, std::string("error"), rapidjson::Value("Message has to be a string"), response.GetAllocator());
    }
    else
    {
        std::string message = messagej->GetString();
        if (message == "reset")
        {
            m_simulation.reset();
        }
        else if (message == "stop motion")
        {
            m_simulation.stop_motion();
        }
        else if (message == "get state")
        {
            auto const& state = m_simulation.get_uav_state();
            autojsoncxx::to_document(state, response);
        }
        else
        {
            jsonutil::add_value(response, std::string("error"), rapidjson::Value("Unknown message"), response.GetAllocator());
        }
    }
    return std::move(response);
}


}
}

#endif
