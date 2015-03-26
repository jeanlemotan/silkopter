#include "BrainStdAfx.h"

#include "HAL.h"
#include "Comms.h"
#include "utils/Json_Util.h"
#include "utils/Timed_Scope.h"

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
#include "processor/Gravity_Filter.h"
#include "processor/Comp_AHRS.h"
#include "processor/Comp_ECEF_Location.h"
#include "processor/Pilot.h"
#include "processor/Motor_Mixer.h"

#include "processor/Rate_Controller.h"
#include "processor/Stability_Controller.h"
#include "processor/Velocity_Controller.h"

#include "simulator/Simulator.h"

#include "processor/Transformer.h"
#include "processor/Transformer_Inv.h"

#include "generator/Factor_Generator.h"
#include "generator/Vec3_Generator.h"
#include "generator/Scalar_Generator.h"


//#include "source/EHealth.h"

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


///////////////////////////////////////////////////////////////

HAL::HAL()
{
}

HAL::~HAL()
{
}

void HAL::save_settings()
{
    TIMED_FUNCTION();

    auto settingsj = std::make_shared<rapidjson::Document>();
    settingsj->SetObject();

    auto& allocator = settingsj->GetAllocator();

    {
        auto busesj = jsonutil::get_or_add_value(*settingsj, q::Path("hal/buses"), rapidjson::kObjectType, allocator);
        if (!busesj)
        {
            QLOGE("Cannot open create buses settings node.");
            return;
        }
        auto const& nodes = get_buses().get_all();
        for (auto const& n: nodes)
        {
            if (!jsonutil::add_value(*busesj, q::Path(n.name + "/type"), rapidjson::Value(n.type.c_str(), n.type.size(), allocator), allocator) ||
                !jsonutil::add_value(*busesj, q::Path(n.name + "/init_params"), jsonutil::clone_value(n.node->get_init_params(), allocator), allocator) ||
                !jsonutil::add_value(*busesj, q::Path(n.name + "/config"), jsonutil::clone_value(n.node->get_config(), allocator), allocator))
            {
                QLOGE("Cannot open create settings node.");
                return;
            }
        }
    }
    {
        auto nodesj = jsonutil::get_or_add_value(*settingsj, q::Path("hal/nodes"), rapidjson::kObjectType, allocator);
        if (!nodesj)
        {
            QLOGE("Cannot open create nodes settings node.");
            return;
        }
        auto const& nodes = get_nodes().get_all();
        for (auto const& n: nodes)
        {
            if (!jsonutil::add_value(*nodesj, q::Path(n.name + "/type"), rapidjson::Value(n.type.c_str(), n.type.size(), allocator), allocator) ||
                !jsonutil::add_value(*nodesj, q::Path(n.name + "/init_params"), jsonutil::clone_value(n.node->get_init_params(), allocator), allocator) ||
                !jsonutil::add_value(*nodesj, q::Path(n.name + "/config"), jsonutil::clone_value(n.node->get_config(), allocator), allocator))
            {
                QLOGE("Cannot open create settings node.");
                return;
            }
        }
    }

    silk::async([=]()
    {
        TIMED_FUNCTION();

        rapidjson::StringBuffer s;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
        settingsj->Accept(writer);    // Accept() traverses the DOM and generates Handler events.
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

auto HAL::get_bus_factory()    -> Factory<node::bus::IBus>&
{
    return m_bus_factory;
}
auto HAL::get_node_factory()  -> Factory<node::INode>&
{
    return m_node_factory;
}
auto HAL::get_buses()    -> Registry<node::bus::IBus>&
{
    return m_buses;
}
auto HAL::get_nodes()  -> Registry<node::INode>&
{
    return m_nodes;
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

auto HAL::create_bus(
        std::string const& type,
        std::string const& name,
        rapidjson::Value const& init_params) -> node::bus::IBus_ptr
{
    if (m_buses.find_by_name<node::bus::IBus>(name))
    {
        QLOGE("Bus '{}' already exist", name);
        return node::bus::IBus_ptr();
    }
    auto node = m_bus_factory.create_node(type);
    if (node && node->init(init_params))
    {
        auto res = m_buses.add(name, type, node); //this has to succeed since we already tested for duplicate names
        QASSERT(res);
        return node;
    }
    return node::bus::IBus_ptr();
}
auto HAL::create_node(
        std::string const& type,
        std::string const& name,
        rapidjson::Value const& init_params) -> node::INode_ptr
{
    if (m_nodes.find_by_name<node::INode>(name))
    {
        QLOGE("Node '{}' already exist", name);
        return node::INode_ptr();
    }
    auto node = m_node_factory.create_node(type);
    if (node && node->init(init_params))
    {
        auto res = m_nodes.add(name, type, node); //this has to succeed since we already tested for duplicate names
        QASSERT(res);
        auto outputs = node->get_outputs();
        for (auto const& x: outputs)
        {
            std::string stream_name = q::util::format2<std::string>("{}/{}", name, x.name);
            if (!m_streams.add(stream_name, std::string(), x.stream))
            {
                QLOGE("Cannot add stream '{}'", stream_name);
                return node::INode_ptr();
            }
        }
        return node;
    }
    return node::INode_ptr();
}

auto HAL::create_buses(rapidjson::Value& json) -> bool
{
    if (!json.IsObject())
    {
        QLOGE("Wrong json type: {}", json.GetType());
        return false;
    }
    auto it = json.MemberBegin();
    for (; it != json.MemberEnd(); ++it)
    {
        std::string name(it->name.GetString());
        auto* typej = jsonutil::find_value(it->value, std::string("type"));
        if (!typej || typej->GetType() != rapidjson::kStringType)
        {
            QLOGE("Node {} is missing the type", name);
            return false;
        }
        std::string type(typej->GetString());
        auto* init_paramsj = jsonutil::find_value(it->value, std::string("init_params"));
        if (!init_paramsj)
        {
            QLOGE("Node {} of type {} is missing the init_params", name, type);
            return false;
        }
        auto node = create_bus(type, name, *init_paramsj);
        if (!node)
        {
            QLOGE("Failed to create node {} of type '{}'", name, type);
            return false;
        }
    }
    return true;
}

auto HAL::create_nodes(rapidjson::Value& json) -> bool
{
    if (!json.IsObject())
    {
        QLOGE("Wrong json type: {}", json.GetType());
        return false;
    }
    for (auto it = json.MemberBegin(); it != json.MemberEnd(); ++it)
    {
        std::string name(it->name.GetString());
        auto* typej = jsonutil::find_value(it->value, std::string("type"));
        if (!typej || typej->GetType() != rapidjson::kStringType)
        {
            QLOGE("Node {} is missing the type", name);
            return false;
        }
        std::string type(typej->GetString());
        auto* init_paramsj = jsonutil::find_value(it->value, std::string("init_params"));
        if (!init_paramsj)
        {
            QLOGE("Node {} of type {} is missing the init_params", name, type);
            return false;
        }
        auto node = create_node(type, name, *init_paramsj);
        if (!node)
        {
            QLOGE("Failed to create node {} of type '{}'", name, type);
            return false;
        }
    }
    for (auto it = json.MemberBegin(); it != json.MemberEnd(); ++it)
    {
        std::string name(it->name.GetString());
        auto* configj = jsonutil::find_value(it->value, std::string("config"));
        if (!configj)
        {
            QLOGE("Node {} is missing the config", name);
            return false;
        }
        auto node = get_nodes().find_by_name<node::INode>(name);
        QASSERT(node);
        if (!node->set_config(*configj))
        {
            QLOGE("Failed to set config for node '{}'", name);
            return false;
        }
    }
    return true;
}


auto HAL::init(Comms& comms) -> bool
{
    using namespace silk::node;

    QLOG_TOPIC("hal::init");

    m_bus_factory.register_node<bus::UART_Linux>("UART Linux");
    m_bus_factory.register_node<bus::I2C_Linux>("I2C Linux");
    m_bus_factory.register_node<bus::SPI_Linux>("SPI Linux");

    m_node_factory.register_node<Simulator>("Simulator", *this);

    //m_node_factory.register_node<EHealth>("EHealth", *this);

    m_node_factory.register_node<MPU9250>("MPU9250", *this);
    m_node_factory.register_node<MS5611>("MS5611", *this);
    m_node_factory.register_node<SRF02>("SRF02", *this);
    m_node_factory.register_node<Raspicam>("Raspicam", *this);
    m_node_factory.register_node<RC5T619>("RC5T619", *this);
    m_node_factory.register_node<UBLOX>("UBLOX", *this);
    m_node_factory.register_node<Comms::Source>("Comms Source", comms);

    m_node_factory.register_node<PIGPIO>("PIGPIO", *this);

    m_node_factory.register_node<Pilot>("Pilot", *this);
    m_node_factory.register_node<ADC_Ammeter>("ADC Ammeter", *this);
    m_node_factory.register_node<ADC_Voltmeter>("ADC Voltmeter", *this);
    m_node_factory.register_node<Comp_AHRS>("Comp AHRS", *this);
    m_node_factory.register_node<Comp_ECEF_Location>("Comp ECEF Location", *this);
    m_node_factory.register_node<Gravity_Filter>("Gravity Filter", *this);
    m_node_factory.register_node<LiPo_Battery>("LiPo Battery", *this);

    m_node_factory.register_node<Factor_Generator>("Factor Generator", *this);

    m_node_factory.register_node<Scalar_Generator<stream::IADC>>("ADC Generator", *this);
    m_node_factory.register_node<Scalar_Generator<stream::ICurrent>>("Current Generator", *this);
    m_node_factory.register_node<Scalar_Generator<stream::IVoltage>>("Voltage Generator", *this);
    m_node_factory.register_node<Scalar_Generator<stream::IPressure>>("Pressure Generator", *this);
    m_node_factory.register_node<Scalar_Generator<stream::ITemperature>>("Temperature Generator", *this);
    m_node_factory.register_node<Scalar_Generator<stream::IPWM>>("PWM Generator", *this);

    m_node_factory.register_node<Vec3_Generator<stream::IAcceleration>>("Acceleration Generator", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IENU_Acceleration>>("Acceleration Generator (ENU)", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IECEF_Acceleration>>("Acceleration Generator (ECEF)", *this);
    m_node_factory.register_node<Vec3_Generator<stream::ILinear_Acceleration>>("Linear Acceleration Generator", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IENU_Linear_Acceleration>>("Linear Acceleration Generator (ENU)", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IECEF_Linear_Acceleration>>("Linear Acceleration Generator (ECEF)", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IAngular_Velocity>>("Angular Velocity Generator", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IENU_Angular_Velocity>>("Angular Velocity Generator (ENU)", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IECEF_Angular_Velocity>>("Angular Velocity Generator (ECEF)", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IMagnetic_Field>>("Magnetic Field Generator", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IENU_Magnetic_Field>>("Magnetic Field Generator (ENU)", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IECEF_Magnetic_Field>>("Magnetic Field Generator (ECEF)", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IForce>>("Force Generator", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IENU_Force>>("Force Generator (ENU)", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IECEF_Force>>("Force Generator (ECEF)", *this);
    m_node_factory.register_node<Vec3_Generator<stream::ITorque>>("Torque Generator", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IENU_Torque>>("Torque Generator (ENU)", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IECEF_Torque>>("Torque Generator (ECEF)", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IVelocity>>("Velocity Generator", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IENU_Velocity>>("Velocity Generator (ENU)", *this);
    m_node_factory.register_node<Vec3_Generator<stream::IECEF_Velocity>>("Velocity Generator (ECEF)", *this);


    m_node_factory.register_node<LPF<stream::IAcceleration>>("Acceleration LPF", *this);
    m_node_factory.register_node<LPF<stream::IENU_Acceleration>>("Acceleration LPF (ENU)", *this);
    m_node_factory.register_node<LPF<stream::IECEF_Acceleration>>("Acceleration LPF (ECEF)", *this);
    m_node_factory.register_node<LPF<stream::ILinear_Acceleration>>("Linear Acceleration LPF", *this);
    m_node_factory.register_node<LPF<stream::IENU_Linear_Acceleration>>("Linear Acceleration LPF (ENU)", *this);
    m_node_factory.register_node<LPF<stream::IECEF_Linear_Acceleration>>("Linear Acceleration LPF (ECEF)", *this);
    m_node_factory.register_node<LPF<stream::IAngular_Velocity>>("Angular Velocity LPF", *this);
    m_node_factory.register_node<LPF<stream::IENU_Angular_Velocity>>("Angular Velocity LPF (ENU)", *this);
    m_node_factory.register_node<LPF<stream::IECEF_Angular_Velocity>>("Angular Velocity LPF (ECEF)", *this);
    m_node_factory.register_node<LPF<stream::IBattery_State>>("Battery State LPF", *this);
    m_node_factory.register_node<LPF<stream::IADC>>("ADC LPF", *this);
    m_node_factory.register_node<LPF<stream::ICurrent>>("Current LPF", *this);
    m_node_factory.register_node<LPF<stream::IVoltage>>("Voltage LPF", *this);
    m_node_factory.register_node<LPF<stream::IECEF_Location>>("Location LPF (ECEF)", *this);
    m_node_factory.register_node<LPF<stream::IDistance>>("Distance LPF", *this);
    m_node_factory.register_node<LPF<stream::IENU_Distance>>("Distance LPF (ENU)", *this);
    m_node_factory.register_node<LPF<stream::IECEF_Distance>>("Distance LPF (ECEF)", *this);
    m_node_factory.register_node<LPF<stream::IMagnetic_Field>>("Magnetic Field LPF", *this);
    m_node_factory.register_node<LPF<stream::IENU_Magnetic_Field>>("Magnetic Field LPF (ENU)", *this);
    m_node_factory.register_node<LPF<stream::IECEF_Magnetic_Field>>("Magnetic Field LPF (ECEF)", *this);
    m_node_factory.register_node<LPF<stream::IPressure>>("Pressure LPF", *this);
    m_node_factory.register_node<LPF<stream::ITemperature>>("Temperature LPF", *this);
    m_node_factory.register_node<LPF<stream::IFrame>>("Frame LPF", *this);
    m_node_factory.register_node<LPF<stream::IENU_Frame>>("Frame LPF (ENU)", *this);
    m_node_factory.register_node<LPF<stream::IPWM>>("PWM LPF", *this);
    m_node_factory.register_node<LPF<stream::IFactor>>("Factor LPF", *this);
    m_node_factory.register_node<LPF<stream::IForce>>("Force LPF", *this);
    m_node_factory.register_node<LPF<stream::IENU_Force>>("Force LPF (ENU)", *this);
    m_node_factory.register_node<LPF<stream::IECEF_Force>>("Force LPF (ECEF)", *this);
    m_node_factory.register_node<LPF<stream::ITorque>>("Torque LPF", *this);
    m_node_factory.register_node<LPF<stream::IENU_Torque>>("Torque LPF (ENU)", *this);
    m_node_factory.register_node<LPF<stream::IECEF_Torque>>("Torque LPF (ECEF)", *this);
    m_node_factory.register_node<LPF<stream::IVelocity>>("Velocity LPF", *this);
    m_node_factory.register_node<LPF<stream::IENU_Velocity>>("Velocity LPF (ENU)", *this);
    m_node_factory.register_node<LPF<stream::IECEF_Velocity>>("Velocity LPF (ECEF)", *this);

    m_node_factory.register_node<Resampler<stream::IAcceleration>>("Acceleration RS", *this);
    m_node_factory.register_node<Resampler<stream::IENU_Acceleration>>("Acceleration RS (ENU)", *this);
    m_node_factory.register_node<Resampler<stream::IECEF_Acceleration>>("Acceleration RS (ECEF)", *this);
    m_node_factory.register_node<Resampler<stream::ILinear_Acceleration>>("Linear Acceleration RS", *this);
    m_node_factory.register_node<Resampler<stream::IENU_Linear_Acceleration>>("Linear Acceleration RS (ENU)", *this);
    m_node_factory.register_node<Resampler<stream::IECEF_Linear_Acceleration>>("Linear Acceleration RS (ECEF)", *this);
    m_node_factory.register_node<Resampler<stream::IAngular_Velocity>>("Angular Velocity RS", *this);
    m_node_factory.register_node<Resampler<stream::IENU_Angular_Velocity>>("Angular Velocity RS (ENU)", *this);
    m_node_factory.register_node<Resampler<stream::IECEF_Angular_Velocity>>("Angular Velocity RS (ECEF)", *this);
    m_node_factory.register_node<Resampler<stream::IBattery_State>>("Battery State RS", *this);
    m_node_factory.register_node<Resampler<stream::IADC>>("ADC RS", *this);
    m_node_factory.register_node<Resampler<stream::ICurrent>>("Current RS", *this);
    m_node_factory.register_node<Resampler<stream::IVoltage>>("Voltage RS", *this);
    m_node_factory.register_node<Resampler<stream::IECEF_Location>>("Location RS (ECEF)", *this);
    m_node_factory.register_node<Resampler<stream::IDistance>>("Distance RS", *this);
    m_node_factory.register_node<Resampler<stream::IENU_Distance>>("Distance RS (ENU)", *this);
    m_node_factory.register_node<Resampler<stream::IECEF_Distance>>("Distance RS (ECEF)", *this);
    m_node_factory.register_node<Resampler<stream::IMagnetic_Field>>("Magnetic Field RS", *this);
    m_node_factory.register_node<Resampler<stream::IENU_Magnetic_Field>>("Magnetic Field RS (ENU)", *this);
    m_node_factory.register_node<Resampler<stream::IECEF_Magnetic_Field>>("Magnetic Field RS (ECEF)", *this);
    m_node_factory.register_node<Resampler<stream::IPressure>>("Pressure RS", *this);
    m_node_factory.register_node<Resampler<stream::ITemperature>>("Temperature RS", *this);
    m_node_factory.register_node<Resampler<stream::IFrame>>("Frame RS", *this);
    m_node_factory.register_node<Resampler<stream::IENU_Frame>>("Frame RS (ENU)", *this);
    m_node_factory.register_node<Resampler<stream::IPWM>>("PWM RS", *this);
    m_node_factory.register_node<Resampler<stream::IFactor>>("Factor RS", *this);
    m_node_factory.register_node<Resampler<stream::IForce>>("Force RS", *this);
    m_node_factory.register_node<Resampler<stream::IENU_Force>>("Force RS (ENU)", *this);
    m_node_factory.register_node<Resampler<stream::IECEF_Force>>("Force RS (ECEF)", *this);
    m_node_factory.register_node<Resampler<stream::ITorque>>("Torque RS", *this);
    m_node_factory.register_node<Resampler<stream::IENU_Torque>>("Torque RS (ENU)", *this);
    m_node_factory.register_node<Resampler<stream::IECEF_Torque>>("Torque RS (ECEF)", *this);
    m_node_factory.register_node<Resampler<stream::IVelocity>>("Velocity RS", *this);
    m_node_factory.register_node<Resampler<stream::IENU_Velocity>>("Velocity RS (ENU)", *this);
    m_node_factory.register_node<Resampler<stream::IECEF_Velocity>>("Velocity RS (ECEF)", *this);

    m_node_factory.register_node<Transformer<stream::IECEF_Acceleration, stream::IENU_Acceleration, stream::IENU_Frame>>("Acceleration (ECEF->ENU)", *this);
    m_node_factory.register_node<Transformer<stream::IENU_Acceleration, stream::IAcceleration, stream::IFrame>>("Acceleration (ENU->Local)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IENU_Acceleration, stream::IECEF_Acceleration, stream::IENU_Frame>>("Acceleration (ENU->ECEF)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IAcceleration, stream::IENU_Acceleration, stream::IFrame>>("Acceleration (Local->ENU)", *this);

    m_node_factory.register_node<Transformer<stream::IECEF_Angular_Velocity, stream::IENU_Angular_Velocity, stream::IENU_Frame>>("Angular Velocity (ECEF->ENU)", *this);
    m_node_factory.register_node<Transformer<stream::IENU_Angular_Velocity, stream::IAngular_Velocity, stream::IFrame>>("Angular Velocity (ENU->Local)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IENU_Angular_Velocity, stream::IECEF_Angular_Velocity, stream::IENU_Frame>>("Angular Velocity (ENU->ECEF)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IAngular_Velocity, stream::IENU_Angular_Velocity, stream::IFrame>>("Angular Velocity (Local->ENU)", *this);

    m_node_factory.register_node<Transformer<stream::IECEF_Magnetic_Field, stream::IENU_Magnetic_Field, stream::IENU_Frame>>("Magnetic Field (ECEF->ENU)", *this);
    m_node_factory.register_node<Transformer<stream::IENU_Magnetic_Field, stream::IMagnetic_Field, stream::IFrame>>("Magnetic Field (ENU->Local)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IENU_Magnetic_Field, stream::IECEF_Magnetic_Field, stream::IENU_Frame>>("Magnetic Field (ENU->ECEF)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IMagnetic_Field, stream::IENU_Magnetic_Field, stream::IFrame>>("Magnetic Field (Local->ENU)", *this);

    m_node_factory.register_node<Transformer<stream::IECEF_Linear_Acceleration, stream::IENU_Linear_Acceleration, stream::IENU_Frame>>("Linear Acceleration (ECEF->ENU)", *this);
    m_node_factory.register_node<Transformer<stream::IENU_Linear_Acceleration, stream::ILinear_Acceleration, stream::IFrame>>("Linear Acceleration (ENU->Local)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IENU_Linear_Acceleration, stream::IECEF_Linear_Acceleration, stream::IENU_Frame>>("Linear Acceleration (ENU->ECEF)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::ILinear_Acceleration, stream::IENU_Linear_Acceleration, stream::IFrame>>("Linear Acceleration (Local->ENU)", *this);

    m_node_factory.register_node<Transformer<stream::IECEF_Distance, stream::IENU_Distance, stream::IENU_Frame>>("Distance (ECEF->ENU)", *this);
    m_node_factory.register_node<Transformer<stream::IENU_Distance, stream::IDistance, stream::IFrame>>("Distance (ENU->Local)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IENU_Distance, stream::IECEF_Distance, stream::IENU_Frame>>("Distance (ENU->ECEF)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IDistance, stream::IENU_Distance, stream::IFrame>>("Distance (Local->ENU)", *this);

    m_node_factory.register_node<Transformer<stream::IECEF_Force, stream::IENU_Force, stream::IENU_Frame>>("Force (ECEF->ENU)", *this);
    m_node_factory.register_node<Transformer<stream::IENU_Force, stream::IForce, stream::IFrame>>("Force (ENU->Local)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IENU_Force, stream::IECEF_Force, stream::IENU_Frame>>("Force (ENU->ECEF)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IForce, stream::IENU_Force, stream::IFrame>>("Force (Local->ENU)", *this);

    m_node_factory.register_node<Transformer<stream::IECEF_Torque, stream::IENU_Torque, stream::IENU_Frame>>("Torque (ECEF->ENU)", *this);
    m_node_factory.register_node<Transformer<stream::IENU_Torque, stream::ITorque, stream::IFrame>>("Torque (ENU->Local)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IENU_Torque, stream::IECEF_Torque, stream::IENU_Frame>>("Torque (ENU->ECEF)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::ITorque, stream::IENU_Torque, stream::IFrame>>("Torque (Local->ENU)", *this);

    m_node_factory.register_node<Transformer<stream::IECEF_Velocity, stream::IENU_Velocity, stream::IENU_Frame>>("Velocity (ECEF->ENU)", *this);
    m_node_factory.register_node<Transformer<stream::IENU_Velocity, stream::IVelocity, stream::IFrame>>("Velocity (ENU->Local)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IENU_Velocity, stream::IECEF_Velocity, stream::IENU_Frame>>("Velocity (ENU->ECEF)", *this);
    m_node_factory.register_node<Transformer_Inv<stream::IVelocity, stream::IENU_Velocity, stream::IFrame>>("Velocity (Local->ENU)", *this);

    m_node_factory.register_node<Motor_Mixer>("Motor Mixer", *this);

    m_node_factory.register_node<Rate_Controller>("Rate Controller", *this);
    m_node_factory.register_node<Stability_Controller>("Stability Controller", *this);
    m_node_factory.register_node<Velocity_Controller>("Velocity Controller", *this);


    get_streams().remove_all();
    get_nodes().remove_all();

    q::data::File_Source fs(k_settings_path);
    if (!fs.is_open())
    {
        QLOGE("Failed to load '{}'", k_settings_path);
        return false;
    }

    auto data = q::data::read_whole_source_as_string<std::string>(fs);
    rapidjson::Document settingsj;
    if (settingsj.Parse(data.c_str()).HasParseError())
    {
        QLOGE("Failed to load '{}': {}:{}", k_settings_path, settingsj.GetParseError(), settingsj.GetErrorOffset());
        return false;
    }

//    uint32_t rate = 1000;
//    std::vector<float> samples;

//    {
//        const size_t elements = 1000;
//        const float noise = 0.3f;
//        std::vector<std::pair<float, float>> freq =
//        {{
//             { 10.f, 1.f },
//             { 70.f, 1.f/7.f },
//             { 130.f, 1.f/5.f }
//         }};
//        samples.resize(elements);
//        std::uniform_real_distribution<float> distribution(-noise, noise); //Values between 0 and 2
//        std::mt19937 engine; // Mersenne twister MT19937
//        auto generator = std::bind(distribution, engine);
//        for (size_t i = 0; i < samples.size(); i++)
//        {
//            float a = float(i) * math::anglef::_2pi / float(rate);
//            float output = 0.f;
//            for (auto& f: freq)
//            {
//                output += math::sin(a * f.first) * f.second;
//            }
//            samples[i] = output + generator();
//        }
//        for (size_t i = 0; i < samples.size(); i++)
//        {
//            samples[i] = i < (rate / 2) ? 1 : 0;
//        }
//    }

//    write_gnu_plot("in.dat", samples);

//    std::vector<float> out_samples;
//    for (size_t f = 1; f < rate / 2; f++)
//    {
//        util::Butterworth<double> lpf;
//        lpf.setup(4, rate, 100);
//        lpf.reset(0);
//        double inputs = 0;
//        double outputs = 0;
//        for (size_t x = 0; x < rate * 10; x++)
//        {
//            double a = x <= rate * 2 ? double(x) * math::angled::_2pi / double(rate) : 0.0;
//            double v = math::sin(a * f);
//            inputs += math::sqrt(math::abs(v));
//            lpf.process(v);
//            outputs += math::sqrt(math::abs(v));
//        }
//        out_samples.push_back(outputs / inputs);
//    }
//    write_gnu_plot("out.dat", out_samples);

    auto* busesj = jsonutil::find_value(static_cast<rapidjson::Value&>(settingsj), q::Path("hal/buses"));
    auto* nodesj = jsonutil::find_value(static_cast<rapidjson::Value&>(settingsj), q::Path("hal/nodes"));

    if ((busesj && !create_buses(*busesj)) ||
        (nodesj && !create_nodes(*nodesj)))
    {
        return false;
    }

    save_settings();

    return true;
}

void HAL::shutdown()
{
}


//static std::vector<double> s_samples;
//static std::vector<double> s_samples_lpf;

void HAL::process()
{
//    for (auto const& n: m_buses.get_all())
//    {
//        n->process();
//    }
    for (auto const& n: m_nodes.get_all())
    {
        n.node->process();
    }

//    auto* stream = get_streams().find_by_name<node::ILocation>("gps0/stream");
//    auto* stream_lpf = get_streams().find_by_name<node::ILocation>("gps0_resampler/stream");
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

