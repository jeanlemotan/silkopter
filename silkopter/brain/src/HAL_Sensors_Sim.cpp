#include "BrainStdAfx.h"
#include "HAL_Sensors_Sim.h"
#include "utils/Json_Helpers.h"

#ifndef RASPBERRY_PI

using namespace silk;
using namespace boost::asio;

HAL_Sensors_Sim::HAL_Sensors_Sim(Sim_Comms& sim_comms)
    : m_sim_comms(sim_comms)
{
    sim_comms.message_received_signal.connect(std::bind(&HAL_Sensors_Sim::handle_message, this, std::placeholders::_1, std::placeholders::_2));

    load_settings();
}

auto HAL_Sensors_Sim::load_settings() -> bool
{
    q::data::File_Source fs(q::Path("io_board.cfg"));
    if (!fs.is_open())
    {
        return false;
    }

    auto size = fs.get_size();

    std::vector<char> data(size + 1);
    auto result = fs.read(reinterpret_cast<uint8_t*>(data.data()), size);
    if (result != size)
    {
        SILK_WARNING("Failed to read settings.");
        return false;
    }

    data[size] = 0;
    if (m_settings.document.Parse<0>(data.data()).HasParseError())
    {
        SILK_WARNING("Failed to parse settings: {}:{}", m_settings.document.GetErrorOffset(), m_settings.document.GetParseError());
        return false;
    }

    auto finder = jsonutil::Member_Finder(m_settings.document, "calibration_config");
    if (!finder.is_valid())
    {
        SILK_WARNING("Cannot find root");
        return false;
    }
    auto& calibration_config = finder.get_value();

//    finder = jsonutil::Member_Finder(m_settings.document, "motor_config");
//    if (!finder.is_valid())
//    {
//        SILK_WARNING("Cannot find root");
//        return false;
//    }
//    auto& motor_config = finder.get_value();

//    finder = jsonutil::Member_Finder(m_settings.document, "pid_config");
//    if (!finder.is_valid())
//    {
//        SILK_WARNING("Cannot find root");
//        return false;
//    }
//    auto& pid_config = finder.get_value();

    {
        auto& json = calibration_config;
        auto abias = jsonutil::get_vec3_value<float>(json, "accelerometer_bias");
        auto ascale = jsonutil::get_vec3_value<float>(json, "accelerometer_scale");
        auto gbias = jsonutil::get_vec3_value<float>(json, "gyroscope_bias");
        auto cbias = jsonutil::get_vec3_value<float>(json, "compass_bias");
        if (!abias || !ascale || !gbias || !cbias)
        {
            SILK_WARNING("Cannot read calibration config");
            return false;
        }
        m_calibration_config.accelerometer_bias = *abias;
        m_calibration_config.accelerometer_scale = *ascale;
        m_calibration_config.gyroscope_bias = *gbias;
        m_calibration_config.compass_bias = *cbias;
    }

//    {
//        auto& json = motor_config;
//        {
//            auto freq = jsonutil::get_uint_value(json, "frequency");
//            if (!freq || *freq > static_cast<uint32_t>(PWM_Frequency::_500HZ))
//            {
//                SILK_WARNING("Cannot read motor frequency");
//                return false;
//            }
//            m_req_motor_config.frequency = static_cast<PWM_Frequency>(*freq);

//            finder = jsonutil::Member_Finder(json, "motors");
//            if (!finder.is_type(rapidjson::kArrayType))
//            {
//                SILK_WARNING("Cannot read motors");
//                return false;
//            }
//            m_req_motor_config.motor_count = math::min<uint8_t>(MAX_MOTOR_COUNT, finder.get_value().Size());
//            if (m_req_motor_config.motor_count < 4 || (m_req_motor_config.motor_count & 1) == 1)
//            {
//                SILK_WARNING("To few motors / odd number of morors specified: {}", m_req_motor_config.motor_count);
//                return false;
//            }
//            for (uint8_t i = 0; i < m_req_motor_config.motor_count; i++)
//            {
//                auto& mj = finder.get_value()[i];
//                auto position = jsonutil::get_vec2_value<float>(mj, "position");
//                auto clockwise = jsonutil::get_bool_value(mj, "clockwise");
//                if (!position || !clockwise)
//                {
//                    SILK_WARNING("Cannot read motor {}", i);
//                    return false;
//                }
//                m_req_motor_config.motors[i].position = *position;
//                m_req_motor_config.motors[i].clockwise = *clockwise;
//            }
//        }
//        m_req_motor_config.id++;
//    }

//    {
//        auto& json = pid_config;
//        {
//            finder = jsonutil::Member_Finder(json, "yaw_rate");
//            if (!finder.is_type(rapidjson::kObjectType))
//            {
//                SILK_WARNING("Cannot read yaw_rate");
//                return false;
//            }
//            auto p = jsonutil::get_float_value(finder.get_value(), "p");
//            auto i = jsonutil::get_float_value(finder.get_value(), "p");
//            auto d = jsonutil::get_float_value(finder.get_value(), "p");
//            if (!p || !i || !d)
//            {
//                SILK_WARNING("Cannot read yaw_rate pid");
//                return false;
//            }
//            m_req_pid_config.yaw_rate.p = *p;
//            m_req_pid_config.yaw_rate.i = *i;
//            m_req_pid_config.yaw_rate.d = *d;
//        }
//        {
//            finder = jsonutil::Member_Finder(json, "pitch_rate");
//            if (!finder.is_type(rapidjson::kObjectType))
//            {
//                SILK_WARNING("Cannot read pitch_rate");
//                return false;
//            }
//            auto p = jsonutil::get_float_value(finder.get_value(), "p");
//            auto i = jsonutil::get_float_value(finder.get_value(), "p");
//            auto d = jsonutil::get_float_value(finder.get_value(), "p");
//            if (!p || !i || !d)
//            {
//                SILK_WARNING("Cannot read pitch_rate pid");
//                return false;
//            }
//            m_req_pid_config.pitch_rate.p = *p;
//            m_req_pid_config.pitch_rate.i = *i;
//            m_req_pid_config.pitch_rate.d = *d;
//        }
//        {
//            finder = jsonutil::Member_Finder(json, "roll_rate");
//            if (!finder.is_type(rapidjson::kObjectType))
//            {
//                SILK_WARNING("Cannot read roll_rate");
//                return false;
//            }
//            auto p = jsonutil::get_float_value(finder.get_value(), "p");
//            auto i = jsonutil::get_float_value(finder.get_value(), "p");
//            auto d = jsonutil::get_float_value(finder.get_value(), "p");
//            if (!p || !i || !d)
//            {
//                SILK_WARNING("Cannot read roll_rate pid");
//                return false;
//            }
//            m_req_pid_config.roll_rate.p = *p;
//            m_req_pid_config.roll_rate.i = *i;
//            m_req_pid_config.roll_rate.d = *d;
//        }

//        m_req_pid_config.id++;
//    }


    return true;
}
void HAL_Sensors_Sim::save_settings()
{
    if (!m_settings.document.IsObject())
    {
        m_settings.document.SetObject();
    }

    auto& allocator = m_settings.document.GetAllocator();

    auto& calibration_config = jsonutil::get_or_add_member(m_settings.document, "calibration_config", rapidjson::kObjectType, allocator);
//    auto& motor_config = jsonutil::get_or_add_member(m_settings.document, "motor_config", rapidjson::kObjectType, allocator);
//    auto& pid_config = jsonutil::get_or_add_member(m_settings.document, "pid_config", rapidjson::kObjectType, allocator);

    {
        auto& json = calibration_config;
        jsonutil::set_vec3_value(json, "accelerometer_bias", m_calibration_config.accelerometer_bias, allocator);
        jsonutil::set_vec3_value(json, "accelerometer_scale", m_calibration_config.accelerometer_scale, allocator);
        jsonutil::set_vec3_value(json, "gyroscope_bias", m_calibration_config.gyroscope_bias, allocator);
        jsonutil::set_vec3_value(json, "compass_bias", m_calibration_config.compass_bias, allocator);
    }
//    {
//        auto& json = motor_config;
//        {
//            auto& vj = jsonutil::get_or_add_member(json, "frequency", rapidjson::kNumberType, allocator);
//            vj.SetInt(static_cast<int>(m_crt_motor_config.frequency));
//        }
//        {
//            auto& vj = jsonutil::get_or_add_member(json, "motors", rapidjson::kArrayType, allocator);
//            vj.Clear();
//            for (uint8_t i = 0; i < m_crt_motor_config.motor_count; i++)
//            {
//                rapidjson::Value v;
//                v.SetObject();
//                jsonutil::set_vec2_value(v, "position", m_crt_motor_config.motors[i].position, allocator);
//                v.AddMember("clockwise", m_crt_motor_config.motors[i].clockwise, allocator);
//                vj.PushBack(v, allocator);
//            }
//        }
//    }
//    {
//        auto& json = pid_config;
//        {
//            auto& vj = jsonutil::get_or_add_member(json, "yaw_rate", rapidjson::kObjectType, allocator);
//            jsonutil::set_float_value(vj, "p", m_crt_pid_config.yaw_rate.p, allocator);
//            jsonutil::set_float_value(vj, "i", m_crt_pid_config.yaw_rate.i, allocator);
//            jsonutil::set_float_value(vj, "d", m_crt_pid_config.yaw_rate.d, allocator);
//        }
//        {
//            auto& vj = jsonutil::get_or_add_member(json, "pitch_rate", rapidjson::kObjectType, allocator);
//            jsonutil::set_float_value(vj, "p", m_crt_pid_config.pitch_rate.p, allocator);
//            jsonutil::set_float_value(vj, "i", m_crt_pid_config.pitch_rate.i, allocator);
//            jsonutil::set_float_value(vj, "d", m_crt_pid_config.pitch_rate.d, allocator);
//        }
//        {
//            auto& vj = jsonutil::get_or_add_member(json, "roll_rate", rapidjson::kObjectType, allocator);
//            jsonutil::set_float_value(vj, "p", m_crt_pid_config.roll_rate.p, allocator);
//            jsonutil::set_float_value(vj, "i", m_crt_pid_config.roll_rate.i, allocator);
//            jsonutil::set_float_value(vj, "d", m_crt_pid_config.roll_rate.d, allocator);
//        }
//    }


    typedef rapidjson::UTF8<> JSON_Charset;
    typedef rapidjson::GenericStringBuffer<JSON_Charset> JSON_Buffer;
    typedef rapidjson::PrettyWriter<JSON_Buffer> JSON_Writer;

    JSON_Buffer buffer;
    JSON_Writer writer(buffer);
    m_settings.document.Accept(writer);

    q::data::File_Sink fs(q::Path("io_board.cfg"));
    if (!fs.is_open())
    {
        SILK_WARNING("Cannot open file to save settings");
        return;
    }

    fs.write(reinterpret_cast<uint8_t const*>(buffer.GetString()), buffer.GetSize());
}

