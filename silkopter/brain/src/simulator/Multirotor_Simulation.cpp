#include "BrainStdAfx.h"
#include "Multirotor_Simulation.h"
#include "physics/constants.h"

#if !defined RASPBERRY_PI

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

namespace silk
{
namespace node
{

constexpr size_t SUBSTEPS = 10;

//////////////////////////////////////////////////////////////////////////

Multirotor_Simulation::Multirotor_Simulation()
{
    auto now = Clock::now();

    m_physics_timestamp = now;
    //    m_last_accelerometer_time_point = now;
    //    m_last_gyroscope_time_point = now;
    //    m_last_compass_time_point = now;

}

Multirotor_Simulation::~Multirotor_Simulation()
{
}

auto Multirotor_Simulation::init(uint32_t rate) -> bool
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

auto Multirotor_Simulation::init_uav(std::shared_ptr<const IMultirotor_Properties> multirotor_properties) -> bool
{
    m_uav.properties = multirotor_properties;

    if (m_uav.state.motors.size() != m_uav.properties->get_motors().size())
    {
        m_uav.state.motors.clear();
        m_uav.state.motors.resize(m_uav.properties->get_motors().size());
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

    m_uav.shape.reset(new btCylinderShapeZ(btVector3(m_uav.properties->get_radius(), m_uav.properties->get_radius(), m_uav.properties->get_height()*0.5f)));

    if (m_is_ground_enabled)
    {
        m_uav.state.enu_position.z = math::max(m_uav.state.enu_position.z, m_uav.properties->get_height()*0.5f);
    }

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(vec3f_to_bt(m_uav.state.enu_position));
    transform.setRotation(quatf_to_bt(m_uav.state.local_to_enu_rotation));

    btVector3 local_inertia(0, 0, 0);
    m_uav.shape->calculateLocalInertia(m_uav.properties->get_mass(), local_inertia);

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    m_uav.motion_state.reset(new btDefaultMotionState(transform));
    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_uav.properties->get_mass(), m_uav.motion_state.get(), m_uav.shape.get(), local_inertia);
    m_uav.body.reset(new btRigidBody(rbInfo));
    m_uav.body->setActivationState(DISABLE_DEACTIVATION);
    m_uav.body->setDamping(0.01f, 0.05f); //simulate air resistance

    m_world->addRigidBody(m_uav.body.get());

    m_uav.body->setLinearVelocity(vec3f_to_bt(m_uav.state.enu_velocity));
    m_uav.body->setAngularVelocity(vec3f_to_bt(m_uav.state.angular_velocity));
    m_uav.body->setGravity(btVector3(0, 0, m_is_gravity_enabled ? -physics::constants::g : 0));

    return true;
}
void Multirotor_Simulation::reset()
{
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3(0, 0, m_uav.properties->get_height()/2.f));
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

void Multirotor_Simulation::stop_motion()
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

void Multirotor_Simulation::process(Clock::duration dt, std::function<void(Multirotor_Simulation&, Clock::duration)> const& callback)
{
    m_physics_duration += dt;
    //m_duration_to_simulate = math::min(m_duration_to_simulate, Clock::duration(std::chrono::milliseconds(100)));

    float dts = std::chrono::duration<float>(m_dt).count();
    while (m_physics_duration >= m_dt)
    {
        auto substep_dts = dts / SUBSTEPS;

        for (size_t i = 0; i < SUBSTEPS; i++)
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
                m_world->stepSimulation(substep_dts, SUBSTEPS, substep_dts);
            }

            process_uav(m_dt / SUBSTEPS);
        }

        process_uav_sensors(m_dt);

        if (callback)
        {
            callback(*this, m_dt);
        }

        //update_world(std::chrono::microseconds(50));
        m_physics_duration -= m_dt;
        m_physics_timestamp += m_dt;
    }
}

void Multirotor_Simulation::set_gravity_enabled(bool yes)
{
    if (m_is_gravity_enabled != yes)
    {
        m_is_gravity_enabled = yes;
        m_uav.body->setGravity(btVector3(0, 0, m_is_gravity_enabled ? -physics::constants::g : 0));
    }
}

