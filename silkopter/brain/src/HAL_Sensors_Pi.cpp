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

    m_impl.reset(new Private);
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
    if (is_initialized())
    {
        return Result::OK;
    }

#ifdef USE_MS5611
    m_impl->baro.init("/dev/i2c-1");
#endif

#ifdef USE_MPU9250
    m_impl->mpu.init("/dev/i2c-1", MPU9250::Gyroscope_Range::_500_DPS, MPU9250::Accelerometer_Range::_4_G);
#endif

    m_is_initialized = true;
    return Result::OK;
}

void HAL_Sensors_Pi::disconnect()
{
    m_is_initialized = false;
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

auto HAL_Sensors_Pi::get_sensor_samples() const -> std::vector<Sensor_Sample> const&
{
    return m_sensor_samples;
}

size_t HAL_Sensors_Pi::get_error_count() const
{
    return m_error_count;
}

void HAL_Sensors_Pi::process()
{
    if (is_disconnected())
    {
        connect();
        return;
    }

    m_sensor_samples.clear();

    auto start = q::Clock::now();

#ifdef USE_MPU9250
    auto a_sample_time = m_impl->mpu.get_sample_time();
    auto g_sample_time = a_sample_time;
    m_impl->mpu.process();
    auto const& g_samples = m_impl->mpu.get_gyroscope_samples();
    auto const& a_samples = m_impl->mpu.get_accelerometer_samples();
    auto compass_sample = m_impl->mpu.read_compass();
    if (compass_sample)
    {
        m_sensors.compass.value = *compass_sample - m_calibration_config.compass_bias;
        m_sensors.compass.sample_idx++;
    }
#endif

#ifdef USE_MS5611
    m_impl->baro.process();
    auto baro_sample = m_impl->baro.read_pressure();
    if (baro_sample)
    {
        m_sensors.barometer.value = *baro_sample;
        m_sensors.barometer.sample_idx++;
    }
    auto temp_sample = m_impl->baro.read_temperature();
    if (temp_sample)
    {
        m_sensors.thermometer.value = *temp_sample;
        m_sensors.thermometer.sample_idx++;
    }
#endif

    QASSERT(a_samples.size() == g_samples.size());
    m_sensor_samples.resize(a_samples.size());
    for (size_t i = 0; i < a_samples.size(); i++)
    {
        auto& sample = m_sensor_samples[i];

        {
            sample.accelerometer.value.acceleration = (a_samples[i] - m_calibration_config.accelerometer_bias) * m_calibration_config.accelerometer_scale;
            sample.accelerometer.value.dt = a_sample_time;
            sample.accelerometer.sample_idx = m_sensors.accelerometer.sample_idx++;
        }
        {
            sample.gyroscope.value.angular_velocity = g_samples[i] - m_calibration_config.gyroscope_bias;
            sample.gyroscope.value.dt = g_sample_time;
            sample.gyroscope.sample_idx = m_sensors.gyroscope.sample_idx++;
        }

        sample.barometer = m_sensors.barometer;
        sample.thermometer = m_sensors.thermometer;
        sample.compass = m_sensors.compass;
    }

//    auto d = q::Clock::now() - start;
//    SILK_INFO("d = {}, {}", d, m_sensor_samples.size());
}


#endif