void HAL_Sensors_Sim::set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale)
{
    m_calibration_config.accelerometer_bias = bias;
    m_calibration_config.accelerometer_scale = scale;
}
void HAL_Sensors_Sim::get_accelerometer_calibration_data(math::vec3f &bias, math::vec3f &scale) const
{
    bias = m_calibration_config.accelerometer_bias;
    scale = m_calibration_config.accelerometer_scale;
}

void HAL_Sensors_Sim::set_gyroscope_calibration_data(math::vec3f const& bias)
{
    m_calibration_config.gyroscope_bias = bias;
}
void HAL_Sensors_Sim::get_gyroscope_calibration_data(math::vec3f &bias) const
{
    bias = m_calibration_config.gyroscope_bias;
}

void HAL_Sensors_Sim::set_compass_calibration_data(math::vec3f const& bias)
{
    m_calibration_config.compass_bias = bias;
}
void HAL_Sensors_Sim::get_compass_calibration_data(math::vec3f &bias) const
{
    bias = m_calibration_config.compass_bias;
}

auto HAL_Sensors_Sim::get_accelerometer_samples() const -> std::vector<Accelerometer_Sample> const&
{
    return m_accelerometer_samples;
}
auto HAL_Sensors_Sim::get_gyroscope_samples() const -> std::vector<Gyroscope_Sample> const&
{
    return m_gyroscope_samples;
}
auto HAL_Sensors_Sim::get_compass_samples() const -> std::vector<Compass_Sample> const&
{
    return m_compass_samples;
}
auto HAL_Sensors_Sim::get_barometer_samples() const -> std::vector<Barometer_Sample> const&
{
    return m_barometer_samples;
}
auto HAL_Sensors_Sim::get_sonar_samples() const -> std::vector<Sonar_Sample> const&
{
    return m_sonar_samples;
}
auto HAL_Sensors_Sim::get_thermometer_samples() const -> std::vector<Thermometer_Sample> const&
{
    return m_thermometer_samples;
}
auto HAL_Sensors_Sim::get_voltage_samples() const -> std::vector<Voltage_Sample> const&
{
    return m_voltage_samples;
}
auto HAL_Sensors_Sim::get_current_samples() const -> std::vector<Current_Sample> const&
{
    return m_current_samples;
}
auto HAL_Sensors_Sim::get_gps_samples() const -> std::vector<GPS_Sample> const&
{
    return m_gps_samples;
}

