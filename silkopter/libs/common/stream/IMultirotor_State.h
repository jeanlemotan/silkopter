#pragma once

#include "Stream_Base.h"

#include "IFrame.h"
#include "ILinear_Acceleration.h"
#include "IBattery_State.h"
#include "IBattery_State.h"
#include "IPosition.h"
#include "IVelocity.h"
#include "IProximity.h"
#include "IFloat.h"
#include "IVideo.h"

#include "IMultirotor_Commands.h"

namespace silk
{
namespace stream
{

class IMultirotor_State : public IScalar_Stream<Semantic::MULTIROTOR_STATE>
{
public:
    typedef std::false_type can_be_filtered_t;

    typedef IMultirotor_Commands::Mode Mode;

    struct Value
    {
//        q::Clock::time_point time_point;
//        IBattery_State::Sample battery_state;
//        IFrame::Sample frame;
//        IECEF_Linear_Acceleration::Sample linear_acceleration;
//        IECEF_Position::Sample home_position;
//        IECEF_Position::Sample position;
//        IECEF_Velocity::Sample velocity;
//        IProximity::Sample proximity;
//        IFloat::Sample thrust;

//        IMultirotor_Commands::Value commands;
        struct Battery_State
        {
            float charge_used = 0; // Amperes-Hour (Ah)
            float average_voltage = 0; // Volts, averaged over one second
            float average_current = 0; // Amperes, averaged over one second
            float capacity_left = 0; //0 is Empty, 1 is Full
        } battery_state;

        math::quatf local_frame; //local space to enu space
        IECEF_Position::Value ecef_position;
        IENU_Velocity::Value enu_velocity;
        float throttle = 0;

        Mode mode = Mode::IDLE;
    };

    typedef stream::Sample<Value>     Sample;

    virtual ~IMultirotor_State() {}

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;
};


}
}



