#pragma once

#include "HAL_Sensors.h"

namespace silk
{

class Sim_Comms;

class HAL_Sensors_Sim : public HAL_Sensors
{
public:
    HAL_Sensors_Sim(Sim_Comms& sim_comms);

    //----------------------------------------------------------------------
    //calibration
    void set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale);
    void get_accelerometer_calibration_data(math::vec3f& bias, math::vec3f& scale) const;

    void set_gyroscope_calibration_data(math::vec3f const& bias);
    void get_gyroscope_calibration_data(math::vec3f& bias) const;

    void set_compass_calibration_data(math::vec3f const& bias);
    void get_compass_calibration_data(math::vec3f& bias) const;

    auto get_samples() const -> std::vector<Sensor_Sample> const&;
    size_t get_error_count() const;
    void process();

private:
    Sim_Comms& m_sim_comms;

    void process_message_sensor_data();
    void process_message_gps_data();

    auto process_accelerometer_sensor() -> Channel::Unpack_Result;
    auto process_gyroscope_sensor() -> Channel::Unpack_Result;
    auto process_compass_sensor() -> Channel::Unpack_Result;
    auto process_barometer_sensor() -> Channel::Unpack_Result;
    auto process_thermometer_sensor() -> Channel::Unpack_Result;
    auto process_sonar_sensor() -> Channel::Unpack_Result;
    auto process_voltage_sensor() -> Channel::Unpack_Result;
    auto process_current_sensor() -> Channel::Unpack_Result;

    Sensor_Sample m_sample;
    std::vector<Sensor_Sample> m_sensor_samples;

    struct Settings
    {
        rapidjson::Document document;
    } m_settings;

    auto load_settings() -> bool;
    void save_settings();
};

}
