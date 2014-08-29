#pragma once

#include "IO_Board.h"

namespace silk
{

class IO_Board_RPi : public IO_Board
{
public:
    IO_Board_RPi();
    ~IO_Board_RPi();

    auto connect() -> Connection_Result;
    void disconnect();

    bool is_disconnected() const;
    bool is_running() const;

    //----------------------------------------------------------------------
    //motors

    void set_motor_output(size_t motor_idx, float output);

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
    auto setup_spi() -> Connection_Result;

    Sensor_Sample m_sensors;
    std::vector<Sensor_Sample> m_sensor_samples;

    struct Calibration_Config
    {
        math::vec3f accelerometer_bias;
        math::vec3f accelerometer_scale = math::vec3f(1);
        math::vec3f gyroscope_bias;
        math::vec3f compass_bias;
    } m_calibration_config;

    PWM_Frequency m_pwm_frequency = PWM_Frequency::_50HZ;
    std::vector<float> m_motor_outputs;

    bool m_is_connected = false;
    size_t m_error_count = 0;

    struct Private;
    std::shared_ptr<Private> m_p;

    struct Settings
    {
        rapidjson::Document document;
    } m_settings;

    auto load_settings() -> bool;
    void save_settings();
};

}
