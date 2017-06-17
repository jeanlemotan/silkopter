#pragma once

#include "HAL.h"
#include "common/stream/IAngular_Velocity.h"
#include "common/stream/IAcceleration.h"
#include "common/stream/IMagnetic_Field.h"
#include "common/stream/IPressure.h"
#include "common/stream/ITemperature.h"
#include "common/stream/IDistance.h"
#include "common/stream/IPosition.h"
#include "common/stream/IGPS_Info.h"
#include "common/stream/IVelocity.h"
#include "common/stream/IThrottle.h"
#include "common/node/IMultirotor_Simulator.h"

#include "Multirotor_Simulation.h"
#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
{
struct Multirotor_Simulator_Descriptor;
struct Multirotor_Simulator_Config;
}
}

namespace silk
{
namespace node
{

class Multirotor_Simulator : public IMultirotor_Simulator
{
public:
    Multirotor_Simulator(HAL& hal);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    ts::Result<std::shared_ptr<messages::INode_Message>> send_message(messages::INode_Message const& message) override;

    ts::Result<void> start(Clock::time_point tp) override;

    ts::Result<void> set_input_stream_path(size_t idx, std::string const& path);
    std::vector<Input> get_inputs() const;
    std::vector<Output> get_outputs() const;

    void process();

private:
    ts::Result<void> init();

    HAL& m_hal;

    std::shared_ptr<hal::Multirotor_Simulator_Descriptor> m_descriptor;
    std::shared_ptr<hal::Multirotor_Simulator_Config> m_config;

    Clock::time_point m_last_tp = Clock::now();

    struct Noise
    {
        std::default_random_engine generator;

        template<typename T> using Distribution = std::uniform_real_distribution<T>;

        Distribution<float> gps_position = Distribution<float>(0, 0);
        Distribution<float> gps_velocity = Distribution<float>(0, 0);
        Distribution<float> gps_pacc = Distribution<float>(0, 0);
        Distribution<float> gps_vacc = Distribution<float>(0, 0);
        Distribution<float> acceleration = Distribution<float>(0, 0);
        Distribution<float> angular_velocity = Distribution<float>(0, 0);
        Distribution<float> magnetic_field = Distribution<float>(0, 0);
        Distribution<float> pressure = Distribution<float>(0, 0);
        Distribution<float> temperature = Distribution<float>(0, 0);
        Distribution<float> ground_distance = Distribution<float>(0, 0);
    } m_noise;

    struct Angular_Velocity : public stream::IAngular_Velocity
    {
        std::vector<Sample> const& get_samples() const { return samples; }
        uint32_t get_rate() const { return rate; }
        uint32_t rate = 0;
        Clock::duration accumulated_dt = Clock::duration{0};
        Clock::duration dt = Clock::duration{0};
        std::vector<Sample> samples;
        Sample last_sample;
    };
    struct Acceleration : public stream::IAcceleration
    {
        std::vector<Sample> const& get_samples() const { return samples; }
        uint32_t get_rate() const { return rate; }
        uint32_t rate = 0;
        Clock::duration accumulated_dt = Clock::duration{0};
        Clock::duration dt = Clock::duration{0};
        std::vector<Sample> samples;
        Sample last_sample;
    };
    struct Magnetic_Field : public stream::IMagnetic_Field
    {
        std::vector<Sample> const& get_samples() const { return samples; }
        uint32_t get_rate() const { return rate; }
        uint32_t rate = 0;
        Clock::duration accumulated_dt = Clock::duration{0};
        Clock::duration dt = Clock::duration{0};
        std::vector<Sample> samples;
        Sample last_sample;
    };
    struct Pressure : public stream::IPressure
    {
        std::vector<Sample> const& get_samples() const { return samples; }
        uint32_t get_rate() const { return rate; }
        uint32_t rate = 0;
        Clock::duration accumulated_dt = Clock::duration{0};
        Clock::duration dt = Clock::duration{0};
        std::vector<Sample> samples;
        Sample last_sample;
    };
    struct Temperature : public stream::ITemperature
    {
        std::vector<Sample> const& get_samples() const { return samples; }
        uint32_t get_rate() const { return rate; }
        uint32_t rate = 0;
        Clock::duration accumulated_dt = Clock::duration{0};
        Clock::duration dt = Clock::duration{0};
        std::vector<Sample> samples;
        Sample last_sample;
    };
    struct Distance : public stream::IDistance
    {
        std::vector<Sample> const& get_samples() const { return samples; }
        uint32_t get_rate() const { return rate; }
        uint32_t rate = 0;
        Clock::duration accumulated_dt = Clock::duration{0};
        Clock::duration dt = Clock::duration{0};
        std::vector<Sample> samples;
        Sample last_sample;
    };
    struct GPS_Info : public stream::IGPS_Info
    {
        std::vector<Sample> const& get_samples() const { return samples; }
        uint32_t get_rate() const { return rate; }
        uint32_t rate = 0;
        Clock::duration accumulated_dt = Clock::duration{0};
        Clock::duration dt = Clock::duration{0};
        std::vector<Sample> samples;
        Sample last_sample;
    };
    struct ECEF_Position : public stream::IECEF_Position
    {
        std::vector<Sample> const& get_samples() const { return samples; }
        uint32_t get_rate() const { return rate; }
        uint32_t rate = 0;
        Clock::duration accumulated_dt = Clock::duration{0};
        Clock::duration dt = Clock::duration{0};
        std::vector<Sample> samples;
        Sample last_sample;
    };
    struct ECEF_Velocity : public stream::IECEF_Velocity
    {
        std::vector<Sample> const& get_samples() const { return samples; }
        uint32_t get_rate() const { return rate; }
        uint32_t rate = 0;
        Clock::duration accumulated_dt = Clock::duration{0};
        Clock::duration dt = Clock::duration{0};
        std::vector<Sample> samples;
        Sample last_sample;
    };
    typedef Basic_Output_Stream<stream::IMultirotor_Simulator_State> Simulator_State_Stream;
    mutable std::shared_ptr<Simulator_State_Stream> m_simulator_state_stream;

    mutable std::shared_ptr<Angular_Velocity> m_angular_velocity_stream;
    mutable std::shared_ptr<Acceleration> m_acceleration_stream;
    mutable std::shared_ptr<Magnetic_Field> m_magnetic_field_stream;
    mutable std::shared_ptr<Pressure> m_pressure_stream;
    mutable std::shared_ptr<Temperature> m_temperature_stream;
    mutable std::shared_ptr<Distance> m_distance_stream;
    mutable std::shared_ptr<GPS_Info> m_gps_info_stream;
    mutable std::shared_ptr<ECEF_Position> m_ecef_position_stream;
    mutable std::shared_ptr<ECEF_Velocity> m_ecef_velocity_stream;

    std::vector<std::weak_ptr<stream::IThrottle>> m_input_throttle_streams;
    std::vector<std::string> m_input_throttle_stream_paths;

    std::weak_ptr<stream::IMultirotor_State> m_input_state_stream;
    std::string m_input_state_stream_path;
    stream::IMultirotor_State::Value m_multirotor_state;

    Multirotor_Simulation m_simulation;
};


}
}

