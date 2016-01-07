#include "BrainStdAfx.h"
#include "Multi_Simulator.h"

#if !defined RASPBERRY_PI

#include "sz_math.hpp"
#include "sz_Multi_Simulator.hpp"
#include "sz_Multi_Simulator_Structs.hpp"

namespace silk
{
namespace node
{

Multi_Simulator::Multi_Simulator(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Multi_Simulator::Init_Params())
    , m_config(new sz::Multi_Simulator::Config())
{
    m_angular_velocity_stream = std::make_shared<Angular_Velocity>();
    m_acceleration_stream = std::make_shared<Acceleration>();
    m_magnetic_field_stream = std::make_shared<Magnetic_Field>();
    m_pressure_stream = std::make_shared<Pressure>();
    m_temperature_stream = std::make_shared<Temperature>();
    m_distance_stream = std::make_shared<Distance>();
    m_gps_info_stream = std::make_shared<GPS_Info>();
    m_ecef_position_stream = std::make_shared<ECEF_Position>();
    m_ecef_velocity_stream = std::make_shared<ECEF_Velocity>();
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
    *m_init_params = sz;
    return init();
}
auto Multi_Simulator::init() -> bool
{
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
    if (m_init_params->gps_rate == 0)
    {
        QLOGE("Bad gps rate: {}Hz", m_init_params->gps_rate);
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

    if (!m_simulation.init_uav(*multi_config))
    {
        return false;
    }

    m_last_tp = q::Clock::now();

    m_input_throttle_streams.resize(multi_config->motors.size());
    m_input_throttle_stream_paths.resize(multi_config->motors.size());

    m_angular_velocity_stream->rate = m_init_params->angular_velocity_rate;
    m_angular_velocity_stream->dt = std::chrono::microseconds(1000000 / m_angular_velocity_stream->rate);

    m_acceleration_stream->rate = m_init_params->acceleration_rate;
    m_acceleration_stream->dt = std::chrono::microseconds(1000000 / m_acceleration_stream->rate);

    m_magnetic_field_stream->rate = m_init_params->magnetic_field_rate;
    m_magnetic_field_stream->dt = std::chrono::microseconds(1000000 / m_magnetic_field_stream->rate);

    m_pressure_stream->rate = m_init_params->pressure_rate;
    m_pressure_stream->dt = std::chrono::microseconds(1000000 / m_pressure_stream->rate);

    m_temperature_stream->rate = m_init_params->temperature_rate;
    m_temperature_stream->dt = std::chrono::microseconds(1000000 / m_temperature_stream->rate);

    m_distance_stream->rate = m_init_params->distance_rate;
    m_distance_stream->dt = std::chrono::microseconds(1000000 / m_distance_stream->rate);

    m_gps_info_stream->rate = m_init_params->gps_rate;
    m_gps_info_stream->dt = std::chrono::microseconds(1000000 / m_gps_info_stream->rate);

    m_ecef_position_stream->rate = m_init_params->gps_rate;
    m_ecef_position_stream->dt = std::chrono::microseconds(1000000 / m_ecef_position_stream->rate);

    m_ecef_velocity_stream->rate = m_init_params->gps_rate;
    m_ecef_velocity_stream->dt = std::chrono::microseconds(1000000 / m_ecef_velocity_stream->rate);


    return true;
}

auto Multi_Simulator::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(m_input_throttle_streams.size());
    for (size_t i = 0; i < m_input_throttle_streams.size(); i++)
    {
        inputs[i].type = stream::IThrottle::TYPE;
        inputs[i].rate = m_init_params->throttle_rate;
        inputs[i].name = q::util::format2<std::string>("Throttle/[{}]", i);
        inputs[i].stream_path = m_input_throttle_stream_paths[i];
    }
    return inputs;
}
auto Multi_Simulator::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {
        {"Angular Velocity", m_angular_velocity_stream},
        {"Acceleration", m_acceleration_stream},
        {"Magnetic Field", m_magnetic_field_stream},
        {"Pressure", m_pressure_stream},
        {"Temperature", m_temperature_stream},
        {"Sonar Distance", m_distance_stream},
        {"GPS Info", m_gps_info_stream},
        {"GPS Position (ecef)", m_ecef_position_stream},
        {"GPS Velocity (ecef)", m_ecef_velocity_stream},
    };
    return outputs;
}

void Multi_Simulator::process()
{
    QLOG_TOPIC("multi_simulator::process");

    m_angular_velocity_stream->samples.clear();
    m_acceleration_stream->samples.clear();
    m_magnetic_field_stream->samples.clear();
    m_pressure_stream->samples.clear();
    m_temperature_stream->samples.clear();
    m_distance_stream->samples.clear();
    m_gps_info_stream->samples.clear();
    m_ecef_position_stream->samples.clear();
    m_ecef_velocity_stream->samples.clear();

    auto now = q::Clock::now();
    auto dt = now - m_last_tp;
    if (dt < std::chrono::milliseconds(1))
    {
        return;
    }
    m_last_tp = now;

    for (size_t i = 0; i < m_input_throttle_streams.size(); i++)
    {
        auto throttle = m_input_throttle_streams[i].lock();
        if (throttle)
        {
            auto const& samples = throttle->get_samples();
            if (!samples.empty())
            {
                m_simulation.set_motor_throttle(i, samples.back().value);
            }
        }
    }

    static const util::coordinates::LLA origin_lla(math::radians(41.390205), math::radians(2.154007), 0.0);
    auto enu_to_ecef_trans = util::coordinates::enu_to_ecef_transform(origin_lla);
    auto enu_to_ecef_rotation = util::coordinates::enu_to_ecef_rotation(origin_lla);

    m_simulation.process(dt, [this, &enu_to_ecef_trans, &enu_to_ecef_rotation](Multi_Simulation& simulation, q::Clock::duration simulation_dt)
    {
        auto const& uav_state = simulation.get_uav_state();
        {
            Angular_Velocity& stream = *m_angular_velocity_stream;
            stream.accumulated_dt += simulation_dt;
            while (stream.accumulated_dt >= stream.dt)
            {
                math::vec3f noise(m_noise.angular_velocity_sd(m_noise.generator), m_noise.angular_velocity_sd(m_noise.generator), m_noise.angular_velocity_sd(m_noise.generator));
                stream.accumulated_dt -= stream.dt;
                stream.last_sample.value = uav_state.angular_velocity + noise;
                stream.last_sample.is_healthy = true;
                stream.samples.push_back(stream.last_sample);
            }
        }
        {
            Acceleration& stream = *m_acceleration_stream;
            stream.accumulated_dt += simulation_dt;
            while (stream.accumulated_dt >= stream.dt)
            {
                math::vec3f noise(m_noise.acceleration_sd(m_noise.generator), m_noise.acceleration_sd(m_noise.generator), m_noise.acceleration_sd(m_noise.generator));
                stream.accumulated_dt -= stream.dt;
                stream.last_sample.value = uav_state.acceleration + noise;
                stream.last_sample.is_healthy = true;
                stream.samples.push_back(stream.last_sample);
            }
        }
        {
            Magnetic_Field& stream = *m_magnetic_field_stream;
            stream.accumulated_dt += simulation_dt;
            while (stream.accumulated_dt >= stream.dt)
            {
                math::vec3f noise(m_noise.magnetic_field_sd(m_noise.generator), m_noise.magnetic_field_sd(m_noise.generator), m_noise.magnetic_field_sd(m_noise.generator));
                stream.accumulated_dt -= stream.dt;
                QASSERT(math::is_finite(uav_state.magnetic_field));
                QASSERT(!math::is_zero(uav_state.magnetic_field, math::epsilon<float>()));
                stream.last_sample.value = uav_state.magnetic_field + noise;
                stream.last_sample.is_healthy = true;
                stream.samples.push_back(stream.last_sample);
            }
        }
        {
            Pressure& stream = *m_pressure_stream;
            stream.accumulated_dt += simulation_dt;
            while (stream.accumulated_dt >= stream.dt)
            {
                float noise = m_noise.pressure_sd(m_noise.generator);
                stream.accumulated_dt -= stream.dt;
                stream.last_sample.value = uav_state.pressure + noise;
                stream.last_sample.is_healthy = true;
                stream.samples.push_back(stream.last_sample);
            }
        }
        {
            Temperature& stream = *m_temperature_stream;
            stream.accumulated_dt += simulation_dt;
            while (stream.accumulated_dt >= stream.dt)
            {
                float noise = m_noise.temperature_sd(m_noise.generator);
                stream.accumulated_dt -= stream.dt;
                stream.last_sample.value = uav_state.temperature + noise;
                stream.last_sample.is_healthy = true;
                stream.samples.push_back(stream.last_sample);
            }
        }
        {
            Distance& stream = *m_distance_stream;
            stream.accumulated_dt += simulation_dt;
            while (stream.accumulated_dt >= stream.dt)
            {
                float noise = m_noise.ground_distance_sd(m_noise.generator);
                stream.accumulated_dt -= stream.dt;
                stream.last_sample.value = uav_state.proximity_distance + noise;
                stream.last_sample.is_healthy = !math::is_zero(uav_state.proximity_distance, std::numeric_limits<float>::epsilon());
                stream.samples.push_back(stream.last_sample);
            }
        }

        {
            GPS_Info& stream = *m_gps_info_stream;
            stream.accumulated_dt += simulation_dt;
            while (stream.accumulated_dt >= stream.dt)
            {
                stream.accumulated_dt -= stream.dt;
                stream.last_sample.value.fix = stream::IGPS_Info::Value::Fix::FIX_3D;
                stream.last_sample.value.visible_satellites = 4;
                stream.last_sample.value.fix_satellites = 4;
                stream.last_sample.value.pacc = m_noise.gps_pacc_sd(m_noise.generator);
                stream.last_sample.value.vacc = m_noise.gps_vacc_sd(m_noise.generator);
                stream.last_sample.is_healthy = true;
                stream.samples.push_back(stream.last_sample);
            }
        }
        {
            ECEF_Position& stream = *m_ecef_position_stream;
            stream.accumulated_dt += simulation_dt;
            while (stream.accumulated_dt >= stream.dt)
            {
                math::vec3d noise(m_noise.gps_position_sd(m_noise.generator), m_noise.gps_position_sd(m_noise.generator), m_noise.gps_position_sd(m_noise.generator));
                stream.accumulated_dt -= stream.dt;
                stream.last_sample.value = math::transform(enu_to_ecef_trans, math::vec3d(uav_state.enu_position)) + noise;
                stream.last_sample.is_healthy = true;
                stream.samples.push_back(stream.last_sample);
            }
        }
        {
            ECEF_Velocity& stream = *m_ecef_velocity_stream;
            stream.accumulated_dt += simulation_dt;
            while (stream.accumulated_dt >= stream.dt)
            {
                math::vec3f noise(m_noise.gps_velocity_sd(m_noise.generator), m_noise.gps_velocity_sd(m_noise.generator), m_noise.gps_velocity_sd(m_noise.generator));
                stream.accumulated_dt -= stream.dt;
                stream.last_sample.value = math::vec3f(math::transform(enu_to_ecef_rotation, math::vec3d(uav_state.enu_velocity))) + noise;
                stream.last_sample.is_healthy = true;
                stream.samples.push_back(stream.last_sample);
            }
        }
    });
}

void Multi_Simulator::set_input_stream_path(size_t idx, q::Path const& path)
{
    auto input_stream = m_hal.get_streams().find_by_name<stream::IThrottle>(path.get_as<std::string>());
    auto rate = input_stream ? input_stream->get_rate() : 0u;
    if (rate != m_init_params->throttle_rate)
    {
        QLOGW("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", path, m_init_params->throttle_rate, rate);
        m_input_throttle_streams[idx].reset();
        m_input_throttle_stream_paths[idx].clear();
    }
    else
    {
        m_input_throttle_streams[idx] = input_stream;
        m_input_throttle_stream_paths[idx] = path;
    }
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
    m_simulation.set_drag_enabled(sz.drag_enabled);
    m_simulation.set_simulation_enabled(sz.simulation_enabled);

    *m_config = sz;

    m_config->noise.gps_position_sd = math::max(m_config->noise.gps_position_sd, 0.f);
    m_noise.gps_position_sd = std::normal_distribution<float>(0, m_config->noise.gps_position_sd);

    m_config->noise.gps_velocity_sd = math::max(m_config->noise.gps_velocity_sd, 0.f);
    m_noise.gps_velocity_sd = std::normal_distribution<float>(0, m_config->noise.gps_velocity_sd);

    m_config->noise.gps_pacc_sd = math::max(m_config->noise.gps_pacc_sd, 0.f);
    m_noise.gps_pacc_sd = std::normal_distribution<float>(0, m_config->noise.gps_pacc_sd);

    m_config->noise.gps_vacc_sd = math::max(m_config->noise.gps_vacc_sd, 0.f);
    m_noise.gps_vacc_sd = std::normal_distribution<float>(0, m_config->noise.gps_vacc_sd);

    m_config->noise.acceleration_sd = math::max(m_config->noise.acceleration_sd, 0.f);
    m_noise.acceleration_sd = std::normal_distribution<float>(0, m_config->noise.acceleration_sd);

    m_config->noise.angular_velocity_sd = math::max(m_config->noise.angular_velocity_sd, 0.f);
    m_noise.angular_velocity_sd = std::normal_distribution<float>(0, m_config->noise.angular_velocity_sd);

    m_config->noise.magnetic_field_sd = math::max(m_config->noise.magnetic_field_sd, 0.f);
    m_noise.magnetic_field_sd = std::normal_distribution<float>(0, m_config->noise.magnetic_field_sd);

    m_config->noise.pressure_sd = math::max(m_config->noise.pressure_sd, 0.f);
    m_noise.pressure_sd = std::normal_distribution<float>(0, m_config->noise.pressure_sd);

    m_config->noise.temperature_sd = math::max(m_config->noise.temperature_sd, 0.f);
    m_noise.temperature_sd = std::normal_distribution<float>(0, m_config->noise.temperature_sd);

    m_config->noise.ground_distance_sd = math::max(m_config->noise.ground_distance_sd, 0.f);
    m_noise.ground_distance_sd = std::normal_distribution<float>(0, m_config->noise.ground_distance_sd);

    return true;
}
auto Multi_Simulator::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Multi_Simulator::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
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
