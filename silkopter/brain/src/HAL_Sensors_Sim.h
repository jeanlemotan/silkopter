#pragma once

#include "HAL_Sensors.h"
#include "Comms.h"

namespace silk
{

class HAL_Sensors_Sim : public HAL_Sensors
{
public:
    HAL_Sensors_Sim(Comms& comms);

    //----------------------------------------------------------------------
    //calibration
    void set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale);
    void get_accelerometer_calibration_data(math::vec3f& bias, math::vec3f& scale) const;

    void set_gyroscope_calibration_data(math::vec3f const& bias);
    void get_gyroscope_calibration_data(math::vec3f& bias) const;

    void set_compass_calibration_data(math::vec3f const& bias);
    void get_compass_calibration_data(math::vec3f& bias) const;

    auto get_accelerometer_samples() const  -> std::vector<sensors::Accelerometer_Sample> const&;
    auto get_gyroscope_samples() const      -> std::vector<sensors::Gyroscope_Sample> const&;
    auto get_compass_samples() const        -> std::vector<sensors::Compass_Sample> const&;
    auto get_barometer_samples() const      -> std::vector<sensors::Barometer_Sample> const&;
    auto get_sonar_samples() const          -> std::vector<sensors::Sonar_Sample> const&;
    auto get_thermometer_samples() const    -> std::vector<sensors::Thermometer_Sample> const&;
    auto get_voltmeter_samples() const        -> std::vector<sensors::Voltmeter_Sample> const&;
    auto get_ammeter_samples() const        -> std::vector<sensors::Ammeter_Sample> const&;
    auto get_gps_samples() const            -> std::vector<sensors::GPS_Sample> const&;

    auto get_last_accelerometer_sample() const  -> sensors::Accelerometer_TP_Sample const&;
    auto get_last_gyroscope_sample() const      -> sensors::Gyroscope_TP_Sample const&;
    auto get_last_compass_sample() const        -> sensors::Compass_TP_Sample const&;
    auto get_last_barometer_sample() const      -> sensors::Barometer_TP_Sample const&;
    auto get_last_sonar_sample() const          -> sensors::Sonar_TP_Sample const&;
    auto get_last_thermometer_sample() const    -> sensors::Thermometer_TP_Sample const&;
    auto get_last_voltage_sample() const        -> sensors::Voltmeter_TP_Sample const&;
    auto get_last_current_sample() const        -> sensors::Ammeter_TP_Sample const&;
    auto get_last_gps_sample() const            -> sensors::GPS_TP_Sample const&;

    size_t get_error_count() const;
    void process();

    struct Config
    {
        math::vec3f accelerometer_bias;
        math::vec3f accelerometer_scale = math::vec3f(1);
        math::vec3f gyroscope_bias;
        math::vec3f compass_bias;
    };

private:
    Comms& m_comms;

    typedef util::Channel<comms::Telemetry_Message, uint16_t> Channel;
    Channel m_comms_channel;

    void process_message_sensor_data(Channel& channel);
    void process_message_gps_data();

    template<class SAMPLE_T>
    auto unpack_sensor_sample(Channel& channel, SAMPLE_T& sample) -> bool;

    auto process_accelerometer_sensor(Channel& channel) -> bool;
    auto process_gyroscope_sensor(Channel& channel) -> bool;
    auto process_compass_sensor(Channel& channel) -> bool;
    auto process_barometer_sensor(Channel& channel) -> bool;
    auto process_thermometer_sensor(Channel& channel) -> bool;
    auto process_sonar_sensor(Channel& channel) -> bool;
    auto process_voltage_sensor(Channel& channel) -> bool;
    auto process_current_sensor(Channel& channel) -> bool;
    auto process_gps_sensor(Channel& channel) -> bool;

    sensors::Accelerometer_TP_Sample m_accelerometer_sample;
    std::vector<sensors::Accelerometer_Sample> m_accelerometer_samples;

    sensors::Gyroscope_TP_Sample m_gyroscope_sample;
    std::vector<sensors::Gyroscope_Sample> m_gyroscope_samples;

    sensors::Compass_TP_Sample m_compass_sample;
    std::vector<sensors::Compass_Sample> m_compass_samples;

    sensors::Barometer_TP_Sample m_barometer_sample;
    std::vector<sensors::Barometer_Sample> m_barometer_samples;

    sensors::Sonar_TP_Sample m_sonar_sample;
    std::vector<sensors::Sonar_Sample> m_sonar_samples;

    sensors::Thermometer_TP_Sample m_thermometer_sample;
    std::vector<sensors::Thermometer_Sample> m_thermometer_samples;

    sensors::Voltmeter_TP_Sample m_voltage_sample;
    std::vector<sensors::Voltmeter_Sample> m_voltage_samples;

    sensors::Ammeter_TP_Sample m_current_sample;
    std::vector<sensors::Ammeter_Sample> m_current_samples;

    sensors::GPS_TP_Sample m_gps_sample;
    std::vector<sensors::GPS_Sample> m_gps_samples;

    Config m_config;

    size_t m_error_count = 0;

    auto load_settings() -> bool;
    void save_settings();
};

}
