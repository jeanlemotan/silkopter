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
#include <boost/optional.hpp>
//#include <experimental/optional>

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
//        Clock::time_point time_point;
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

        IFrame::Value local_frame; //local space to enu space
        boost::optional<IECEF_Position::Value> home_ecef_position;
        IECEF_Position::Value ecef_position;
        IENU_Velocity::Value enu_velocity;
        float throttle = 0;

        Mode mode = Mode::IDLE;
    };

    typedef stream::Sample<Value>     Sample;

    virtual ~IMultirotor_State() {}

    virtual std::vector<Sample> const& get_samples() const = 0;
};


}
}



namespace util
{
namespace serialization
{

inline void serialize_ecef_position(Buffer_t& buffer, silk::stream::IECEF_Position::Value const& value, size_t& off)
{
    util::coordinates::LLA lla_position = util::coordinates::ecef_to_lla(value);

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
        int64_t v64 = static_cast<int64_t>(lla_position.altitude * 10.0);
        int16_t v = static_cast<int16_t>(math::clamp(v64, int64_t(-32767), int64_t(32767)));
        serialize(buffer, v, off);
    }
}
inline bool deserialize_ecef_position(Buffer_t const& buffer, silk::stream::IECEF_Position::Value& value, size_t& off)
{
    uint8_t v1, v2, v3, v4, v5;

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

    int16_t s1;

    //altitude
    if (!deserialize(buffer, s1, off))
    {
        return false;
    }

    {
        double v = s1 / 10.0;
        lla_position.altitude = v;
    }

    value = util::coordinates::lla_to_ecef(lla_position);

    return true;
}

///////////////////////////////////////////////////////////////////////////

inline void serialize_part1(Buffer_t& buffer, silk::stream::IMultirotor_State::Value const& value, size_t& off)
{
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

    serialize(buffer, static_cast<uint8_t>((math::clamp(value.throttle, 0.f, 1.f) * 255.f)), off);
    serialize(buffer, value.mode, off);
}

inline auto deserialize_part1(Buffer_t const& buffer, silk::stream::IMultirotor_State::Value& value, size_t& off) -> bool
{
    uint8_t v1, v2, v3, v4;

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

///////////////////////////////////////////////////////////////////////////

inline void serialize_part2(Buffer_t& buffer, silk::stream::IMultirotor_State::Value const& value, size_t& off)
{
    serialize_ecef_position(buffer, value.ecef_position, off);

    {
        math::vec3<int16_t> v(math::clamp(value.enu_velocity.x, -327.f, 327.f) * 100.f,
                              math::clamp(value.enu_velocity.y, -327.f, 327.f) * 100.f,
                              math::clamp(value.enu_velocity.z, -327.f, 327.f) * 100.f);
        serialize(buffer, v, off);
    }
}

inline auto deserialize_part2(Buffer_t const& buffer, silk::stream::IMultirotor_State::Value& value, size_t& off) -> bool
{
    if (!deserialize_ecef_position(buffer, value.ecef_position, off))
    {
        return false;
    }

    int16_t s1, s2, s3;

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

    return true;
}

///////////////////////////////////////////////////////////////////////////

inline void serialize_home(Buffer_t& buffer, silk::stream::IMultirotor_State::Value const& value, size_t& off)
{
    serialize(buffer, value.home_ecef_position.is_initialized(), off);
    if (value.home_ecef_position)
    {
        serialize_ecef_position(buffer, *value.home_ecef_position, off);
    }
}

inline auto deserialize_home(Buffer_t const& buffer, silk::stream::IMultirotor_State::Value& value, size_t& off) -> bool
{
    bool valid = false;
    if (!deserialize(buffer, valid, off))
    {
        return false;
    }
    if (!valid)
    {
        value.home_ecef_position = boost::none;
    }
    else
    {
        silk::stream::IECEF_Position::Value position;
        if (!deserialize_ecef_position(buffer, position, off))
        {
            return false;
        }
        value.home_ecef_position = position;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////

template<> inline void serialize(Buffer_t& buffer, silk::stream::IMultirotor_State::Value const& value, size_t& off)
{
    serialize_part1(buffer, value, off);
    serialize_part2(buffer, value, off);
    serialize_home(buffer, value, off);
}

template<> inline auto deserialize(Buffer_t const& buffer, silk::stream::IMultirotor_State::Value& value, size_t& off) -> bool
{
    return deserialize_part1(buffer, value, off) &&
            deserialize_part2(buffer, value, off) &&
            deserialize_home(buffer, value, off);
}


}
}


