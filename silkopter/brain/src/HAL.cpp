#include "BrainStdAfx.h"

#include "HAL.h"
#include "utils/Json_Helpers.h"
#include "utils/Timed_Scope.h"

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/stringbuffer.h"



/////////////////////////////////////////////////////////////////////////////////////

#include "bus/I2C_Linux.h"
#include "bus/SPI_Linux.h"
#include "bus/UART_Linux.h"

#include "source/Raspicam.h"
#include "source/MPU9250.h"
#include "source/MS5611.h"
#include "source/RC5T619.h"
#include "source/SRF02.h"
#include "source/UBLOX.h"
#include "sink/PIGPIO.h"

#include "processor/ADC_Voltmeter.h"
#include "processor/ADC_Ammeter.h"
#include "processor/LPF_Butterworth.h"

//#include "common/node/IAHRS.h"

#include "autojsoncxx/boost_types.hpp"
#include "sz_math.hpp"
#include "sz_hal_buses.hpp"
#include "sz_hal_devices.hpp"


namespace silk
{

static const q::Path k_settings_path("settings.json");

using namespace boost::asio;

//wrapper to keep all devices in the same container
struct IDevice_Wrapper : q::util::Noncopyable
{
    virtual void process() = 0;
};
template<class T> struct Device_Wrapper : public IDevice_Wrapper
{
    template<class... Args>
    Device_Wrapper(Args&&... args) : device(new T(std::forward<Args>(args)...)) {}
    void process() { device->process(); }
    std::unique_ptr<T> device;
};


struct HAL::Hardware
{
    std::vector<std::unique_ptr<node::bus::IBus>> buses;
    std::vector<std::unique_ptr<IDevice_Wrapper>> devices;
};

///////////////////////////////////////////////////////////////

HAL::HAL()
{
    QLOG_TOPIC("hal");

    m_hw.reset(new Hardware);

    load_settings();
    save_settings();
}

HAL::~HAL()
{
}

auto HAL::load_settings() -> bool
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
void HAL::save_settings()
{
    TIMED_FUNCTION();

    if (!m_settings.IsObject())
    {
        m_settings.SetObject();
        get_settings(q::Path("hal/buses"));
        get_settings(q::Path("hal/devices"));
    }

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

auto HAL::get_settings(q::Path const& path) -> rapidjson::Value&
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

auto HAL::get_buses()    -> Registry<node::bus::IBus>&
{
    return m_buses;
}
auto HAL::get_sources()  -> Registry<node::source::ISource>&
{
    return m_sources;
}
auto HAL::get_sinks()    -> Registry<node::sink::ISink>&
{
    return m_sinks;
}
auto HAL::get_processors()  -> Registry<node::processor::IProcessor>&
{
    return m_processors;
}
auto HAL::get_streams()  -> Registry<node::stream::IStream>&
{
    return m_streams;
}


auto HAL::init() -> bool
{
    using namespace silk::node;

    {
        auto& settings = get_settings(q::Path("hal/buses"));

        autojsoncxx::error::ErrorStack result;
        auto it = settings.MemberBegin();
        for (; it != settings.MemberEnd(); ++it)
        {
            std::string type(it->name.GetString());
            if (type == "I2C_Linux")
            {
                sz::I2C_Linux sz;
                if (!autojsoncxx::from_value(sz, it->value, result))
                {
                    break;
                }
                auto bus = std::make_unique<bus::I2C_Linux>(*this);
                bus::I2C_Linux::Init_Params params;
                params.name = sz.name;
                params.dev = sz.dev;
                if (!bus->init(params))
                {
                    return false;
                }
                m_hw->buses.push_back(std::move(bus));
            }
            else if (type == "SPI_Linux")
            {
                sz::SPI_Linux sz;
                if (!autojsoncxx::from_value(sz, it->value, result))
                {
                    break;
                }
                auto bus = std::make_unique<bus::SPI_Linux>(*this);
                bus::SPI_Linux::Init_Params params;
                params.name = sz.name;
                params.dev = sz.dev;
                if (!bus->init(params))
                {
                    return false;
                }
                m_hw->buses.push_back(std::move(bus));
            }
            else if (type == "UART_Linux")
            {
                sz::UART_Linux sz;
                if (!autojsoncxx::from_value(sz, it->value, result))
                {
                    break;
                }
                auto bus = std::make_unique<bus::UART_Linux>(*this);
                bus::UART_Linux::Init_Params params;
                params.name = sz.name;
                params.dev = sz.dev;
                if (!bus->init(params))
                {
                    return false;
                }
                m_hw->buses.push_back(std::move(bus));
            }
        }
        if (!result.empty())
        {
            std::ostringstream ss;
            ss << result;
            QLOGE("Failed to parse bus {}: {}", it->name.GetString(), ss.str());
            return false;
        }
    }

    {
        auto& settings = get_settings(q::Path("hal/devices"));

        autojsoncxx::error::ErrorStack result;
        auto it = settings.MemberBegin();
        for (; it != settings.MemberEnd(); ++it)
        {
            std::string type(it->name.GetString());
            if (type == "PIGPIO")
            {
                sink::PIGPIO::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }

                auto wrapper = std::make_unique<Device_Wrapper<sink::PIGPIO>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
            else if (type == "MPU9250")
            {
                source::MPU9250::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }
                auto wrapper = std::make_unique<Device_Wrapper<source::MPU9250>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
            else if (type == "MS5611")
            {
                source::MS5611::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }
                auto wrapper = std::make_unique<Device_Wrapper<source::MS5611>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
            else if (type == "Raspicam")
            {
                source::Raspicam::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }
                auto wrapper = std::make_unique<Device_Wrapper<source::Raspicam>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
            else if (type == "SRF02")
            {
                source::SRF02::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }
                auto wrapper = std::make_unique<Device_Wrapper<source::SRF02>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
            else if (type == "UBLOX")
            {
                source::UBLOX::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }
                auto wrapper = std::make_unique<Device_Wrapper<source::UBLOX>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
            else if (type == "RC5T619")
            {
                source::RC5T619::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }
                auto wrapper = std::make_unique<Device_Wrapper<source::RC5T619>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
            else if (type == "ADC_Ammeter")
            {
                processor::ADC_Ammeter::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }
                auto wrapper = std::make_unique<Device_Wrapper<processor::ADC_Ammeter>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
            else if (type == "ADC_Voltmeter")
            {
                processor::ADC_Voltmeter::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }
                auto wrapper = std::make_unique<Device_Wrapper<processor::ADC_Voltmeter>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
            else if (type == "Acceleration_LPF_Butterworth")
            {
                typedef processor::LPF_Butterworth<stream::IAcceleration> Processor;
                Processor::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }
                auto wrapper = std::make_unique<Device_Wrapper<Processor>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
            else if (type == "Angular_Velocity_LPF_Butterworth")
            {
                typedef processor::LPF_Butterworth<stream::IAngular_Velocity> Processor;
                Processor::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }
                auto wrapper = std::make_unique<Device_Wrapper<Processor>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
            else if (type == "Magnetic_Field_LPF_Butterworth")
            {
                typedef processor::LPF_Butterworth<stream::IMagnetic_Field> Processor;
                Processor::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }
                auto wrapper = std::make_unique<Device_Wrapper<Processor>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
            else if (type == "Voltage_LPF_Butterworth")
            {
                typedef processor::LPF_Butterworth<stream::IVoltage> Processor;
                Processor::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }
                auto wrapper = std::make_unique<Device_Wrapper<Processor>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
            else if (type == "Current_LPF_Butterworth")
            {
                typedef processor::LPF_Butterworth<stream::ICurrent> Processor;
                Processor::Init_Params params;
                if (!autojsoncxx::from_value(params, it->value, result))
                {
                    break;
                }
                auto wrapper = std::make_unique<Device_Wrapper<Processor>>(*this);
                if (!wrapper->device->init(params))
                {
                    return false;
                }
                m_hw->devices.push_back(std::move(wrapper));
            }
        }

        if (!result.empty())
        {
            std::ostringstream ss;
            ss << result;
            QLOGE("Failed to parse device {}: {}", it->name.GetString(), ss.str());
            return false;
        }
    }

    return true;
}

void HAL::shutdown()
{
}

void HAL::process()
{
}

//auto HAL::init() -> bool
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
////            m_sensordev.gyroscope.mpu9250 = selector.init(cfg.gyroscope);
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

//void HAL::shutdown()
//{
//    QASSERT(m_is_initialized);
//    m_is_initialized = false;
//}

//void HAL::set_accelerometer_calibration_data(math::vec3f const& bias, math::vec3f const& scale)
//{
//    m_config.accelerometer.bias = bias;
//    m_config.accelerometer.scale = scale;
//    silk::async([=]()
//    {
//        save_settings(m_config);
//    });
//}
//void HAL::get_accelerometer_calibration_data(math::vec3f &bias, math::vec3f &scale) const
//{
//    bias = m_config.accelerometer.bias;
//    scale = m_config.accelerometer.scale;
//}

//void HAL::set_gyroscope_calibration_data(math::vec3f const& bias)
//{
//    m_config.gyroscope.bias = bias;
//    silk::async([=]()
//    {
//        save_settings(m_config);
//    });
//}
//void HAL::get_gyroscope_calibration_data(math::vec3f &bias) const
//{
//    bias = m_config.gyroscope.bias;
//}

//void HAL::set_compass_calibration_data(math::vec3f const& bias)
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

