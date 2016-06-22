#pragma once

#include "common/node/IBrain.h"

#include "common/stream/IAngular_Velocity.h"
#include "common/stream/IFrame.h"
#include "common/stream/IFloat.h"
#include "common/stream/ILinear_Acceleration.h"
#include "common/stream/IBattery_State.h"
#include "common/stream/IPosition.h"
#include "common/stream/IVelocity.h"
#include "common/stream/IAcceleration.h"
#include "common/stream/IProximity.h"

#include "common/stream/IMultirotor_Commands.h"
#include "common/stream/IMultirotor_State.h"

#include "Multirotor_Properties.h"

#include "Comms.h"
#include "UAV.h"
#include "utils/PID.h"
#include "utils/Butterworth.h"
#include "LiPo_Battery.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace silk
{
namespace uav
{
struct Multirotor_Brain_Descriptor;
struct Multirotor_Brain_Config;
}
}

namespace silk
{
namespace node
{

class Multirotor_Brain : public IBrain
{
public:
    Multirotor_Brain(UAV& uav);

    bool init(uav::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const uav::INode_Descriptor> get_descriptor() const override;

    bool set_config(uav::INode_Config const& config) override;
    std::shared_ptr<const uav::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    UAV& m_uav;

    std::shared_ptr<uav::Multirotor_Brain_Descriptor> m_descriptor;
    std::shared_ptr<uav::Multirotor_Brain_Config> m_config;

    std::shared_ptr<const Multirotor_Properties> m_multirotor_properties;

    LiPo_Battery m_battery;

    Sample_Accumulator<stream::IMultirotor_Commands> m_commands_accumulator;

    Sample_Accumulator<
        stream::IUAV_Frame,
        stream::IECEF_Position,
        stream::IECEF_Velocity,
        stream::IECEF_Linear_Acceleration,
        stream::IProximity,
        stream::IVoltage,
        stream::ICurrent
        > m_sensor_accumulator;

    typedef Basic_Output_Stream<stream::IMultirotor_State> State_Output_Stream;
    mutable std::shared_ptr<State_Output_Stream> m_state_output_stream;

    typedef Basic_Output_Stream<stream::IAngular_Velocity> Rate_Output_Stream;
    mutable std::shared_ptr<Rate_Output_Stream> m_rate_output_stream;

    typedef Basic_Output_Stream<stream::IFloat> Thrust_Output_Stream;
    mutable std::shared_ptr<Thrust_Output_Stream> m_thrust_output_stream;

    float m_dts = 0;

    struct Inputs
    {
        template<class T> struct Data
        {
            T previous_sample;
            T sample;
            q::Clock::time_point last_valid_tp;
        };

        Data<stream::IMultirotor_Commands::Sample> local_commands;
        Data<stream::IMultirotor_Commands::Sample> remote_commands;
        Data<stream::IUAV_Frame::Sample> frame;
        Data<stream::IECEF_Position::Sample> position;
        Data<stream::IECEF_Velocity::Sample> velocity;
        Data<stream::IECEF_Linear_Acceleration::Sample> linear_acceleration;
        Data<stream::IProximity::Sample> proximity;
    } m_inputs;

    void refresh_inputs(stream::IUAV_Frame::Sample const& frame,
                        stream::IECEF_Position::Sample const& position,
                        stream::IECEF_Velocity::Sample const& velocity,
                        stream::IECEF_Linear_Acceleration::Sample const& linear_acceleration,
                        stream::IProximity::Sample const& proximity);

    float compute_ff_thrust(float target_altitude);
    math::vec2f compute_horizontal_rate_for_angle(math::vec2f const& angle);

    void process_state_mode_idle();
    void process_state_mode_armed();
    void state_mode_armed_apply_commands(const stream::IMultirotor_Commands::Value& prev_commands, stream::IMultirotor_Commands::Value& commands);
    void process_return_home_toggle(const stream::IMultirotor_Commands::Value& prev_commands, stream::IMultirotor_Commands::Value& commands);

    void process_state();

    void acquire_home_position();
    struct Home
    {
        bool is_acquired = false;
        util::coordinates::ECEF position;
        std::deque<util::coordinates::ECEF> position_history;
        math::trans3dd enu_to_ecef_transform;
        math::trans3dd ecef_to_enu_transform;
        math::mat3d enu_to_ecef_rotation;
        math::mat3d ecef_to_enu_rotation;
    } m_home;

    math::vec3f m_enu_position;
    math::vec3f m_enu_velocity;

    stream::IBattery_State::Sample m_battery_state_sample;

    typedef util::PID<float, float, float> PID;
    typedef util::PID<float, math::vec2f, math::vec2f> PID2;

    struct Vertical_Altitude_Data
    {
        PID speed_pi;
        PID position_p;
        util::Butterworth<float> dsp;
    } m_vertical_altitude_data;

    struct Horizontal_Angle_Data
    {
        PID x_pid;
        PID y_pid;
    } m_horizontal_angle_data;

    struct Horizontal_Position_Data
    {
        PID2 velocity_pi;
        PID2 position_p;
        util::Butterworth<math::vec2f> dsp;
    } m_horizontal_position_data;

    struct Yaw_Stable_Angle_Rate_Data
    {
        PID pid;
    } m_yaw_stable_angle_rate_data;
};



}
}
