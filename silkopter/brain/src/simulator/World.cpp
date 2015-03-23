#include "BrainStdAfx.h"
#include "World.h"
#include "physics/constants.h"

namespace silk
{
namespace node
{

//////////////////////////////////////////////////////////////////////////

World::World()
{
    auto now = q::Clock::now();

    m_physics_timestamp = now;
    //    m_last_accelerometer_time_point = now;
    //    m_last_gyroscope_time_point = now;
    //    m_last_compass_time_point = now;

}

World::~World()
{
}

auto World::init_world(uint32_t rate) -> bool
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
    //m_world->addRigidBody(m_ground_body.get());

    return true;
}

auto World::init_uav(UAV_Config const& config) -> bool
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

    m_uav.motors.clear();
    m_uav.motors.resize(m_uav.config.motors.size());
    q::util::Rand rnd;
    for (auto& m: m_uav.motors)
    {
        m.drag = rnd.get_positive_float() * 0.2f + 0.1f;
    }

    if (m_uav.body)
    {
        m_world->removeRigidBody(m_uav.body.get());
    }
    m_uav.body.reset();
    m_uav.motion_state.reset();
    m_uav.shape.reset();

    m_uav.shape.reset(new btCylinderShapeZ(btVector3(m_uav.config.radius, m_uav.config.radius, m_uav.config.height*0.5f)));

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(0, 0, m_uav.config.height));

    btVector3 local_inertia(0, 0, 0);
    m_uav.shape->calculateLocalInertia(m_uav.config.mass, local_inertia);

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    m_uav.motion_state.reset(new btDefaultMotionState(transform));
    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_uav.config.mass, m_uav.motion_state.get(), m_uav.shape.get(), local_inertia);
    m_uav.body.reset(new btRigidBody(rbInfo));
    m_uav.body->setActivationState(DISABLE_DEACTIVATION);
    m_uav.body->setDamping(0.01f, 0.05f); //simulate air resistance

    m_world->addRigidBody(m_uav.body.get());

    return true;
}
void World::reset()
{
    //reset the uav
    //m_uav.reset();
}

void World::process(q::Clock::duration dt, std::function<void(World&, q::Clock::duration)> const& callback)
{
    m_physics_duration += dt;
    //m_duration_to_simulate = math::min(m_duration_to_simulate, q::Clock::duration(std::chrono::milliseconds(100)));

    //float dts = q::Seconds(dt).count();
    while (m_physics_duration >= m_dt)
    {
        process_world(m_dt);
        if (callback)
        {
            callback(*this, m_dt);
        }

        //update_world(std::chrono::microseconds(50));
        m_physics_duration -= m_dt;
        m_physics_timestamp += m_dt;
    }
}

void World::set_gravity_enabled(bool yes)
{
    if (m_is_gravity_enabled != yes)
    {
        m_is_gravity_enabled = yes;
        if (m_is_gravity_enabled)
        {
            m_uav.body->setGravity(btVector3(0, 0, -physics::constants::g));
        }
        else
        {
            m_uav.body->setGravity(btVector3(0, 0, 0));
        }
    }
}

void World::set_ground_enabled(bool yes)
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

void World::set_simulation_enabled(bool yes)
{
    m_is_simulation_enabled = yes;
}

//void World::process_sensors()
//{
//    if (m_state != State::RUNNING)
//    {
//        return;
//    }

//    q::util::Rand rand;

//    auto now = m_physics_timestamp;
//    Sensors sensors;

//    auto dt = now - m_last_accelerometer_time_point;
//    if (dt >= m_config.accelerometer.sample_period)
//    {
//        m_last_accelerometer_time_point = now;

//        auto acceleration = m_uav.get_acceleration();

//        auto noise = math::vec3f(rand.get_float(), rand.get_float(), rand.get_float()) * m_config.accelerometer.noise;
//        auto acc = acceleration + noise;

//        m_accelerometer_sample.dt = dt;
//        m_accelerometer_sample.sample_idx++;
//        m_accelerometer_sample.value = acc;
//        m_accelerometer_sample.value += m_config.accelerometer.bias;
//        m_accelerometer_sample.value *= m_config.accelerometer.scale;
//        sensors.set(Sensor::ACCELEROMETER);
//    }
//    dt = now - m_last_gyroscope_time_point;
//    if (dt >= m_config.gyroscope.sample_period)
//    {
//        m_last_gyroscope_time_point = now;

