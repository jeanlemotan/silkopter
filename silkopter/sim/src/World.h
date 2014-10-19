#pragma once

#include "Config.h"
#include "UAV.h"
#include "common/sensors/Sensor_Samples.h"

class World : q::util::Noncopyable
{
public:

    World(boost::asio::io_service& io_service, Config& config);
	~World();

	void init();
	void reset();

    void process(q::Clock::duration dt);
    void render(Render_Context& context);

    auto get_uav() -> UAV&;
    auto get_uav() const -> UAV const&;
    void set_uav(UAV const& uav);

    enum class Result
    {
        OK,
        FAILED,
    };

    auto connect(boost::asio::ip::address const& address, uint16_t port) -> Result;
    void disconnect();
    auto is_connected() const -> bool;
    auto get_remote_address() const -> boost::asio::ip::address;

private:
    void process_world(q::Clock::duration dt);

    void render_ground(Render_Context& context);
    void render_uav(Render_Context& context);

    enum class Message : uint8_t
    {
        STAY_ALIVE,
        CONFIG,
        SENSOR_DATA,
        //---
        MOTOR_OUTPUTS,
    };

    enum class Sensor : uint16_t
    {
        ACCELEROMETER   = 1 << 0,   //uint8 - dt ms, vec3s16 - acceleration
        GYROSCOPE       = 1 << 1,   //uint8 - dt ms,  vec3s16 - angular velocity
        COMPASS         = 1 << 2,   //vec3s16 - quantized normalized direction
        BAROMETER       = 1 << 3,   //float - pressure
        THERMOMETER     = 1 << 4,   //int16 - temperature
        SONAR           = 1 << 5,   //uint16 - distance
        VOLTAGE         = 1 << 6,   //uint16 - voltage
        CURRENT         = 1 << 7,   //uint16 - amperes / second
        GPS             = 1 << 8
    };
    typedef q::util::Flag_Set<Sensor, uint16_t> Sensors;

    boost::asio::io_service& m_io_service;
    uint16_t m_port = 0;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::endpoint m_remote_endpoint;
    typedef util::Channel<Message, uint8_t, boost::asio::ip::tcp::socket> Channel;
    mutable Channel m_channel;

    Config& m_config;

    math::vec3f m_old_linear_velocity;
    math::quatf m_old_rotation;

    silk::Accelerometer_Sample m_accelerometer_sample;
    q::Clock::time_point m_last_accelerometer_time_point;

    silk::Gyroscope_Sample m_gyroscope_sample;
    q::Clock::time_point m_last_gyroscope_time_point;

    silk::Compass_Sample m_compass_sample;
    q::Clock::time_point m_last_compass_time_point;

    silk::Barometer_Sample m_barometer_sample;
    q::Clock::time_point m_last_barometer_time_point;

    silk::Thermometer_Sample m_thermometer_sample;
    q::Clock::time_point m_last_thermometer_time_point;

    silk::Sonar_Sample m_sonar_sample;
    q::Clock::time_point m_last_sonar_time_point;

    silk::GPS_Sample m_gps_sample;
    q::Clock::time_point m_last_gps_time_point;

    void process_sensors();

    bool m_is_gravity_enabled = true;
    bool m_is_ground_enabled = true;
    void process_environment();

	std::unique_ptr<btDefaultCollisionConfiguration> m_collision_configuration;
	std::unique_ptr<btCollisionDispatcher> m_dispatcher;
	std::unique_ptr<btDbvtBroadphase> m_broadphase;
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;

	std::unique_ptr<btCollisionShape> m_ground_shape;
	std::unique_ptr<btRigidBody> m_ground_body;

	std::unique_ptr<btDiscreteDynamicsWorld> m_world;

    q::Clock::time_point m_physics_timestamp;
    q::Clock::duration m_physics_duration{0};
//	q::Clock::duration m_uav_duration;

	uint32_t m_imu_sample_idx = 0;

    UAV m_uav;

    void process_uav(q::Clock::duration dt);
    void process_air_drag(q::Clock::duration dt);


    enum class State : uint8_t
    {
        DISCONNECTED,   //not connected yet
        HANDSHAKE,
        RUNNING,
    };

    Message m_message;
    bool m_has_message = false;

    State m_state = State::HANDSHAKE;

    void process_state_handshake();
    void process_state_running();
    void process_config_requests();
};
