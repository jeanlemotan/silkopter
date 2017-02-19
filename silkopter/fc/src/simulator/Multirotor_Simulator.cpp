#include "FCStdAfx.h"
#include "Multirotor_Simulator.h"
#include "uav_properties/IMultirotor_Properties.h"

#include "hal.def.h"
#include "messages.def.h"
//#include "sz_Multirotor_Simulator.hpp"
//#include "sz_Multirotor_Simulator_Structs.hpp"

namespace silk
{
namespace node
{

Multirotor_Simulator::Multirotor_Simulator(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::Multirotor_Simulator_Descriptor())
    , m_config(new hal::Multirotor_Simulator_Config())
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
    m_simulator_state_stream = std::make_shared<Simulator_State_Stream>();
}

ts::Result<void> Multirotor_Simulator::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("multirotor_simulator::init");

    auto specialized = dynamic_cast<hal::Multirotor_Simulator_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}
ts::Result<void> Multirotor_Simulator::init()
{
    std::shared_ptr<const IMultirotor_Properties> multirotor_properties = m_hal.get_specialized_uav_properties<IMultirotor_Properties>();
    if (!multirotor_properties)
    {
        return make_error("No multi properties found");
    }

    if (!m_simulation.init(1000))
    {
        return make_error("Cannot initialize simulator world");
    }

    if (!m_simulation.init_uav(multirotor_properties))
    {
        return make_error("Cannot initialize UAV simulator");
    }

    m_input_throttle_streams.resize(multirotor_properties->get_motors().size());
    m_input_throttle_stream_paths.resize(multirotor_properties->get_motors().size());

    m_angular_velocity_stream->rate = m_descriptor->get_angular_velocity_rate();
    m_angular_velocity_stream->dt = std::chrono::microseconds(1000000 / m_angular_velocity_stream->rate);

    m_acceleration_stream->rate = m_descriptor->get_acceleration_rate();
    m_acceleration_stream->dt = std::chrono::microseconds(1000000 / m_acceleration_stream->rate);

    m_magnetic_field_stream->rate = m_descriptor->get_magnetic_field_rate();
    m_magnetic_field_stream->dt = std::chrono::microseconds(1000000 / m_magnetic_field_stream->rate);

    m_pressure_stream->rate = m_descriptor->get_pressure_rate();
    m_pressure_stream->dt = std::chrono::microseconds(1000000 / m_pressure_stream->rate);

    m_temperature_stream->rate = m_descriptor->get_temperature_rate();
    m_temperature_stream->dt = std::chrono::microseconds(1000000 / m_temperature_stream->rate);

    m_distance_stream->rate = m_descriptor->get_distance_rate();
    m_distance_stream->dt = std::chrono::microseconds(1000000 / m_distance_stream->rate);

    m_gps_info_stream->rate = m_descriptor->get_gps_rate();
    m_gps_info_stream->dt = std::chrono::microseconds(1000000 / m_gps_info_stream->rate);

    m_ecef_position_stream->rate = m_descriptor->get_gps_rate();
    m_ecef_position_stream->dt = std::chrono::microseconds(1000000 / m_ecef_position_stream->rate);

    m_ecef_velocity_stream->rate = m_descriptor->get_gps_rate();
    m_ecef_velocity_stream->dt = std::chrono::microseconds(1000000 / m_ecef_velocity_stream->rate);

    m_simulator_state_stream->set_rate(30);

    return ts::success;
}

ts::Result<void> Multirotor_Simulator::start(Clock::time_point tp)
{
    m_last_tp = tp;
    m_simulator_state_stream->set_tp(tp);
    return ts::success;
}

auto Multirotor_Simulator::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs(m_input_throttle_streams.size() + 1);
    for (size_t i = 0; i < m_input_throttle_streams.size(); i++)
    {
        inputs[i].type = stream::IThrottle::TYPE;
        inputs[i].rate = m_descriptor->get_throttle_rate();
        inputs[i].name = q::util::format<std::string>("throttle_{}", i);
        inputs[i].stream_path = m_input_throttle_stream_paths[i];
    }
    inputs.back().type = stream::IMultirotor_State::TYPE;
    inputs.back().rate = m_descriptor->get_state_rate();
    inputs.back().name = "state";
    inputs.back().stream_path = m_input_state_stream_path;

    return inputs;
}
auto Multirotor_Simulator::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs =
    {
        {"angular_velocity", m_angular_velocity_stream},
        {"acceleration", m_acceleration_stream},
        {"magnetic_field", m_magnetic_field_stream},
        {"pressure", m_pressure_stream},
        {"temperature", m_temperature_stream},
        {"sonar_distance", m_distance_stream},
        {"gps_info", m_gps_info_stream},
        {"gps_position", m_ecef_position_stream},
        {"gps_velocity", m_ecef_velocity_stream},
        {"simulator_state", m_simulator_state_stream},
    };
    return outputs;
}

