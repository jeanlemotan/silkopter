#include "BrainStdAfx.h"

#if defined RASPBERRY_PI

#include "HAL_Sensors_HW.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"

#include "sz_math.hpp"
#include "sz_hal_sensors_hw_config.hpp"



/////////////////////////////////////////////////////////////////////////////////////
// Sensors
// Choose the sensor and the bus that you have

#define USE_MPU9250_I2C
//#define USE_MPU9250_SPI

#define USE_MS5611
#define USE_ODROIDW_ADC
#define USE_SRF02

#define USE_GPS_DETECTOR_UART
//#define USE_GPS_DETECTOR_SPI
//#define USE_GPS_DETECTOR_I2C

/////////////////////////////////////////////////////////////////////////////////////


#if defined USE_MPU9250_I2C
#   define USE_MPU9250
#   include "sensors/MPU9250_I2C.h"
#elif defined USE_MPU9250_SPI
#   define USE_MPU9250
#   include "sensors/MPU9250_SPI.h"
#else
#   error No IMU selected
#endif

#if defined USE_MS5611
#   include "sensors/MS5611.h"
#else
#   error No Barometer selected
#endif

#if defined USE_ODROIDW_ADC
#   include "sensors/OdroidW_ADC.h"
#else
#   error No ADC Selected
#endif

#if defined USE_SRF02
#   include "sensors/SRF02.h"
#else
#   error No Sonar Selected
#endif

#if defined USE_GPS_DETECTOR_UART
#   define USE_GPS_DETECTOR
#   include "sensors/GPS_Detector_UART.h"
#elif defined USE_GPS_DETECTOR_SPI
#   include "sensors/GPS_Detector_SPI.h"
#elif defined USE_GPS_DETECTOR_I2C
#   include "sensors/GPS_Detector_I2C.h"
#else
#   error No GPS Selected
#endif


using namespace silk;
using namespace boost::asio;

struct HAL_Sensors_HW::Sensors
{
#if defined USE_MPU9250_I2C
    MPU9250_I2C mpu;
#elif defined USE_MPU9250_SPI
    MPU9250_SPI mpu;
#endif

#if defined USE_MS5611
    MS5611 baro;
#endif

#if defined USE_ODROIDW_ADC
    OdroidW_ADC adc;
#endif

#if defined USE_SRF02
    SRF02 sonar;
#endif

#if defined USE_GPS_DETECTOR_UART
    GPS_Detector_UART gps_detector;
#elif defined USE_GPS_DETECTOR_SPI
    GPS_Detector_SPI gps_detector;
#elif defined USE_GPS_DETECTOR_I2C
    GPS_Detector_I2C gps_detector;
#endif
};

///////////////////////////////////////////////////////////////

HAL_Sensors_HW::HAL_Sensors_HW()
{
    QLOG_TOPIC("sensors");

    m_sensors.reset(new Sensors);

    m_config.barometer_i2c_device = "/dev/i2c-0";
    m_config.mpu_i2c_device = "/dev/i2c-0";

    load_settings();
    save_settings(m_config);
}

HAL_Sensors_HW::~HAL_Sensors_HW()
{
}

auto HAL_Sensors_HW::load_settings() -> bool
{
    TIMED_FUNCTION();

    autojsoncxx::ParsingResult result;
    Config cfg;
    if (!autojsoncxx::from_json_file("sensors_pi.cfg", cfg, result))
    {
        QLOGW("Failed to load sensors_pi.cfg: {}", result.description());
        return false;
    }

    m_config = cfg;
    if (m_config.barometer_i2c_device.empty())
    {
        m_config.barometer_i2c_device = "/dev/i2c-0";
    }
    if (m_config.mpu_i2c_device.empty())
    {
        m_config.mpu_i2c_device = "/dev/i2c-0";
    }
    if (m_config.gps_device.empty())
    {
        m_config.gps_device = "/dev/ttyAMA0";
    }

    return true;
}
void HAL_Sensors_HW::save_settings(Config const& config)
{
    TIMED_FUNCTION();

    autojsoncxx::to_pretty_json_file("sensors_pi.cfg", config);
}

