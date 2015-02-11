#include "BrainStdAfx.h"
#include "HAL_Sensors_Sim.h"
#include "utils/Json_Helpers.h"

//#ifndef RASPBERRY_PI

////#include "sz_math.hpp"
////#include "sz_hal_sensors_sim_config.hpp"


//using namespace silk;
//using namespace boost::asio;

//HAL_Sensors_Sim::HAL_Sensors_Sim(Sim_Comms& sim_comms)
//    : m_sim_comms(sim_comms)
//{
//    sim_comms.message_received_signal.connect(std::bind(&HAL_Sensors_Sim::handle_message, this, std::placeholders::_1, std::placeholders::_2));

//    load_settings();
//    save_settings();
//}

//auto HAL_Sensors_Sim::load_settings() -> bool
//{
////    autojsoncxx::ParsingResult result;
////    Config cfg;
////    if (!autojsoncxx::from_json_file("sensors_sim.cfg", cfg, result))
////    {
////        LOG_WARNING("Failed to load sensors_sim.cfg: {}", result.description());
////        return false;
////    }

//    m_config = cfg;

//    return true;
//}
//void HAL_Sensors_Sim::save_settings()
//{
////    autojsoncxx::to_pretty_json_file("sensors_sim.cfg", m_config);
//}

//void HAL_Sensors_Sim::set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale)
//{
//    m_config.accelerometer_bias = bias;
//    m_config.accelerometer_scale = scale;
//    save_settings();
//}
//void HAL_Sensors_Sim::get_accelerometer_calibration_data(math::vec3f &bias, math::vec3f &scale) const
//{
//    bias = m_config.accelerometer_bias;
//    scale = m_config.accelerometer_scale;
//}

//void HAL_Sensors_Sim::set_gyroscope_calibration_data(math::vec3f const& bias)
//{
//    m_config.gyroscope_bias = bias;
//    save_settings();
//}
//void HAL_Sensors_Sim::get_gyroscope_calibration_data(math::vec3f &bias) const
//{
//    bias = m_config.gyroscope_bias;
//}

//void HAL_Sensors_Sim::set_compass_calibration_data(math::vec3f const& bias)
//{
//    m_config.compass_bias = bias;
//    save_settings();
//}
//void HAL_Sensors_Sim::get_compass_calibration_data(math::vec3f &bias) const
//{
//    bias = m_config.compass_bias;
//}

//auto HAL_Sensors_Sim::get_accelerometer_samples() const -> std::vector<Accelerometer_Sample> const&
//{
//    return m_accelerometer_samples;
//}
//auto HAL_Sensors_Sim::get_gyroscope_samples() const -> std::vector<Gyroscope_Sample> const&
//{
//    return m_gyroscope_samples;
//}
//auto HAL_Sensors_Sim::get_compass_samples() const -> std::vector<Compass_Sample> const&
//{
//    return m_compass_samples;
//}
//auto HAL_Sensors_Sim::get_barometer_samples() const -> std::vector<Barometer_Sample> const&
//{
//    return m_barometer_samples;
//}
//auto HAL_Sensors_Sim::get_sonar_samples() const -> std::vector<Sonar_Sample> const&
//{
//    return m_sonar_samples;
//}
//auto HAL_Sensors_Sim::get_thermometer_samples() const -> std::vector<Thermometer_Sample> const&
//{
//    return m_thermometer_samples;
//}
//auto HAL_Sensors_Sim::get_voltage_samples() const -> std::vector<Voltage_Sample> const&
//{
//    return m_voltage_samples;
//}
//auto HAL_Sensors_Sim::get_current_samples() const -> std::vector<Current_Sample> const&
//{
//    return m_current_samples;
//}
//auto HAL_Sensors_Sim::get_gps_samples() const -> std::vector<GPS_Sample> const&
//{
//    return m_gps_samples;
//}

