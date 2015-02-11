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

    auto get_accelerometer_samples() const  -> std::vector<node::IAccelerometer::Sample> const&;
    auto get_gyroscope_samples() const      -> std::vector<node::IGyroscope::Sample> const&;
    auto get_compass_samples() const        -> std::vector<node::ICompass::Sample> const&;
    auto get_barometer_samples() const      -> std::vector<node::IBarometer::Sample> const&;
    auto get_sonar_samples() const          -> std::vector<node::ISonar::Sample> const&;
    auto get_thermometer_samples() const    -> std::vector<node::IThermometer::Sample> const&;
    auto get_voltmeter_samples() const        -> std::vector<node::IVoltmeter::Sample> const&;
    auto get_ammeter_samples() const        -> std::vector<node::IAmmeter::Sample> const&;
    auto get_gps_samples() const            -> std::vector<node::IGPS::Sample> const&;

    auto get_last_accelerometer_sample() const  -> node::IAccelerometer::TP_Sample const&;
    auto get_last_gyroscope_sample() const      -> node::IGyroscope::TP_Sample const&;
    auto get_last_compass_sample() const        -> node::ICompass::TP_Sample const&;
    auto get_last_barometer_sample() const      -> node::IBarometer::TP_Sample const&;
    auto get_last_sonar_sample() const          -> node::ISonar::TP_Sample const&;
    auto get_last_thermometer_sample() const    -> node::IThermometer::TP_Sample const&;
    auto get_last_voltage_sample() const        -> node::IVoltmeter::TP_Sample const&;
    auto get_last_current_sample() const        -> node::IAmmeter::TP_Sample const&;
    auto get_last_gps_sample() const            -> node::IGPS::TP_Sample const&;

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

    node::IAccelerometer::TP_Sample m_accelerometer_sample;
    std::vector<node::IAccelerometer::Sample> m_accelerometer_samples;

    node::IGyroscope::TP_Sample m_gyroscope_sample;
    std::vector<node::IGyroscope::Sample> m_gyroscope_samples;

    node::ICompass::TP_Sample m_compass_sample;
    std::vector<node::ICompass::Sample> m_compass_samples;

    node::IBarometer::TP_Sample m_barometer_sample;
    std::vector<node::IBarometer::Sample> m_barometer_samples;

    node::ISonar::TP_Sample m_sonar_sample;
    std::vector<node::ISonar::Sample> m_sonar_samples;

    node::IThermometer::TP_Sample m_thermometer_sample;
    std::vector<node::IThermometer::Sample> m_thermometer_samples;

    node::IVoltmeter::TP_Sample m_voltage_sample;
    std::vector<node::IVoltmeter::Sample> m_voltage_samples;

    node::IAmmeter::TP_Sample m_current_sample;
    std::vector<node::IAmmeter::Sample> m_current_samples;

    node::IGPS::TP_Sample m_gps_sample;
    std::vector<node::IGPS::Sample> m_gps_samples;

    Config m_config;

    size_t m_error_count = 0;

    auto load_settings() -> bool;
    void save_settings();
};

}
