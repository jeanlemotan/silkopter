#include "BrainStdAfx.h"

#ifdef RASPBERRY_PI

#include "HAL_Sensors_Pi.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"

#include "sz_math.hpp"
#include "sz_hal_sensors_config_pi.hpp"


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
    m_impl.reset(new Impl);

    load_settings();
    save_settings();
}

HAL_Sensors_Pi::~HAL_Sensors_Pi()
{
}

auto HAL_Sensors_Pi::load_settings() -> bool
{
    autojsoncxx::ParsingResult result;
    Config cfg;
    if (!autojsoncxx::from_json_file("sensors_pi.cfg", cfg, result))
    {
        SILK_WARNING("Failed to load sensors_pi.cfg: {}", result.description());
        return false;
    }

    m_config = cfg;
    if (m_config.barometer_i2c_device.empty())
    {
        m_config.barometer_i2c_device = "/dev/i2c-1";
    }
    if (m_config.mpu_i2c_device.empty())
    {
        m_config.mpu_i2c_device = "/dev/i2c-1";
    }

    return true;
}
void HAL_Sensors_Pi::save_settings()
{
    autojsoncxx::to_pretty_json_file("sensors_pi.cfg", m_config);
}

auto HAL_Sensors_Pi::init() -> bool
{
    QASSERT(!m_is_initialized);
    if (m_is_initialized)
    {
        return true;
    }

#ifdef USE_MS5611
    if (!m_impl->baro.init(m_config.barometer_i2c_device))
    {
        return false;
    }
#endif

#ifdef USE_MPU9250
    MPU9250::Gyroscope_Range g_range;
    switch (m_config.gyroscope_range)
    {
    case 250: g_range = MPU9250::Gyroscope_Range::_250_DPS; break;
    case 500: g_range = MPU9250::Gyroscope_Range::_500_DPS; break;
    case 1000: g_range = MPU9250::Gyroscope_Range::_1000_DPS; break;
    case 2000: g_range = MPU9250::Gyroscope_Range::_2000_DPS; break;
    default:
        SILK_WARNING("Invalid gyroscope range {}. Using 500 DPS", m_config.gyroscope_range);
        g_range = MPU9250::Gyroscope_Range::_500_DPS;
        break;
    }

    MPU9250::Accelerometer_Range a_range;
    switch (m_config.accelerometer_range)
    {
    case 2: a_range = MPU9250::Accelerometer_Range::_2_G; break;
    case 4: a_range = MPU9250::Accelerometer_Range::_4_G; break;
    case 8: a_range = MPU9250::Accelerometer_Range::_8_G; break;
    case 16: a_range = MPU9250::Accelerometer_Range::_16_G; break;
    default:
        SILK_WARNING("Invalid accelerometer range {}. Using 4G", m_config.accelerometer_range);
        a_range = MPU9250::Accelerometer_Range::_4_G;
        break;
    }

    if (!m_impl->mpu.init(m_config.mpu_i2c_device, g_range, a_range))
    {
        return false;
    }
#endif

    auto now = q::Clock::now();

    m_is_initialized = true;
    return true;
}

void HAL_Sensors_Pi::set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale)
{
    m_config.accelerometer_bias = bias;
    m_config.accelerometer_scale = scale;
    save_settings();
}
void HAL_Sensors_Pi::get_accelerometer_calibration_data(math::vec3f &bias, math::vec3f &scale) const
{
    bias = m_config.accelerometer_bias;
    scale = m_config.accelerometer_scale;
}

void HAL_Sensors_Pi::set_gyroscope_calibration_data(math::vec3f const& bias)
{
    m_config.gyroscope_bias = bias;
    save_settings();
}
void HAL_Sensors_Pi::get_gyroscope_calibration_data(math::vec3f &bias) const
{
    bias = m_config.gyroscope_bias;
}

void HAL_Sensors_Pi::set_compass_calibration_data(math::vec3f const& bias)
{
    m_config.compass_bias = bias;
    save_settings();
}
void HAL_Sensors_Pi::get_compass_calibration_data(math::vec3f &bias) const
{
    bias = m_config.compass_bias;
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

    auto const& g_samples = m_impl->mpu.get_gyroscope_samples();
    auto const& a_samples = m_impl->mpu.get_accelerometer_samples();
    QASSERT(a_samples.size() == g_samples.size());

    m_gyroscope_samples.resize(g_samples.size());
    for (size_t i = 0; i < g_samples.size(); i++)
    {
        m_gyroscope_sample.value = g_samples[i] - m_config.gyroscope_bias;
        m_gyroscope_sample.dt = m_impl->mpu.get_gyroscope_sample_time();
        m_gyroscope_sample.sample_idx++;
        m_gyroscope_samples[i] = m_gyroscope_sample;
    }

    m_accelerometer_samples.resize(a_samples.size());
    for (size_t i = 0; i < a_samples.size(); i++)
    {
        m_accelerometer_sample.value = (a_samples[i] - m_config.accelerometer_bias) * m_config.accelerometer_scale;
        m_accelerometer_sample.dt = m_impl->mpu.get_accelerometer_sample_time();
        m_accelerometer_sample.sample_idx++;
        m_accelerometer_samples[i] = m_accelerometer_sample;
    }

    auto compass_sample = m_impl->mpu.read_compass();
    if (compass_sample)
    {
        m_compass_sample.value = *compass_sample - m_config.compass_bias;
        m_compass_sample.sample_idx++;
        m_compass_sample.dt = m_impl->mpu.get_compass_sample_time();
        m_compass_samples.push_back(m_compass_sample);
    }
#endif

#ifdef USE_MS5611
    m_impl->baro.process();
    auto baro_sample = m_impl->baro.read_barometer();
    if (baro_sample)
    {
        m_barometer_sample.value = *baro_sample;
        m_barometer_sample.sample_idx++;
        m_barometer_sample.dt = m_impl->baro.get_barometer_sample_time();
        m_barometer_samples.push_back(m_barometer_sample);
    }
    auto temp_sample = m_impl->baro.read_thermometer();
    if (temp_sample)
    {
        m_thermometer_sample.value = *temp_sample;
        m_thermometer_sample.sample_idx++;
        m_thermometer_sample.dt = m_impl->baro.get_thermometer_sample_time();
        m_thermometer_samples.push_back(m_thermometer_sample);
    }
#endif

//    auto d = q::Clock::now() - start;
//    SILK_INFO("d = {}, {}", d, m_sensor_samples.size());
}


#endif
