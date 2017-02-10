#pragma once

#include "utils/Clock.h"
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

#include "uav_properties/IMultirotor_Properties.h"

#include "RC_Comms.h"
#include "HAL.h"
#include "utils/PID.h"
#include "utils/Butterworth.h"
#include "LiPo_Battery.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
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
    Multirotor_Brain(HAL& hal);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    ts::Result<std::shared_ptr<messages::INode_Message>> send_message(messages::INode_Message const& message) override;

    ts::Result<void> start(Clock::time_point tp) override;

    ts::Result<void> set_input_stream_path(size_t idx, std::string const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();

    HAL& m_hal;

    std::shared_ptr<hal::Multirotor_Brain_Descriptor> m_descriptor;
    std::shared_ptr<hal::Multirotor_Brain_Config> m_config;

    std::shared_ptr<const IMultirotor_Properties> m_multirotor_properties;

    LiPo_Battery m_battery;

    Sample_Accumulator<stream::IMultirotor_Commands> m_commands_accumulator;

    Sample_Accumulator<
        stream::IFrame,
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
            Clock::time_point last_valid_tp; //when was the last valid sample
            Clock::time_point last_invalid_tp; //when was the last invalid sample
            bool is_stable = false; //is the data stable, without invalid samples lately?
        };

        Data<stream::IMultirotor_Commands::Sample> commands;
        Data<stream::IFrame::Sample> frame;
        Data<stream::IECEF_Position::Sample> position;
        Data<stream::IECEF_Velocity::Sample> velocity;
        Data<stream::IECEF_Linear_Acceleration::Sample> linear_acceleration;
        Data<stream::IProximity::Sample> proximity;
    } m_inputs;

    template<typename T> void process_input_data(Inputs::Data<T>& data, T const& new_sample);

    typedef stream::IMultirotor_Commands::Mode Mode;
    typedef stream::IMultirotor_Commands::Vertical_Mode Vertical_Mode;
    typedef stream::IMultirotor_Commands::Horizontal_Mode Horizontal_Mode;
    typedef stream::IMultirotor_Commands::Yaw_Mode Yaw_Mode;

    float compute_ff_thrust(float target_altitude) const;
    math::vec2f compute_horizontal_rate_for_angle(math::vec2f const& target_angle);
    math::vec2f compute_horizontal_rate_for_position(math::vec2f const& target_pos);
    float compute_thrust_for_altitude(float target_alt);
    float compute_yaw_rate_for_angle(float target_angle);
    void process_vhy_modes(Vertical_Mode vertical_mode, Horizontal_Mode horizontal_mode, Yaw_Mode yaw_mode, stream::IMultirotor_Commands::Sticks const& sticks);

    void process_idle_mode();

    void process_flight_modes();
    void process_fly_mode();
    void process_return_home_mode();

    void process_mode();

    Mode m_mode = Mode::IDLE;
    void set_mode(Mode mode);

    Vertical_Mode m_vertical_mode = Vertical_Mode::THRUST;
    void set_vertical_mode(Vertical_Mode mode);

    Horizontal_Mode m_horizontal_mode = Horizontal_Mode::ANGLE;
    void set_horizontal_mode(Horizontal_Mode mode);

    Yaw_Mode m_yaw_mode = Yaw_Mode::ANGLE_RATE;
    void set_yaw_mode(Yaw_Mode mode);


    ts::Result<void> check_pre_flight_conditions() const;

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

    std::deque<float> m_thrust_history;

    math::vec3f m_enu_position;
    math::vec3f m_enu_velocity;

    stream::IBattery_State::Sample m_battery_state_sample;

    typedef util::PID<float, float, float> PID;
    typedef util::PID<float, math::vec2f, math::vec2f> PID2;

    struct Vertical_Mode_Data
    {
        float target_altitude = 0;

        PID speed_pi;
        PID altitude_p;
        util::Butterworth<float> altitude_dsp;
    } m_vertical_mode_data;

    struct Horizontal_Mode_Data
    {
        math::vec2f target_enu_position;

        PID angle_x_pid;
        PID angle_y_pid;

        PID2 velocity_pi;
        PID2 position_p;
        util::Butterworth<math::vec2f> position_dsp;
    } m_horizontal_mode_data;

    struct Yaw_Mode_Data
    {
        float target_angle = 0.f;

        PID angle_pid;
    } m_yaw_mode_data;

    struct Fly_Mode_Data
    {
        enum class State
        {
            NORMAL,
            ALERT_HOLD,
        };

        State state = State::NORMAL;
        Vertical_Mode old_vertical_mode;
        Horizontal_Mode old_horizontal_mode;
        Yaw_Mode old_yaw_mode;
    };

    Fly_Mode_Data m_fly_mode_data;

    struct Return_Home_Mode_Data
    {
    };

//    Clock::time_point m_last_invalid_commands_tp = Clock::now();
};



}
}
