#pragma once

#include "common/node/IBrain.h"

#include "common/stream/IAngular_Velocity.h"
#include "common/stream/IFrame.h"
#include "common/stream/ILinear_Acceleration.h"
#include "common/stream/IBattery_State.h"
#include "common/stream/IPosition.h"
#include "common/stream/IVelocity.h"
#include "common/stream/IAcceleration.h"
#include "common/stream/IProximity.h"

#include "common/stream/IMulti_Input.h"
#include "common/stream/IMulti_State.h"

#include "Comms.h"
#include "HAL.h"
#include "utils/PID.h"
#include "utils/Butterworth.h"

#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace sz
{
namespace Multi_Brain
{
struct Init_Params;
struct Config;
}
}

namespace silk
{
namespace node
{

class Multi_Brain : public IBrain
{
public:
    Multi_Brain(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::Multi_Brain::Init_Params> m_init_params;
    std::shared_ptr<sz::Multi_Brain::Config> m_config;

    Sample_Accumulator<
        stream::IMulti_Input,
        stream::IFrame,
        stream::IECEF_Position,
        stream::IECEF_Velocity,
        stream::IECEF_Acceleration
//        stream::IBattery_State,
//        stream::IECEF_Linear_Acceleration,
//        stream::IECEF_Velocity,
//        stream::IProximity,
        > m_accumulator;

    typedef Basic_Output_Stream<stream::IMulti_State> State_Output_Stream;
    mutable std::shared_ptr<State_Output_Stream> m_state_output_stream;

    typedef Basic_Output_Stream<stream::IAngular_Velocity> Rate_Output_Stream;
    mutable std::shared_ptr<Rate_Output_Stream> m_rate_output_stream;

    typedef Basic_Output_Stream<stream::IForce> Thrust_Output_Stream;
    mutable std::shared_ptr<Thrust_Output_Stream> m_thrust_output_stream;
    float m_reference_thrust = 0;

    float m_dts = 0;

    struct Valid_State
    {
        template<class T> struct Data
        {
            T last_value;
            T value;
            q::Clock::time_point last_updated_tp;
        };

        Data<stream::IMulti_Input::Value> input;
        Data<stream::IFrame::Value> frame;
        Data<stream::IECEF_Position::Value> position;
        Data<stream::IECEF_Velocity::Value> velocity;
        Data<stream::IECEF_Acceleration::Value> acceleration;

        silk::config::Multi config;
    } m_state;
    void refresh_state(stream::IMulti_Input::Sample const& input,
                            stream::IFrame::Sample const& frame,
                            stream::IECEF_Position::Sample const& position,
                            stream::IECEF_Velocity::Sample const& velocity,
                            stream::IECEF_Acceleration::Sample const& acceleration);

    void process_state_mode_idle();
    void process_state_mode_armed();
    void process_state();

    void acquire_home_position();
    struct Home
    {
        bool is_acquired = false;
        util::coordinates::LLA lla_position;
        util::coordinates::ECEF ecef_position;
        std::deque<util::coordinates::ECEF> ecef_position_history;
        math::trans3dd enu_to_ecef_trans;
        math::trans3dd ecef_to_enu_trans;
    } m_home;

    typedef util::PID<float, float, float> PID;

    struct Altitude_Data
    {
        float reference_altitude = 0.f;
        util::Butterworth<float> velocity_dsp;
        util::Butterworth<float> acceleration_dsp;

        PID acceleration_pid;
        PID velocity_pd;
        PID altitude_p;
    } m_altitude_data;

    struct Horizontal_Angle_Data
    {
        PID x_pid;
        PID y_pid;
    } m_horizontal_angle_data;
};



}
}
