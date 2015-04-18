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
        float max_z_torque = 0; //Nm
        float max_thrust = 0; //N
        float acceleration = 0; //rpm/s
        float deceleration = 0; //rpm/s
    };

    std::string name;
    float radius = 0; //m
    float height = 0; //m
    float mass = 0; //Kg
    std::vector<Motor> motors;
};

}
}
