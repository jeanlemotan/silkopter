#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class ISimulator : public INode_Base<Type::SIMULATOR>
{
public:
    struct Motor_Config
    {
        math::vec2f position;
        bool clockwise = true;
        float max_thrust = 0; //N
        float max_rpm = 0; //rot/min
        float acceleration = 0;
        float deceleration = 0;
    };

    struct UAV_Config
    {
        float radius = 0.5f;
        float height = 0.3f;
        float mass = 0.7f; //Kg
        std::vector<Motor_Config> motors;
    };


    struct Motor_State
    {
        float drag_factor = 0;
        float throttle = 0;
        float thrust = 0;
        float rpm = 0;
    };

    struct UAV_State
    {
        math::vec3f enu_position;
        math::vec3f enu_velocity;
        math::vec3f enu_linear_acceleration;
        math::vec3f acceleration;
        math::quatf local_to_enu_rotation;
        math::vec3f angular_velocity;
        std::vector<Motor_State> motors;
    };

};



}
}

