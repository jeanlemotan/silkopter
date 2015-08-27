#pragma once

#include "common/node/INode.h"

namespace silk
{
namespace node
{

class IMulti_Simulator : public Node_Base<Type::SIMULATOR>
{
public:
    struct Motor_State
    {
        double drag_factor = 0;
        double throttle = 0;
        double thrust = 0;
    };

    struct UAV_State
    {
        math::vec3d enu_position;
        math::vec3d enu_velocity;
        math::vec3d enu_linear_acceleration;
        math::vec3d acceleration;
        math::quatd local_to_enu_rotation;
        math::vec3d angular_velocity;
        math::vec3d magnetic_field;
        double pressure = 0;
        double temperature = 0;
        math::vec3d proximity_distance;
        std::vector<Motor_State> motors;
    };

};



}
}

