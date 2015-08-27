#pragma once

namespace silk
{
namespace config
{

class Multi
{
public:
    struct Motor
    {
        math::vec3d position;
        bool clockwise = false;
    };

    std::string name;
    double radius = 0; //m
    double height = 0; //m
    double mass = 0; //Kg
    double moment_of_inertia; //computed
    double motor_z_torque = 0; //Nm
    double motor_thrust = 0; //N
    double motor_acceleration = 0; //rpm/s
    double motor_deceleration = 0; //rpm/s
    std::vector<Motor> motors;
};

}
}
