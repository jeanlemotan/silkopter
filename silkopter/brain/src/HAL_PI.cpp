#include "BrainStdAfx.h"

#if defined RASPBERRY_PI

#include "HAL_PI.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/stringbuffer.h"

#include "sz_math.hpp"
//#include "sz_hal_pi.hpp"


/////////////////////////////////////////////////////////////////////////////////////
// Sensors
// Choose the sensor and the bus that you have

//#define USE_MPU9250_I2C
//#define USE_MPU9250_SPI

//#define USE_MS5611
//#define USE_ODROIDW_ADC
//#define USE_SRF02

//#define USE_GPS_DETECTOR_UART
//#define USE_GPS_DETECTOR_SPI
//#define USE_GPS_DETECTOR_I2C

/////////////////////////////////////////////////////////////////////////////////////

#include "buses/Bus_I2C_Pi.h"
#include "buses/Bus_SPI_Pi.h"
#include "buses/Bus_UART_Pi.h"

#include "sensors/Raspicam.h"
#include "sensors/MPU9250.h"
#include "sensors/MS5611.h"
#include "sensors/OdroidW_ADC.h"
#include "sensors/SRF02.h"
#include "sensors/GPS_UBLOX.h"

#include "actuators/Motors_PiGPIO.h"
#include "actuators/Gimbal_Servo_PiGPIO.h"


namespace silk
{

static const q::Path k_settings_path("settings.json");

using namespace boost::asio;

struct HAL_Pi::Hardware
{
    struct
    {
        buses::Bus_I2C_Pi_uptr i2c0;
        buses::Bus_I2C_Pi_uptr i2c1;
        buses::Bus_SPI_Pi_uptr spi0;
        buses::Bus_SPI_Pi_uptr spi1;
        buses::Bus_UART_Pi_uptr uart;
    } buses;

    struct
    {
        std::vector<sensors::Raspicam_uptr> raspicam;
        std::vector<sensors::MPU9250_uptr> mpu9250;
        std::vector<sensors::GPS_UBLOX_uptr> ublox;
        std::vector<sensors::MS5611_uptr> ms5611;
        sensors::OdroidW_ADC_uptr odroid_adc;
        std::vector<sensors::SRF02_uptr> srf02;
    } sensors;

    struct
    {
        actuators::Motors_PiGPIO_uptr motors_pigpio;
        actuators::Gimbal_Servo_PiGPIO_uptr gimbal_servo_pigpio;
    } actuators;

    std::vector<buses::IBus_I2C*> i2cs;
    std::vector<buses::IBus_SPI*> spis;
    std::vector<buses::IBus_UART*> uarts;

    std::vector<sensors::ICamera*> cameras;
    std::vector<sensors::IGyroscope*> gyroscopes;
    std::vector<sensors::IAccelerometer*> accelerometers;
    std::vector<sensors::ICompass*> compasses;
    std::vector<sensors::IBarometer*> barometers;
    std::vector<sensors::IThermometer*> thermometers;
    std::vector<sensors::IVoltmeter*> voltmeters;
    std::vector<sensors::IAmmeter*> ammeters;
    std::vector<sensors::IGPS*> gpses;
    std::vector<sensors::ISonar*> sonars;

