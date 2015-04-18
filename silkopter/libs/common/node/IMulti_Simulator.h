#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class IMulti_Simulator : public INode_Base<Type::SIMULATOR>
{
public:
    struct Motor_State
    {
        float drag_factor = 0;
        float throttle = 0;
        float thrust = 0;
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

