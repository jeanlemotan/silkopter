#pragma once

#include "HAL_Sensors.h"

namespace silk
{

class HAL_Sensors_Pi : public HAL_Sensors
{
public:
    HAL_Sensors_Pi();
    ~HAL_Sensors_Pi();

    auto init() -> bool;

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

    auto get_accelerometer_samples() const -> std::vector<Accelerometer_Sample> const&;
    auto get_gyroscope_samples() const -> std::vector<Gyroscope_Sample> const&;
    auto get_compass_samples() const -> std::vector<Compass_Sample> const&;
    auto get_barometer_samples() const -> std::vector<Barometer_Sample> const&;
    auto get_sonar_samples() const -> std::vector<Sonar_Sample> const&;
    auto get_thermometer_samples() const -> std::vector<Thermometer_Sample> const&;
    auto get_voltage_samples() const -> std::vector<Voltage_Sample> const&;
    auto get_current_samples() const -> std::vector<Current_Sample> const&;
    auto get_gps_samples() const -> std::vector<GPS_Sample> const&; 

    auto get_last_accelerometer_sample() const  -> Accelerometer_Sample const&;
    auto get_last_gyroscope_sample() const      -> Gyroscope_Sample const&;
    auto get_last_compass_sample() const        -> Compass_Sample const&;
    auto get_last_barometer_sample() const      -> Barometer_Sample const&;
    auto get_last_sonar_sample() const          -> Sonar_Sample const&;
    auto get_last_thermometer_sample() const    -> Thermometer_Sample const&;
    auto get_last_voltage_sample() const        -> Voltage_Sample const&;
    auto get_last_current_sample() const        -> Current_Sample const&;
    auto get_last_gps_sample() const            -> GPS_Sample const&;

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

        float current_scale = 32.2f;
        float voltage_scale = 39.33f;
    };

private:
    Accelerometer_Sample m_accelerometer_sample;
    std::vector<Accelerometer_Sample> m_accelerometer_samples;

    Gyroscope_Sample m_gyroscope_sample;
    std::vector<Gyroscope_Sample> m_gyroscope_samples;

    Compass_Sample m_compass_sample;
    std::vector<Compass_Sample> m_compass_samples;

    Barometer_Sample m_barometer_sample;
    std::vector<Barometer_Sample> m_barometer_samples;

    Sonar_Sample m_sonar_sample;
    std::vector<Sonar_Sample> m_sonar_samples;

    Thermometer_Sample m_thermometer_sample;
    std::vector<Thermometer_Sample> m_thermometer_samples;

    Voltage_Sample m_voltage_sample;
    std::vector<Voltage_Sample> m_voltage_samples;

    Current_Sample m_current_sample;
    std::vector<Current_Sample> m_current_samples;

    GPS_Sample m_gps_sample;
    std::vector<GPS_Sample> m_gps_samples;

    bool m_is_initialized = false;
    size_t m_error_count = 0;

    Config m_config;

    struct Impl;
    std::shared_ptr<Impl> m_impl;

    auto load_settings() -> bool;
    void save_settings();
};

}