void Multirotor_Simulation::set_ground_enabled(bool yes)
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

void Multirotor_Simulation::set_simulation_enabled(bool yes)
{
    m_is_simulation_enabled = yes;
}
void Multirotor_Simulation::set_drag_enabled(bool yes)
{
    m_is_drag_enabled = yes;
}

void Multirotor_Simulation::set_motor_throttle(size_t motor, float throttle)
{
    m_uav.state.motors[motor].throttle = throttle;
}


void Multirotor_Simulation::process_uav(Clock::duration dt)
{
    if (!m_uav.body)
    {
        return;
    }

    auto dts = std::chrono::duration<float>(dt).count();

    btTransform wt;
    m_uav.motion_state->getWorldTransform(wt);

    math::trans3df local_to_enu_trans(bt_to_vec3f(wt.getOrigin()), bt_to_quatf(wt.getRotation()), math::vec3f::one);

    {
        math::quatf rotation = bt_to_quatf(wt.getRotation());
        QASSERT(math::is_finite(rotation));
        math::quatf delta = (~m_uav.state.local_to_enu_rotation) * rotation;
        QASSERT(math::is_finite(delta));
        m_uav.state.local_to_enu_rotation = rotation;

        math::vec3f euler;
        delta.get_as_euler_xyz(euler);
        QASSERT(math::is_finite(euler));
        m_accumulated_data.angular_velocity_sum += euler / dts;
        QASSERT(math::is_finite(m_accumulated_data.angular_velocity_sum));
    }

    auto velocity = bt_to_vec3f(m_uav.body->getLinearVelocity());
    {
        auto new_position = bt_to_vec3f(wt.getOrigin());

        m_uav.state.enu_position = new_position;
        m_accumulated_data.enu_linear_acceleration_sum += (velocity - m_accumulated_data.prev_enu_velocity) / dts;
        m_accumulated_data.prev_enu_velocity = velocity;

        m_accumulated_data.enu_velocity_sum += velocity;

        //m_uav.state.acceleration = math::rotate(enu_to_local_rotation, m_uav.state.enu_linear_acceleration + math::vec3f(0, 0, physics::constants::g));
        //QLOGI("v: {.4} / la:{.4} / a: {.4}", m_uav.state.enu_velocity, m_uav.state.enu_linear_acceleration, m_uav.state.acceleration);
    }

    //motors
    math::vec3f z_torque;
    float total_force = 0.f;
    {
        for (size_t i = 0; i < m_uav.state.motors.size(); i++)
        {
            auto& m = m_uav.state.motors[i];
            auto& mc = m_uav.properties->get_motors()[i];

            {
                auto target_thrust = math::square(m.throttle) * m_uav.properties->get_motor_thrust();
                if (!math::equals(m.thrust, target_thrust))
                {
                    auto delta = (target_thrust - m.thrust) / m_uav.properties->get_motor_thrust();
                    float acc = delta > 0 ? 1.f / m_uav.properties->get_motor_acceleration() : 1.f / m_uav.properties->get_motor_deceleration();
                    float d = math::min(math::abs(delta), acc * std::chrono::duration<float>(dt).count());
                    m.thrust += math::sgn(delta) * d * m_uav.properties->get_motor_thrust();
                    m.thrust = math::clamp(m.thrust, 0.f, m_uav.properties->get_motor_thrust());
                }
            }
            z_torque += mc.thrust_vector * m_uav.properties->get_motor_z_torque() * (mc.clockwise ? 1 : -1) * (m.thrust / m_uav.properties->get_motor_thrust());

//            total_rpm += m.rpm * (mc.clockwise ? 1.f : -1.f);

            total_force += m.thrust;
            math::vec3f local_pos = mc.position;
            auto pos = math::rotate(local_to_enu_trans, local_pos);

            const auto dir = math::rotate(local_to_enu_trans, mc.thrust_vector);
            m_uav.body->applyForce(vec3f_to_bt(dir * m.thrust), vec3f_to_bt(pos));
        }
    }

    //SILK_INFO("total_force {}", total_force);

    //yaw
    {
        auto torque = math::rotate(local_to_enu_trans, z_torque);
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

            float intensity = math::abs(math::dot(local_to_enu_trans.get_axis_z(), velocity_normalized));
            float drag = intensity * drag_factor;
            auto force = -velocity_normalized * drag * speed2;
            m_uav.body->applyForce(vec3f_to_bt(force), btVector3(0, 0, 0));
        }

        for (size_t i = 0; i < m_uav.state.motors.size(); i++)
        {
            auto& m = m_uav.state.motors[i];
            auto& mc = m_uav.properties->get_motors()[i];

            float intensity = -math::dot(local_to_enu_trans.get_axis_z(), velocity_normalized);
            float drag = intensity * m.drag_factor;
            auto force = local_to_enu_trans.get_axis_z() * drag * speed2;

            math::vec3f local_pos(mc.position);
            m_uav.body->applyForce(vec3f_to_bt(force), vec3f_to_bt(local_pos));
        }
    }
}

