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
        math::vec3f position;
        math::vec3f thrust_vector = math::vec3f(0, 0, 1);
        bool clockwise = false;
    };

    std::string name;
    float radius = 0; //m
    float height = 0; //m
    float mass = 0; //Kg
    float moment_of_inertia; //computed
    float motor_z_torque = 0; //Nm
    float motor_thrust = 0; //N
    float motor_acceleration = 0; //rpm/s
    float motor_deceleration = 0; //rpm/s
    std::vector<Motor> motors;
};

}
}
