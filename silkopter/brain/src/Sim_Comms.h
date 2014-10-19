#pragma once


namespace silk
{

class HAL_Sensors_Sim;
class HAL_Motors_Sim;

class Sim_Comms : q::util::Noncopyable
{
    friend class HAL_Sensors_Sim;
    friend class HAL_Motors_Sim;
public:
    Sim_Comms(boost::asio::io_service& io_service);

    auto connect() -> bool;
    void disconnect();

    bool is_connected() const;

    size_t get_error_count() const;

    //----------------------------------------------------------------------

    void process();

protected:
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
    };

    typedef util::Channel<Message, uint8_t, boost::asio::ip::tcp::socket> Channel;

    auto get_channel() -> Channel&;

    q::util::Signal<void(Message message, Channel& channel)> message_received_signal;

private:
    boost::asio::io_service& m_io_service;

    boost::asio::ip::tcp::socket m_socket;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
    Channel m_channel;
    void handle_accept(boost::system::error_code const& error);

    q::Clock::time_point m_resend_timestamp;
    q::Clock::time_point m_stay_alive_timestamp;

    void process_state_disconnected();
    void process_state_handshake();
    void process_state_sending_config();
    void process_state_running();

    Message m_message;
    bool m_has_message = false;
    q::Clock::time_point m_last_message_timestamp;
    size_t m_message_count = 0;

    Received_Config m_received_config;

    void set_state(State state);
    State m_state = State::DISCONNECTED;
    size_t m_error_count = 0;
};

}