auto HAL_Sensors_HW::init() -> bool
{
    QLOG_TOPIC("sensors::init");
    TIMED_FUNCTION();

    QASSERT(!m_is_initialized);
    if (m_is_initialized)
    {
        return true;
    }

#if defined USE_MS5611
    if (!m_sensors->baro.init(m_config.barometer_i2c_device))
    {
        return false;
    }
#endif

#if defined USE_MPU9250
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

        QLOGI("Gyroscope range {} DPS (requested {} DPS)", static_cast<size_t>(g_range), m_config.gyroscope_range);
        QLOGI("Accelerometer range {}G (requested {}G)", static_cast<size_t>(a_range), m_config.accelerometer_range);

        if (!m_sensors->mpu.open(m_config.mpu_i2c_device) ||
            !m_sensors->mpu.init(g_range, a_range))
        {
            return false;
        }
    }
#endif

#if defined USE_ODROIDW_ADC
    if (!m_sensors->adc.init())
    {
        return false;
    }
#endif

#if defined USE_SRF02
    if (!m_sensors->sonar.init(std::chrono::milliseconds(0)))
    {
        return false;
    }
#endif

#if defined USE_GPS_DETECTOR_UART
    if (!m_sensors->gps_detector.init(m_config.gps_device, m_config.gps_baud))
    {
        return false;
    }
#endif

    m_is_initialized = true;
    return true;
}

void HAL_Sensors_HW::shutdown()
{
    QASSERT(m_is_initialized);
    m_is_initialized = false;
}

void HAL_Sensors_HW::set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale)
{
    m_config.accelerometer_bias = bias;
    m_config.accelerometer_scale = scale;
    silk::async([=]()
    {
        save_settings(m_config);
    });
}
void HAL_Sensors_HW::get_accelerometer_calibration_data(math::vec3f &bias, math::vec3f &scale) const
{
    bias = m_config.accelerometer_bias;
    scale = m_config.accelerometer_scale;
}

void HAL_Sensors_HW::set_gyroscope_calibration_data(math::vec3f const& bias)
{
    m_config.gyroscope_bias = bias;
    silk::async([=]()
    {
        save_settings(m_config);
    });
}
void HAL_Sensors_HW::get_gyroscope_calibration_data(math::vec3f &bias) const
{
    bias = m_config.gyroscope_bias;
}

void HAL_Sensors_HW::set_compass_calibration_data(math::vec3f const& bias)
{
    m_config.compass_bias = bias;
    silk::async([=]()
    {
        save_settings(m_config);
    });
}
void HAL_Sensors_HW::get_compass_calibration_data(math::vec3f &bias) const
{
    bias = m_config.compass_bias;
}

void HAL_Sensors_HW::set_current_calibration_data(float scale)
{
    m_config.current_scale = scale;
    silk::async([=]()
    {
        save_settings(m_config);
    });
}

void HAL_Sensors_HW::get_current_calibration_data(float& scale) const
{
    scale = m_config.current_scale;
}

void HAL_Sensors_HW::set_voltage_calibration_data(float scale)
{
    m_config.voltage_scale = scale;
    silk::async([=]()
    {
        save_settings(m_config);
    });
}
void HAL_Sensors_HW::get_voltage_calibration_data(float& scale) const
{
    scale = m_config.voltage_scale;
}




