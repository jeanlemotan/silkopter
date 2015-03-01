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
    : m_bus_factory(*this)
    , m_source_factory(*this)
    , m_sink_factory(*this)
    , m_processor_factory(*this)
{
    using namespace node;

    QLOG_TOPIC("hal");

    m_hw.reset(new Hardware);

    m_bus_factory.register_node<bus::UART_Linux>("UART_Linux");
    m_bus_factory.register_node<bus::I2C_Linux>("I2C_Linux");
    m_bus_factory.register_node<bus::SPI_Linux>("SPI_Linux");

    m_source_factory.register_node<source::MPU9250>("MPU9250");
    m_source_factory.register_node<source::MS5611>("MS5611");
    m_source_factory.register_node<source::SRF02>("SRF02");
    m_source_factory.register_node<source::Raspicam>("Raspicam");
    m_source_factory.register_node<source::RC5T619>("RC5T619");
    m_source_factory.register_node<source::UBLOX>("UBLOX");

    m_sink_factory.register_node<sink::PIGPIO>("PIGPIO");

    m_processor_factory.register_node<processor::ADC_Ammeter>("ADC_Ammeter");
    m_processor_factory.register_node<processor::ADC_Voltmeter>("ADC_Voltmeter");
    m_processor_factory.register_node<processor::Comp_AHRS>("Comp_AHRS");
    m_processor_factory.register_node<processor::Comp_Location>("Comp_Location");
    m_processor_factory.register_node<processor::Inertial>("Inertial");
    m_processor_factory.register_node<processor::LiPo_Battery>("LiPo_Battery");
    m_processor_factory.register_node<processor::LPF<stream::IAcceleration>>("Acceleration_LPF");
    m_processor_factory.register_node<processor::LPF<stream::IAngular_Velocity>>("Angular_Velocity_LPF");
    m_processor_factory.register_node<processor::LPF<stream::IADC_Value>>("ADC_Value_LPF");
    m_processor_factory.register_node<processor::LPF<stream::ICurrent>>("Current_LPF");
    m_processor_factory.register_node<processor::LPF<stream::IVoltage>>("Voltage_LPF");
    m_processor_factory.register_node<processor::LPF<stream::ILocation>>("Location_LPF");
    m_processor_factory.register_node<processor::LPF<stream::IDistance>>("Distance_LPF");
    m_processor_factory.register_node<processor::LPF<stream::IMagnetic_Field>>("Magnetic_Field_LPF");
    m_processor_factory.register_node<processor::LPF<stream::IPressure>>("Pressure_LPF");
    m_processor_factory.register_node<processor::LPF<stream::ITemperature>>("Temperature_LPF");
    m_processor_factory.register_node<processor::Resampler<stream::IAcceleration>>("Acceleration_Resampler");
    m_processor_factory.register_node<processor::Resampler<stream::IAngular_Velocity>>("Angular_Velocity_Resampler");
    m_processor_factory.register_node<processor::Resampler<stream::IADC_Value>>("ADC_Value_Resampler");
    m_processor_factory.register_node<processor::Resampler<stream::ICurrent>>("Current_Resampler");
    m_processor_factory.register_node<processor::Resampler<stream::IVoltage>>("Voltage_Resampler");
    m_processor_factory.register_node<processor::Resampler<stream::ILocation>>("Location_Resampler");
    m_processor_factory.register_node<processor::Resampler<stream::IDistance>>("Distance_Resampler");
    m_processor_factory.register_node<processor::Resampler<stream::IMagnetic_Field>>("Magnetic_Field_Resampler");
    m_processor_factory.register_node<processor::Resampler<stream::IPressure>>("Pressure_Resampler");
    m_processor_factory.register_node<processor::Resampler<stream::ITemperature>>("Temperature_Resampler");
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

auto HAL::get_bus_factory()    -> Factory<node::bus::IBus>&
{
    return m_bus_factory;
}
auto HAL::get_source_factory()  -> Factory<node::source::ISource>&
{
    return m_source_factory;
}
auto HAL::get_sink_factory()    -> Factory<node::sink::ISink>&
{
    return m_sink_factory;
}
auto HAL::get_processor_factory()  -> Factory<node::processor::IProcessor>&
{
    return m_processor_factory;
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

    m_bus_factory.create_all();

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
            QLOGE("Bus {} is missing the {}", type, init_params ? "config" : "init_params");
            return false;
        }
        if (!create_node<Base>(type, *init_params, *config))
        {
            QLOGE("Failed to create node of type '{}'", type);
            return false;
        }
    }
    return true;
}
template<>
auto HAL::create_node<node::bus::IBus>(std::string const& type, rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool
{
    auto node = m_bus_factory.create_node(type);
    return node && node->init(init_params, config);
}
template<>
auto HAL::create_node<node::source::ISource>(std::string const& type, rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool
{
    auto node = m_source_factory.create_node(type);
    return node && node->init(init_params, config);
}
template<>
auto HAL::create_node<node::sink::ISink>(std::string const& type, rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool
{
    auto node = m_sink_factory.create_node(type);
    return node && node->init(init_params, config);
}
template<>
auto HAL::create_node<node::processor::IProcessor>(std::string const& type, rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool
{
    auto node = m_processor_factory.create_node(type);
    return node && node->init(init_params, config);
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



}