//        auto noise = math::vec3f(rand.get_float(), rand.get_float(), rand.get_float()) * math::radians(m_config.gyroscope.noise_degrees);
//        m_gyroscope_sample.value = m_uav.get_angular_velocity() + noise;
//        m_gyroscope_sample.value += math::radians(m_config.gyroscope.bias_degrees);
//        m_gyroscope_sample.dt = dt;
//        m_gyroscope_sample.sample_idx++;

//        sensors.set(Sensor::GYROSCOPE);
//    }
//    dt = now - m_last_compass_time_point;
//    if (dt >= m_config.compass.sample_period)
//    {
//        m_last_compass_time_point = now;

//        auto direction = math::transform(m_uav.m_world_to_local_mat, math::vec3f(-1000, 0, 0));
//        direction += math::vec3f(rand.get_float(), rand.get_float(), rand.get_float()) * m_config.compass.noise;
//        direction += m_config.compass.bias;

//        m_compass_sample.value = direction;
//        m_compass_sample.dt = dt;
//        m_compass_sample.sample_idx++;
//        sensors.set(Sensor::COMPASS);
//    }


//    if (sensors.any())
//    {
//        m_channel.begin_pack();
//        m_channel.pack_param(sensors);

//        if (sensors.test(Sensor::ACCELEROMETER))
//        {
//            m_channel.pack_param(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(m_accelerometer_sample.dt).count()));
//            m_channel.pack_param(m_accelerometer_sample.value);
//        }
//        if (sensors.test(Sensor::GYROSCOPE))
//        {
//            m_channel.pack_param(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(m_gyroscope_sample.dt).count()));
//            m_channel.pack_param(m_gyroscope_sample.value);
//        }
//        if (sensors.test(Sensor::COMPASS))
//        {
//            m_channel.pack_param(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(m_compass_sample.dt).count()));
//            m_channel.pack_param(m_compass_sample.value);
//        }
//        if (sensors.test(Sensor::BAROMETER))
//        {
//            QASSERT(0);
////            m_channel.pack_param(m_barometer_data.pressure);
//        }
//        if (sensors.test(Sensor::THERMOMETER))
//        {
//            QASSERT(0);
////            m_channel.pack_param(m_thermometer_data.temperature);
//        }
//        if (sensors.test(Sensor::SONAR))
//        {
//            QASSERT(0);
////            m_channel.pack_param(m_sonar_data.distance);
//        }
//        if (sensors.test(Sensor::VOLTAGE))
//        {
//            QASSERT(0);
////            uint16_t d = m_sonar_data.distance * 1000.f;
////            m_channel.pack_param(d);
//        }
//        if (sensors.test(Sensor::CURRENT))
//        {
//            QASSERT(0);
////            uint16_t d = m_sonar_data.distance * 1000.f;
////            m_channel.pack_param(d);
//        }

//        m_channel.end_pack(Message::SENSOR_DATA);
//    }

//}

void World::process_world(q::Clock::duration dt)
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
        m_world->stepSimulation(q::Seconds(dt).count(), 100, q::Seconds(m_dt).count());
    }

    process_uav(dt);
}

auto World::get_uav_enu_position() const -> math::vec3f const&
{
    return m_uav.enu_position;
}
auto World::get_uav_local_to_enu_rotation() const -> math::quatf const&
{
    return m_uav.local_to_enu_rotation;
}
auto World::get_uav_acceleration() const -> math::vec3f const&
{
    return m_uav.acceleration;
}
auto World::get_uav_angular_velocity() const -> math::vec3f const&
{
    return m_uav.angular_velocity;
}


