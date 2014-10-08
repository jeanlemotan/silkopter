#include "BrainStdAfx.h"

#ifdef RASPBERRY_PI

#include "HAL_Sensors_Pi.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"

#define USE_MPU9250
#define USE_MS5611


#ifdef USE_MPU9250
#   include "chips/MPU9250.h"
#else
#   error "No IMU selected"
#endif

#ifdef USE_MS5611
#   include "chips/MS5611.h"
#else
#   error "No Barometer selected"
#endif

using namespace silk;
using namespace boost::asio;

struct HAL_Sensors_Pi::Impl
{
#ifdef USE_MPU9250
    MPU9250 mpu;
#endif

#ifdef USE_MS5611
    MS5611 baro;
#endif
};

///////////////////////////////////////////////////////////////

HAL_Sensors_Pi::HAL_Sensors_Pi()
{
    load_settings();

    m_impl.reset(new Impl);
}

HAL_Sensors_Pi::~HAL_Sensors_Pi()
{
}

auto HAL_Sensors_Pi::load_settings() -> bool
{
    q::data::File_Source fs(q::Path("sensors.cfg"));
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

    return true;
}
void HAL_Sensors_Pi::save_settings()
{
    TIMED_FUNCTION();
    if (!m_settings.document.IsObject())
    {
        m_settings.document.SetObject();
    }

    auto& allocator = m_settings.document.GetAllocator();

    auto& calibration_config = jsonutil::get_or_add_member(m_settings.document, "calibration_config", rapidjson::kObjectType, allocator);

    {
        auto& json = calibration_config;
        jsonutil::set_vec3_value(json, "accelerometer_bias", m_calibration_config.accelerometer_bias, allocator);
        jsonutil::set_vec3_value(json, "accelerometer_scale", m_calibration_config.accelerometer_scale, allocator);
        jsonutil::set_vec3_value(json, "gyroscope_bias", m_calibration_config.gyroscope_bias, allocator);
        jsonutil::set_vec3_value(json, "compass_bias", m_calibration_config.compass_bias, allocator);
    }

    typedef rapidjson::UTF8<> JSON_Charset;
    typedef rapidjson::GenericStringBuffer<JSON_Charset> JSON_Buffer;
    typedef rapidjson::PrettyWriter<JSON_Buffer> JSON_Writer;

    JSON_Buffer buffer;
    JSON_Writer writer(buffer);
    m_settings.document.Accept(writer);

    q::data::File_Sink fs(q::Path("sensors.cfg"));
    if (!fs.is_open())
    {
        SILK_WARNING("Cannot open file to save settings");
        return;
    }

    fs.write(reinterpret_cast<uint8_t const*>(buffer.GetString()), buffer.GetSize());
}

auto HAL_Sensors_Pi::init() -> Result
{
    QASSERT(!m_is_initialized);
    if (m_is_initialized)
    {
        return Result::OK;
    }

#ifdef USE_MS5611
    m_impl->baro.init("/dev/i2c-1");
#endif

#ifdef USE_MPU9250
    m_impl->mpu.init("/dev/i2c-1", MPU9250::Gyroscope_Range::_500_DPS, MPU9250::Accelerometer_Range::_4_G);
#endif

    auto now = q::Clock::now();
    m_accelerometer_sample.time_point = now;
    m_gyroscope_sample.time_point = now;
    m_compass_sample.time_point = now;
    m_barometer_sample.time_point = now;
    m_sonar_sample.time_point = now;
    m_thermometer_sample.time_point = now;
    m_voltage_sample.time_point = now;
    m_current_sample.time_point = now;
    m_gps_sample.time_point = now;

    m_is_initialized = true;
    return Result::OK;
}

void HAL_Sensors_Pi::set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale)
{
    m_calibration_config.accelerometer_bias = bias;
    m_calibration_config.accelerometer_scale = scale;
    save_settings();
}
void HAL_Sensors_Pi::get_accelerometer_calibration_data(math::vec3f &bias, math::vec3f &scale) const
{
    bias = m_calibration_config.accelerometer_bias;
    scale = m_calibration_config.accelerometer_scale;
}

void HAL_Sensors_Pi::set_gyroscope_calibration_data(math::vec3f const& bias)
{
    m_calibration_config.gyroscope_bias = bias;
    save_settings();
}
void HAL_Sensors_Pi::get_gyroscope_calibration_data(math::vec3f &bias) const
{
    bias = m_calibration_config.gyroscope_bias;
}

void HAL_Sensors_Pi::set_compass_calibration_data(math::vec3f const& bias)
{
    m_calibration_config.compass_bias = bias;
    save_settings();
}
void HAL_Sensors_Pi::get_compass_calibration_data(math::vec3f &bias) const
{
    bias = m_calibration_config.compass_bias;
}