auto HAL_Sensors_HW::get_accelerometer_samples() const -> std::vector<sensors::Accelerometer_Sample> const&
{
    return m_accelerometer_samples;
}
auto HAL_Sensors_HW::get_gyroscope_samples() const -> std::vector<sensors::Gyroscope_Sample> const&
{
    return m_gyroscope_samples;
}
auto HAL_Sensors_HW::get_compass_samples() const -> std::vector<sensors::Compass_Sample> const&
{
    return m_compass_samples;
}
auto HAL_Sensors_HW::get_barometer_samples() const -> std::vector<sensors::Barometer_Sample> const&
{
    return m_barometer_samples;
}
auto HAL_Sensors_HW::get_sonar_samples() const -> std::vector<sensors::Sonar_Sample> const&
{
    return m_sonar_samples;
}
auto HAL_Sensors_HW::get_thermometer_samples() const -> std::vector<sensors::Thermometer_Sample> const&
{
    return m_thermometer_samples;
}
auto HAL_Sensors_HW::get_voltage_samples() const -> std::vector<sensors::Voltage_Sample> const&
{
    return m_voltage_samples;
}
auto HAL_Sensors_HW::get_current_samples() const -> std::vector<sensors::Current_Sample> const&
{
    return m_current_samples;
}
auto HAL_Sensors_HW::get_gps_samples() const -> std::vector<sensors::GPS_Sample> const&
{
    return m_gps_samples;
}
auto HAL_Sensors_HW::get_last_accelerometer_sample() const  -> sensors::Accelerometer_TP_Sample const&
{
    return m_last_accelerometer_sample;
}
auto HAL_Sensors_HW::get_last_gyroscope_sample() const      -> sensors::Gyroscope_TP_Sample const&
{
    return m_last_gyroscope_sample;
}
auto HAL_Sensors_HW::get_last_compass_sample() const        -> sensors::Compass_TP_Sample const&
{
    return m_last_compass_sample;
}
auto HAL_Sensors_HW::get_last_barometer_sample() const      -> sensors::Barometer_TP_Sample const&
{
    return m_last_barometer_sample;
}
auto HAL_Sensors_HW::get_last_sonar_sample() const          -> sensors::Sonar_TP_Sample const&
{
    return m_last_sonar_sample;
}
auto HAL_Sensors_HW::get_last_thermometer_sample() const    -> sensors::Thermometer_TP_Sample const&
{
    return m_last_thermometer_sample;
}
auto HAL_Sensors_HW::get_last_voltage_sample() const        -> sensors::Voltage_TP_Sample const&
{
    return m_last_voltage_sample;
}
auto HAL_Sensors_HW::get_last_current_sample() const        -> sensors::Current_TP_Sample const&
{
    return m_last_current_sample;
}
auto HAL_Sensors_HW::get_last_gps_sample() const            -> sensors::GPS_TP_Sample const&
{
    return m_last_gps_sample;
}

class Butterworth //10hz
{
public:
    static constexpr size_t NZEROS  = 2;
    static constexpr size_t NPOLES  = 2;
    static constexpr float GAIN    = 1.058546241e+03;
    math::vec3f xv[NZEROS+1], yv[NPOLES+1];
    math::vec3f process(math::vec3f const& t)
    {
        xv[0] = xv[1]; xv[1] = xv[2];
        xv[2] = t / GAIN;
        yv[0] = yv[1]; yv[1] = yv[2];
        yv[2] =   (xv[0] + xv[2]) + 2.f * xv[1]
                   + ( -0.9149758348f * yv[0]) + (  1.9111970674f * yv[1]);
        return yv[2];
    }
};

Butterworth bg, ba, bc;

