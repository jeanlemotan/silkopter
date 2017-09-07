#include "FCStdAfx.h"

#include <fstream>

#include "HAL.h"
#include "RC_Comms.h"
#include "GS_Comms.h"
#include "utils/Timed_Scope.h"

/////////////////////////////////////////////////////////////////////////////////////

#include "bus/I2C_Linux_Bus.h"
#include "bus/SPI_Linux_Bus.h"
#include "bus/I2C_BCM_Bus.h"
#include "bus/SPI_BCM_Bus.h"
#include "bus/UART_Linux_Bus.h"
#include "bus/UART_BB_Bus.h"

#include "source/Raspicam.h"
#include "source/MPU9250.h"
#include "source/MS5611.h"
#include "source/RC5T619.h"
#include "source/ADS1115_Source.h"
#include "source/AVRADC.h"
#include "source/SRF01.h"
#include "source/SRF02.h"
#include "source/MaxSonar.h"
#include "source/UBLOX.h"
#include "source/CPPM_Receiver.h"
#include "sink/PIGPIO.h"
#include "sink/PCA9685.h"

#include "common/stream/IForce.h"

#include "processor/ADC_Voltmeter.h"
#include "processor/ADC_Ammeter.h"
#include "processor/Gravity_Filter.h"
#include "processor/Comp_AHRS.h"
//#include "processor/Comp_ECEF.h"
#include "processor/KF_ECEF.h"
#include "processor/Motor_Mixer.h"
#include "processor/Quad_Multirotor_Motor_Mixer.h"
#include "processor/Servo_Gimbal.h"
#include "processor/Throttle_To_PWM.h"
#include "processor/Proximity.h"
#include "processor/Pressure_Velocity.h"
#include "processor/ENU_Frame_System.h"

#include "combiner/Combiner.h"
#include "lpf/LPF.h"
#include "resampler/Resampler.h"
#include "brain/Multirotor_Brain.h"
#include "pilot/Multirotor_Pilot.h"

#include "controller/Rate_Controller.h"

#include "simulator/Multirotor_Simulator.h"

#include "transformer/Transformer.h"
#include "transformer/Transformer_Inv.h"

#include "generator/Oscillator.h"
#include "generator/Vec3_Generator.h"
#include "generator/Scalar_Generator.h"

#include "common/stream/IThrottle.h"

#include "def_lang/Serialization.h"
#include "def_lang/JSON_Serializer.h"

#include "uav_properties/Tri_Multirotor_Properties.h"
#include "uav_properties/Quad_Multirotor_Properties.h"
#include "uav_properties/Hexa_Multirotor_Properties.h"
#include "uav_properties/Hexatri_Multirotor_Properties.h"
#include "uav_properties/Octo_Multirotor_Properties.h"
#include "uav_properties/Octoquad_Multirotor_Properties.h"

#ifdef RASPBERRY_PI

extern "C"
{
    #include "utils/hw/pigpio.h"
    #include "utils/hw/bcm2835.h"
}

///////////////////////////////////////////////////////////////////

std::chrono::microseconds PIGPIO_PERIOD(2);

static auto initialize_pigpio() -> bool
{
    static bool initialized = false;
    if (initialized)
    {
        return true;
    }

    QLOGI("Initializing pigpio");
    if (gpioCfgClock(PIGPIO_PERIOD.count(), 1, 0) < 0 ||
        gpioCfgPermissions(static_cast<uint64_t>(-1)))
    {
        QLOGE("Cannot configure pigpio");
        return false;
    }
    if (gpioInitialise() < 0)
    {
        QLOGE("Cannot initialize pigpio");
        return false;
    }

    initialized = true;
    return true;
}
static auto shutdown_pigpio() -> bool
{
    gpioTerminate();
    return true;
}

static auto initialize_bcm() -> bool
{
    static bool initialized = false;
    if (initialized)
    {
        return true;
    }

    QLOGI("Initializing bcm2835");
    if (!bcm2835_init())
    {
        QLOGE("bcm 2835 library initialization failed");
        return false;
    }

    initialized = true;
    return true;
}

static auto shutdown_bcm() -> bool
{
    QLOGI("Shutting down bcm2835");
    //bcm2835_spi_end(); //cannot call this as it breaks the SPI because it sets all the pins to inputs
    //bcm2835_i2c_end(); //cannot call this as it breaks the I2C because it sets all the pins to inputs
    bcm2835_close();
    return true;
}

#endif


namespace silk
{

static const std::string k_settings_filename("settings.json");
extern std::string s_program_path;


//wrapper to keep all nodes in the same container
class INode_Wrapper
{
public:
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

    hal::Settings settings;

    settings.set_uav_descriptor(hal::Poly<const hal::IUAV_Descriptor>(m_uav_descriptor));

    std::vector<hal::Settings::Node_Data>& node_datas = settings.get_nodes();
    auto const& nodes = get_node_registry().get_all();
    for (auto const& n: nodes)
    {
        hal::Settings::Node_Data node_data;

        node_data.set_name(n.name);
        node_data.set_type(n.type);
        node_data.set_descriptor(hal::Poly<const hal::INode_Descriptor>(n.ptr->get_descriptor()));
        node_data.set_config(hal::Poly<const hal::INode_Config>(n.ptr->get_config()));

        for (auto const& si: n.ptr->get_inputs())
        {
            node_data.get_input_paths().push_back(si.stream_path);
        }

        node_datas.push_back(std::move(node_data));
    }