//auto HAL_Sensors_Sim::get_last_accelerometer_sample() const  -> Accelerometer_Sample const&
//{
//    return m_accelerometer_sample;
//}
//auto HAL_Sensors_Sim::get_last_gyroscope_sample() const      -> Gyroscope_Sample const&
//{
//    return m_gyroscope_sample;
//}
//auto HAL_Sensors_Sim::get_last_compass_sample() const        -> Compass_Sample const&
//{
//    return m_compass_sample;
//}
//auto HAL_Sensors_Sim::get_last_barometer_sample() const      -> Barometer_Sample const&
//{
//    return m_barometer_sample;
//}
//auto HAL_Sensors_Sim::get_last_sonar_sample() const          -> Sonar_Sample const&
//{
//    return m_sonar_sample;
//}
//auto HAL_Sensors_Sim::get_last_thermometer_sample() const    -> Thermometer_Sample const&
//{
//    return m_thermometer_sample;
//}
//auto HAL_Sensors_Sim::get_last_voltage_sample() const        -> Voltage_Sample const&
//{
//    return m_voltage_sample;
//}
//auto HAL_Sensors_Sim::get_last_current_sample() const        -> Current_Sample const&
//{
//    return m_current_sample;
//}
//auto HAL_Sensors_Sim::get_last_gps_sample() const            -> GPS_Sample const&
//{
//    return m_gps_sample;
//}

//size_t HAL_Sensors_Sim::get_error_count() const
//{
//    return m_error_count + m_sim_comms.get_error_count();
//}

//template<class SAMPLE_T>
//auto HAL_Sensors_Sim::unpack_sensor_sample(Sim_Comms::Channel& channel, SAMPLE_T& sample) -> bool
//{
//    uint32_t dt = 0;
//    decltype(sample.value) v;
//    if (!channel.unpack_param(dt) || !channel.unpack_param(v))
//    {
//        LOG_WARNING("Failed to receive sensor sample");
//        m_error_count++;
//        return false;
//    }

//    sample.value = v;
//    sample.dt = std::chrono::microseconds(dt);
//    sample.sample_idx++;
//    return true;
//}

//auto HAL_Sensors_Sim::process_accelerometer_sensor(Sim_Comms::Channel& channel) -> bool
//{
//    if (unpack_sensor_sample(channel, m_accelerometer_sample))
//    {
//        m_accelerometer_sample.value = (m_accelerometer_sample.value - m_config.accelerometer_bias) * m_config.accelerometer_scale;
//        m_accelerometer_samples.push_back(m_accelerometer_sample);
//        return true;
//    }
//    return false;
//}
//auto HAL_Sensors_Sim::process_gyroscope_sensor(Sim_Comms::Channel& channel) -> bool
//{
//    if (unpack_sensor_sample(channel, m_gyroscope_sample))
//    {
//        m_gyroscope_sample.value = m_gyroscope_sample.value - m_config.accelerometer_bias;
//        m_gyroscope_samples.push_back(m_gyroscope_sample);
//        return true;
//    }
//    return false;
//}
//auto HAL_Sensors_Sim::process_compass_sensor(Sim_Comms::Channel& channel) -> bool
//{
//    if (unpack_sensor_sample(channel, m_compass_sample))
//    {
//        m_compass_sample.value = m_compass_sample.value - m_config.accelerometer_bias;
//        m_compass_samples.push_back(m_compass_sample);
//        return true;
//    }
//    return false;
//}