void HAL_Sensors_HW::process()
{
    QLOG_TOPIC("sensors::process");

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

    auto now = q::Clock::now();

#if defined USE_GPS_DETECTOR
    m_sensors->gps_detector.process();
#endif

#if defined USE_MPU9250
    m_sensors->mpu.process();
    {
        auto const& g_samples = m_sensors->mpu.get_gyroscope_samples();
        auto const& a_samples = m_sensors->mpu.get_accelerometer_samples();
        QASSERT(a_samples.size() == g_samples.size());
        auto const& c_samples = m_sensors->mpu.get_compass_samples();

        {
            auto dt = m_sensors->mpu.get_gyroscope_sample_time();
            m_gyroscope_samples.resize(g_samples.size());
            for (size_t i = 0; i < g_samples.size(); i++)
            {
                auto& sample = m_gyroscope_samples[i];
                sample.value = bg.process(g_samples[i] - m_config.gyroscope_bias);
                sample.dt = dt;
                sample.sample_idx = ++m_last_gyroscope_sample.sample_idx;
            }
            if (!m_gyroscope_samples.empty())
            {
                static_cast<sensors::Gyroscope_Sample&>(m_last_gyroscope_sample) = m_gyroscope_samples.back();
                m_last_gyroscope_sample.time_point = now;
            }
        }

        {
            auto dt = m_sensors->mpu.get_accelerometer_sample_time();
            m_accelerometer_samples.resize(a_samples.size());
            for (size_t i = 0; i < a_samples.size(); i++)
            {
                auto& sample = m_accelerometer_samples[i];
                sample.value = ba.process((a_samples[i] - m_config.accelerometer_bias) * m_config.accelerometer_scale);
                sample.dt = dt;
                sample.sample_idx = ++m_last_accelerometer_sample.sample_idx;
            }
            if (!m_accelerometer_samples.empty())
            {
                static_cast<sensors::Accelerometer_Sample&>(m_last_accelerometer_sample) = m_accelerometer_samples.back();
                m_last_accelerometer_sample.time_point = now;
            }
        }

        {
            auto dt = m_sensors->mpu.get_compass_sample_time();
            m_compass_samples.resize(c_samples.size());
            for (size_t i = 0; i < c_samples.size(); i++)
            {
                auto& sample = m_compass_samples[i];
                sample.value = bc.process(c_samples[i] - m_config.compass_bias);
                sample.dt = dt;
                sample.sample_idx = ++m_last_compass_sample.sample_idx;
            }
            if (!m_compass_samples.empty())
            {
                static_cast<sensors::Compass_Sample&>(m_last_compass_sample) = m_compass_samples.back();
                m_last_compass_sample.time_point = now;
            }
        }
    }
#endif

#if defined USE_ODROIDW_ADC
    m_sensors->adc.process();
    {
        auto data = m_sensors->adc.get_current_data();
        if (data)
        {
            m_last_current_sample.time_point = now;
            m_last_current_sample.value = data->value * m_config.current_scale;
            m_last_current_sample.dt = data->dt;
            m_last_current_sample.sample_idx++;
            m_current_samples.push_back(m_last_current_sample);
        }

        data = m_sensors->adc.get_voltage_data();
        if (data)
        {
            m_last_voltage_sample.time_point = now;
            m_last_voltage_sample.value = data->value * m_config.voltage_scale;
            m_last_voltage_sample.dt = data->dt;
            m_last_voltage_sample.sample_idx++;
            m_voltage_samples.push_back(m_last_voltage_sample);
        }
    }
#endif

#if defined USE_SRF02
    m_sensors->sonar.process();
    {
        auto data = m_sensors->sonar.get_distance_data();
        if (data)
        {
            //LOG_INFO("DISTANCE: {}", *val);
            m_last_sonar_sample.time_point = now;
            m_last_sonar_sample.value = data->value;
            m_last_sonar_sample.dt = data->dt;
            m_last_sonar_sample.sample_idx++;
            m_sonar_samples.push_back(m_last_sonar_sample);
        }
    }
#endif

#if defined USE_MS5611
    //*******************************************************************//
    //KEEP BARO LAST to avoid i2c noise from talking to other sensors!!!!!!!!!
    //*******************************************************************//
    m_sensors->baro.process();
    {
        auto b_data = m_sensors->baro.get_barometer_data();
        if (b_data)
        {
            m_last_barometer_sample.time_point = now;
            m_last_barometer_sample.value = b_data->value;
            m_last_barometer_sample.sample_idx++;
            m_last_barometer_sample.dt = b_data->dt;
            m_barometer_samples.push_back(m_last_barometer_sample);
        }
        auto t_data = m_sensors->baro.get_thermometer_data();
        if (t_data)
        {
            m_last_thermometer_sample.time_point = now;
            m_last_thermometer_sample.value = t_data->value;
            m_last_thermometer_sample.sample_idx++;
            m_last_thermometer_sample.dt = t_data->dt;
            m_thermometer_samples.push_back(m_last_thermometer_sample);
        }
    }
#endif

    process_gps();

//    auto d = q::Clock::now() - start;
//    LOG_INFO("d = {}, {}", d, m_sensor_samples.size());
}

void HAL_Sensors_HW::process_gps()
{


}


#endif
