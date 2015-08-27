#pragma once

namespace physics
{
namespace constants
{

    constexpr double g = 9.80665; //newton's constant

    constexpr math::vec3d local_front_vector = math::vec3d(0, 1, 0);
    constexpr math::vec3d local_right_vector = math::vec3d(1, 0, 0);
    constexpr math::vec3d local_up_vector = math::vec3d(0, 0, 1);

    constexpr math::vec3d world_gravity = math::vec3d(0, 0, g);

}
}