auto HAL_Sensors_Pi::get_accelerometer_samples() const -> std::vector<Accelerometer_Sample> const&
{
    return m_accelerometer_samples;
}
auto HAL_Sensors_Pi::get_gyroscope_samples() const -> std::vector<Gyroscope_Sample> const&
{
    return m_gyroscope_samples;
}
auto HAL_Sensors_Pi::get_compass_samples() const -> std::vector<Compass_Sample> const&
{
    return m_compass_samples;
}
auto HAL_Sensors_Pi::get_barometer_samples() const -> std::vector<Barometer_Sample> const&
{
    return m_barometer_samples;
}
auto HAL_Sensors_Pi::get_sonar_samples() const -> std::vector<Sonar_Sample> const&
{
    return m_sonar_samples;
}
auto HAL_Sensors_Pi::get_thermometer_samples() const -> std::vector<Thermometer_Sample> const&
{
    return m_thermometer_samples;
}
auto HAL_Sensors_Pi::get_voltage_samples() const -> std::vector<Voltage_Sample> const&
{
    return m_voltage_samples;
}
auto HAL_Sensors_Pi::get_current_samples() const -> std::vector<Current_Sample> const&
{
    return m_current_samples;
}
auto HAL_Sensors_Pi::get_gps_samples() const -> std::vector<GPS_Sample> const&
{
    return m_gps_samples;
}

size_t HAL_Sensors_Pi::get_error_count() const
{
    return m_error_count;
}

void HAL_Sensors_Pi::process()
{
    QASSERT(m_is_initialized);
    if (!m_is_initialized)
    {
        return;
    }

    m_accelerometer_samples.clear();
    m_gyroscope_samples.clear();
    m_compass_samples.clear();
    m_barometer_samples.clear();
    m_sonar_samples.clear();
    m_thermometer_samples.clear();
    m_voltage_samples.clear();
    m_current_samples.clear();
    m_gps_samples.clear();

    auto start = q::Clock::now();

#ifdef USE_MPU9250
    m_impl->mpu.process();

    auto a_sample_time = m_impl->mpu.get_sample_time();
    auto g_sample_time = a_sample_time;
    auto const& g_samples = m_impl->mpu.get_gyroscope_samples();
    auto const& a_samples = m_impl->mpu.get_accelerometer_samples();
    QASSERT(a_samples.size() == g_samples.size());

    m_gyroscope_samples.resize(g_samples.size());
    for (size_t i = 0; i < g_samples.size(); i++)
    {
        m_gyroscope_sample.value = g_samples[i] - m_calibration_config.gyroscope_bias;
        m_gyroscope_sample.dt = g_sample_time;
        m_gyroscope_sample.time_point += g_sample_time;
        m_gyroscope_sample.sample_idx++;

        m_gyroscope_samples[i] = m_gyroscope_sample;
    }
    QASSERT(start - m_gyroscope_sample.time_point >= std::chrono::milliseconds(0) && start - m_gyroscope_sample.time_point < std::chrono::milliseconds(200));

    m_accelerometer_samples.resize(a_samples.size());
    for (size_t i = 0; i < a_samples.size(); i++)
    {
        m_accelerometer_sample.value = (a_samples[i] - m_calibration_config.accelerometer_bias) * m_calibration_config.accelerometer_scale;
        m_accelerometer_sample.dt = g_sample_time;
        m_accelerometer_sample.time_point += g_sample_time;
        m_accelerometer_sample.sample_idx++;

        m_accelerometer_samples[i] = m_accelerometer_sample;
    }
    QASSERT(start - m_accelerometer_sample.time_point >= std::chrono::milliseconds(0) && start - m_accelerometer_sample.time_point < std::chrono::milliseconds(200));

    auto compass_sample = m_impl->mpu.read_compass();
    if (compass_sample)
    {
        m_compass_sample.value = *compass_sample - m_calibration_config.compass_bias;
        m_compass_sample.sample_idx++;
        m_compass_sample.dt = start - m_compass_sample.time_point;
        m_compass_sample.time_point = start;
        m_compass_samples.push_back(m_compass_sample);
    }
#endif

#ifdef USE_MS5611
    m_impl->baro.process();
    auto baro_sample = m_impl->baro.read_pressure();
    if (baro_sample)
    {
        m_barometer_sample.value = *baro_sample;
        m_barometer_sample.sample_idx++;
        m_barometer_sample.dt = start - m_barometer_sample.time_point;
        m_barometer_sample.time_point = start;
        m_barometer_samples.push_back(m_barometer_sample);
    }
    auto temp_sample = m_impl->baro.read_temperature();
    if (temp_sample)
    {
        m_thermometer_sample.value = *temp_sample;
        m_thermometer_sample.sample_idx++;
        m_thermometer_sample.dt = start - m_thermometer_sample.time_point;
        m_thermometer_sample.time_point = start;
        m_thermometer_samples.push_back(m_thermometer_sample);
    }
#endif

//    auto d = q::Clock::now() - start;
//    SILK_INFO("d = {}, {}", d, m_sensor_samples.size());
}


#endif
