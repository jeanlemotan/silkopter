#pragma once

#include "UAV_Config.h"
#include "def_lang/Mapper.h"
#include "common/Math_Mappers.h"

namespace silk
{

class Multirotor_Config : public UAV_Config
{
public:
    static constexpr Type TYPE = Type::MULTIROTOR;

    Multirotor_Config()
        : UAV_Config(TYPE)
    {}

    struct Motor
    {
        math::vec3f position;
        math::vec3f thrust_vector = math::vec3f(0, 0, 1);
        bool clockwise = false;
    };

    float radius = 1.f; //m
    float height = 1.f; //m
    float motor_z_torque = 0.f; //Nm
    float motor_thrust = 1.f; //N
    float motor_acceleration = 10.f; //N/s
    float motor_deceleration = 10.f; //N/s
    std::vector<Motor> motors;
};

}

namespace ts
{
namespace mapper
{

template<>
inline Result<void> get(IValue const& src, silk::Multirotor_Config::Motor& dst)
{
    auto result = get(src, "position", dst.position); if (result != ts::success) return result;
    result = get(src, "thrust_vector", dst.thrust_vector); if (result != ts::success) return result;
    result = get(src, "clockwise", dst.clockwise); if (result != ts::success) return result;
    return ts::success;
}
template<>
inline Result<void> set(IValue& dst, silk::Multirotor_Config::Motor const& src)
{
    auto result = set(dst, "position", src.position); if (result != ts::success) return result;
    result = set(dst, "thrust_vector", src.thrust_vector); if (result != ts::success) return result;
    result = set(dst, "clockwise", src.clockwise); if (result != ts::success) return result;
    return ts::success;
}

template<>
inline Result<void> get(IValue const& src, silk::Multirotor_Config& dst)
{
    auto result = get(src, "radius", dst.radius); if (result != ts::success) return result;
    result = get(src, "height", dst.height); if (result != ts::success) return result;
    result = get(src, "motor_z_torque", dst.motor_z_torque); if (result != ts::success) return result;
    result = get(src, "motor_thrust", dst.motor_thrust); if (result != ts::success) return result;
    result = get(src, "motor_acceleration", dst.motor_acceleration); if (result != ts::success) return result;
    result = get(src, "motor_deceleration", dst.motor_deceleration); if (result != ts::success) return result;
    result = get(src, "motors", dst.motors); if (result != ts::success) return result;
    return ts::success;
}
template<>
inline Result<void> set(IValue& dst, silk::Multirotor_Config const& src)
{
    auto result = set(dst, "radius", src.radius); if (result != ts::success) return result;
    result = set(dst, "height", src.height); if (result != ts::success) return result;
    result = set(dst, "motor_z_torque", src.motor_z_torque); if (result != ts::success) return result;
    result = set(dst, "motor_thrust", src.motor_thrust); if (result != ts::success) return result;
    result = set(dst, "motor_acceleration", src.motor_acceleration); if (result != ts::success) return result;
    result = set(dst, "motor_deceleration", src.motor_deceleration); if (result != ts::success) return result;
    result = set(dst, "motors", src.motors); if (result != ts::success) return result;
    return ts::success;
}


}
}
