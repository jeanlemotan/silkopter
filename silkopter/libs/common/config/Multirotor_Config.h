#pragma once

#include "UAV_Config.h"

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