size_t HAL_Sensors_Sim::get_error_count() const
{
    return m_error_count + m_sim_comms.get_error_count();
}

auto HAL_Sensors_Sim::process_accelerometer_sensor(Sim_Comms::Channel& channel) -> bool
{
    uint8_t dt = 0;
    math::vec3f v;
    if (!channel.unpack_param(dt) || !channel.unpack_param(v))
    {
        SILK_WARNING("Failed to receive accelerometer data");
        m_error_count++;
        return false;
    }

    auto& sample = m_accelerometer_sample;
    sample.value = (v - m_calibration_config.accelerometer_bias) * m_calibration_config.accelerometer_scale;
    sample.dt = std::chrono::milliseconds(dt);
    sample.time_point += std::chrono::milliseconds(dt);
    sample.sample_idx++;
    m_accelerometer_samples.push_back(sample);
    return true;
}
auto HAL_Sensors_Sim::process_gyroscope_sensor(Sim_Comms::Channel& channel) -> bool
{
    uint8_t dt = 0;
    math::vec3f v;
    if (!channel.unpack_param(dt) || !channel.unpack_param(v))
    {
        SILK_WARNING("Failed to receive gyroscope data");
        m_error_count++;
        return false;
    }

    auto& sample = m_gyroscope_sample;
    sample.value = v - m_calibration_config.gyroscope_bias;
    sample.dt = std::chrono::milliseconds(dt);
    sample.time_point += std::chrono::milliseconds(dt);
    sample.sample_idx++;
    m_gyroscope_samples.push_back(sample);
    return false;
}
auto HAL_Sensors_Sim::process_compass_sensor(Sim_Comms::Channel& channel) -> bool
{
    math::vec3f v;
    if (!channel.unpack_param(v))
    {
        SILK_WARNING("Failed to receive compass data");
        m_error_count++;
        return false;
    }

    auto now = q::Clock::now();
    auto& sample = m_compass_sample;
    sample.value = v - m_calibration_config.gyroscope_bias;
    sample.dt = sample.time_point - now;
    sample.time_point = now;
    sample.sample_idx++;
    m_compass_samples.push_back(sample);
    return true;
}

