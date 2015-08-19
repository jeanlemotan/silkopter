#pragma once

#include "common/node/IBrain.h"

#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IFrame.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/IBattery_State.h"
#include "common/node/stream/IPosition.h"
#include "common/node/stream/IVelocity.h"
#include "common/node/stream/IProximity.h"

#include "common/node/stream/IMulti_Input.h"
#include "common/node/stream/IMulti_State.h"

#include "Comms.h"
#include "HAL.h"

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
        stream::IECEF_Position
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


    stream::IMulti_State::Value m_state;

    void process_input_mode_idle(stream::IMulti_Input::Value& new_input, silk::config::Multi const& multi_config);
    void process_input_mode_armed(stream::IMulti_Input::Value& new_input, silk::config::Multi const& multi_config);

    void process_input(stream::IMulti_Input::Value const& new_input, silk::config::Multi const& multi_config);

    void acquire_home_position(stream::IECEF_Position::Sample const& position);
    struct Home
    {
        util::coordinates::LLA lla_position;
        util::coordinates::ECEF ecef_position;
        math::trans3dd enu_to_ecef_trans;
        math::trans3dd ecef_to_enu_trans;
    } m_home;
};



}
}