void Multirotor_Simulator::process()
{
    QLOG_TOPIC("multirotor_simulator::process");

    m_angular_velocity_stream->samples.clear();
    m_acceleration_stream->samples.clear();
    m_magnetic_field_stream->samples.clear();
    m_pressure_stream->samples.clear();
    m_temperature_stream->samples.clear();
    m_distance_stream->samples.clear();
    m_gps_info_stream->samples.clear();
    m_ecef_position_stream->samples.clear();
    m_ecef_velocity_stream->samples.clear();
    m_simulator_state_stream->clear();

    for (size_t i = 0; i < m_input_throttle_streams.size(); i++)
    {
        auto stream = m_input_throttle_streams[i].lock();
        if (stream)
        {
            auto const& samples = stream->get_samples();
            if (!samples.empty())
            {
                m_simulation.set_motor_throttle(i, samples.back().value);
            }
        }
    }

    {
        auto stream = m_input_state_stream.lock();
        if (stream)
        {
            auto const& samples = stream->get_samples();
            if (!samples.empty())
            {
                m_multirotor_state = samples.back().value;
            }
        }
    }

    auto now = Clock::now();
    auto dt = now - m_last_tp;
    if (dt < std::chrono::milliseconds(1))
    {
        return;
    }
    m_last_tp = now;

    static const util::coordinates::LLA origin_lla(math::radians(41.390205), math::radians(2.154007), 37.5);
    auto enu_to_ecef_trans = util::coordinates::enu_to_ecef_transform(origin_lla);
    auto enu_to_ecef_rotation = util::coordinates::enu_to_ecef_rotation(origin_lla);

    m_simulation.process(dt, [this, &enu_to_ecef_trans, &enu_to_ecef_rotation](Multirotor_Simulation& simulation, Clock::duration simulation_dt)
    {
        auto const& uav_state = simulation.get_state();
        {
            Angular_Velocity& stream = *m_angular_velocity_stream;
            stream.accumulated_dt += simulation_dt;
            while (stream.accumulated_dt >= stream.dt)
            {
                math::vec3f noise(m_noise.angular_velocity(m_noise.generator), m_noise.angular_velocity(m_noise.generator), m_noise.angular_velocity(m_noise.generator));
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
                math::vec3f noise(m_noise.acceleration(m_noise.generator), m_noise.acceleration(m_noise.generator), m_noise.acceleration(m_noise.generator));
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
                math::vec3f noise(m_noise.magnetic_field(m_noise.generator), m_noise.magnetic_field(m_noise.generator), m_noise.magnetic_field(m_noise.generator));
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
                double noise = m_noise.pressure(m_noise.generator);
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
                float noise = m_noise.temperature(m_noise.generator);
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
                float noise = m_noise.ground_distance(m_noise.generator);
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
                stream.last_sample.value.pacc = m_noise.gps_pacc(m_noise.generator);
                stream.last_sample.value.vacc = m_noise.gps_vacc(m_noise.generator);
                stream.last_sample.is_healthy = true;
                stream.samples.push_back(stream.last_sample);
            }
        }
        {
            ECEF_Position& stream = *m_ecef_position_stream;
            stream.accumulated_dt += simulation_dt;
            while (stream.accumulated_dt >= stream.dt)
            {
                math::vec3d noise(m_noise.gps_position(m_noise.generator), m_noise.gps_position(m_noise.generator), m_noise.gps_position(m_noise.generator));
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
                math::vec3f noise(m_noise.gps_velocity(m_noise.generator), m_noise.gps_velocity(m_noise.generator), m_noise.gps_velocity(m_noise.generator));
                stream.accumulated_dt -= stream.dt;
                stream.last_sample.value = math::vec3f(math::transform(enu_to_ecef_rotation, math::vec3d(uav_state.enu_velocity))) + noise;
                stream.last_sample.is_healthy = true;
                stream.samples.push_back(stream.last_sample);
            }
        }
    });

    {
        size_t samples_needed = m_simulator_state_stream->compute_samples_needed();
        Multirotor_Simulation::State simulation_state = m_simulation.get_state();
        simulation_state.multirotor_state = m_multirotor_state;
        for (size_t i = 0; i < samples_needed; i++)
        {
            m_simulator_state_stream->push_sample(simulation_state, true);
        }
    }
}

ts::Result<void> Multirotor_Simulator::set_input_stream_path(size_t idx, std::string const& path)
{
    if (idx < m_input_throttle_streams.size())
    {
        m_input_throttle_streams[idx].reset();
        m_input_throttle_stream_paths[idx].clear();
        if (!path.empty())
        {
            std::shared_ptr<stream::IThrottle> input_stream = m_hal.get_stream_registry().find_by_name<stream::IThrottle>(path);
            if (input_stream)
            {
                uint32_t rate = input_stream->get_rate();
                if (rate != m_descriptor->get_throttle_rate())
                {
                    return make_error("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", path, m_descriptor->get_throttle_rate(), rate);
                }
                else
                {
                    m_input_throttle_streams[idx] = input_stream;
                    m_input_throttle_stream_paths[idx] = path;
                }
            }
            else
            {
                return make_error("Cannot find stream '{}'", path);
            }
        }
    }
    else
    {
        m_input_state_stream.reset();
        m_input_state_stream_path.clear();
        if (!path.empty())
        {
            std::shared_ptr<stream::IMultirotor_State> input_stream = m_hal.get_stream_registry().find_by_name<stream::IMultirotor_State>(path);
            if (input_stream)
            {
                uint32_t rate = input_stream->get_rate();
                if (rate != m_descriptor->get_state_rate())
                {
                    return make_error("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", path, m_descriptor->get_state_rate(), rate);
                }
                else
                {
                    m_input_state_stream = input_stream;
                    m_input_state_stream_path = path;
                }
            }
            else
            {
                return make_error("Cannot find stream '{}'", path);
            }
        }
    }
    return ts::success;
}

