#pragma once

#include "Stream_Base.h"
#include "IMultirotor_State.h"

namespace silk
{
namespace stream
{

class IMultirotor_Simulator_State : public IScalar_Stream<Semantic::MULTIROTOR_SIMULATOR_STATE>
{
public:
    typedef std::false_type can_be_filtered_t;

    struct Value
    {
        struct Motor_State
        {
            float drag_factor = 0;
            float throttle = 0;
            float thrust = 0;

            //the following come from the config
            float max_thrust = 0;
            math::vec3f position;
        };

        float radius = 0.f; //comes from the config directly

        math::vec3f enu_position;
        math::quatf local_to_enu_rotation;

        math::vec3f enu_velocity;
        math::vec3f enu_linear_acceleration;
        math::vec3f acceleration;
        math::vec3f angular_velocity;
        math::vec3f magnetic_field;
        double pressure = 0;
        float temperature = 0;
        math::vec3f proximity_distance;

        std::vector<Motor_State> motors;

        IMultirotor_State::Value multirotor_state; //comes from the brain, it's what the brain thinks
    };

    typedef stream::Sample<Value>     Sample;

    virtual ~IMultirotor_Simulator_State() = default;

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}



namespace util
{
namespace serialization
{

template<> inline void serialize(Buffer_t& buffer, silk::stream::IMultirotor_Simulator_State::Value::Motor_State const& value, size_t& off)
{
    serialize(buffer, value.throttle, off);
    serialize(buffer, value.thrust, off);
    serialize(buffer, value.max_thrust, off);
    serialize(buffer, value.position, off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::IMultirotor_Simulator_State::Value::Motor_State& value, size_t& off) -> bool
{
    return deserialize(buffer, value.throttle, off) &&
            deserialize(buffer, value.thrust, off) &&
            deserialize(buffer, value.max_thrust, off) &&
            deserialize(buffer, value.position, off);
}

template<> inline void serialize(Buffer_t& buffer, silk::stream::IMultirotor_Simulator_State::Value const& value, size_t& off)
{
    serialize(buffer, value.enu_position, off);
    serialize(buffer, value.local_to_enu_rotation, off);
    serialize(buffer, value.enu_velocity, off);
    serialize(buffer, value.enu_linear_acceleration, off);
    serialize(buffer, value.acceleration, off);
    serialize(buffer, value.angular_velocity, off);
    serialize(buffer, value.magnetic_field, off);
    serialize(buffer, value.pressure, off);
    serialize(buffer, value.temperature, off);
    serialize(buffer, value.proximity_distance, off);
    serialize(buffer, value.motors, off);
    serialize(buffer, value.multirotor_state, off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::IMultirotor_Simulator_State::Value& value, size_t& off) -> bool
{
    return deserialize(buffer, value.enu_position, off) &&
        deserialize(buffer, value.local_to_enu_rotation, off) &&
        deserialize(buffer, value.enu_velocity, off) &&
        deserialize(buffer, value.enu_linear_acceleration, off) &&
        deserialize(buffer, value.acceleration, off) &&
        deserialize(buffer, value.angular_velocity, off) &&
        deserialize(buffer, value.magnetic_field, off) &&
        deserialize(buffer, value.pressure, off) &&
        deserialize(buffer, value.temperature, off) &&
        deserialize(buffer, value.proximity_distance, off) &&
        deserialize(buffer, value.motors, off) &&
        deserialize(buffer, value.multirotor_state, off);
}


}
}


