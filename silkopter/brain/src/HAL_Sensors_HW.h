#pragma once

#include "HAL_Sensors.h"

namespace silk
{

class HAL_Sensors_HW : public HAL_Sensors
{
public:
    HAL_Sensors_HW();
    ~HAL_Sensors_HW();

    auto init() -> bool;
    void shutdown();

    //----------------------------------------------------------------------
    //calibration
    void set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale);
    void get_accelerometer_calibration_data(math::vec3f& bias, math::vec3f& scale) const;

    void set_gyroscope_calibration_data(math::vec3f const& bias);
    void get_gyroscope_calibration_data(math::vec3f& bias) const;

    void set_compass_calibration_data(math::vec3f const& bias);
    void get_compass_calibration_data(math::vec3f& bias) const;

    void set_current_calibration_data(float scale);
    void get_current_calibration_data(float& scale) const;

    void set_voltage_calibration_data(float scale);
    void get_voltage_calibration_data(float& scale) const;

    auto get_accelerometer_samples() const  -> std::vector<sensors::Accelerometer_Sample> const&;
    auto get_gyroscope_samples() const      -> std::vector<sensors::Gyroscope_Sample> const&;
    auto get_compass_samples() const        -> std::vector<sensors::Compass_Sample> const&;
    auto get_barometer_samples() const      -> std::vector<sensors::Barometer_Sample> const&;
    auto get_sonar_samples() const          -> std::vector<sensors::Sonar_Sample> const&;
    auto get_thermometer_samples() const    -> std::vector<sensors::Thermometer_Sample> const&;
    auto get_voltage_samples() const        -> std::vector<sensors::Voltage_Sample> const&;
    auto get_current_samples() const        -> std::vector<sensors::Current_Sample> const&;
    auto get_gps_samples() const            -> std::vector<sensors::GPS_Sample> const&;

    auto get_last_accelerometer_sample() const  -> sensors::Accelerometer_TP_Sample const&;
    auto get_last_gyroscope_sample() const      -> sensors::Gyroscope_TP_Sample const&;
    auto get_last_compass_sample() const        -> sensors::Compass_TP_Sample const&;
    auto get_last_barometer_sample() const      -> sensors::Barometer_TP_Sample const&;
    auto get_last_sonar_sample() const          -> sensors::Sonar_TP_Sample const&;
    auto get_last_thermometer_sample() const    -> sensors::Thermometer_TP_Sample const&;
    auto get_last_voltage_sample() const        -> sensors::Voltage_TP_Sample const&;
    auto get_last_current_sample() const        -> sensors::Current_TP_Sample const&;
    auto get_last_gps_sample() const            -> sensors::GPS_TP_Sample const&;

    //----------------------------------------------------------------------
    size_t get_error_count() const;

    //----------------------------------------------------------------------
    void process();

    struct Config
    {
        math::vec3f accelerometer_bias;
        math::vec3f accelerometer_scale = math::vec3f(1);
        math::vec3f gyroscope_bias;
        math::vec3f compass_bias;
        size_t gyroscope_range = 500;
        size_t accelerometer_range = 4;
        std::string mpu_i2c_device;
        std::string barometer_i2c_device;
        std::string gps_device;
        size_t gps_baud = 38400;

        float current_scale = 24.8f;
        float voltage_scale = 39.33f;
    };

private:
    sensors::Accelerometer_TP_Sample m_last_accelerometer_sample;
    std::vector<sensors::Accelerometer_Sample> m_accelerometer_samples;

    sensors::Gyroscope_TP_Sample m_last_gyroscope_sample;
    std::vector<sensors::Gyroscope_Sample> m_gyroscope_samples;

    sensors::Compass_TP_Sample m_last_compass_sample;
    std::vector<sensors::Compass_Sample> m_compass_samples;

    sensors::Barometer_TP_Sample m_last_barometer_sample;
    std::vector<sensors::Barometer_Sample> m_barometer_samples;

    sensors::Sonar_TP_Sample m_last_sonar_sample;
    std::vector<sensors::Sonar_Sample> m_sonar_samples;

    sensors::Thermometer_TP_Sample m_last_thermometer_sample;
    std::vector<sensors::Thermometer_Sample> m_thermometer_samples;

    sensors::Voltage_TP_Sample m_last_voltage_sample;
    std::vector<sensors::Voltage_Sample> m_voltage_samples;

    sensors::Current_TP_Sample m_last_current_sample;
    std::vector<sensors::Current_Sample> m_current_samples;

    sensors::GPS_TP_Sample m_last_gps_sample;
    std::vector<sensors::GPS_Sample> m_gps_samples;

    void process_gps();

    bool m_is_initialized = false;

    Config m_config;

    struct Sensors;
    std::shared_ptr<Sensors> m_sensors;

    auto load_settings() -> bool;
    void save_settings();
};

}