ts::Result<void> Multirotor_Simulator::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("multirotor_simulator::set_config");

    auto specialized = dynamic_cast<hal::Multirotor_Simulator_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
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

    std::shared_ptr<const IMultirotor_Properties> multirotor_properties = m_hal.get_specialized_uav_properties<IMultirotor_Properties>();
    if (!multirotor_properties)
    {
        return make_error("No multi properties found");
    }

    if (!m_simulation.init_uav(multirotor_properties))
    {
        return make_error("Cannot configure UAV simulator");
    }
    *m_config = *specialized;

    m_simulation.set_gravity_enabled(m_config->get_gravity_enabled());
    m_simulation.set_ground_enabled(m_config->get_ground_enabled());
    m_simulation.set_drag_enabled(m_config->get_drag_enabled());
    m_simulation.set_simulation_enabled(m_config->get_simulation_enabled());

    auto const& noise = m_config->get_noise();

    m_noise.gps_position = Noise::Distribution<float>(-noise.get_gps_position()*0.5f, noise.get_gps_position()*0.5f);
    m_noise.gps_velocity = Noise::Distribution<float>(-noise.get_gps_velocity()*0.5f, noise.get_gps_velocity()*0.5f);
    m_noise.gps_pacc = Noise::Distribution<float>(-noise.get_gps_pacc()*0.5f, noise.get_gps_pacc()*0.5f);
    m_noise.gps_vacc = Noise::Distribution<float>(-noise.get_gps_vacc()*0.5f, noise.get_gps_vacc()*0.5f);
    m_noise.acceleration = Noise::Distribution<float>(-noise.get_acceleration()*0.5f, noise.get_acceleration()*0.5f);
    m_noise.angular_velocity = Noise::Distribution<float>(-noise.get_angular_velocity()*0.5f, noise.get_angular_velocity()*0.5f);
    m_noise.magnetic_field = Noise::Distribution<float>(-noise.get_magnetic_field()*0.5f, noise.get_magnetic_field()*0.5f);
    m_noise.pressure = Noise::Distribution<float>(-noise.get_pressure()*0.5f, noise.get_pressure()*0.5f);
    m_noise.temperature = Noise::Distribution<float>(-noise.get_temperature()*0.5f, noise.get_temperature()*0.5f);
    m_noise.ground_distance = Noise::Distribution<float>(-noise.get_ground_distance()*0.5f, noise.get_ground_distance()*0.5f);

    return ts::success;
}
auto Multirotor_Simulator::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto Multirotor_Simulator::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

ts::Result<std::shared_ptr<messages::INode_Message>> Multirotor_Simulator::send_message(messages::INode_Message const& _message)
{
    if (dynamic_cast<messages::Multirotor_Simulator_Reset_Message const*>(&_message))
    {
        m_simulation.reset();
        return nullptr;
    }
    else if (dynamic_cast<messages::Multirotor_Simulator_Stop_Motion_Message const*>(&_message))
    {
        m_simulation.stop_motion();
        return nullptr;
    }
    else if (messages::Multirotor_Simulator_Set_Gravity_Enabled_Message const* message = dynamic_cast<messages::Multirotor_Simulator_Set_Gravity_Enabled_Message const*>(&_message))
    {
        m_simulation.set_gravity_enabled(message->get_enabled());
        return nullptr;
    }
    else if (messages::Multirotor_Simulator_Set_Ground_Enabled_Message const* message = dynamic_cast<messages::Multirotor_Simulator_Set_Ground_Enabled_Message const*>(&_message))
    {
        m_simulation.set_ground_enabled(message->get_enabled());
        return nullptr;
    }
    else if (messages::Multirotor_Simulator_Set_Simulation_Enabled_Message const* message = dynamic_cast<messages::Multirotor_Simulator_Set_Simulation_Enabled_Message const*>(&_message))
    {
        m_simulation.set_simulation_enabled(message->get_enabled());
        return nullptr;
    }
    else if (messages::Multirotor_Simulator_Set_Drag_Enabled_Message const* message = dynamic_cast<messages::Multirotor_Simulator_Set_Drag_Enabled_Message const*>(&_message))
    {
        m_simulation.set_drag_enabled(message->get_enabled());
        return nullptr;
    }
    else
    {
        return make_error("Unknown message");
    }
}


}
}
