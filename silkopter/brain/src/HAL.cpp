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
#include "processor/LPF.h"
#include "processor/Resampler.h"
#include "processor/LiPo_Battery.h"
#include "processor/Inertial.h"
#include "processor/Comp_AHRS.h"
#include "processor/Comp_Location.h"

//#include "common/node/IAHRS.h"

#include "autojsoncxx/boost_types.hpp"


namespace silk
{

static const q::Path k_settings_path("settings.json");

using namespace boost::asio;

//wrapper to keep all nodes in the same container
struct INode_Wrapper : q::util::Noncopyable
{
    virtual void process() = 0;
};
template<class T> struct Node_Wrapper : public INode_Wrapper
{
    template<class... Args>
    Node_Wrapper(Args&&... args) : node(new T(std::forward<Args>(args)...)) {}
    void process() { node->process(); }
    std::unique_ptr<T> node;
};


struct HAL::Hardware
{
    std::vector<std::unique_ptr<node::bus::IBus>> buses;
    std::vector<std::unique_ptr<INode_Wrapper>> nodes;
};

///////////////////////////////////////////////////////////////

HAL::HAL()
{
    using namespace node;

    QLOG_TOPIC("hal");

    m_hw.reset(new Hardware);

    m_factory.register_node<bus::UART_Linux>(*this, "UART_Linux");
    m_factory.register_node<bus::I2C_Linux>(*this, "I2C_Linux");
    m_factory.register_node<bus::SPI_Linux>(*this, "SPI_Linux");

    m_factory.register_node<source::MPU9250>(*this, "MPU9250");
    m_factory.register_node<source::MS5611>(*this, "MS5611");
    m_factory.register_node<source::SRF02>(*this, "SRF02");
    m_factory.register_node<source::Raspicam>(*this, "Raspicam");
    m_factory.register_node<source::RC5T619>(*this, "RC5T619");
    m_factory.register_node<source::UBLOX>(*this, "UBLOX");

    m_factory.register_node<sink::PIGPIO>(*this, "PIGPIO");

    m_factory.register_node<processor::ADC_Ammeter>(*this, "ADC_Ammeter");
    m_factory.register_node<processor::ADC_Voltmeter>(*this, "ADC_Voltmeter");
    m_factory.register_node<processor::Comp_AHRS>(*this, "Comp_AHRS");
    m_factory.register_node<processor::Comp_Location>(*this, "Comp_Location");
    m_factory.register_node<processor::Inertial>(*this, "Inertial");
    m_factory.register_node<processor::LiPo_Battery>(*this, "LiPo_Battery");
    m_factory.register_node<processor::LPF<stream::IAcceleration>>(*this, "LPF_Acceleration");
    m_factory.register_node<processor::LPF<stream::IAngular_Velocity>>(*this, "LPF_Angular_Velocity");
    m_factory.register_node<processor::LPF<stream::IADC_Value>>(*this, "LPF_ADC_Value");
    m_factory.register_node<processor::LPF<stream::ICurrent>>(*this, "LPF_Current");
    m_factory.register_node<processor::LPF<stream::IVoltage>>(*this, "LPF_Voltage");
    m_factory.register_node<processor::LPF<stream::ILocation>>(*this, "LPF_Location");
    m_factory.register_node<processor::LPF<stream::IDistance>>(*this, "LPF_Distance");
    m_factory.register_node<processor::LPF<stream::IMagnetic_Field>>(*this, "LPF_Magnetic_Field");
    m_factory.register_node<processor::LPF<stream::IPressure>>(*this, "LPF_Pressure");
    m_factory.register_node<processor::LPF<stream::ITemperature>>(*this, "LPF_Temperature");
    m_factory.register_node<processor::Resampler<stream::IAcceleration>>(*this, "Resampler_Acceleration");
    m_factory.register_node<processor::Resampler<stream::IAngular_Velocity>>(*this, "Resampler_Angular_Velocity");
    m_factory.register_node<processor::Resampler<stream::IADC_Value>>(*this, "Resampler_ADC_Value");
    m_factory.register_node<processor::Resampler<stream::ICurrent>>(*this, "Resampler_Current");
    m_factory.register_node<processor::Resampler<stream::IVoltage>>(*this, "Resampler_Voltage");
    m_factory.register_node<processor::Resampler<stream::ILocation>>(*this, "Resampler_Location");
    m_factory.register_node<processor::Resampler<stream::IDistance>>(*this, "Resampler_Distance");
    m_factory.register_node<processor::Resampler<stream::IMagnetic_Field>>(*this, "Resampler_Magnetic_Field");
    m_factory.register_node<processor::Resampler<stream::IPressure>>(*this, "Resampler_Pressure");
    m_factory.register_node<processor::Resampler<stream::ITemperature>>(*this, "Resampler_Temperature");
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
        get_settings(q::Path("hal/nodes"));
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

template<class T>
void write_gnu_plot(std::string const& name, std::vector<T> const& samples)
{
    q::data::File_Sink fs((q::Path(name)));
    std::string header("#x y\n");
    fs.write((uint8_t const*)header.c_str(), header.size());

    for(size_t i = 0; i < samples.size(); i++)
    {
        auto l = q::util::format2<std::string>("{} {.8}\n", i, samples[i]);
        fs.write((uint8_t const*)l.c_str(), l.size());
    }
}

auto HAL::init() -> bool
{
    using namespace silk::node;

    get_sinks().remove_all();
    get_processors().remove_all();
    get_sources().remove_all();
    get_streams().remove_all();

    m_hw->nodes.clear();
    m_hw->buses.clear();

    if (!load_settings())
    {
        return false;
    }

//    struct Stream : public stream::IADC_Value
//    {
//        auto get_samples() const -> std::vector<Sample> const& { return samples; }
//        auto get_rate() const -> uint32_t { return 1000; }
//        auto get_name() const -> std::string const& { return ""; }
//        std::vector<Sample> samples;
//    } stream;

//    {
//        const size_t elements = 1000;
//        const float noise = 0.3f;
//        std::vector<std::pair<float, float>> freq =
//        {{
//             { 10.f, 1.f },
//             { 70.f, 1.f/7.f },
//             { 130.f, 1.f/5.f }
//         }};
//        stream.samples.resize(elements);
//        std::uniform_real_distribution<float> distribution(-noise, noise); //Values between 0 and 2
//        std::mt19937 engine; // Mersenne twister MT19937
//        auto generator = std::bind(distribution, engine);
//        for (size_t i = 0; i < stream.samples.size(); i++)
//        {
//            float a = float(i) * math::anglef::_2pi / float(stream.get_rate());
//            float output = 0.f;
//            for (auto& f: freq)
//            {
//                output += math::sin(a * f.first) * f.second;
//            }
//            stream.samples[i].value = output + generator();
//            stream.samples[i].dt = std::chrono::microseconds(1000000 / stream.get_rate());
//        }
//    }

//    write_gnu_plot("in.dat", stream.samples);

//    processor::LPF<Stream> lpf(*this);
//    {
//        processor::LPF<Stream>::Init_Params params;
//        params.input_stream = &stream;
//        params.cutoff_frequency = 30;
//        params.poles = 3;
//        lpf.init(params);
//    }
//    lpf.process();
//    write_gnu_plot("out.dat", lpf.get_output_stream(0).get_samples());

//    processor::Resampler<Stream> resampler(*this);
//    {
//        processor::Resampler<Stream>::Init_Params params;
//        params.output_rate = 100;
//        params.input_stream = &stream;
//        resampler.init(params);
//    }
//    resampler.process();

//    write_gnu_plot("rsout.dat", resampler.get_output_stream(0).get_samples());


    if (!create_nodes<bus::IBus>(get_settings(q::Path("hal/buses"))) ||
        !create_nodes<source::ISource>(get_settings(q::Path("hal/sources"))) ||
        !create_nodes<processor::IProcessor>(get_settings(q::Path("hal/processors"))) ||
        !create_nodes<sink::ISink>(get_settings(q::Path("hal/sinks"))))
    {
        return false;
    }

    return true;
}

void HAL::shutdown()
{
}

template<class Base>
auto HAL::create_nodes(rapidjson::Value& json) -> bool
{
    if (!json.IsObject())
    {
        QLOGE("Wrong json type: {}", json.GetType());
        return false;
    }
    auto it = json.MemberBegin();
    for (; it != json.MemberEnd(); ++it)
    {
        std::string type(it->name.GetString());
        auto* init_params = jsonutil::find_value(it->value, std::string("init_params"));
        auto* config = jsonutil::find_value(it->value, std::string("config"));
        if (!init_params || !config)
        {
            QLOGE("Bus {} is missing the {}", init_params ? "config" : "init_params");
            return false;
        }
        if (!create_node<Base>(type, *init_params, *config))
        {
            return false;
        }
    }
    return true;
}
template<class Base>
auto HAL::create_node(std::string const& type, rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool
{
    auto node = m_factory.create_node<Base>(*this, type);
    if (!node)
    {
        QLOGE("Cannot create node of type '{}'", type);
    }
    if (!node->init(init_params) ||
        !node->set_config(config))
    {
        return false;
    }
    return true;
}

//static std::vector<double> s_samples;
//static std::vector<double> s_samples_lpf;

void HAL::process()
{
    for (auto& n: m_hw->nodes)
    {
        n->process();
    }

//    auto* stream = get_streams().find_by_name<node::stream::ILocation>("gps0/stream");
//    auto* stream_lpf = get_streams().find_by_name<node::stream::ILocation>("gps0_resampler/stream");
//    for (auto& s: stream->get_samples())
//    {
//        QLOGI("{.8}", s.value.latitude);
//        s_samples.push_back(s.value.latitude);
//    }
//    for (auto& s: stream_lpf->get_samples())
//    {
//        QLOGI("\t\t{.8}", s.value.latitude);
//        s_samples_lpf.push_back(s.value.latitude);
//    }

//    if (s_samples.size() == 50)
//    {
//        write_gnu_plot("out.dat", s_samples);
//        write_gnu_plot("rsout.dat", s_samples_lpf);
//    }
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

