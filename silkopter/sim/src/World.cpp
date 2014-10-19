#include "stdafx.h"
#include "World.h"
#include "physics/constants.h"
#include "Axis.h"

constexpr uint32_t VERSION = 1;

namespace board
{
namespace clock
{
	extern void advance_clock(q::Clock::duration dt);
}
}

const std::chrono::microseconds k_physics_step_duration(1000);

using namespace boost::asio;

//////////////////////////////////////////////////////////////////////////

World::World(boost::asio::io_service& io_service, Config& config)
    : m_io_service(io_service)
    , m_socket(io_service)
    , m_channel(m_socket)
    , m_config(config)
    , m_uav(config.uav)
{
    auto now = q::Clock::now();

    m_physics_timestamp = now;
    m_last_accelerometer_time_point = now;
    m_last_gyroscope_time_point = now;
    m_last_compass_time_point = now;
}

World::~World()
{
}

void World::init()
{
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

	//////////////////////////////////////////////////////////////////////////
	//UAV

    m_uav.m_shape.reset(new btCylinderShapeZ(btVector3(m_config.uav.radius, m_config.uav.radius, m_config.uav.height*0.5f)));
	transform.setIdentity();
    transform.setOrigin(btVector3(0, 0, m_config.uav.height));

	btVector3 local_inertia(0, 0, 0);
    m_uav.m_shape->calculateLocalInertia(m_config.uav.mass, local_inertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    m_uav.m_motion_state.reset(new btDefaultMotionState(transform));
    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_config.uav.mass, m_uav.m_motion_state.get(), m_uav.m_shape.get(), local_inertia);
    m_uav.m_body.reset(new btRigidBody(rbInfo));
    m_uav.m_body->setActivationState(DISABLE_DEACTIVATION);
    m_uav.m_body->setDamping(0.01f, 0.05f); //simulate air resistance

    m_world->addRigidBody(m_uav.m_body.get());

	//////////////////////////////////////////////////////////////////////////
	//

}

void World::reset()
{
    //reset the uav
    m_uav.reset();
}

auto World::connect(boost::asio::ip::address const& address, uint16_t port) -> Result
{
    try
    {
        disconnect();

        m_remote_endpoint = ip::tcp::endpoint(address, port);
        boost::system::error_code error = boost::asio::error::host_not_found;
        m_socket.connect(m_remote_endpoint, error);

        if (!error)
        {
            SILK_INFO("Connected to {}:{}", address.to_string(), port);
            m_channel.start();
        }
        else
        {
            m_socket.close();
            SILK_WARNING("Connect failed: {}", error.message());
            return Result::FAILED;
        }
    }
    catch(...)
    {
        m_socket.close();
        SILK_WARNING("Connect failed");
        return Result::FAILED;
    }

    return Result::OK;
}
void World::disconnect()
{
    m_socket.close();
}

auto World::is_connected() const -> bool
{
    return m_socket.is_open();
}

auto World::get_remote_address() const -> boost::asio::ip::address
{
    return m_remote_endpoint.address();
}

void World::process_state_handshake()
{
    static auto start = q::Clock::now();
    auto now = q::Clock::now();

    if (now - start > std::chrono::milliseconds(200))
    {
        start = now;

        SILK_INFO("Sending config...");

        m_channel.send(Message::CONFIG,
                       VERSION,
                       m_config.barometer.is_enabled,
                       m_config.sonar.is_enabled,
                       false,//config.has_gps,
                       false,//config.has_voltage_sensor,
                       false//config.has_current_sensor,
                       );

        m_state = State::RUNNING;
    }
}

void World::process_state_running()
{
    if (!m_has_message)
    {
        return;
    }

    switch (m_message)
    {
    case Message::MOTOR_OUTPUTS:
        {
            uint8_t motor_count = 0;
            m_channel.begin_unpack();
            auto res = m_channel.unpack_param(motor_count);
            if (res && motor_count > 0)
            {
                auto& motors = m_uav.m_motors;
                motor_count = math::min<size_t>(motor_count, motors.size());
                for (size_t i = 0; i < motor_count; i++)
                {
                    uint16_t v = 0;
                    if (m_channel.unpack_param(v))
                    {
                        float throttle = float(v) / 65535.f;
                        motors[i].set_throttle(throttle);
                    }
                }
            }
            m_channel.end_unpack();
        }
        break;
    }
}


