#pragma once

namespace physics
{
namespace constants
{

    constexpr float g = 9.80665f; //newton's constant

    constexpr math::vec3f local_front_vector = math::vec3f(0, 1, 0);
    constexpr math::vec3f local_right_vector = math::vec3f(1, 0, 0);
    constexpr math::vec3f local_up_vector = math::vec3f(0, 0, 1);

    constexpr math::vec3f world_gravity = math::vec3f(0, 0, g);

}
}
