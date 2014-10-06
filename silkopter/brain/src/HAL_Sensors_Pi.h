#pragma once

#include "Sensor_Interface.h"

namespace silk
{

class Sensor_Interface_Pi : public Sensor_Interface
{
public:
    Sensor_Interface_Pi();
    ~Sensor_Interface_Pi();

    auto init() -> Result;
    void shutdown();

    auto is_initialized() const -> bool;

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
    auto setup_spi() -> Result;

    Sample m_sensors;
    std::vector<Sample> m_samples;

    struct Calibration_Config
    {
        math::vec3f accelerometer_bias;
        math::vec3f accelerometer_scale = math::vec3f(1);
        math::vec3f gyroscope_bias;
        math::vec3f compass_bias;
    } m_calibration_config;

    bool m_is_initialized = false;
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