void World::process(q::Clock::duration dt)
{
    do
    {
        m_has_message = m_channel.get_next_message(m_message);
        switch (m_state)
        {
        case State::HANDSHAKE: process_state_handshake(); break;
        case State::RUNNING: process_state_running(); break;
        }

        //process_config_requests();
    } while (m_has_message);


	m_physics_duration += dt;
	//m_duration_to_simulate = math::min(m_duration_to_simulate, q::Clock::duration(std::chrono::milliseconds(100)));

    process_environment();

	//float dts = q::Seconds(dt).count();
	while (m_physics_duration >= k_physics_step_duration)
	{
		if ((QApplication::queryKeyboardModifiers() & Qt::ShiftModifier))
		{
            process_world(k_physics_step_duration / 10);
		}
		else
		{
            process_world(k_physics_step_duration);
		}

        process_sensors();

		//update_world(std::chrono::microseconds(50));
		m_physics_duration -= k_physics_step_duration;
        m_physics_timestamp += k_physics_step_duration;
	}
}

void World::process_environment()
{
    if (m_config.environment.is_ground_enabled != m_is_ground_enabled)
    {
        m_is_ground_enabled = m_config.environment.is_ground_enabled;
        if (m_is_ground_enabled)
        {
            m_world->addRigidBody(m_ground_body.get());
        }
        else
        {
            m_world->removeRigidBody(m_ground_body.get());
        }
    }

    if (m_config.environment.is_gravity_enabled != m_is_gravity_enabled)
    {
        m_is_gravity_enabled = m_config.environment.is_gravity_enabled;
        if (m_is_gravity_enabled)
        {
            m_uav.m_body->setGravity(btVector3(0, 0, -physics::constants::g));
        }
        else
        {
            m_uav.m_body->setGravity(btVector3(0, 0, 0));
        }
    }
}

void World::process_sensors()
{
    if (m_state != State::RUNNING)
    {
        return;
    }

    q::util::Rand rand;

    auto now = m_physics_timestamp;
    Sensors sensors;

    auto dt = now - m_last_accelerometer_time_point;
    if (dt >= m_config.accelerometer.sample_period)
    {
        m_last_accelerometer_time_point = now;

        auto acceleration = m_uav.get_acceleration();

        auto noise = math::vec3f(rand.get_float(), rand.get_float(), rand.get_float()) * m_config.accelerometer.noise;
        auto acc = acceleration + noise;

        m_accelerometer_sample.dt = dt;
        m_accelerometer_sample.sample_idx++;
        m_accelerometer_sample.value = acc;
        m_accelerometer_sample.value += m_config.accelerometer.bias;
        m_accelerometer_sample.value *= m_config.accelerometer.scale;
        sensors.set(Sensor::ACCELEROMETER);
    }
    dt = now - m_last_gyroscope_time_point;
    if (dt >= m_config.gyroscope.sample_period)
    {
        m_last_gyroscope_time_point = now;

        auto noise = math::vec3f(rand.get_float(), rand.get_float(), rand.get_float()) * math::radians(m_config.gyroscope.noise_degrees);
        m_gyroscope_sample.value = m_uav.get_angular_velocity() + noise;
        m_gyroscope_sample.value += math::radians(m_config.gyroscope.bias_degrees);
        m_gyroscope_sample.dt = dt;
        m_gyroscope_sample.sample_idx++;

        sensors.set(Sensor::GYROSCOPE);
    }
    dt = now - m_last_compass_time_point;
    if (dt >= m_config.compass.sample_period)
    {
        m_last_compass_time_point = now;

        auto direction = math::transform(m_uav.m_world_to_local_mat, math::vec3f(-1000, 0, 0));
        direction += math::vec3f(rand.get_float(), rand.get_float(), rand.get_float()) * m_config.compass.noise;
        direction += m_config.compass.bias;

        m_compass_sample.value = direction;
        m_compass_sample.dt = dt;
        m_compass_sample.sample_idx++;
        sensors.set(Sensor::COMPASS);
    }


    if (sensors.any())
    {
        m_channel.begin_pack();
        m_channel.pack_param(sensors);

        if (sensors.test(Sensor::ACCELEROMETER))
        {
            m_channel.pack_param(static_cast<uint8_t>(std::chrono::duration_cast<std::chrono::microseconds>(m_accelerometer_sample.dt).count()));
            m_channel.pack_param(m_accelerometer_sample.value);
        }
        if (sensors.test(Sensor::GYROSCOPE))
        {
            m_channel.pack_param(static_cast<uint8_t>(std::chrono::duration_cast<std::chrono::microseconds>(m_gyroscope_sample.dt).count()));
            m_channel.pack_param(m_gyroscope_sample.value);
        }
        if (sensors.test(Sensor::COMPASS))
        {
            m_channel.pack_param(static_cast<uint8_t>(std::chrono::duration_cast<std::chrono::microseconds>(m_compass_sample.dt).count()));
            m_channel.pack_param(m_compass_sample.value);
        }
        if (sensors.test(Sensor::BAROMETER))
        {
            QASSERT(0);
//            m_channel.pack_param(m_barometer_data.pressure);
        }
        if (sensors.test(Sensor::THERMOMETER))
        {
            QASSERT(0);
//            m_channel.pack_param(m_thermometer_data.temperature);
        }
        if (sensors.test(Sensor::SONAR))
        {
            QASSERT(0);
//            m_channel.pack_param(m_sonar_data.distance);
        }
        if (sensors.test(Sensor::VOLTAGE))
        {
            QASSERT(0);
//            uint16_t d = m_sonar_data.distance * 1000.f;
//            m_channel.pack_param(d);
        }
        if (sensors.test(Sensor::CURRENT))
        {
            QASSERT(0);
//            uint16_t d = m_sonar_data.distance * 1000.f;
//            m_channel.pack_param(d);
        }

        m_channel.end_pack(Message::SENSOR_DATA);
    }

}