    std::vector<hal::Settings::Bus_Data>& bus_datas = settings.get_buses();
    auto const& buses = get_bus_registry().get_all();
    for (auto const& b: buses)
    {
        hal::Settings::Bus_Data bus_data;

        bus_data.set_name(b.name);
        bus_data.set_type(b.type);
        bus_data.set_descriptor(hal::Poly<const hal::IBus_Descriptor>(b.ptr->get_descriptor()));

        bus_datas.push_back(std::move(bus_data));
    }

    ts::sz::Value sz_value = hal::serialize(settings);

    silk::async(std::function<void()>([sz_value]()
    {
        TIMED_FUNCTION();

        std::string json = ts::sz::to_json(sz_value, true);

        std::string settings_path = s_program_path + "/" + k_settings_filename;
        std::ofstream fs(settings_path);
        if (fs.is_open())
        {
            fs.write(json.data(), json.size());
            fs.flush();
        }
        else
        {
            QLOGE("Cannot open '{}' to save settings.", settings_path);
        }
    }));
}

HAL::Telemetry_Data const& HAL::get_telemetry_data() const
{
    return m_telemetry_data;
}

HAL::Bus_Factory const& HAL::get_bus_factory() const
{
    return m_bus_factory;
}
HAL::Node_Factory const& HAL::get_node_factory() const
{
    return m_node_factory;
}
HAL::Bus_Registry const& HAL::get_bus_registry() const
{
    return m_buses;
}
HAL::Node_Registry const& HAL::get_node_registry() const
{
    return m_nodes;
}
HAL::Stream_Registry const& HAL::get_stream_registry() const
{
    return m_streams;
}
auto HAL::get_uav_descriptor() const -> std::shared_ptr<const hal::IUAV_Descriptor>
{
    return m_uav_descriptor;
}
ts::Result<void> HAL::set_uav_descriptor(std::shared_ptr<const hal::IUAV_Descriptor> descriptor)
{
    if (!descriptor)
    {
        return make_error("Cannot set null descriptor");
    }

    std::shared_ptr<IUAV_Properties> new_properties;

    if (auto* d = dynamic_cast<hal::Tri_Multirotor_Descriptor const*>(descriptor.get()))
    {
        return make_error("Tri not supported");
    }
    else if (auto* d = dynamic_cast<hal::Quad_Multirotor_Descriptor const*>(descriptor.get()))
    {
        std::shared_ptr<Quad_Multirotor_Properties> p = std::make_shared<Quad_Multirotor_Properties>();
        auto result = p->init(*d);
        if (result != ts::success)
        {
            return result;
        }
        new_properties = p;
    }
    else if (auto* d = dynamic_cast<hal::Hexa_Multirotor_Descriptor const*>(descriptor.get()))
    {
        return make_error("Hexa not supported");
    }
    else if (auto* d = dynamic_cast<hal::Hexatri_Multirotor_Descriptor const*>(descriptor.get()))
    {
        return make_error("Hexatri not supported");
    }
    else if (auto* d = dynamic_cast<hal::Octo_Multirotor_Descriptor const*>(descriptor.get()))
    {
        return make_error("Octo not supported");
    }
    else if (auto* d = dynamic_cast<hal::Octaquad_Multirotor_Descriptor const*>(descriptor.get()))
    {
        return make_error("Octaquad not supported");
    }
    else if (auto* d = dynamic_cast<hal::Custom_Multirotor_Descriptor const*>(descriptor.get()))
    {
        return make_error("Custom multirotor not supported");
    }
    else
    {
        return make_error("Unknown multirotor descriptor type");
    }

    m_uav_descriptor = descriptor;
    std::swap(m_uav_properties, new_properties);

    return ts::success;
}
//auto hal::set_multirotor_descriptor(hal::Multirotor_Descriptor const& descriptor) -> bool
//{
//    QLOG_TOPIC("hal::set_multirotor_descriptor");

//    if (descriptor.get_motors().size() < 2)
//    {
//        QLOGE("Bad motor count: {}", descriptor.get_motors().size());
//        return false;
//    }
//    for (auto const& m: descriptor.get_motors())
//    {
//        if (math::is_zero(m.get_position(), math::epsilon<float>()))
//        {
//            QLOGE("Bad motor position: {}", m.get_position());
//            return false;
//        }
//    }

//    //http://en.wikipedia.org/wiki/List_of_moments_of_inertia
//    m_uav_descriptor.reset(new hal::Multirotor_Descriptor(descriptor)); //make a copy
//    if (math::is_zero(descriptor.get_moment_of_inertia(), math::epsilon<float>()))
//    {
//        m_uav_descriptor->set_moment_of_inertia((1.f / 12.f) * descriptor.get_mass() * (3.f * math::square(descriptor.get_radius()) + math::square(descriptor.get_height())));
//    }


//    return true;
//}

auto HAL::get_uav_properties() const -> std::shared_ptr<const IUAV_Properties>
{
    return m_uav_properties;
}

auto HAL::remove_node(std::shared_ptr<node::INode> node) -> bool
{
    m_nodes.remove(node);
    std::vector<node::INode::Output> outputs = node->get_outputs();
    for (node::INode::Output const& output: outputs)
    {
        m_streams.remove(output.stream);
    }
    return true;
}

template<class T>
void write_gnu_plot(std::string const& name, std::vector<T> const& samples)
{
    std::ofstream fs(name);
    std::string header("#x y\n");
    fs.write(header.c_str(), header.size());

    for(size_t i = 0; i < samples.size(); i++)
    {
        auto l = q::util::format<std::string>("{} {.8}\n", i, samples[i]);
        fs.write((uint8_t const*)l.c_str(), l.size());
    }
}

ts::Result<std::shared_ptr<bus::IBus>> HAL::create_bus(std::string const& type, std::string const& name, hal::IBus_Descriptor const& descriptor)
{
    if (m_buses.find_by_name<bus::IBus>(name))
    {
        return make_error("Bus '{}' already exist", name);
    }
    auto node = m_bus_factory.create(type);
    if (!node)
    {
        return make_error("Cannot create bus type '{}'", type);
    }
    auto result = node->init(descriptor);
    if (result != ts::success)
    {
        return result.error();
    }
    auto res = m_buses.add(name, type, node); //this has to succeed since we already tested for duplicate names
    QASSERT(res);
    return node;
}
ts::Result<std::shared_ptr<node::INode>> HAL::create_node(std::string const& type, std::string const& name, hal::INode_Descriptor const& descriptor)
{
    if (m_nodes.find_by_name<node::INode>(name))
    {
        return make_error("Node '{}' already exist", name);
    }
    std::shared_ptr<node::INode> node = m_node_factory.create(type);
    if (!node)
    {
        return make_error("Cannot create  node type '{}", type);
    }
    ts::Result<void> result = node->init(descriptor);
    if (result != ts::success)
    {
        return result.error();
    }

    result = node->set_config(*node->get_config());//apply default config
    if (result != ts::success)
    {
        return result.error();
    }

    bool res = m_nodes.add(name, type, node); //this has to succeed since we already tested for duplicate names
    QASSERT(res);
    std::vector<node::INode::Output> outputs = node->get_outputs();
    for (node::INode::Output const& x: outputs)
    {
        std::string stream_name = q::util::format<std::string>("{}/{}", name, x.name);
        if (!m_streams.add(stream_name, std::string(), x.stream))
        {
            m_nodes.remove(node);
            return make_error("Cannot add stream '{}'", stream_name);
        }
    }
    return node;
}

void HAL::sort_nodes(std::shared_ptr<node::INode> first_node)
{
    QASSERT(first_node);
    if (!first_node)
    {
        return;
    }
}

auto HAL::init(RC_Comms& rc_comms, GS_Comms& gs_comms) -> bool
{
    using namespace silk::node;

    QLOG_TOPIC("hal::init");

#if defined (RASPBERRY_PI)
    if (!initialize_pigpio())
    {
        QLOGE("Cannot initialize pigpio");
        return false;
    }
    if (!initialize_bcm())
    {
        QLOGE("Cannot initialize bcm");
        return false;
    }
#endif

    m_bus_factory.add<bus::UART_Linux_Bus>("UART Linux");
    m_bus_factory.add<bus::UART_BB_Bus>("UART BB");
    m_bus_factory.add<bus::I2C_Linux_Bus>("I2C Linux");
    m_bus_factory.add<bus::SPI_Linux_Bus>("SPI Linux");
    m_bus_factory.add<bus::I2C_BCM_Bus>("I2C BCM");
    m_bus_factory.add<bus::SPI_BCM_Bus>("SPI BCM");

    m_node_factory.add<Multirotor_Simulator>("Multirotor Simulator", *this);
    m_node_factory.add<MPU9250>("MPU9250", *this);
    m_node_factory.add<MS5611>("MS5611", *this);
    m_node_factory.add<SRF01>("SRF01", *this);
    m_node_factory.add<SRF02>("SRF02", *this);
    m_node_factory.add<MaxSonar>("MaxSonar", *this);
    m_node_factory.add<Raspicam>("Raspicam", *this);
    m_node_factory.add<RC5T619>("RC5T619", *this);
    m_node_factory.add<ADS1115_Source>("ADS1115", *this);
    m_node_factory.add<AVRADC>("AVRADC", *this);
    m_node_factory.add<UBLOX>("UBLOX", *this);
    m_node_factory.add<CPPM_Receiver>("CPPM Receiver", *this);

    m_node_factory.add<PIGPIO>("PIGPIO", *this);
    m_node_factory.add<PCA9685>("PCA9685", *this);

    m_node_factory.add<Multirotor_Brain>("Multirotor Brain", *this);
    m_node_factory.add<Multirotor_Pilot>("Multirotor Pilot", *this, rc_comms);

    m_node_factory.add<ADC_Ammeter>("ADC Ammeter", *this);
    m_node_factory.add<ADC_Voltmeter>("ADC Voltmeter", *this);
    m_node_factory.add<Comp_AHRS>("Comp AHRS", *this);
//    m_node_factory.add<Comp_ECEF>("Comp ECEF", *this);
    m_node_factory.add<KF_ECEF>("EKF ECEF", *this);
    m_node_factory.add<Gravity_Filter>("Gravity Filter", *this);
    m_node_factory.add<Throttle_To_PWM>("Throttle To PWM", *this);
    m_node_factory.add<Proximity>("Proximity", *this);
    m_node_factory.add<Pressure_Velocity>("Pressure Velocity", *this);
    m_node_factory.add<ENU_Frame_System>("ENU Frame System", *this);

    m_node_factory.add<Oscillator>("Oscillator", *this);

    m_node_factory.add<Scalar_Generator<stream::IADC>>("ADC Generator", *this);
    m_node_factory.add<Scalar_Generator<stream::ICurrent>>("Current Generator", *this);
    m_node_factory.add<Scalar_Generator<stream::IVoltage>>("Voltage Generator", *this);
    m_node_factory.add<Scalar_Generator<stream::IPressure>>("Pressure Generator", *this);
    m_node_factory.add<Scalar_Generator<stream::ITemperature>>("Temperature Generator", *this);
    m_node_factory.add<Scalar_Generator<stream::IPWM>>("PWM Generator", *this);
    m_node_factory.add<Scalar_Generator<stream::IThrottle>>("Throttle Generator", *this);
    m_node_factory.add<Scalar_Generator<stream::IFloat>>("Float Generator", *this);

    m_node_factory.add<Vec3_Generator<stream::IAcceleration>>("Acceleration Generator", *this);
    m_node_factory.add<Vec3_Generator<stream::IENU_Acceleration>>("Acceleration Generator (ENU)", *this);
//    m_node_factory.add<Vec3_Generator<stream::IECEF_Acceleration>>("Acceleration Generator (ECEF)", *this);
    m_node_factory.add<Vec3_Generator<stream::ILinear_Acceleration>>("Linear Acceleration Generator", *this);
    m_node_factory.add<Vec3_Generator<stream::IENU_Linear_Acceleration>>("Linear Acceleration Generator (ENU)", *this);
    m_node_factory.add<Vec3_Generator<stream::IECEF_Linear_Acceleration>>("Linear Acceleration Generator (ECEF)", *this);
    m_node_factory.add<Vec3_Generator<stream::IAngular_Velocity>>("Angular Velocity Generator", *this);
    m_node_factory.add<Vec3_Generator<stream::IENU_Angular_Velocity>>("Angular Velocity Generator (ENU)", *this);
    m_node_factory.add<Vec3_Generator<stream::IECEF_Angular_Velocity>>("Angular Velocity Generator (ECEF)", *this);
    m_node_factory.add<Vec3_Generator<stream::IMagnetic_Field>>("Magnetic Field Generator", *this);
    m_node_factory.add<Vec3_Generator<stream::IENU_Magnetic_Field>>("Magnetic Field Generator (ENU)", *this);
    m_node_factory.add<Vec3_Generator<stream::IECEF_Magnetic_Field>>("Magnetic Field Generator (ECEF)", *this);
    m_node_factory.add<Vec3_Generator<stream::IForce>>("Force Generator", *this);
    m_node_factory.add<Vec3_Generator<stream::IENU_Force>>("Force Generator (ENU)", *this);
    m_node_factory.add<Vec3_Generator<stream::IECEF_Force>>("Force Generator (ECEF)", *this);
    m_node_factory.add<Vec3_Generator<stream::ITorque>>("Torque Generator", *this);
    m_node_factory.add<Vec3_Generator<stream::IENU_Torque>>("Torque Generator (ENU)", *this);
    m_node_factory.add<Vec3_Generator<stream::IECEF_Torque>>("Torque Generator (ECEF)", *this);
    m_node_factory.add<Vec3_Generator<stream::IVelocity>>("Velocity Generator", *this);
    m_node_factory.add<Vec3_Generator<stream::IENU_Velocity>>("Velocity Generator (ENU)", *this);
    m_node_factory.add<Vec3_Generator<stream::IECEF_Velocity>>("Velocity Generator (ECEF)", *this);
//    m_node_factory.add<Vec3_Generator<stream::IPosition>>("Position Generator", *this);
//    m_node_factory.add<Vec3_Generator<stream::IENU_Position>>("Position Generator (ENU)", *this);
    m_node_factory.add<Vec3_Generator<stream::IECEF_Position>>("Position Generator (ECEF)", *this);

    m_node_factory.add<Combiner<stream::IAcceleration>>("Acceleration CMB", *this);
    m_node_factory.add<Combiner<stream::IENU_Acceleration>>("Acceleration CMB (ENU)", *this);
    m_node_factory.add<Combiner<stream::ILinear_Acceleration>>("Linear Acceleration CMB", *this);
    m_node_factory.add<Combiner<stream::IENU_Linear_Acceleration>>("Linear Acceleration CMB (ENU)", *this);
    m_node_factory.add<Combiner<stream::IECEF_Linear_Acceleration>>("Linear Acceleration CMB (ECEF)", *this);
    m_node_factory.add<Combiner<stream::IAngular_Velocity>>("Angular Velocity CMB", *this);
    m_node_factory.add<Combiner<stream::IENU_Angular_Velocity>>("Angular Velocity CMB (ENU)", *this);
    m_node_factory.add<Combiner<stream::IECEF_Angular_Velocity>>("Angular Velocity CMB (ECEF)", *this);
    m_node_factory.add<Combiner<stream::IADC>>("ADC CMB", *this);
    m_node_factory.add<Combiner<stream::ICurrent>>("Current CMB", *this);
    m_node_factory.add<Combiner<stream::IVoltage>>("Voltage CMB", *this);
    m_node_factory.add<Combiner<stream::IECEF_Position>>("Position CMB (ECEF)", *this);
    m_node_factory.add<Combiner<stream::IDistance>>("Distance CMB", *this);
    m_node_factory.add<Combiner<stream::IENU_Distance>>("Distance CMB (ENU)", *this);
    m_node_factory.add<Combiner<stream::IECEF_Distance>>("Distance CMB (ECEF)", *this);
    m_node_factory.add<Combiner<stream::IMagnetic_Field>>("Magnetic Field CMB", *this);
    m_node_factory.add<Combiner<stream::IENU_Magnetic_Field>>("Magnetic Field CMB (ENU)", *this);
    m_node_factory.add<Combiner<stream::IECEF_Magnetic_Field>>("Magnetic Field CMB (ECEF)", *this);
    m_node_factory.add<Combiner<stream::IPressure>>("Pressure CMB", *this);
    m_node_factory.add<Combiner<stream::ITemperature>>("Temperature CMB", *this);
    m_node_factory.add<Combiner<stream::IPWM>>("PWM CMB", *this);
    m_node_factory.add<Combiner<stream::IFloat>>("Float CMB", *this);
    m_node_factory.add<Combiner<stream::IForce>>("Force CMB", *this);
    m_node_factory.add<Combiner<stream::IENU_Force>>("Force CMB (ENU)", *this);
    m_node_factory.add<Combiner<stream::IECEF_Force>>("Force CMB (ECEF)", *this);
    m_node_factory.add<Combiner<stream::ITorque>>("Torque CMB", *this);
    m_node_factory.add<Combiner<stream::IENU_Torque>>("Torque CMB (ENU)", *this);
    m_node_factory.add<Combiner<stream::IECEF_Torque>>("Torque CMB (ECEF)", *this);
    m_node_factory.add<Combiner<stream::IVelocity>>("Velocity CMB", *this);
    m_node_factory.add<Combiner<stream::IENU_Velocity>>("Velocity CMB (ENU)", *this);
    m_node_factory.add<Combiner<stream::IECEF_Velocity>>("Velocity CMB (ECEF)", *this);

    m_node_factory.add<LPF<stream::IAcceleration>>("Acceleration LPF", *this);
    m_node_factory.add<LPF<stream::IENU_Acceleration>>("Acceleration LPF (ENU)", *this);
//    m_node_factory.add<LPF<stream::IECEF_Acceleration>>("Acceleration LPF (ECEF)", *this);
    m_node_factory.add<LPF<stream::ILinear_Acceleration>>("Linear Acceleration LPF", *this);
    m_node_factory.add<LPF<stream::IENU_Linear_Acceleration>>("Linear Acceleration LPF (ENU)", *this);
    m_node_factory.add<LPF<stream::IECEF_Linear_Acceleration>>("Linear Acceleration LPF (ECEF)", *this);
    m_node_factory.add<LPF<stream::IAngular_Velocity>>("Angular Velocity LPF", *this);
    m_node_factory.add<LPF<stream::IENU_Angular_Velocity>>("Angular Velocity LPF (ENU)", *this);
    m_node_factory.add<LPF<stream::IECEF_Angular_Velocity>>("Angular Velocity LPF (ECEF)", *this);
    m_node_factory.add<LPF<stream::IADC>>("ADC LPF", *this);
    m_node_factory.add<LPF<stream::ICurrent>>("Current LPF", *this);
    m_node_factory.add<LPF<stream::IVoltage>>("Voltage LPF", *this);
    m_node_factory.add<LPF<stream::IECEF_Position>>("Position LPF (ECEF)", *this);
    m_node_factory.add<LPF<stream::IDistance>>("Distance LPF", *this);
    m_node_factory.add<LPF<stream::IENU_Distance>>("Distance LPF (ENU)", *this);
    m_node_factory.add<LPF<stream::IECEF_Distance>>("Distance LPF (ECEF)", *this);
    m_node_factory.add<LPF<stream::IMagnetic_Field>>("Magnetic Field LPF", *this);
    m_node_factory.add<LPF<stream::IENU_Magnetic_Field>>("Magnetic Field LPF (ENU)", *this);
    m_node_factory.add<LPF<stream::IECEF_Magnetic_Field>>("Magnetic Field LPF (ECEF)", *this);
    m_node_factory.add<LPF<stream::IPressure>>("Pressure LPF", *this);
    m_node_factory.add<LPF<stream::ITemperature>>("Temperature LPF", *this);
//    m_node_factory.add<LPF<stream::IFrame>>("Frame LPF", *this);
//    m_node_factory.add<LPF<stream::IENU_Frame>>("Frame LPF (ENU)", *this);
    m_node_factory.add<LPF<stream::IPWM>>("PWM LPF", *this);
    m_node_factory.add<LPF<stream::IFloat>>("Float LPF", *this);
    m_node_factory.add<LPF<stream::IForce>>("Force LPF", *this);
    m_node_factory.add<LPF<stream::IENU_Force>>("Force LPF (ENU)", *this);
    m_node_factory.add<LPF<stream::IECEF_Force>>("Force LPF (ECEF)", *this);
    m_node_factory.add<LPF<stream::ITorque>>("Torque LPF", *this);
    m_node_factory.add<LPF<stream::IENU_Torque>>("Torque LPF (ENU)", *this);
    m_node_factory.add<LPF<stream::IECEF_Torque>>("Torque LPF (ECEF)", *this);
    m_node_factory.add<LPF<stream::IVelocity>>("Velocity LPF", *this);
    m_node_factory.add<LPF<stream::IENU_Velocity>>("Velocity LPF (ENU)", *this);
    m_node_factory.add<LPF<stream::IECEF_Velocity>>("Velocity LPF (ECEF)", *this);

    m_node_factory.add<Resampler<stream::IAcceleration>>("Acceleration RS", *this);
    m_node_factory.add<Resampler<stream::IENU_Acceleration>>("Acceleration RS (ENU)", *this);
//    m_node_factory.add<Resampler<stream::IECEF_Acceleration>>("Acceleration RS (ECEF)", *this);
    m_node_factory.add<Resampler<stream::ILinear_Acceleration>>("Linear Acceleration RS", *this);
    m_node_factory.add<Resampler<stream::IENU_Linear_Acceleration>>("Linear Acceleration RS (ENU)", *this);
    m_node_factory.add<Resampler<stream::IECEF_Linear_Acceleration>>("Linear Acceleration RS (ECEF)", *this);
    m_node_factory.add<Resampler<stream::IAngular_Velocity>>("Angular Velocity RS", *this);
    m_node_factory.add<Resampler<stream::IENU_Angular_Velocity>>("Angular Velocity RS (ENU)", *this);
    m_node_factory.add<Resampler<stream::IECEF_Angular_Velocity>>("Angular Velocity RS (ECEF)", *this);
    m_node_factory.add<Resampler<stream::IADC>>("ADC RS", *this);
    m_node_factory.add<Resampler<stream::ICurrent>>("Current RS", *this);
    m_node_factory.add<Resampler<stream::IVoltage>>("Voltage RS", *this);
    m_node_factory.add<Resampler<stream::IECEF_Position>>("Position RS (ECEF)", *this);
    m_node_factory.add<Resampler<stream::IDistance>>("Distance RS", *this);
    m_node_factory.add<Resampler<stream::IENU_Distance>>("Distance RS (ENU)", *this);
    m_node_factory.add<Resampler<stream::IECEF_Distance>>("Distance RS (ECEF)", *this);
    m_node_factory.add<Resampler<stream::IMagnetic_Field>>("Magnetic Field RS", *this);
    m_node_factory.add<Resampler<stream::IENU_Magnetic_Field>>("Magnetic Field RS (ENU)", *this);
    m_node_factory.add<Resampler<stream::IECEF_Magnetic_Field>>("Magnetic Field RS (ECEF)", *this);
    m_node_factory.add<Resampler<stream::IPressure>>("Pressure RS", *this);
    m_node_factory.add<Resampler<stream::ITemperature>>("Temperature RS", *this);
    m_node_factory.add<Resampler<stream::IGimbal_Frame>>("Gimbal Frame RS", *this);
    m_node_factory.add<Resampler<stream::IFrame>>("UAV Frame RS", *this);
//    m_node_factory.add<Resampler<stream::IENU_Frame>>("Frame RS (ENU)", *this);
    m_node_factory.add<Resampler<stream::IPWM>>("PWM RS", *this);
    m_node_factory.add<Resampler<stream::IFloat>>("Float RS", *this);
    m_node_factory.add<Resampler<stream::IForce>>("Force RS", *this);
    m_node_factory.add<Resampler<stream::IENU_Force>>("Force RS (ENU)", *this);
    m_node_factory.add<Resampler<stream::IECEF_Force>>("Force RS (ECEF)", *this);
    m_node_factory.add<Resampler<stream::ITorque>>("Torque RS", *this);
    m_node_factory.add<Resampler<stream::IENU_Torque>>("Torque RS (ENU)", *this);
    m_node_factory.add<Resampler<stream::IECEF_Torque>>("Torque RS (ECEF)", *this);
    m_node_factory.add<Resampler<stream::IVelocity>>("Velocity RS", *this);
    m_node_factory.add<Resampler<stream::IENU_Velocity>>("Velocity RS (ENU)", *this);
    m_node_factory.add<Resampler<stream::IECEF_Velocity>>("Velocity RS (ECEF)", *this);
    m_node_factory.add<Resampler<stream::IMultirotor_Commands>>("Multirotor Commands", *this);
    m_node_factory.add<Resampler<stream::IMultirotor_State>>("Multirotor State", *this);
    m_node_factory.add<Resampler<stream::IProximity>>("Proximity RS", *this);
    m_node_factory.add<Resampler<stream::IGPS_Info>>("GPS Info RS", *this);

    m_node_factory.add<Transformer<stream::IECEF_Acceleration, stream::IENU_Acceleration, stream::IENU_Frame>>("Acceleration (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Acceleration, stream::IAcceleration, stream::IFrame>>("Acceleration (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Acceleration, stream::IECEF_Acceleration, stream::IENU_Frame>>("Acceleration (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::IAcceleration, stream::IENU_Acceleration, stream::IFrame>>("Acceleration (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Angular_Velocity, stream::IENU_Angular_Velocity, stream::IENU_Frame>>("Angular Velocity (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Angular_Velocity, stream::IAngular_Velocity, stream::IFrame>>("Angular Velocity (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Angular_Velocity, stream::IECEF_Angular_Velocity, stream::IENU_Frame>>("Angular Velocity (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::IAngular_Velocity, stream::IENU_Angular_Velocity, stream::IFrame>>("Angular Velocity (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Magnetic_Field, stream::IENU_Magnetic_Field, stream::IENU_Frame>>("Magnetic Field (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Magnetic_Field, stream::IMagnetic_Field, stream::IFrame>>("Magnetic Field (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Magnetic_Field, stream::IECEF_Magnetic_Field, stream::IENU_Frame>>("Magnetic Field (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::IMagnetic_Field, stream::IENU_Magnetic_Field, stream::IFrame>>("Magnetic Field (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Linear_Acceleration, stream::IENU_Linear_Acceleration, stream::IENU_Frame>>("Linear Acceleration (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Linear_Acceleration, stream::ILinear_Acceleration, stream::IFrame>>("Linear Acceleration (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Linear_Acceleration, stream::IECEF_Linear_Acceleration, stream::IENU_Frame>>("Linear Acceleration (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::ILinear_Acceleration, stream::IENU_Linear_Acceleration, stream::IFrame>>("Linear Acceleration (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Distance, stream::IENU_Distance, stream::IENU_Frame>>("Distance (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Distance, stream::IDistance, stream::IFrame>>("Distance (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Distance, stream::IECEF_Distance, stream::IENU_Frame>>("Distance (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::IDistance, stream::IENU_Distance, stream::IFrame>>("Distance (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Force, stream::IENU_Force, stream::IENU_Frame>>("Force (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Force, stream::IForce, stream::IFrame>>("Force (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Force, stream::IECEF_Force, stream::IENU_Frame>>("Force (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::IForce, stream::IENU_Force, stream::IFrame>>("Force (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Torque, stream::IENU_Torque, stream::IENU_Frame>>("Torque (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Torque, stream::ITorque, stream::IFrame>>("Torque (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Torque, stream::IECEF_Torque, stream::IENU_Frame>>("Torque (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::ITorque, stream::IENU_Torque, stream::IFrame>>("Torque (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Velocity, stream::IENU_Velocity, stream::IENU_Frame>>("Velocity (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Velocity, stream::IVelocity, stream::IFrame>>("Velocity (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Velocity, stream::IECEF_Velocity, stream::IENU_Frame>>("Velocity (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::IVelocity, stream::IENU_Velocity, stream::IFrame>>("Velocity (Local->ENU)", *this);

    m_node_factory.add<Motor_Mixer>("Motor Mixer", *this);
    m_node_factory.add<Quad_Multirotor_Motor_Mixer>("Quad Multirotor Motor Mixer", *this);
    m_node_factory.add<Servo_Gimbal>("Servo Gimbal", *this);

    m_node_factory.add<Rate_Controller>("Rate Controller", *this);


    //clear
    m_streams.remove_all();
    m_nodes.remove_all();

    std::string data;

    std::string settings_path = s_program_path + "/" + k_settings_filename;
    {
        //read the data
        std::ifstream fs(settings_path, std::ifstream::in | std::ifstream::binary);
        if (!fs.is_open())
        {
            QLOGW("Failed to load '{}'", settings_path);
            generate_settings_file();
            return false;
        }

        fs.seekg (0, fs.end);
        size_t size = fs.tellg();
        fs.seekg (0, fs.beg);

        data.resize(size + 1);
        fs.read(&data[0], size);
    }

    ts::Result<ts::sz::Value> json_result = ts::sz::from_json(data);
    if (json_result != ts::success)
    {
        QLOGE("Failed to load '{}': {}", settings_path, json_result.error().what());
        return false;
    }

    hal::Settings settings;
    auto reserialize_result = hal::deserialize(settings, json_result.payload());
    if (reserialize_result != ts::success)
    {
        QLOGE("Failed to deserialize settings: {}", reserialize_result.error().what());
        return false;
    }

    if (settings.get_uav_descriptor())
    {
        auto result = set_uav_descriptor(settings.get_uav_descriptor().get_shared_ptr());
        if (result != ts::success)
        {
            QLOGE("Error setting UAV descriptor: {}", result.error().what());
            return false;
        }
    }

    for (hal::Settings::Bus_Data const& data: settings.get_buses())
    {
        auto result = create_bus(data.get_type(), data.get_name(), *data.get_descriptor());
        if (result != ts::success)
        {
            QLOGE("Failed to create bus {} of type '{}': {}", data.get_name(), data.get_type(), result.error().what());
            return false;
        }
    }
    for (hal::Settings::Node_Data const& data: settings.get_nodes())
    {
        auto result = create_node(data.get_type(), data.get_name(), *data.get_descriptor());
        if (result != ts::success)
        {
            QLOGE("Failed to create node {} of type '{}': {}", data.get_name(), data.get_type(), result.error().what());
            return false;
        }
    }

    //set input paths and configs
    for (hal::Settings::Node_Data const& data: settings.get_nodes())
    {
        std::shared_ptr<node::INode> node = m_nodes.find_by_name<node::INode>(data.get_name());
        if (!node)
        {
            QLOGE("Internal inconsistency, cannot find node {} of type '{}'", data.get_name(), data.get_type());
            return false;
        }

        size_t idx = 0;
        for (std::string const& input_path: data.get_input_paths())
        {
            auto result = node->set_input_stream_path(idx++, input_path);
            if (result != ts::success)
            {
                QLOGE("Failed to set input stream path for node {} of type '{}': {}", data.get_name(), data.get_type(), result.error().what());
                return false;
            }
        }

        auto result = node->set_config(*data.get_config());
        if (result != ts::success)
        {
            QLOGE("Failed to set config for node {} of type '{}': {}", data.get_name(), data.get_type(), result.error().what());
            return false;
        }
    }

    //start the system
    auto now = Clock::now();
    for (auto const& n: m_nodes.get_all())
    {
        auto result = n.ptr->start(now);
        if (result != ts::success)
        {
            QLOGE("Failed to start node {} of type '{}': {}", n.name, n.type, result.error().what());
            return false;
        }
    }

    save_settings();

    return true;
}

void HAL::shutdown()
{
#if defined (RASPBERRY_PI)
    shutdown_bcm();
    shutdown_pigpio();
#endif
}

void HAL::remove_add_nodes()
{
    m_streams.remove_all();
    m_nodes.remove_all();
}

void HAL::generate_settings_file()
{
#if defined RASPBERRY_PI

//    for (size_t i = 0; i < 2; i++)
//    {
//        auto node = m_bus_factory.create_node("SPI Linux");
//        QASSERT(node);
//        rapidjson::Document json;
//        jsonutil::clone_value(json, node->get_init_params(), json.GetAllocator());
//        auto* valj = jsonutil::get_or_add_value(json, q::Path("dev"), rapidjson::Type::kStringType, json.GetAllocator());
//        QASSERT(valj);
//        valj->SetString(q::util::format<std::string>("/dev/spidev0.{}", i), json.GetAllocator());
//        valj = jsonutil::get_or_add_value(json, q::Path("speed"), rapidjson::Type::kNumberType, json.GetAllocator());
//        QASSERT(valj);
//        valj->SetInt(1000000);
//        if (node->init(json))
//        {
//            auto res = m_buses.add(q::util::format<std::string>("spi{}", i), "SPI Linux", node);
//            QASSERT(res);
//        }
//    }
//    for (size_t i = 0; i < 2; i++)
//    {
//        auto node = m_bus_factory.create("SPI BCM");
//        QASSERT(node);
//        rapidjson::Document json;
//        jsonutil::clone_value(json, node->get_init_params(), json.GetAllocator());
//        auto* valj = jsonutil::get_or_add_value(json, q::Path("dev"), rapidjson::Type::kNumberType, json.GetAllocator());
//        QASSERT(valj);
//        valj->SetInt(i);
//        valj = jsonutil::get_or_add_value(json, q::Path("speed"), rapidjson::Type::kNumberType, json.GetAllocator());
//        QASSERT(valj);
//        valj->SetInt(1000000);
//        if (node->init(json))
//        {
//            auto res = m_buses.add(q::util::format<std::string>("spi{}", i), "SPI BCM", node);
//            QASSERT(res);
//        }
//    }

//    {
//        auto node = m_bus_factory.create("I2C Linux");
//        QASSERT(node);
//        rapidjson::Document json;
//        jsonutil::clone_value(json, node->get_init_params(), json.GetAllocator());
//        auto* valj = jsonutil::get_or_add_value(json, q::Path("dev"), rapidjson::Type::kStringType, json.GetAllocator());
//        QASSERT(valj);
//        valj->SetString("/dev/i2c-1");
//        if (node->init(json))
//        {
//            auto res = m_buses.add("i2c1", "I2C Linux", node);
//            QASSERT(res);
//        }
//    }

//    {
//        auto node = m_bus_factory.create("UART Linux");
//        QASSERT(node);
//        rapidjson::Document json;
//        jsonutil::clone_value(json, node->get_init_params(), json.GetAllocator());
//        auto* valj = jsonutil::get_or_add_value(json, q::Path("dev"), rapidjson::Type::kStringType, json.GetAllocator());
//        QASSERT(valj);
//        valj->SetString("/dev/ttyAMA0");
//        valj = jsonutil::get_or_add_value(json, q::Path("baud"), rapidjson::Type::kNumberType, json.GetAllocator());
//        QASSERT(valj);
//        valj->SetInt(115200);
//        if (node->init(json))
//        {
//            auto res = m_buses.add("uart0", "UART Linux", node);
//            QASSERT(res);
//        }
//    }

#else


#endif

    save_settings();
}

//static std::vector<float> s_samples;
//static std::vector<float> s_samples_lpf;

void HAL::process()
{
//    for (auto const& n: m_buses.get_all())
//    {
//        n->process();
//    }

    auto now = Clock::now();

    auto total_start = now;
    auto node_start = total_start;

    for (auto const& n: m_nodes.get_all())
    {
        n.ptr->process();

        auto now = Clock::now();
        Telemetry_Data::Node& node_telemetry = m_telemetry_data.nodes[n.name];
        node_telemetry.crt_process_duration += now - node_start;
        node_telemetry.crt_max_process_duration = std::max(node_telemetry.crt_max_process_duration, now - node_start);
        node_start = now;
    }

    {
        auto dt = Clock::now() - total_start;
        m_telemetry_data.crt_total_duration += dt;
        m_telemetry_data.crt_max_total_duration = std::max(m_telemetry_data.crt_max_total_duration, dt);
    }


    {
        auto now = Clock::now();
        auto dt = now - m_last_telemetry_data_latch_tp;
        if (dt >= std::chrono::milliseconds(100))
        {
            m_last_telemetry_data_latch_tp = now;
            m_telemetry_data.version++;

            float mu = 1.f / std::chrono::duration<float>(dt).count();
            m_telemetry_data.total_duration = std::chrono::duration_cast<Clock::duration>(m_telemetry_data.crt_total_duration * mu);
            m_telemetry_data.max_total_duration = std::chrono::duration_cast<Clock::duration>(m_telemetry_data.crt_max_total_duration);

            m_telemetry_data.crt_total_duration = Clock::duration(0);
            m_telemetry_data.crt_max_total_duration = Clock::duration(0);

            for (auto& pair: m_telemetry_data.nodes)
            {
                Telemetry_Data::Node& node = pair.second;
                node.process_duration = std::chrono::duration_cast<Clock::duration>(node.crt_process_duration * mu);
                node.max_process_duration = std::chrono::duration_cast<Clock::duration>(node.crt_max_process_duration);

                node.crt_process_duration = Clock::duration(0);
                node.crt_max_process_duration = Clock::duration(0);
            }
        }
    }
}



}

