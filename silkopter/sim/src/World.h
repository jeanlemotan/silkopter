#pragma once

#include "Config.h"
#include "UAV.h"

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
        GPS_DATA,
        //---
        MOTOR_OUTPUTS,
        //---
        PWM_CONFIG,
    };

    enum class Sensor : uint8_t
    {
        ACCELEROMETER   = 1 << 0,   //uint8 - dt ms, vec3s16 - acceleration
        GYROSCOPE       = 1 << 1,   //uint8 - dt ms,  vec3s16 - angular velocity
        COMPASS         = 1 << 2,   //vec3s16 - quantized normalized direction
        BAROMETER       = 1 << 3,   //float - pressure
        THERMOMETER     = 1 << 4,   //int16 - temperature
        SONAR           = 1 << 5,   //uint16 - distance
        VOLTAGE         = 1 << 6,   //uint16 - voltage
        CURRENT         = 1 << 7,   //uint16 - amperes / second
    };
    typedef q::util::Flag_Set<Sensor, uint8_t> Sensors;

    boost::asio::io_service& m_io_service;
    uint16_t m_port = 0;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::endpoint m_remote_endpoint;
    typedef util::Channel<Message, uint8_t, boost::asio::ip::tcp::socket> Channel;
    mutable Channel m_channel;

    Config& m_config;

    template<class T>
    struct Sensor_Data
    {
        T value;
        T quantization_scale;
        T quantization_bias;
        q::Clock::time_point last_timestamp;
    };

    math::vec3f m_old_linear_velocity;
    math::quatf m_old_rotation;

    Sensor_Data<math::vec3f> m_accelerometer;
    Sensor_Data<math::vec3f> m_gyroscope;
    Sensor_Data<math::vec3f> m_compass;
    Sensor_Data<float> m_barometer;
    Sensor_Data<float> m_thermometer;
    Sensor_Data<float> m_sonar;

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