void World::process_world(q::Clock::duration dt)
{
	//limit angular velocity
	{
        auto vel = bt_to_vec3f(m_uav.m_body->getAngularVelocity());
		vel = math::clamp(vel, math::vec3f(-10.f), math::vec3f(10.f));// + math::vec3f(0.001f, 0.f, 0.f);
        m_uav.m_body->setAngularVelocity(vec3f_to_bt(vel));
	}

    if (m_config.environment.is_simulation_enabled)
    {
        m_world->stepSimulation(q::Seconds(dt).count(), 100, 0.001f);
    }

    process_uav(dt);
}

void World::process_uav(q::Clock::duration dt)
{
    m_uav.process(dt);
}

void World::render(Render_Context& context)
{
    render_ground(context);
    render_uav(context);
}

void World::render_ground(Render_Context& context)
{
    math::vec3s32 offset(m_uav.m_position);
	offset.z = 0;

	math::trans3df trans;
	trans.set_translation(math::vec3f(offset));
    context.painter.push_post_clip_transform(trans);

    auto mat = context.materials.primitive;
	mat.get_render_state(0).set_depth_test(true);
	mat.get_render_state(0).set_depth_write(false);
	mat.get_render_state(0).set_culling(false);
	mat.get_render_state(0).set_blend_formula(q::video::Render_State::Blend_Formula::Preset::ALPHA);
    context.painter.set_material(mat);

	const float k_size = 10000;
	const int k_line_count = 60;

    context.painter.fill_rectangle(q::draw::Vertex(math::vec3f(-k_size, -k_size, 0), 0x20FFFFFF), q::draw::Vertex(math::vec3f(k_size, k_size, 0), 0x20FFFFFF));
	for (int i = 0; i < k_line_count; i++)
	{
		float x = float(i) - k_line_count/2;
		uint32_t color = 0x20FFFFFF;
        context.painter.draw_line(q::draw::Vertex(math::vec3f(-k_line_count/2, x, 0), color), q::draw::Vertex(math::vec3f(k_line_count/2, x, 0), color));
        context.painter.draw_line(q::draw::Vertex(math::vec3f(x, -k_line_count/2, 0), color), q::draw::Vertex(math::vec3f(x, k_line_count/2, 0), color));
	}

    context.painter.pop_post_clip_transform();
}

void World::render_uav(Render_Context& context)
{
	math::trans3df trans;
    trans.set_rotation(m_uav.m_local_to_world_mat);
    trans.set_translation(m_uav.m_position);
    context.painter.push_post_clip_transform(trans);

    context.painter.set_material(context.materials.primitive);
    //render_axes(context.painter, 1.f);

	//render motors
	{
		//float pitch = math::dot(m_local_to_world_mat.get_axis_y(), math::vec3f(0, 0, 1));
		//float roll = math::dot(m_local_to_world_mat.get_axis_x(), math::vec3f(0, 0, 1));
		//m_uav.get_motor_mixer().set_data(0.5f, 0, -pitch, -roll);

        const float motor_radius = 0.12f;
        for (auto const& m : m_uav.m_motors)
		{
            context.painter.draw_circle(q::draw::Vertex(m.m_position*m_config.uav.radius, 0xFFFFFFFF), motor_radius);
            float ratio = m.get_rpm() / m.get_max_rpm();
            context.painter.fill_circle(q::draw::Vertex(m.m_position*m_config.uav.radius, 0xFF00FF00), motor_radius * ratio);
		}
	}

    context.painter.pop_post_clip_transform();

//	//RENDER IMU DATA
//	{
//		math::trans3df trans;
//        trans.set_rotation(m_uav.get_rotation());
//        trans.set_translation(m_uav.m_position);
//        context.painter.push_post_clip_transform(trans);

//        context.painter.set_material(context.materials.primitive);
//        render_axes(context.painter, 0.5f);
		
//        context.painter.pop_post_clip_transform();
//	}
}

auto World::get_uav() const -> UAV const&
{
    return m_uav;
}
auto World::get_uav() -> UAV&
{
    return m_uav;
}


