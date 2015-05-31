#include "BrainStdAfx.h"
#include "Multi_Simulation.h"
#include "physics/constants.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

static math::vec3f bt_to_vec3f(btVector3 const& v)
{
    return math::vec3f(v.x(), v.y(), v.z());
}
static math::quatf bt_to_quatf(btQuaternion const& v)
{
    return math::quatf(v.x(), v.y(), v.z(), v.w());
}
static btVector3 vec3f_to_bt(math::vec3f const& v)
{
    return btVector3(v.x, v.y, v.z);
}
static btQuaternion quatf_to_bt(math::quatf const& v)
{
    return btQuaternion(v.x, v.y, v.z, v.w);
}


#if !defined RASPBERRY_PI

namespace silk
{
namespace node
{

//////////////////////////////////////////////////////////////////////////

Multi_Simulation::Multi_Simulation()
{
    auto now = q::Clock::now();

    m_physics_timestamp = now;
    //    m_last_accelerometer_time_point = now;
    //    m_last_gyroscope_time_point = now;
    //    m_last_compass_time_point = now;

}

Multi_Simulation::~Multi_Simulation()
{
}

auto Multi_Simulation::init(uint32_t rate) -> bool
{
    if (rate == 0)
    {
        QLOGE("Bad rate: {}Hz", rate);
        return false;
    }
    m_dt = std::chrono::microseconds(1000000 / rate);

    ///collision configuration contains default setup for memory, collision setup
    m_collision_configuration.reset(new btDefaultCollisionConfiguration());
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_dispatcher.reset(new btCollisionDispatcher(m_collision_configuration.get()));

    m_broadphase.reset(new btDbvtBroadphase());

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_solver.reset(new btSequentialImpulseConstraintSolver);
    m_world.reset(new btDiscreteDynamicsWorld(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collision_configuration.get()));

    m_world->setGravity(btVector3(0, 0, -physics::constants::g));

    //////////////////////////////////////////////////////////////////////////
    //ground

    m_ground_shape.reset(new btStaticPlaneShape(btVector3(0, 0, 1), 0));

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(0, 0, 0));

    btRigidBody::btRigidBodyConstructionInfo info(0.f, nullptr, m_ground_shape.get());
    m_ground_body.reset(new btRigidBody(info));
    m_world->addRigidBody(m_ground_body.get());

    return true;
}

auto Multi_Simulation::init_uav(config::Multi const& config) -> bool
{
    if (math::is_zero(config.mass, math::epsilon<float>()))
    {
        QLOGE("Bad mass: {}g", config.mass);
        return false;
    }
    if (math::is_zero(config.height, math::epsilon<float>()))
    {
        QLOGE("Bad height: {}g", config.height);
        return false;
    }
    if (math::is_zero(config.radius, math::epsilon<float>()))
    {
        QLOGE("Bad radius: {}g", config.radius);
        return false;
    }

    m_uav.config = config;

    if (m_uav.state.motors.size() != m_uav.config.motors.size())
    {
        m_uav.state.motors.clear();
        m_uav.state.motors.resize(m_uav.config.motors.size());
        q::util::Rand rnd;
        for (auto& m: m_uav.state.motors)
        {
            m.drag_factor = rnd.get_positive_float() * 0.01f + 0.01f;
        }
    }

    if (m_uav.body)
    {
        m_world->removeRigidBody(m_uav.body.get());
    }
    m_uav.body.reset();
    m_uav.motion_state.reset();
    m_uav.shape.reset();

    m_uav.shape.reset(new btCylinderShapeZ(btVector3(m_uav.config.radius, m_uav.config.radius, m_uav.config.height*0.5f)));

    if (m_is_ground_enabled)
    {
        m_uav.state.enu_position.z = math::max(m_uav.state.enu_position.z, m_uav.config.height*0.5f);
    }

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(vec3f_to_bt(m_uav.state.enu_position));
    transform.setRotation(quatf_to_bt(m_uav.state.local_to_enu_rotation));

    btVector3 local_inertia(0, 0, 0);
    m_uav.shape->calculateLocalInertia(m_uav.config.mass, local_inertia);

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    m_uav.motion_state.reset(new btDefaultMotionState(transform));
    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_uav.config.mass, m_uav.motion_state.get(), m_uav.shape.get(), local_inertia);
    m_uav.body.reset(new btRigidBody(rbInfo));
    m_uav.body->setActivationState(DISABLE_DEACTIVATION);
    m_uav.body->setDamping(0.01f, 0.05f); //simulate air resistance

    m_world->addRigidBody(m_uav.body.get());

    m_uav.body->setLinearVelocity(vec3f_to_bt(m_uav.state.enu_velocity));
    m_uav.body->setAngularVelocity(vec3f_to_bt(m_uav.state.angular_velocity));
    m_uav.body->setGravity(btVector3(0, 0, m_is_gravity_enabled ? -physics::constants::g : 0));

