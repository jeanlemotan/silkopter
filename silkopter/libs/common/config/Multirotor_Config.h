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

    float radius = 0; //m
    float height = 0; //m
    float motor_z_torque = 0; //Nm
    float motor_thrust = 0; //N
    float motor_acceleration = 0; //rpm/s
    float motor_deceleration = 0; //rpm/s
    std::vector<Motor> motors;
};

}
