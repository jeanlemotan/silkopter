#include "stdafx.h"
#include "UAV.h"
#include "physics/constants.h"

UAV::UAV(Config::UAV& config)
    : m_config(config)
{
    for (auto const& m: config.motors)
    {
        Motor motor(m.position, m.clockwise, m.max_rpm, m.acceleration, m.decceleration, m.drag);

        for (auto const& c: m.thrust_curve)
        {
            motor.get_thrust_curve().add_point(c.first, c.second);
        }
        for (auto const& c: m.amps_curve)
        {
            motor.get_amps_curve().add_point(c.first, c.second);
        }
        m_motors.push_back(motor);
    }

}

auto UAV::get_motors() const -> std::vector<Motor> const&
{
    return m_motors;
}

auto UAV::get_position() const -> math::vec3f const&
{
    return m_position;
}
void UAV::set_position(const math::vec3f& v)
{
    m_position = v;
    btTransform trans = m_body->getWorldTransform();
    trans.setOrigin(vec3f_to_bt(v));
    m_body->setWorldTransform(trans);
}
void UAV::set_rotation(const math::quatf& v)
{
    m_rotation = v;
    btTransform trans = m_body->getWorldTransform();
    trans.setRotation(quatf_to_bt(v));
    m_body->setWorldTransform(trans);
}

auto UAV::get_rotation() const -> math::quatf const&
{
    return m_rotation;
}
auto UAV::get_angular_velocity() const -> math::vec3f const&
{
    return m_angular_velocity;
}
auto UAV::get_velocity() const -> math::vec3f const&
{
    return m_velocity;
}
auto UAV::get_acceleration() const -> math::vec3f const&
{
    return m_acceleration;
}
auto UAV::get_linear_acceleration() const -> math::vec3f const&
{
    return m_linear_acceleration;
}


void UAV::process(q::Clock::duration dt)
{
    auto dts = q::Seconds(dt).count();

    {
        auto rotation = bt_to_quatf(m_body->getWorldTransform().getRotation());
        auto delta = (~m_rotation) * rotation;
        m_rotation = rotation;

        m_rotation.get_as_mat3_and_inv<math::fast>(m_local_to_world_mat, m_world_to_local_mat);

        math::vec3f euler;
        delta.get_as_euler_xyz(euler);
        m_angular_velocity = euler / dts;
    }

    {
        auto new_position = bt_to_vec3f(m_body->getWorldTransform().getOrigin());
        auto new_velocity = (new_position - m_position) / dts;
        m_position = new_position;
        m_linear_acceleration = (new_velocity - m_velocity) / dts;
        m_velocity = new_velocity;
        m_acceleration = math::transform(m_world_to_local_mat, m_linear_acceleration + math::vec3f(0, 0, physics::constants::g));
        //SILK_INFO("v: {.4} / a: {.4}", m_velocity, m_linear_acceleration);
    }

    for (auto& m: m_motors)
    {
        m.process(dt);
    }

    //motors

    //float pitch = math::dot(m_local_to_world_mat.get_axis_y(), math::vec3f(0, 0, 1));
    //float roll = math::dot(m_local_to_world_mat.get_axis_x(), math::vec3f(0, 0, 1));
    //m_uav.get_motor_mixer().set_data(0.5f, 0, -pitch, -roll);

    //motors
    float total_rpm = 0.f;
    for (auto const& m: m_motors)
    {
        total_rpm += m.get_rpm() * (m.m_is_clockwise ? 1.f : -1.f);
        auto force = m_local_to_world_mat.get_axis_z() * m.get_force();
        math::vec3f local_pos(m.m_position * m_config.radius);
        auto pos = math::transform(m_local_to_world_mat, local_pos);
        m_body->applyForce(vec3f_to_bt(force), vec3f_to_bt(pos));
    }

    //yaw
    {
        auto torque = math::transform(m_local_to_world_mat, math::vec3f(0, 0, total_rpm * 0.0001f));
        m_body->applyTorque(vec3f_to_bt(torque));
    }

    //air drag
    auto velocity = bt_to_vec3f(m_body->getLinearVelocity());
    if (math::length_sq(velocity) != 0)
    {
        for (auto const& m : m_motors)
        {
            float intensity = math::abs(math::dot(m_local_to_world_mat.get_axis_z(), math::normalized(velocity)));
            float drag = intensity * m.get_air_drag();
            auto force = (-velocity) * drag;

            math::vec3f local_pos(m.m_position * m_config.radius);
            auto pos = math::transform(m_local_to_world_mat, local_pos);

            m_body->applyForce(vec3f_to_bt(force), vec3f_to_bt(pos));
        }
    }

    \
    //UI movement interaction
    {
        m_force = math::lerp(m_force, m_target_force, dts * 10.f);
        m_body->applyForce(vec3f_to_bt(m_force), btVector3(0, 0, 0));
    }
}


void UAV::reset()
{
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3(0, 0, m_config.height/2.f));
    m_body->setWorldTransform(trans);
    m_body->setAngularVelocity(btVector3(0, 0, 0));
    m_body->setLinearVelocity(btVector3(0, 0, 0));
    m_body->clearForces();

    m_position = math::vec3f::zero;
    m_rotation = math::quatf::identity;
    for (auto& m: m_motors)
    {
        m.reset();
    }
}

void UAV::stop()
{
    m_body->setAngularVelocity(btVector3(0, 0, 0));
    m_body->setLinearVelocity(btVector3(0, 0, 0));
    m_body->clearForces();
}

auto UAV::get_body() -> btRigidBody&
{
    return *m_body;
}


void UAV::set_force(const math::vec3f& force)
{
    m_target_force = force;
}