    return true;
}
void Multi_Simulation::reset()
{
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3(0, 0, m_uav.config.height/2.f));
    m_uav.body->setWorldTransform(trans);
    m_uav.body->setAngularVelocity(btVector3(0, 0, 0));
    m_uav.body->setLinearVelocity(btVector3(0, 0, 0));
    m_uav.body->clearForces();

    m_uav.state.acceleration = math::vec3f::zero;
    m_uav.state.angular_velocity = math::vec3f::zero;
    m_uav.state.enu_linear_acceleration = math::vec3f::zero;
    m_uav.state.enu_position = math::vec3f::zero;
    m_uav.state.enu_velocity = math::vec3f::zero;
    m_uav.state.local_to_enu_rotation  = math::quatf::identity;
    for (auto& m: m_uav.state.motors)
    {
        m.throttle = 0;
        m.thrust = 0;
    }
}

void Multi_Simulation::stop_motion()
{
    m_uav.body->clearForces();
    m_uav.body->setLinearVelocity(btVector3(0, 0, 0));
    m_uav.body->setAngularVelocity(btVector3(0, 0, 0));

    btTransform wt;
    m_uav.motion_state->getWorldTransform(wt);

    {
        auto rotation = bt_to_quatf(wt.getRotation());
        m_uav.state.local_to_enu_rotation = rotation;
        m_uav.state.angular_velocity = math::vec3f::zero;
    }

    {
        auto new_position = bt_to_vec3f(wt.getOrigin());
        m_uav.state.enu_position = new_position;
        m_uav.state.enu_linear_acceleration = math::vec3f::zero;
        m_uav.state.enu_velocity = math::vec3f::zero;
        auto enu_to_local = math::inverse(m_uav.state.local_to_enu_rotation);
        m_uav.state.acceleration = math::rotate(enu_to_local, m_uav.state.enu_linear_acceleration + math::vec3f(0, 0, physics::constants::g));
    }

}

void Multi_Simulation::process(q::Clock::duration dt, std::function<void(Multi_Simulation&, q::Clock::duration)> const& callback)
{
    m_physics_duration += dt;
    //m_duration_to_simulate = math::min(m_duration_to_simulate, q::Clock::duration(std::chrono::milliseconds(100)));

    float dts = q::Seconds(m_dt).count();
    while (m_physics_duration >= m_dt)
    {
        //limit angular velocity
        if (m_uav.body)
        {
            auto vel = bt_to_vec3f(m_uav.body->getAngularVelocity());
            vel = math::clamp(vel, math::vec3f(-10.f), math::vec3f(10.f));// + math::vec3f(0.001f, 0.f, 0.f);
            m_uav.body->setAngularVelocity(vec3f_to_bt(vel));
        }

        if (m_is_simulation_enabled)
        {
            m_world->stepSimulation(dts, 1, dts * 0.5f);
        }

        process_uav(m_dt);

        if (callback)
        {
            callback(*this, m_dt);
        }

        //update_world(std::chrono::microseconds(50));
        m_physics_duration -= m_dt;
        m_physics_timestamp += m_dt;
    }
}

void Multi_Simulation::set_gravity_enabled(bool yes)
{
    if (m_is_gravity_enabled != yes)
    {
        m_is_gravity_enabled = yes;
        m_uav.body->setGravity(btVector3(0, 0, m_is_gravity_enabled ? -physics::constants::g : 0));
    }
}

void Multi_Simulation::set_ground_enabled(bool yes)
{
    if (m_is_ground_enabled != yes)
    {
        m_is_ground_enabled = yes;
        if (m_is_ground_enabled)
        {
            m_world->addRigidBody(m_ground_body.get());
        }
        else
        {
            m_world->removeRigidBody(m_ground_body.get());
        }
    }
}

void Multi_Simulation::set_simulation_enabled(bool yes)
{
    m_is_simulation_enabled = yes;
}
void Multi_Simulation::set_drag_enabled(bool yes)
{
    m_is_drag_enabled = yes;
}

void Multi_Simulation::set_motor_throttle(size_t motor, float throttle)
{
    m_uav.state.motors[motor].throttle = throttle;
}


