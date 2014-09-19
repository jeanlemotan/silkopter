#pragma once

#include "IO_Board.h"

namespace silk
{

class IO_Board_Sim : public IO_Board
{
public:
    IO_Board_Sim(boost::asio::io_service& io_service);

    auto connect() -> Connection_Result;
    void disconnect();

    bool is_disconnected() const;
    bool is_running() const;

    //----------------------------------------------------------------------
    //motors

    void set_motor_throttles(float const* throttles, size_t count);

    void set_camera_rotation(math::quatf const& rot);

    //----------------------------------------------------------------------
    //calibration
    void set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale);
    void get_accelerometer_calibration_data(math::vec3f& bias, math::vec3f& scale) const;

    void set_gyroscope_calibration_data(math::vec3f const& bias);
    void get_gyroscope_calibration_data(math::vec3f& bias) const;

    void set_compass_calibration_data(math::vec3f const& bias);
    void get_compass_calibration_data(math::vec3f& bias) const;

    auto get_sensor_samples() const -> std::vector<Sensor_Sample> const&;

//    auto get_gps_data() -> std::vector<GPS_Data> const&;

    //----------------------------------------------------------------------
    size_t get_error_count() const;

    //----------------------------------------------------------------------
    void process();

private:
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

    enum class State : uint8_t
    {
        DISCONNECTED,   //not connected yet
        HANDSHAKE,
        RUNNING,
    };

    struct Received_Config
    {
        uint32_t version = 0;
        bool has_barometer = false;
        bool has_sonar = false;
        bool has_gps = false;
        bool has_voltage_sensor = false;
        bool has_current_sensor = false;

        struct Accelerometer
        {
            math::vec3f bias;
            math::vec3f scale = math::vec3f::one;
        } accelerometer;
        struct Gyroscope
        {
            math::vec3f bias;
            math::vec3f scale = math::vec3f::one;
        } gyroscope;
        struct Compass
        {
            math::vec3f bias;
            math::vec3f scale = math::vec3f::one;
        } compass;
        struct Barometer
        {
            float bias = 0.f;
            float scale = 1.f;
        } barometer;
        struct Sonar
        {
            float bias = 0.f;
            float scale = 1.f;
        } sonar;
        struct Thermometer
        {
            float bias = 0.f;
            float scale = 1.f;
        } thermometer;
    };

    boost::asio::io_service& m_io_service;

    boost::asio::ip::tcp::socket m_socket;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
    typedef util::Channel<Message, uint8_t, boost::asio::ip::tcp::socket> Channel;
    Channel m_channel;
    void handle_accept(boost::system::error_code const& error);

    q::Clock::time_point m_resend_timestamp;
    q::Clock::time_point m_stay_alive_timestamp;

    void process_message_sensor_data();
    void process_message_gps_data();
    void process_message_pwm_config();

    auto process_accelerometer_sensor() -> Channel::Unpack_Result;
    auto process_gyroscope_sensor() -> Channel::Unpack_Result;
    auto process_compass_sensor() -> Channel::Unpack_Result;
    auto process_barometer_sensor() -> Channel::Unpack_Result;
    auto process_thermometer_sensor() -> Channel::Unpack_Result;
    auto process_sonar_sensor() -> Channel::Unpack_Result;
    auto process_voltage_sensor() -> Channel::Unpack_Result;
    auto process_current_sensor() -> Channel::Unpack_Result;

    void process_state_disconnected();
    void process_state_handshake();
    void process_state_sending_config();
    void process_state_running();

    Message m_message;
    bool m_has_message = false;
    q::Clock::time_point m_last_message_timestamp;
    size_t m_message_count = 0;


    struct Watchdog
    {
        struct Timeout
        {
            bool is_active = false;
            q::Clock::time_point timestamp;
            q::Clock::duration duration;
            State failed_state = State::HANDSHAKE;
        } timeout;

    } m_watchdog;

    void start_timeout(q::Clock::duration d, State failed_state);
    void update_watchdog();

    Sensor_Sample m_sensors;
    std::vector<Sensor_Sample> m_sensor_samples;

    Received_Config m_received_config;

    struct Calibration_Config
    {
        math::vec3f accelerometer_bias;
        math::vec3f accelerometer_scale = math::vec3f(1);
        math::vec3f gyroscope_bias;
        math::vec3f compass_bias;
    } m_calibration_config;

    PWM_Frequency m_pwm_frequency = PWM_Frequency::SERVO_50HZ;
    std::vector<float> m_motor_outputs;

    void send_configs_if_changed();

    void set_state(State state);
    State m_state = State::DISCONNECTED;
    size_t m_error_count = 0;

    struct Settings
    {
        rapidjson::Document document;
    } m_settings;

    auto load_settings() -> bool;
    void save_settings();
};

}