template<class SAMPLE_T>
auto HAL_Sensors_Sim::unpack_sensor_sample(Sim_Comms::Channel& channel, SAMPLE_T& sample, std::vector<SAMPLE_T>& samples) -> bool
{
    decltype(sample.value) v;
    if (!channel.unpack_param(v))
    {
        SILK_WARNING("Failed to receive sensor sample");
        m_error_count++;
        return false;
    }

    auto now = q::Clock::now();
    sample.value = v;
    sample.dt = sample.time_point - now;
    sample.time_point = now;
    sample.sample_idx++;
    samples.push_back(sample);
    return true;
}

auto HAL_Sensors_Sim::process_barometer_sensor(Sim_Comms::Channel& channel) -> bool
{
    return unpack_sensor_sample(channel, m_barometer_sample, m_barometer_samples);
}
auto HAL_Sensors_Sim::process_thermometer_sensor(Sim_Comms::Channel& channel) -> bool
{
    return unpack_sensor_sample(channel, m_thermometer_sample, m_thermometer_samples);
}
auto HAL_Sensors_Sim::process_sonar_sensor(Sim_Comms::Channel& channel) -> bool
{
    return unpack_sensor_sample(channel, m_sonar_sample, m_sonar_samples);
}
auto HAL_Sensors_Sim::process_voltage_sensor(Sim_Comms::Channel& channel) -> bool
{
    return unpack_sensor_sample(channel, m_voltage_sample, m_voltage_samples);
}
auto HAL_Sensors_Sim::process_current_sensor(Sim_Comms::Channel& channel) -> bool
{
    return unpack_sensor_sample(channel, m_current_sample, m_current_samples);
}
auto HAL_Sensors_Sim::process_gps_sensor(Sim_Comms::Channel& channel) -> bool
{
    return unpack_sensor_sample(channel, m_gps_sample, m_gps_samples);
}