void World::process_uav(q::Clock::duration dt)
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
        auto delta = (~m_uav.local_to_enu_rotation) * rotation;
        m_uav.local_to_enu_rotation = rotation;

        m_uav.enu_to_local_rotation = math::inverse(m_uav.local_to_enu_rotation);

        math::vec3f euler;
        delta.get_as_euler_xyz(euler);
        m_uav.angular_velocity = euler / dts;
    }

    {
        auto new_position = bt_to_vec3f(wt.getOrigin());
        auto new_velocity = bt_to_vec3f(m_uav.body->getLinearVelocity());//(new_position - m_uav.enu_position) / dts;
        m_uav.enu_position = new_position;
        m_uav.enu_linear_acceleration = (new_velocity - m_uav.enu_velocity) / dts;
        m_uav.enu_velocity = new_velocity;
        m_uav.acceleration = math::rotate(m_uav.enu_to_local_rotation, m_uav.enu_linear_acceleration + math::vec3f(0, 0, physics::constants::g));
        QLOGI("v: {.4} / la:{.4} / a: {.4}", m_uav.enu_velocity, m_uav.enu_linear_acceleration, m_uav.acceleration);
    }

    math::mat3f local_to_enu_mat;
    math::mat3f enu_to_local_mat;
    m_uav.local_to_enu_rotation.get_as_mat3_and_inv<math::fast>(local_to_enu_mat, enu_to_local_mat);

    auto velocity = bt_to_vec3f(m_uav.body->getLinearVelocity());
    float air_speed = math::dot(local_to_enu_mat.get_axis_z(), velocity);

    //float pitch = math::dot(m_local_to_world_mat.get_axis_y(), math::vec3f(0, 0, 1));
    //float roll = math::dot(m_local_to_world_mat.get_axis_x(), math::vec3f(0, 0, 1));
    //m_uav.get_motor_mixer().set_data(0.5f, 0, -pitch, -roll);


    //motors
    float total_rpm = 0.f;
    float total_force = 0.f;
    {
        const auto dir = local_to_enu_mat.get_axis_z();
        for (size_t i = 0; i < m_uav.motors.size(); i++)
        {
            auto& m = m_uav.motors[i];
            auto& mc = m_uav.config.motors[i];

            {
                auto target_rpm = m.throttle * mc.max_rpm;
                if (!math::equals(m.rpm, target_rpm))
                {
                    auto delta = target_rpm - m.rpm;
                    float acc = delta > 0 ? mc.acceleration : mc.deceleration;
                    float d = math::min(math::abs(delta), acc * q::Seconds(dt).count());
                    m.rpm += math::sgn(delta) * d;
                    m.rpm = math::clamp(m.rpm, 0.f, mc.max_rpm);
                }
            }
            total_rpm += m.rpm * (mc.clockwise ? 1.f : -1.f);

            float force = m.throttle * m.throttle * mc.max_thrust;
            total_force += force;
            math::vec3f local_pos(mc.position * m_uav.config.radius);
            auto pos = math::transform(local_to_enu_mat, local_pos);
            m_uav.body->applyForce(vec3f_to_bt(dir * force), vec3f_to_bt(pos));
        }
    }

    //SILK_INFO("total_force {}", total_force);

    //yaw
    {
        auto torque = math::transform(local_to_enu_mat, math::vec3f(0, 0, total_rpm * 0.0001f));
        m_uav.body->applyTorque(vec3f_to_bt(torque));
    }

    //air drag
    if (math::length_sq(velocity) != 0)
    {
        //body
        {
            float drag_factor = 0.3f;

            float intensity = math::abs(math::dot(local_to_enu_mat.get_axis_z(), math::normalized(velocity)));
            float drag = intensity * drag_factor;
            auto force = (-velocity) * drag;
            m_uav.body->applyForce(vec3f_to_bt(force), vec3f_to_bt(m_uav.enu_position));
        }

        for (size_t i = 0; i < m_uav.motors.size(); i++)
        {
            auto& m = m_uav.motors[i];
            auto& mc = m_uav.config.motors[i];

            float intensity = math::abs(math::dot(local_to_enu_mat.get_axis_z(), math::normalized(velocity)));
            float drag = intensity * m.drag;
            auto force = (-velocity) * drag;

            math::vec3f local_pos(mc.position * m_uav.config.radius);
            auto pos = math::transform(local_to_enu_mat, local_pos);
            m_uav.body->applyForce(vec3f_to_bt(force), vec3f_to_bt(pos));
        }
    }
}


}
}