    actuators::IMotors* motors = nullptr;
    std::vector<actuators::IGimbal*> gimbals;
};

///////////////////////////////////////////////////////////////

HAL_Pi::HAL_Pi()
{
    QLOG_TOPIC("sensors");

    m_hw.reset(new Hardware);

    load_settings();
    save_settings();
}

HAL_Pi::~HAL_Pi()
{
}

auto HAL_Pi::load_settings() -> bool
{
    TIMED_FUNCTION();

    q::data::File_Source fs(k_settings_path);
    if (!fs.is_open())
    {
        QLOGE("Failed to load '{}'", k_settings_path);
        return false;
    }

    auto data = q::data::read_whole_source_as_string<std::string>(fs);
    if (m_settings.Parse(data.c_str()).HasParseError())
    {
        QLOGE("Failed to load '{}': {}:{}", k_settings_path, m_settings.GetParseError(), m_settings.GetErrorOffset());
        return false;
    }

    return true;
}
void HAL_Pi::save_settings() const
{
    TIMED_FUNCTION();

    auto copy = std::make_shared<rapidjson::Document>();
    jsonutil::clone_value(*copy, m_settings, copy->GetAllocator());

    silk::async([=]()
    {
        TIMED_FUNCTION();

        rapidjson::StringBuffer s;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
        copy->Accept(writer);    // Accept() traverses the DOM and generates Handler events.
        q::data::File_Sink fs(k_settings_path);
        if (fs.is_open())
        {
            fs.write(reinterpret_cast<uint8_t const*>(s.GetString()), s.GetSize());
        }
        else
        {
            QLOGE("Cannot open '{}' to save settings.", k_settings_path);
        }
    });

    //autojsoncxx::to_pretty_json_file("sensors_pi.cfg", config);
}

auto HAL_Pi::get_settings(q::Path const& path) -> rapidjson::Value&
{
    auto v = jsonutil::get_or_add_value(m_settings, path, rapidjson::kObjectType, m_settings.GetAllocator());
    if (v == nullptr)
    {
        QLOGE("Wrong json type for settings '{}'. Settings will be lost!!!!", path);
        m_emptyValue.SetObject();
        return m_emptyValue;
    }
    return *v;
}


auto HAL_Pi::get_all_cameras() const        -> std::vector<sensors::ICamera*> const&
{
    return m_hw->cameras;
}
auto HAL_Pi::get_all_accelerometers() const -> std::vector<sensors::IAccelerometer*> const&
{
    return m_hw->accelerometers;
}
auto HAL_Pi::get_all_gyroscopes() const     -> std::vector<sensors::IGyroscope*> const&
{
    return m_hw->gyroscopes;
}
auto HAL_Pi::get_all_compasses() const      -> std::vector<sensors::ICompass*> const&
{
    return m_hw->compasses;
}
auto HAL_Pi::get_all_barometers() const     -> std::vector<sensors::IBarometer*> const&
{
    return m_hw->barometers;
}
auto HAL_Pi::get_all_thermometers() const   -> std::vector<sensors::IThermometer*> const&
{
    return m_hw->thermometers;
}
auto HAL_Pi::get_all_voltmeters() const     -> std::vector<sensors::IVoltmeter*> const&
{
    return m_hw->voltmeters;
}
auto HAL_Pi::get_all_ammeters() const       -> std::vector<sensors::IAmmeter*> const&
{
    return m_hw->ammeters;
}
auto HAL_Pi::get_all_gpses() const          -> std::vector<sensors::IGPS*> const&
{
    return m_hw->gpses;
}
auto HAL_Pi::get_all_sonars() const         -> std::vector<sensors::ISonar*> const&
{
    return m_hw->sonars;
}

auto HAL_Pi::get_motors() const             -> actuators::IMotors* const&
{
    return m_hw->motors;
}
auto HAL_Pi::get_all_gimbals() const        -> std::vector<actuators::IGimbal*> const&
{
    return m_hw->gimbals;
}



auto HAL_Pi::init() -> bool
{
    auto& bus_settings = get_settings(q::Path("hal/busses"));

    auto& sensors_settings = get_settings(q::Path("hal/sensors"));

    return true;
}

void HAL_Pi::shutdown()
{
}

void HAL_Pi::process()
{
}

//auto HAL_PI::init() -> bool
//{
//    QLOG_TOPIC("sensors::init");
//    TIMED_FUNCTION();

//    QASSERT(!m_is_initialized);
//    if (m_is_initialized)
//    {
//        return true;
//    }


//    //autojsoncxx::to_json()



////    Bus_Selector<MPU9250_I2C, MPU9250_SPI, Null_Device> selector;

////    {
////        boost::algorithm::to_lower(cfg.gyroscope.sensor);
////        if (cfg.gyroscope.sensor == "mpu9250")
////        {
////            m_sensors->gyroscope.mpu9250 = selector.init(cfg.gyroscope);
////            if (!m_sensors->gyroscope.mpu9250)
////            {
////                return false;
////            }
////        }
////        else
////        {
////            QLOGE("Unknown sensor: {}", cfg.gyroscope.sensor);
////            return false;
////        }
////    }
////    {
////        boost::algorithm::to_lower(cfg.accelerometer.sensor);
////        if (cfg.accelerometer.sensor == "mpu9250")
////        {
////            m_sensors->accelerometer.mpu9250 = selector.init(cfg.accelerometer);
////            if (!m_sensors->accelerometer.mpu9250)
////            {
////                return false;
////            }
////        }
////        else
////        {
////            QLOGE("Unknown sensor: {}", cfg.gyroscope.sensor);
////            return false;
////        }
////    }


////#if defined USE_MS5611
////    if (!m_sensors->baro.init(m_config.barometer_bus))
////    {
////        return false;
////    }
////#endif

////#if defined USE_MPU9250
////    {
////        typedef MPU9250::Gyroscope_Range G_Range;
////        typedef MPU9250::Accelerometer_Range A_Range;

////        std::vector<G_Range> g_ranges = { G_Range::_250_DPS, G_Range::_500_DPS, G_Range::_1000_DPS, G_Range::_2000_DPS };
////        std::vector<A_Range> a_ranges = { A_Range::_2_G, A_Range::_4_G, A_Range::_8_G, A_Range::_16_G };

////        std::nth_element(g_ranges.begin(), g_ranges.begin(), g_ranges.end(), [&](G_Range a, G_Range b)
////        {
////            return math::abs(static_cast<int>(a) - m_config.gyroscope_range) < math::abs(static_cast<int>(b) - m_config.gyroscope_range);
////        });
////        G_Range g_range = g_ranges.front();

////        std::nth_element(a_ranges.begin(), a_ranges.begin(), a_ranges.end(), [&](A_Range a, A_Range b)
////        {
////            return math::abs(static_cast<int>(a) - m_config.accelerometer_range) < math::abs(static_cast<int>(b) - m_config.accelerometer_range);
////        });
////        A_Range a_range = a_ranges.front();

////        QLOGI("Gyroscope range {} DPS (requested {} DPS)", static_cast<size_t>(g_range), m_config.gyroscope_range);
////        QLOGI("Accelerometer range {}G (requested {}G)", static_cast<size_t>(a_range), m_config.accelerometer_range);

////        if (!m_sensors->mpu.open(m_config.mpu_bus) ||
////            !m_sensors->mpu.init(g_range, a_range))
////        {
////            return false;
////        }
////    }
////#endif

////#if defined USE_ODROIDW_ADC
////    if (!m_sensors->adc.init())
////    {
////        return false;
////    }
////#endif

////#if defined USE_SRF02
////    if (!m_sensors->sonar.init(std::chrono::milliseconds(0)))
////    {
////        return false;
////    }
////#endif

////#if defined USE_GPS_DETECTOR_UART
////    if (!m_sensors->gps_detector.init(m_config.gps_bus, m_config.gps_baud))
////    {
////        return false;
////    }
////#endif

//    m_is_initialized = true;
//    return true;
//}

//void HAL_PI::shutdown()
//{
//    QASSERT(m_is_initialized);
//    m_is_initialized = false;
//}

//void HAL_PI::set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale)
//{
//    m_config.accelerometer.bias = bias;
//    m_config.accelerometer.scale = scale;
//    silk::async([=]()
//    {
//        save_settings(m_config);
//    });
//}
//void HAL_PI::get_accelerometer_calibration_data(math::vec3f &bias, math::vec3f &scale) const
//{
//    bias = m_config.accelerometer.bias;
//    scale = m_config.accelerometer.scale;
//}

//void HAL_PI::set_gyroscope_calibration_data(math::vec3f const& bias)
//{
//    m_config.gyroscope.bias = bias;
//    silk::async([=]()
//    {
//        save_settings(m_config);
//    });
//}
//void HAL_PI::get_gyroscope_calibration_data(math::vec3f &bias) const
//{
//    bias = m_config.gyroscope.bias;
//}

//void HAL_PI::set_compass_calibration_data(math::vec3f const& bias)
//{
//    m_config.compass.bias = bias;
//    silk::async([=]()
//    {
//        save_settings(m_config);
//    });
//}
//void HAL_PI::get_compass_calibration_data(math::vec3f &bias) const
//{
//    bias = m_config.compass.bias;
//}

//void HAL_PI::set_current_calibration_data(float scale)
//{
//    m_config.current.scale = scale;
//    silk::async([=]()
//    {
//        save_settings(m_config);
//    });
//}

//void HAL_PI::get_current_calibration_data(float& scale) const
//{
//    scale = m_config.current.scale;
//}

//void HAL_PI::set_voltage_calibration_data(float scale)
//{
//    m_config.voltage.scale = scale;
//    silk::async([=]()
//    {
//        save_settings(m_config);
//    });
//}
//void HAL_PI::get_voltage_calibration_data(float& scale) const
//{
//    scale = m_config.voltage.scale;
//}




//auto HAL_PI::get_accelerometer_samples() const -> std::vector<sensors::Accelerometer_Sample> const&
//{
//    return m_accelerometer_samples;
//}
//auto HAL_PI::get_gyroscope_samples() const -> std::vector<sensors::Gyroscope_Sample> const&
//{
//    return m_gyroscope_samples;
//}
//auto HAL_PI::get_compass_samples() const -> std::vector<sensors::Compass_Sample> const&
//{
//    return m_compass_samples;
//}
//auto HAL_PI::get_barometer_samples() const -> std::vector<sensors::Barometer_Sample> const&
//{
//    return m_barometer_samples;
//}
//auto HAL_PI::get_sonar_samples() const -> std::vector<sensors::Sonar_Sample> const&
//{
//    return m_sonar_samples;
//}
//auto HAL_PI::get_thermometer_samples() const -> std::vector<sensors::Thermometer_Sample> const&
//{
//    return m_thermometer_samples;
//}
//auto HAL_PI::get_voltmeter_samples() const -> std::vector<sensors::Voltmeter_Sample> const&
//{
//    return m_voltage_samples;
//}
//auto HAL_PI::get_ammeter_samples() const -> std::vector<sensors::Ammeter_Sample> const&
//{
//    return m_ammeter_samples;
//}
//auto HAL_PI::get_gps_samples() const -> std::vector<sensors::GPS_Sample> const&
//{
//    return m_gps_samples;
//}
//auto HAL_PI::get_last_accelerometer_sample() const  -> sensors::Accelerometer_TP_Sample const&
//{
//    return m_last_accelerometer_sample;
//}
//auto HAL_PI::get_last_gyroscope_sample() const      -> sensors::Gyroscope_TP_Sample const&
//{
//    return m_last_gyroscope_sample;
//}
//auto HAL_PI::get_last_compass_sample() const        -> sensors::Compass_TP_Sample const&
//{
//    return m_last_compass_sample;
//}
//auto HAL_PI::get_last_barometer_sample() const      -> sensors::Barometer_TP_Sample const&
//{
//    return m_last_barometer_sample;
//}
//auto HAL_PI::get_last_sonar_sample() const          -> sensors::Sonar_TP_Sample const&
//{
//    return m_last_sonar_sample;
//}
//auto HAL_PI::get_last_thermometer_sample() const    -> sensors::Thermometer_TP_Sample const&
//{
//    return m_last_thermometer_sample;
//}
//auto HAL_PI::get_last_voltage_sample() const        -> sensors::Voltmeter_TP_Sample const&
//{
//    return m_last_voltage_sample;
//}
//auto HAL_PI::get_last_current_sample() const        -> sensors::Ammeter_TP_Sample const&
//{
//    return m_last_current_sample;
//}
//auto HAL_PI::get_last_gps_sample() const            -> sensors::GPS_TP_Sample const&
//{
//    return m_last_gps_sample;
//}

//class Butterworth //10hz
//{
//public:
//    static constexpr size_t NZEROS  = 2;
//    static constexpr size_t NPOLES  = 2;
//    static constexpr float GAIN    = 1.058546241e+03;
//    math::vec3f xv[NZEROS+1], yv[NPOLES+1];
//    math::vec3f process(math::vec3f const& t)
//    {
//        xv[0] = xv[1]; xv[1] = xv[2];
//        xv[2] = t / GAIN;
//        yv[0] = yv[1]; yv[1] = yv[2];
//        yv[2] =   (xv[0] + xv[2]) + 2.f * xv[1]
//                   + ( -0.9149758348f * yv[0]) + (  1.9111970674f * yv[1]);
//        return yv[2];
//    }
//};

//Butterworth bg, ba, bc;

//void HAL_PI::process()
//{
//    QLOG_TOPIC("sensors::process");

//    QASSERT(m_is_initialized);
//    if (!m_is_initialized)
//    {
//        return;
//    }

//    m_accelerometer_samples.clear();
//    m_gyroscope_samples.clear();
//    m_compass_samples.clear();
//    m_barometer_samples.clear();
//    m_sonar_samples.clear();
//    m_thermometer_samples.clear();
//    m_voltage_samples.clear();
//    m_current_samples.clear();
//    m_gps_samples.clear();

//    auto now = q::Clock::now();

//#if defined USE_GPS_DETECTOR
//    m_sensors->gps_detector.process();
//#endif

//#if defined USE_MPU9250
//    m_sensors->mpu.process();
//    {
//        auto const& g_samples = m_sensors->mpu.get_gyroscope_samples();
//        auto const& a_samples = m_sensors->mpu.get_accelerometer_samples();
//        QASSERT(a_samples.size() == g_samples.size());
//        auto const& c_samples = m_sensors->mpu.get_compass_samples();

//        {
//            auto dt = m_sensors->mpu.get_gyroscope_sample_time();
//            m_gyroscope_samples.resize(g_samples.size());
//            for (size_t i = 0; i < g_samples.size(); i++)
//            {
//                auto& sample = m_gyroscope_samples[i];
//                sample.value = bg.process(g_samples[i] - m_config.gyroscope_bias);
//                sample.dt = dt;
//                sample.sample_idx = ++m_last_gyroscope_sample.sample_idx;
//            }
//            if (!m_gyroscope_samples.empty())
//            {
//                static_cast<sensors::Gyroscope_Sample&>(m_last_gyroscope_sample) = m_gyroscope_samples.back();
//                m_last_gyroscope_sample.time_point = now;
//            }
//        }

//        {
//            auto dt = m_sensors->mpu.get_accelerometer_sample_time();
//            m_accelerometer_samples.resize(a_samples.size());
//            for (size_t i = 0; i < a_samples.size(); i++)
//            {
//                auto& sample = m_accelerometer_samples[i];
//                sample.value = ba.process((a_samples[i] - m_config.accelerometer_bias) * m_config.accelerometer_scale);
//                sample.dt = dt;
//                sample.sample_idx = ++m_last_accelerometer_sample.sample_idx;
//            }
//            if (!m_accelerometer_samples.empty())
//            {
//                static_cast<sensors::Accelerometer_Sample&>(m_last_accelerometer_sample) = m_accelerometer_samples.back();
//                m_last_accelerometer_sample.time_point = now;
//            }
//        }

//        {
//            auto dt = m_sensors->mpu.get_compass_sample_time();
//            m_compass_samples.resize(c_samples.size());
//            for (size_t i = 0; i < c_samples.size(); i++)
//            {
//                auto& sample = m_compass_samples[i];
//                sample.value = bc.process(c_samples[i] - m_config.compass_bias);
//                sample.dt = dt;
//                sample.sample_idx = ++m_last_compass_sample.sample_idx;
//            }
//            if (!m_compass_samples.empty())
//            {
//                static_cast<sensors::Compass_Sample&>(m_last_compass_sample) = m_compass_samples.back();
//                m_last_compass_sample.time_point = now;
//            }
//        }
//    }
//#endif

//#if defined USE_ODROIDW_ADC
//    m_sensors->adc.process();
//    {
//        auto data = m_sensors->adc.get_current_data();
//        if (data)
//        {
//            m_last_current_sample.time_point = now;
//            m_last_current_sample.value = data->value * m_config.current_scale;
//            m_last_current_sample.dt = data->dt;
//            m_last_current_sample.sample_idx++;
//            m_current_samples.push_back(m_last_current_sample);
//        }

//        data = m_sensors->adc.get_voltage_data();
//        if (data)
//        {
//            m_last_voltage_sample.time_point = now;
//            m_last_voltage_sample.value = data->value * m_config.voltage_scale;
//            m_last_voltage_sample.dt = data->dt;
//            m_last_voltage_sample.sample_idx++;
//            m_voltage_samples.push_back(m_last_voltage_sample);
//        }
//    }
//#endif

//#if defined USE_SRF02
//    m_sensors->sonar.process();
//    {
//        auto data = m_sensors->sonar.get_distance_data();
//        if (data)
//        {
//            //LOG_INFO("DISTANCE: {}", *val);
//            m_last_sonar_sample.time_point = now;
//            m_last_sonar_sample.value = data->value;
//            m_last_sonar_sample.dt = data->dt;
//            m_last_sonar_sample.sample_idx++;
//            m_sonar_samples.push_back(m_last_sonar_sample);
//        }
//    }
//#endif

//#if defined USE_MS5611
//    //*******************************************************************//
//    //KEEP BARO LAST to avoid i2c noise from talking to other sensors!!!!!!!!!
//    //*******************************************************************//
//    m_sensors->baro.process();
//    {
//        auto b_data = m_sensors->baro.get_barometer_data();
//        if (b_data)
//        {
//            m_last_barometer_sample.time_point = now;
//            m_last_barometer_sample.value = b_data->value;
//            m_last_barometer_sample.sample_idx++;
//            m_last_barometer_sample.dt = b_data->dt;
//            m_barometer_samples.push_back(m_last_barometer_sample);
//        }
//        auto t_data = m_sensors->baro.get_thermometer_data();
//        if (t_data)
//        {
//            m_last_thermometer_sample.time_point = now;
//            m_last_thermometer_sample.value = t_data->value;
//            m_last_thermometer_sample.sample_idx++;
//            m_last_thermometer_sample.dt = t_data->dt;
//            m_thermometer_samples.push_back(m_last_thermometer_sample);
//        }
//    }
//#endif

//    process_gps();

////    auto d = q::Clock::now() - start;
////    LOG_INFO("d = {}, {}", d, m_sensor_samples.size());
//}

//void HAL_PI::process_gps()
//{


//}


}

#endif
