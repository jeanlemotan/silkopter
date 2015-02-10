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

    auto get_accelerometer_samples() const  -> std::vector<sensor::Accelerometer_Sample> const&;
    auto get_gyroscope_samples() const      -> std::vector<sensor::Gyroscope_Sample> const&;
    auto get_compass_samples() const        -> std::vector<sensor::Compass_Sample> const&;
    auto get_barometer_samples() const      -> std::vector<sensor::Barometer_Sample> const&;
    auto get_sonar_samples() const          -> std::vector<sensor::Sonar_Sample> const&;
    auto get_thermometer_samples() const    -> std::vector<sensor::Thermometer_Sample> const&;
    auto get_voltmeter_samples() const        -> std::vector<sensor::Voltmeter_Sample> const&;
    auto get_ammeter_samples() const        -> std::vector<sensor::Ammeter_Sample> const&;
    auto get_gps_samples() const            -> std::vector<sensor::GPS_Sample> const&;

    auto get_last_accelerometer_sample() const  -> sensor::Accelerometer_TP_Sample const&;
    auto get_last_gyroscope_sample() const      -> sensor::Gyroscope_TP_Sample const&;
    auto get_last_compass_sample() const        -> sensor::Compass_TP_Sample const&;
    auto get_last_barometer_sample() const      -> sensor::Barometer_TP_Sample const&;
    auto get_last_sonar_sample() const          -> sensor::Sonar_TP_Sample const&;
    auto get_last_thermometer_sample() const    -> sensor::Thermometer_TP_Sample const&;
    auto get_last_voltage_sample() const        -> sensor::Voltmeter_TP_Sample const&;
    auto get_last_current_sample() const        -> sensor::Ammeter_TP_Sample const&;
    auto get_last_gps_sample() const            -> sensor::GPS_TP_Sample const&;

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

    sensor::Accelerometer_TP_Sample m_accelerometer_sample;
    std::vector<sensor::Accelerometer_Sample> m_accelerometer_samples;

    sensor::Gyroscope_TP_Sample m_gyroscope_sample;
    std::vector<sensor::Gyroscope_Sample> m_gyroscope_samples;

    sensor::Compass_TP_Sample m_compass_sample;
    std::vector<sensor::Compass_Sample> m_compass_samples;

    sensor::Barometer_TP_Sample m_barometer_sample;
    std::vector<sensor::Barometer_Sample> m_barometer_samples;

    sensor::Sonar_TP_Sample m_sonar_sample;
    std::vector<sensor::Sonar_Sample> m_sonar_samples;

    sensor::Thermometer_TP_Sample m_thermometer_sample;
    std::vector<sensor::Thermometer_Sample> m_thermometer_samples;

    sensor::Voltmeter_TP_Sample m_voltage_sample;
    std::vector<sensor::Voltmeter_Sample> m_voltage_samples;

    sensor::Ammeter_TP_Sample m_current_sample;
    std::vector<sensor::Ammeter_Sample> m_current_samples;

    sensor::GPS_TP_Sample m_gps_sample;
    std::vector<sensor::GPS_Sample> m_gps_samples;

    Config m_config;

    size_t m_error_count = 0;

    auto load_settings() -> bool;
    void save_settings();
};

}