void Multirotor_Simulation::process_uav_sensors(Clock::duration dt)
{
    if (!m_uav.body)
    {
        return;
    }

    m_uav.state.enu_velocity = m_accumulated_data.enu_velocity_sum / SUBSTEPS;
    m_accumulated_data.enu_velocity_sum = math::vec3f::zero;

    m_uav.state.enu_linear_acceleration = m_accumulated_data.enu_linear_acceleration_sum / SUBSTEPS;
    m_accumulated_data.enu_linear_acceleration_sum = math::vec3f::zero;

    m_uav.state.angular_velocity = m_accumulated_data.angular_velocity_sum / SUBSTEPS;
    m_accumulated_data.angular_velocity_sum = math::vec3f::zero;

    btTransform wt;
    m_uav.motion_state->getWorldTransform(wt);

    math::trans3df local_to_enu_trans(bt_to_vec3f(wt.getOrigin()), bt_to_quatf(wt.getRotation()), math::vec3f::one);
    math::trans3df enu_to_local_trans = math::inverse(local_to_enu_trans);

    auto enu_to_local_rotation = math::inverse(m_uav.state.local_to_enu_rotation);

    m_uav.state.acceleration = math::rotate(enu_to_local_rotation, m_uav.state.enu_linear_acceleration + math::vec3f(0, 0, physics::constants::g));

    {
        math::vec3f enu_magnetic_field(0, 50.f, 0); //50 uT
        m_uav.state.magnetic_field = math::rotate(enu_to_local_rotation, enu_magnetic_field);
    }

    {
        //https://en.wikipedia.org/wiki/Atmospheric_pressure
        double h = m_uav.state.enu_position.z; //height
        double p0 = 101325.0; //sea level standard atmospheric pressure
        double M = 0.0289644; //molar mass of dry air
        double R = 8.31447; //universal gas constant
        double T0 = 288.15; //sea level standard temperature (K)
        m_uav.state.pressure = (p0 * std::exp(-(physics::constants::g * M * h) / (R * T0)));
        m_uav.state.pressure *= 0.001; //kilopascals
    }

    {
        float h = m_uav.state.enu_position.z; //height
        m_uav.state.temperature = 20.f + h / 1000.f;
    }

    {
        math::planef ground(math::vec3f::zero, math::vec3f(0, 0, 1));
        math::vec3f start_point = m_uav.state.enu_position;
        math::vec3f ray_dir = math::rotate(m_uav.state.local_to_enu_rotation, math::vec3f(0, 0, -1));
        float t = 0;
        if (math::ray_intersect_plane(start_point, ray_dir, ground, t) && t > 0.f)
        {
            math::vec3f point = start_point + ray_dir * t;
            m_uav.state.proximity_distance = math::transform(enu_to_local_trans, point);
        }
        else
        {
            m_uav.state.proximity_distance = math::vec3f::zero;
        }
    }
}


auto Multirotor_Simulation::get_uav_state() const -> IMultirotor_Simulator::UAV_State const&
{
    return m_uav.state;
}


}
}

#endif