//auto HAL_Sensors_Sim::process_barometer_sensor(Sim_Comms::Channel& channel) -> bool
//{
//    if (unpack_sensor_sample(channel, m_barometer_sample))
//    {
//        m_barometer_samples.push_back(m_barometer_sample);
//        return true;
//    }
//    return false;
//}
//auto HAL_Sensors_Sim::process_thermometer_sensor(Sim_Comms::Channel& channel) -> bool
//{
//    if (unpack_sensor_sample(channel, m_thermometer_sample))
//    {
//        m_thermometer_samples.push_back(m_thermometer_sample);
//        return true;
//    }
//    return false;
//}
//auto HAL_Sensors_Sim::process_sonar_sensor(Sim_Comms::Channel& channel) -> bool
//{
//    if (unpack_sensor_sample(channel, m_sonar_sample))
//    {
//        m_sonar_samples.push_back(m_sonar_sample);
//        return true;
//    }
//    return false;
//}
//auto HAL_Sensors_Sim::process_voltage_sensor(Sim_Comms::Channel& channel) -> bool
//{
//    if (unpack_sensor_sample(channel, m_voltage_sample))
//    {
//        m_voltage_samples.push_back(m_voltage_sample);
//        return true;
//    }
//    return false;
//}
//auto HAL_Sensors_Sim::process_current_sensor(Sim_Comms::Channel& channel) -> bool
//{
//    if (unpack_sensor_sample(channel, m_current_sample))
//    {
//        m_current_samples.push_back(m_current_sample);
//        return true;
//    }
//    return false;
//}
//auto HAL_Sensors_Sim::process_gps_sensor(Sim_Comms::Channel& channel) -> bool
//{
//    if (unpack_sensor_sample(channel, m_gps_sample))
//    {
//        m_gps_samples.push_back(m_gps_sample);
//        return true;
//    }
//    return false;
//}

//void HAL_Sensors_Sim::process_message_sensor_data(Sim_Comms::Channel& channel)
//{
//    bool result = false;
//    if (channel.begin_unpack())
//    {
//        Sim_Comms::Sensors sensors;
//        if (channel.unpack_param(sensors))
//        {
//            if (sensors.test(Sim_Comms::Sensor::ACCELEROMETER))
//            {
//                result = process_accelerometer_sensor(channel);
//            }
//            if (sensors.test(Sim_Comms::Sensor::GYROSCOPE))
//            {
//                result = process_gyroscope_sensor(channel);
//            }
//            if (sensors.test(Sim_Comms::Sensor::COMPASS))
//            {
//                result = process_compass_sensor(channel);
//            }
//            if (sensors.test(Sim_Comms::Sensor::BAROMETER))
//            {
//                result = process_barometer_sensor(channel);
//            }
//            if (sensors.test(Sim_Comms::Sensor::THERMOMETER))
//            {
//                result = process_thermometer_sensor(channel);
//            }
//            if (sensors.test(Sim_Comms::Sensor::SONAR))
//            {
//                result = process_sonar_sensor(channel);
//            }
//            if (sensors.test(Sim_Comms::Sensor::VOLTAGE))
//            {
//                result = process_voltage_sensor(channel);
//            }
//            if (sensors.test(Sim_Comms::Sensor::CURRENT))
//            {
//                result = process_current_sensor(channel);
//            }
//            if (sensors.test(Sim_Comms::Sensor::GPS))
//            {
//                result = process_gps_sensor(channel);
//            }
//        }
//    }
//    channel.end_unpack();

//    if (!result)
//    {
//        LOG_WARNING("Failed to receive sensor data");
//        m_error_count++;
//    }
//}
////----------------------------------------------------------------------------------

//void HAL_Sensors_Sim::process()
//{
//    m_accelerometer_samples.clear();
//    m_gyroscope_samples.clear();
//    m_compass_samples.clear();
//    m_barometer_samples.clear();
//    m_sonar_samples.clear();
//    m_thermometer_samples.clear();
//    m_voltage_samples.clear();
//    m_current_samples.clear();
//    m_gps_samples.clear();

//    if (!m_sim_comms.is_connected())
//    {
//        m_sim_comms.connect();
//    }
//    else
//    {
//        m_sim_comms.process();
//    }
//}


//void HAL_Sensors_Sim::handle_message(Sim_Comms::Message message, Sim_Comms::Channel& channel)
//{
//    if (message == Sim_Comms::Message::SENSOR_DATA)
//    {
//        process_message_sensor_data(channel);
//    }
//}

//#endif
