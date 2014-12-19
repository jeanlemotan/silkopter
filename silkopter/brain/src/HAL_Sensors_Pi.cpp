#include "BrainStdAfx.h"

#ifdef RASPBERRY_PI

#include "HAL_Sensors_Pi.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"

#include "sz_math.hpp"
#include "sz_hal_sensors_config_pi.hpp"


#define USE_MPU9250
#define USE_MS5611
#define USE_ODROIDW_ADC

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

#ifdef USE_ODROIDW_ADC
#   include "chips/OdroidW_ADC.h"
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

#ifdef USE_ODROIDW_ADC
    OdroidW_ADC adc;
#endif
};

///////////////////////////////////////////////////////////////

HAL_Sensors_Pi::HAL_Sensors_Pi()
{
    m_impl.reset(new Impl);

    m_config.barometer_i2c_device = "/dev/i2c-1";
    m_config.mpu_i2c_device = "/dev/i2c-1";

    load_settings();
    save_settings();
}

HAL_Sensors_Pi::~HAL_Sensors_Pi()
{
}

auto HAL_Sensors_Pi::load_settings() -> bool
{
    TIMED_FUNCTION();

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
    TIMED_FUNCTION();

    autojsoncxx::to_pretty_json_file("sensors_pi.cfg", m_config);
}

auto HAL_Sensors_Pi::init() -> bool
{
    TIMED_FUNCTION();

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
    {
        typedef MPU9250::Gyroscope_Range G_Range;
        typedef MPU9250::Accelerometer_Range A_Range;

        std::vector<G_Range> g_ranges = { G_Range::_250_DPS, G_Range::_500_DPS, G_Range::_1000_DPS, G_Range::_2000_DPS };
        std::vector<A_Range> a_ranges = { A_Range::_2_G, A_Range::_4_G, A_Range::_8_G, A_Range::_16_G };

        std::nth_element(g_ranges.begin(), g_ranges.begin(), g_ranges.end(), [&](G_Range a, G_Range b)
        {
            return math::abs(static_cast<int>(a) - m_config.gyroscope_range) < math::abs(static_cast<int>(b) - m_config.gyroscope_range);
        });
        G_Range g_range = g_ranges.front();

        std::nth_element(a_ranges.begin(), a_ranges.begin(), a_ranges.end(), [&](A_Range a, A_Range b)
        {
            return math::abs(static_cast<int>(a) - m_config.accelerometer_range) < math::abs(static_cast<int>(b) - m_config.accelerometer_range);
        });
        A_Range a_range = a_ranges.front();

        SILK_INFO("Gyroscope range {} DPS (requested {} DPS)", static_cast<size_t>(g_range), m_config.gyroscope_range);
        SILK_INFO("Accelerometer range {}G (requested {}G)", static_cast<size_t>(a_range), m_config.accelerometer_range);

        if (!m_impl->mpu.init(m_config.mpu_i2c_device, g_range, a_range))
        {
            return false;
        }
    }
#endif

#ifdef USE_ODROIDW_ADC
    if (!m_impl->adc.init(std::chrono::milliseconds(0), std::chrono::milliseconds(500)))
    {
        return false;
    }
#endif

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

void HAL_Sensors_Pi::set_current_calibration_data(float scale)
{
    m_config.current_scale = scale;
    save_settings();
}

void HAL_Sensors_Pi::get_current_calibration_data(float& scale) const
{
    scale = m_config.current_scale;
}

void HAL_Sensors_Pi::set_voltage_calibration_data(float scale)
{
    m_config.voltage_scale = scale;
    save_settings();
}
void HAL_Sensors_Pi::get_voltage_calibration_data(float& scale) const
{
    scale = m_config.voltage_scale;
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
auto HAL_Sensors_Pi::get_last_accelerometer_sample() const  -> Accelerometer_Sample const&
{
    return m_accelerometer_sample;
}
auto HAL_Sensors_Pi::get_last_gyroscope_sample() const      -> Gyroscope_Sample const&
{
    return m_gyroscope_sample;
}
auto HAL_Sensors_Pi::get_last_compass_sample() const        -> Compass_Sample const&
{
    return m_compass_sample;
}
auto HAL_Sensors_Pi::get_last_barometer_sample() const      -> Barometer_Sample const&
{
    return m_barometer_sample;
}
auto HAL_Sensors_Pi::get_last_sonar_sample() const          -> Sonar_Sample const&
{
    return m_sonar_sample;
}
auto HAL_Sensors_Pi::get_last_thermometer_sample() const    -> Thermometer_Sample const&
{
    return m_thermometer_sample;
}
auto HAL_Sensors_Pi::get_last_voltage_sample() const        -> Voltage_Sample const&
{
    return m_voltage_sample;
}
auto HAL_Sensors_Pi::get_last_current_sample() const        -> Current_Sample const&
{
    return m_current_sample;
}
auto HAL_Sensors_Pi::get_last_gps_sample() const            -> GPS_Sample const&
{
    return m_gps_sample;
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

    {
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
    }
#endif

#ifdef USE_MS5611
    m_impl->baro.process();
    {
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
    }
#endif

#ifdef USE_ODROIDW_ADC
    m_impl->adc.process();
    {
        auto val = m_impl->adc.read_adc0_sample();
        if (val)
        {
            m_current_sample.value = *val * m_config.current_scale;
            m_current_sample.dt = m_impl->adc.get_adc0_sample_time();
            m_current_sample.sample_idx++;
            m_current_samples.push_back(m_current_sample);
        }

        val = m_impl->adc.read_adc1_sample();
        if (val)
        {
            m_voltage_sample.value = *val * m_config.voltage_scale;
            m_voltage_sample.dt = m_impl->adc.get_adc1_sample_time();
            m_voltage_sample.sample_idx++;
            m_voltage_samples.push_back(m_voltage_sample);
        }
    }
#endif

//    auto d = q::Clock::now() - start;
//    SILK_INFO("d = {}, {}", d, m_sensor_samples.size());
}


#endif