namespace util
{
namespace serialization
{

template<> inline void serialize(Buffer_t& buffer, silk::stream::IMultirotor_State::Value const& value, size_t& off)
{
//    serialize(buffer, value.time_point.time_since_epoch().count(), off);
//    serialize(buffer, value.battery_state, off);
//    serialize(buffer, value.frame, off);
//    serialize(buffer, value.linear_acceleration, off);
//    serialize(buffer, value.position, off);
//    serialize(buffer, value.home_position, off);
//    serialize(buffer, value.velocity, off);
//    serialize(buffer, value.proximity, off);
//    serialize(buffer, value.thrust, off);
//    serialize(buffer, value.commands, off);
    serialize(buffer, static_cast<uint8_t>(math::clamp(value.battery_state.charge_used, 0.f, 25.5f) * 10.f), off);
    serialize(buffer, static_cast<uint8_t>(math::clamp(value.battery_state.average_voltage, 0.f, 25.5f) * 10.f), off);
    serialize(buffer, static_cast<uint8_t>(math::clamp(value.battery_state.average_current, 0.f, 51.0f) * 5.f), off);
    serialize(buffer, static_cast<uint8_t>(math::clamp(value.battery_state.capacity_left, 0.f, 1.f) * 255.f), off);

    {
        math::vec4<int16_t> v(math::clamp(value.local_frame.x, -1.f, 1.f) * 32767.f,
                              math::clamp(value.local_frame.y, -1.f, 1.f) * 32767.f,
                              math::clamp(value.local_frame.z, -1.f, 1.f) * 32767.f,
                              math::clamp(value.local_frame.w, -1.f, 1.f) * 32767.f);
        serialize(buffer, v, off);
    }

    util::coordinates::LLA lla_position = util::coordinates::ecef_to_lla(value.ecef_position);

    {
        int64_t v = lla_position.latitude * 100000000.0; //8 decimal places
        uint8_t const* data = reinterpret_cast<uint8_t const*>(&v);
        serialize(buffer, data[0], off);
        serialize(buffer, data[1], off);
        serialize(buffer, data[2], off);
        serialize(buffer, data[3], off);
        serialize(buffer, data[4], off);
    }
    {
        int64_t v = lla_position.longitude * 100000000.0; //8 decimal places
        uint8_t const* data = reinterpret_cast<uint8_t const*>(&v);
        serialize(buffer, data[0], off);
        serialize(buffer, data[1], off);
        serialize(buffer, data[2], off);
        serialize(buffer, data[3], off);
        serialize(buffer, data[4], off);
    }
    {
        int16_t v = static_cast<int16_t>(math::clamp(lla_position.altitude, -327.0, 327.0) * 10.0);
        serialize(buffer, v, off);
    }

    {
        math::vec3<int16_t> v(math::clamp(value.enu_velocity.x, -327.f, 327.f) * 100.f,
                              math::clamp(value.enu_velocity.y, -327.f, 327.f) * 100.f,
                              math::clamp(value.enu_velocity.z, -327.f, 327.f) * 100.f);
        serialize(buffer, v, off);
    }

    serialize(buffer, static_cast<uint8_t>((math::clamp(value.throttle, 0.f, 1.f) * 255.f)), off);
    serialize(buffer, value.mode, off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::IMultirotor_State::Value& value, size_t& off) -> bool
{
    uint8_t v1, v2, v3, v4, v5;

    //battery state
    if (!deserialize(buffer, v1, off) ||
        !deserialize(buffer, v2, off) ||
        !deserialize(buffer, v3, off) ||
        !deserialize(buffer, v4, off))
    {
        return false;
    }

    value.battery_state.charge_used = v1 / 10.f;
    value.battery_state.average_voltage = v2 / 10.f;
    value.battery_state.average_current = v3 / 5.f;
    value.battery_state.capacity_left = v4 / 255.f;

    //local frame
    int16_t s1, s2, s3, s4;
    if (!deserialize(buffer, s1, off) ||
        !deserialize(buffer, s2, off) ||
        !deserialize(buffer, s3, off) ||
        !deserialize(buffer, s4, off))
    {
        return false;
    }

    {
        math::quatf v(s1 / 32767.f, s2 / 32767.f, s3 / 32767.f, s4 / 32767.f);
        value.local_frame = math::normalized<float, math::safe>(v);
    }

    util::coordinates::LLA lla_position;

    //latitude
    if (!deserialize(buffer, v1, off) ||
        !deserialize(buffer, v2, off) ||
        !deserialize(buffer, v3, off) ||
        !deserialize(buffer, v4, off) ||
        !deserialize(buffer, v5, off))
    {
        return false;
    }

    {
        int64_t v = 0;
        if (((v5 >> 7) & 1) == 1)
        {
            v = -1;
        }
        uint8_t* data = reinterpret_cast<uint8_t*>(&v);
        data[0] = v1;
        data[1] = v2;
        data[2] = v3;
        data[3] = v4;
        data[4] = v5;
        lla_position.latitude = v / 100000000.0;
    }

    //longitude
    if (!deserialize(buffer, v1, off) ||
        !deserialize(buffer, v2, off) ||
        !deserialize(buffer, v3, off) ||
        !deserialize(buffer, v4, off) ||
        !deserialize(buffer, v5, off))
    {
        return false;
    }

    {
        int64_t v = 0;
        if (((v5 >> 7) & 1) == 1)
        {
            v = -1;
        }
        uint8_t* data = reinterpret_cast<uint8_t*>(&v);
        data[0] = v1;
        data[1] = v2;
        data[2] = v3;
        data[3] = v4;
        data[4] = v5;
        lla_position.longitude = v / 100000000.0;
    }

    //altitude
    if (!deserialize(buffer, s1, off))
    {
        return false;
    }

    {
        double v = s1 / 100.0;
        lla_position.altitude = v;
    }

    value.ecef_position = util::coordinates::lla_to_ecef(lla_position);


    //enu velocity
    if (!deserialize(buffer, s1, off) ||
        !deserialize(buffer, s2, off) ||
        !deserialize(buffer, s3, off))
    {
        return false;
    }

    {
        math::vec3f v(s1 / 100.f, s2 / 100.f, s3 / 100.f);
        value.enu_velocity = v;
    }

    //sticks

    if (!deserialize(buffer, v1, off))
    {
        return false;
    }
    value.throttle = v1 / 255.f;

    if (!deserialize(buffer, value.mode, off))
    {
        return false;
    }

    return true;
}


}
}