void Multi_Simulation::process_uav(q::Clock::duration dt)
{
    if (!m_uav.body)
    {
        return;
    }

    auto dts = q::Seconds(dt).count();

    btTransform wt;
    m_uav.motion_state->getWorldTransform(wt);

    {
        auto rotation = bt_to_quatf(wt.getRotation());
        auto delta = (~m_uav.state.local_to_enu_rotation) * rotation;
        m_uav.state.local_to_enu_rotation = rotation;

        math::vec3f euler;
        delta.get_as_euler_xyz(euler);
        m_uav.state.angular_velocity = euler / dts;
    }

    auto enu_to_local = math::inverse(m_uav.state.local_to_enu_rotation);

    {
        auto new_position = bt_to_vec3f(wt.getOrigin());
        auto new_velocity = bt_to_vec3f(m_uav.body->getLinearVelocity());//(new_position - m_uav.enu_position) / dts;
        m_uav.state.enu_position = new_position;
        m_uav.state.enu_linear_acceleration = (new_velocity - m_uav.state.enu_velocity) / dts;
        m_uav.state.enu_velocity = new_velocity;
        m_uav.state.acceleration = math::rotate(enu_to_local, m_uav.state.enu_linear_acceleration + math::vec3f(0, 0, physics::constants::g));
        //QLOGI("v: {.4} / la:{.4} / a: {.4}", m_uav.state.enu_velocity, m_uav.state.enu_linear_acceleration, m_uav.state.acceleration);
    }

    math::mat3f local_to_enu_mat;
    math::mat3f enu_to_local_mat;
    m_uav.state.local_to_enu_rotation.get_as_mat3_and_inv<math::fast>(local_to_enu_mat, enu_to_local_mat);

    auto velocity = bt_to_vec3f(m_uav.body->getLinearVelocity());
    float air_speed = math::dot(local_to_enu_mat.get_axis_z(), velocity);

    //float pitch = math::dot(m_local_to_world_mat.get_axis_y(), math::vec3f(0, 0, 1));
    //float roll = math::dot(m_local_to_world_mat.get_axis_x(), math::vec3f(0, 0, 1));
    //m_uav.get_motor_mixer().set_data(0.5f, 0, -pitch, -roll);


    //motors
    math::vec3f z_torque;
    float total_force = 0.f;
    {
        const auto dir = local_to_enu_mat.get_axis_z();
        for (size_t i = 0; i < m_uav.state.motors.size(); i++)
        {
            auto& m = m_uav.state.motors[i];
            auto& mc = m_uav.config.motors[i];

            {
                auto target_thrust = math::square(m.throttle) * m_uav.config.motor_thrust;
                if (!math::equals(m.thrust, target_thrust))
                {
                    auto delta = (target_thrust - m.thrust) / m_uav.config.motor_thrust;
                    float acc = delta > 0 ? 1.f / m_uav.config.motor_acceleration : 1.f / m_uav.config.motor_deceleration;
                    float d = math::min(math::abs(delta), acc * q::Seconds(dt).count());
                    m.thrust += math::sgn(delta) * d * m_uav.config.motor_thrust;
                    m.thrust = math::clamp(m.thrust, 0.f, m_uav.config.motor_thrust);
                }
            }
            z_torque.z += m_uav.config.motor_z_torque * (mc.clockwise ? 1 : -1) * (m.thrust / m_uav.config.motor_thrust);

//            total_rpm += m.rpm * (mc.clockwise ? 1.f : -1.f);

            total_force += m.thrust;
            math::vec3f local_pos = mc.position;
            auto pos = math::transform(local_to_enu_mat, local_pos);
            m_uav.body->applyForce(vec3f_to_bt(dir * m.thrust), vec3f_to_bt(pos));
        }
    }

    //SILK_INFO("total_force {}", total_force);

    //yaw
    {
        auto torque = math::transform(local_to_enu_mat, z_torque);
        m_uav.body->applyTorque(vec3f_to_bt(torque));
    }

    //air drag
    if (m_is_drag_enabled && math::length_sq(velocity) != 0)
    {
        auto velocity_normalized = math::normalized(velocity);
        auto speed2 = math::length_sq(velocity);

        //body
        {
            float drag_factor = 0.05f;

            float intensity = math::abs(math::dot(local_to_enu_mat.get_axis_z(), velocity_normalized));
            float drag = intensity * drag_factor;
            auto force = -velocity_normalized * drag * speed2;
            m_uav.body->applyForce(vec3f_to_bt(force), btVector3(0, 0, 0));
        }

        for (size_t i = 0; i < m_uav.state.motors.size(); i++)
        {
            auto& m = m_uav.state.motors[i];
            auto& mc = m_uav.config.motors[i];

            float intensity = math::abs(math::dot(local_to_enu_mat.get_axis_z(), velocity_normalized));
            float drag = intensity * m.drag_factor;
            auto force = -velocity_normalized * drag * speed2;

            math::vec3f local_pos(mc.position);
            auto pos = math::transform(local_to_enu_mat, local_pos);
            m_uav.body->applyForce(vec3f_to_bt(force), vec3f_to_bt(pos));
        }
    }
}


auto Multi_Simulation::get_uav_state() const -> IMulti_Simulator::UAV_State const&
{
    return m_uav.state;
}


}
}

#endif