void HAL_Sensors_Sim::process_message_sensor_data(Sim_Comms::Channel& channel)
{
    bool result = false;
    if (channel.begin_unpack())
    {
        Sim_Comms::Sensors sensors;
        if (channel.unpack_param(sensors))
        {
            if (sensors.test(Sim_Comms::Sensor::ACCELEROMETER))
            {
                result = process_accelerometer_sensor(channel);
            }
            if (sensors.test(Sim_Comms::Sensor::GYROSCOPE))
            {
                result = process_gyroscope_sensor(channel);
            }
            if (sensors.test(Sim_Comms::Sensor::COMPASS))
            {
                result = process_compass_sensor(channel);
            }
            if (sensors.test(Sim_Comms::Sensor::BAROMETER))
            {
                result = process_barometer_sensor(channel);
            }
            if (sensors.test(Sim_Comms::Sensor::THERMOMETER))
            {
                result = process_thermometer_sensor(channel);
            }
            if (sensors.test(Sim_Comms::Sensor::SONAR))
            {
                result = process_sonar_sensor(channel);
            }
            if (sensors.test(Sim_Comms::Sensor::VOLTAGE))
            {
                result = process_voltage_sensor(channel);
            }
            if (sensors.test(Sim_Comms::Sensor::CURRENT))
            {
                result = process_current_sensor(channel);
            }
            if (sensors.test(Sim_Comms::Sensor::GPS))
            {
                result = process_gps_sensor(channel);
            }
        }
    }
    channel.end_unpack();

    if (!result)
    {
        SILK_WARNING("Failed to receive sensor data");
        m_error_count++;
    }
}
//----------------------------------------------------------------------------------

void HAL_Sensors_Sim::process()
{
    if (!m_sim_comms.is_connected())
    {
        m_sim_comms.connect();
    }
    else
    {
        m_sim_comms.process();
    }
}


void HAL_Sensors_Sim::handle_message(Sim_Comms::Message message, Sim_Comms::Channel& channel)
{
    switch (message)
    {
    case Sim_Comms::Message::SENSOR_DATA: process_message_sensor_data(channel); break;
    }
}

#endif
