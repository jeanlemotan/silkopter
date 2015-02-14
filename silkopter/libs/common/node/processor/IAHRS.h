#pragma once

#include "IProcessor.h"

namespace silk
{
namespace node
{
namespace processor
{

class IAHRS : public IProcessor
{
public:
    struct Reference_Frame
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
    };

    typedef node::Sample<Reference_Frame>         Sample;
    typedef node::TP_Sample<Reference_Frame>      TP_Sample;

    virtual auto get_samples() const -> std::vector<Sample> const& = 0;

    //constants
    static constexpr math::vec3f k_local_front_vector = math::vec3f(0, 1, 0);
    static constexpr math::vec3f k_local_right_vector = math::vec3f(1, 0, 0);
    static constexpr math::vec3f k_local_up_vector = math::vec3f(0, 0, 1);

    static constexpr math::vec3f k_world_gravity_acceleration = math::vec3f(0, 0, 9.80665f);
};



}
}
}
