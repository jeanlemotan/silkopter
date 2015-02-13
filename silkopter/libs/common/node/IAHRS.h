#pragma once

#include "common/node/Node_Data.h"

namespace silk
{

class IAHRS : q::util::Noncopyable
{
public:
    struct
    {
        struct
        {        
            math::vec3f angular_velocity;
            math::vec3f gravity_acceleration;
            math::vec3f linear_acceleration;
        } local;
        struct
        {        
            math::vec3f front_vector;
            math::vec3f right_vector;
            math::vec3f up_vector;
        } world;

        math::quatf local_to_world;
        math::quatf world_to_local;
    } Reference_Frame;

    typedef Sample<Reference_Frame>         Sample;
    typedef TP_Sample<Reference_Frame>      TP_Sample;

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    //constants
    constexpr math::vec3f k_local_front_vector(0, 1, 0);
    constexpr math::vec3f k_local_right_vector(1, 0, 0);
    constexpr math::vec3f k_local_up_vector(0, 0, 1);

    constexpr math::vec3f k_world_gravity_acceleration(0, 0, 9.80665f);
};

}
