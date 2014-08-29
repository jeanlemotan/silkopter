#pragma once

#include "Motor.h"
#include "Config.h"

class World;

class UAV
{
    friend class World;
public:
    auto get_motors() const -> std::vector<Motor> const&;

    void set_position(math::vec3f const&v);
    auto get_position() const -> math::vec3f const&;

    void set_rotation(math::quatf const& v);
    auto get_rotation() const -> math::quatf const&;

    auto get_angular_velocity() const -> math::vec3f const&;
    auto get_velocity() const -> math::vec3f const&;
    auto get_acceleration() const -> math::vec3f const&;
    auto get_linear_acceleration() const -> math::vec3f const&;

    void set_force(math::vec3f const& force);

    auto get_body() -> btRigidBody&;

    void stop();

protected:
    UAV(Config::UAV& config);

    void reset();
    void process(q::Clock::duration dt);

    Config::UAV& m_config;

    std::unique_ptr<btDefaultMotionState> m_motion_state;
    std::unique_ptr<btCollisionShape> m_shape;
    std::unique_ptr<btRigidBody> m_body;

    std::vector<Motor> m_motors;
    math::vec3f m_position;
    math::vec3f m_velocity;
    math::vec3f m_acceleration;
    math::vec3f m_linear_acceleration;
    math::quatf m_rotation;
    math::vec3f m_angular_velocity;

    math::vec3f m_target_force;
    math::vec3f m_force;

    math::mat3f m_local_to_world_mat;
    math::mat3f m_world_to_local_mat;
};
