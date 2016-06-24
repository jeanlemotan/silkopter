#include "BrainStdAfx.h"

#include "HAL.h"
#include "Comms.h"
#include "utils/Timed_Scope.h"

/////////////////////////////////////////////////////////////////////////////////////

#include "bus/I2C_Linux.h"
#include "bus/SPI_Linux.h"
#include "bus/I2C_BCM.h"
#include "bus/SPI_BCM.h"
#include "bus/UART_Linux.h"
#include "bus/UART_BBang.h"

#include "source/Raspicam.h"
#include "source/MPU9250.h"
#include "source/MS5611.h"
#include "source/RC5T619.h"
#include "source/ADS1115.h"
#include "source/AVRADC.h"
#include "source/SRF01.h"
#include "source/SRF02.h"
#include "source/MaxSonar.h"
#include "source/UBLOX.h"
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

#include "uav_properties/Tri_Properties.h"
#include "uav_properties/Quad_Properties.h"
#include "uav_properties/Hexa_Properties.h"
#include "uav_properties/Hexatri_Properties.h"
#include "uav_properties/Octo_Properties.h"
#include "uav_properties/Octoquad_Properties.h"

#ifdef RASPBERRY_PI

extern "C"
{
    #include "hw/pigpio.h"
    #include "hw/bcm2835.h"
}

///////////////////////////////////////////////////////////////////

std::chrono::microseconds PIGPIO_PERIOD(5);

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
    bcm2835_spi_end();
    bcm2835_i2c_end();
    bcm2835_close();
    return true;
}

#endif


namespace silk
{

static const q::Path k_settings_path("settings.json");

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
            node_data.get_input_paths().push_back(si.stream_path.get_as<std::string>());
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

        q::data::File_Sink fs(k_settings_path);
        if (fs.is_open())
        {
            fs.write(reinterpret_cast<uint8_t const*>(json.data()), json.size());
            fs.flush();
        }
        else
        {
            QLOGE("Cannot open '{}' to save settings.", k_settings_path);
        }
    }));



//    auto settingsj = std::make_shared<rapidjson::Document>();
//    settingsj->SetObject();

//    auto& allocator = settingsj->GetAllocator();

//    std::shared_ptr<const Multirotor_Config> multirotor_config = get_specialized_uav_descriptor<Multirotor_Config>();
//    if (multirotor_config)
//    {
//        auto* configj = jsonutil::get_or_add_value(*settingsj, q::Path("uav/multirotor_config"), rapidjson::kObjectType, allocator);
//        if (!configj)
//        {
//            QLOGE("Cannot create multirotor config node.");
//            return;
//        }
//        rapidjson::Document json;
//        autojsoncxx::to_document(*multirotor_config, json);
//        jsonutil::clone_value(*configj, json, allocator);
//    }

//    {
//        auto busesj = jsonutil::get_or_add_value(*settingsj, q::Path("uav/buses"), rapidjson::kObjectType, allocator);
//        if (!busesj)
//        {
//            QLOGE("Cannot create buses settings node.");
//            return;
//        }
////        auto const& nodes = get_buses().get_all();
////        for (auto const& n: nodes)
////        {
////            if (!jsonutil::add_value(*busesj, q::Path(n.name + "/type"), rapidjson::Value(n.type.c_str(), n.type.size(), allocator), allocator) ||
////                !jsonutil::add_value(*busesj, q::Path(n.name + "/init_params"), jsonutil::clone_value(n.ptr->get_init_params(), allocator), allocator) ||
////                !jsonutil::add_value(*busesj, q::Path(n.name + "/config"), jsonutil::clone_value(n.ptr->get_config(), allocator), allocator))
////            {
////                QLOGE("Cannot create settings node.");
////                return;
////            }
////        }
//    }
//    {
//        auto nodesj = jsonutil::get_or_add_value(*settingsj, q::Path("uav/nodes"), rapidjson::kArrayType, allocator);
//        if (!nodesj)
//        {
//            QLOGE("Cannot create nodes settings node.");
//            return;
//        }
//        auto const& nodes = get_nodes().get_all();
//        for (auto const& n: nodes)
//        {
//            rapidjson::Value nodej;
//            nodej.SetObject();

//            rapidjson::Value input_pathsj;
//            {
//                auto inputs = n.ptr->get_inputs();
//                input_pathsj.SetArray();
//                for (auto const& si: inputs)
//                {
//                    input_pathsj.PushBack(rapidjson::Value(si.stream_path.get_as<std::string>(), allocator), allocator);
//                }
//            }

////            if (!jsonutil::add_value(nodej, std::string("name"), rapidjson::Value(n.name.c_str(), n.name.size(), allocator), allocator) ||
////                !jsonutil::add_value(nodej, std::string("type"), rapidjson::Value(n.type.c_str(), n.type.size(), allocator), allocator) ||
////                !jsonutil::add_value(nodej, std::string("init_params"), jsonutil::clone_value(n.ptr->get_init_params(), allocator), allocator) ||
////                !jsonutil::add_value(nodej, std::string("config"), jsonutil::clone_value(n.ptr->get_config(), allocator), allocator) ||
////                !jsonutil::add_value(nodej, std::string("input_paths"), std::move(input_pathsj), allocator))
////            {
////                QLOGE("Cannot create settings node.");
////                return;
////            }

//            nodesj->PushBack(nodej, allocator);
//        }
//    }

//    silk::async(std::function<void()>([=]()
//    {
//        TIMED_FUNCTION();

////        rapidjson::StringBuffer s;
////        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
////        settingsj->Accept(writer);    // Accept() traverses the DOM and generates Handler events.
////        q::data::File_Sink fs(k_settings_path);
////        if (fs.is_open())
////        {
////            fs.write(reinterpret_cast<uint8_t const*>(s.GetString()), s.GetSize());
////        }
////        else
////        {
////            QLOGE("Cannot open '{}' to save settings.", k_settings_path);
////        }
//    }));

    //autojsoncxx::to_pretty_json_file("sensors_pi.cfg", config);
}

auto HAL::get_telemetry_data() const -> Telemetry_Data const&
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
auto HAL::set_uav_descriptor(std::shared_ptr<const hal::IUAV_Descriptor> descriptor) -> bool
{
    if (!descriptor)
    {
        QLOGE("Cannot set null descriptor");
        return false;
    }

    std::shared_ptr<IUAV_Properties> new_properties;

    if (auto* d = dynamic_cast<hal::Tri_Descriptor const*>(descriptor.get()))
    {
        QLOGE("Tri not supported");
        return false;
    }
    else if (auto* d = dynamic_cast<hal::Quad_Descriptor const*>(descriptor.get()))
    {
        std::shared_ptr<Quad_Properties> p = std::make_shared<Quad_Properties>();
        if (!p->init(*d))
        {
            return false;
        }
        new_properties = p;
    }
    else if (auto* d = dynamic_cast<hal::Hexa_Descriptor const*>(descriptor.get()))
    {
        QLOGE("Hexa not supported");
        return false;
    }
    else if (auto* d = dynamic_cast<hal::Hexatri_Descriptor const*>(descriptor.get()))
    {
        QLOGE("Hexatri not supported");
        return false;
    }
    else if (auto* d = dynamic_cast<hal::Octo_Descriptor const*>(descriptor.get()))
    {
        QLOGE("Octo not supported");
        return false;
    }
    else if (auto* d = dynamic_cast<hal::Octaquad_Descriptor const*>(descriptor.get()))
    {
        QLOGE("Octaquad not supported");
        return false;
    }
    else if (auto* d = dynamic_cast<hal::Custom_Multirotor_Descriptor const*>(descriptor.get()))
    {
        QLOGE("Custom multirotor not supported");
        return false;
    }
    else
    {
        QLOGE("Unknown multirotor descriptor type");
        return false;
    }

    m_uav_descriptor = descriptor;
    std::swap(m_uav_properties, new_properties);

    uav_properties_changed_signal.execute(*this);

    return true;
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
    q::data::File_Sink fs((q::Path(name)));
    std::string header("#x y\n");
    fs.write((uint8_t const*)header.c_str(), header.size());

    for(size_t i = 0; i < samples.size(); i++)
    {
        auto l = q::util::format<std::string>("{} {.8}\n", i, samples[i]);
        fs.write((uint8_t const*)l.c_str(), l.size());
    }
}

auto HAL::create_bus(std::string const& type, std::string const& name, hal::IBus_Descriptor const& descriptor) -> std::shared_ptr<bus::IBus>
{
    if (m_buses.find_by_name<bus::IBus>(name))
    {
        QLOGE("Bus '{}' already exist", name);
        return std::shared_ptr<bus::IBus>();
    }
    auto node = m_bus_factory.create(type);
    if (node && node->init(descriptor))
    {
        auto res = m_buses.add(name, type, node); //this has to succeed since we already tested for duplicate names
        QASSERT(res);
        return node;
    }
    return std::shared_ptr<bus::IBus>();
}
auto HAL::create_node(std::string const& type, std::string const& name, hal::INode_Descriptor const& descriptor) -> std::shared_ptr<node::INode>
{
    if (m_nodes.find_by_name<node::INode>(name))
    {
        QLOGE("Node '{}' already exist", name);
        return std::shared_ptr<node::INode>();
    }
    auto node = m_node_factory.create(type);
    if (node && node->init(descriptor))
    {
        node->set_config(*node->get_config());//apply default config
        auto res = m_nodes.add(name, type, node); //this has to succeed since we already tested for duplicate names
        QASSERT(res);
        auto outputs = node->get_outputs();
        for (auto const& x: outputs)
        {
            std::string stream_name = q::util::format<std::string>("{}/{}", name, x.name);
            if (!m_streams.add(stream_name, std::string(), x.stream))
            {
                QLOGE("Cannot add stream '{}'", stream_name);
                return std::shared_ptr<node::INode>();
            }
        }
        return node;
    }
    return std::shared_ptr<node::INode>();
}

//auto hal::create_buses(rapidjson::Value& json) -> bool
//{
//    if (!json.IsObject())
//    {
//        QLOGE("Wrong json type: {}", json.GetType());
//        return false;
//    }
//    auto it = json.MemberBegin();
//    for (; it != json.MemberEnd(); ++it)
//    {
//        std::string name(it->name.GetString());
//        auto* typej = jsonutil::find_value(it->value, std::string("type"));
//        if (!typej || typej->GetType() != rapidjson::kStringType)
//        {
//            QLOGE("Node {} is missing the type", name);
//            return false;
//        }
//        std::string type(typej->GetString());
//        auto* init_paramsj = jsonutil::find_value(it->value, std::string("init_params"));
//        if (!init_paramsj)
//        {
//            QLOGE("Node {} of type {} is missing the init_params", name, type);
//            return false;
//        }
//        auto node = create_bus(type, name, *init_paramsj);
//        if (!node)
//        {
//            QLOGE("Failed to create node {} of type '{}'", name, type);
//            return false;
//        }
//    }
//    return true;
//}

//static bool read_input_stream_paths(std::string const& node_name, silk::node::INode& node, rapidjson::Value const& value)
//{
//    auto* input_pathsj = jsonutil::find_value(value, std::string("input_paths"));
//    if (!input_pathsj || !input_pathsj->IsArray())
//    {
//        QLOGE("Node {} is missing the stream input paths", node_name);
//        return false;
//    }
//    size_t input_idx = 0;
//    for (auto it = input_pathsj->Begin(); it != input_pathsj->End(); ++it, input_idx++)
//    {
//        if (!it->IsString())
//        {
//            QLOGE("Node {} has a bad stream input paths", node_name);
//            return false;
//        }
//        node.set_input_stream_path(input_idx, q::Path(it->GetString()));
//    }
//    return true;
//}

//auto hal::create_nodes(rapidjson::Value& json) -> bool
//{
//    if (!json.IsArray())
//    {
//        QLOGE("Wrong json type: {}", json.GetType());
//        return false;
//    }
//    for (auto it = json.Begin(); it != json.End(); ++it)
//    {
//        auto* namej = jsonutil::find_value(*it, std::string("name"));
//        if (!namej || namej->GetType() != rapidjson::kStringType)
//        {
//            QLOGE("Node is missing the name");
//            return false;
//        }
//        std::string name(namej->GetString());

//        auto* typej = jsonutil::find_value(*it, std::string("type"));
//        if (!typej || typej->GetType() != rapidjson::kStringType)
//        {
//            QLOGE("Node {} is missing the type", name);
//            return false;
//        }
//        std::string type(typej->GetString());

//        auto* init_paramsj = jsonutil::find_value(*it, std::string("init_params"));
//        if (!init_paramsj)
//        {
//            QLOGE("Node {} of type {} is missing the init_params", name, type);
//            return false;
//        }
//        auto node = create_node(type, name, *init_paramsj);
//        if (!node)
//        {
//            QLOGE("Failed to create node {} of type '{}'", name, type);
//            return false;
//        }
//    }

//    auto nodes = m_nodes.get_all();
//    for (auto it = json.Begin(); it != json.End(); ++it)
//    {
//        const auto& item = nodes[std::distance(json.Begin(), it)];
//        const std::string& name = item.name;
//        std::shared_ptr<node::INode> node = item.ptr;
//        QASSERT(node);

//        if (!read_input_stream_paths(name, *node, *it))
//        {
//            return false;
//        }

//        auto* configj = jsonutil::find_value(*it, std::string("config"));
//        if (!configj)
//        {
//            QLOGE("Node {} is missing the config", name);
//            return false;
//        }
////        if (!node->set_config(*configj))
////        {
////            QLOGE("Failed to set config for node '{}'", name);
////            return false;
////        }
//    }
//    return true;
//}

void HAL::sort_nodes(std::shared_ptr<node::INode> first_node)
{
    QASSERT(first_node);
    if (!first_node)
    {
        return;
    }

    typedef Registry<node::INode>::Item Item;

    std::vector<Item> items = m_nodes.get_all();
    auto it = std::find_if(items.begin(), items.end(), [first_node](const Item& item) { return item.ptr == first_node; });
    QASSERT(it != items.end());
    if (it == items.end())
    {
        return;
    }

    std::vector<Item> sorted;
    sorted.reserve(items.size() * 2); //to avoid allocations

    Item first_item = *it;
    items.erase(it);

    sorted.push_back(first_item);
    for (auto it = sorted.begin(); it != sorted.end(); ++it)
    {
        const std::string& node_name = it->name;

        //now find all the nodes that this node uses as input
        if (it->ptr != first_node)
        {
            std::vector<node::INode::Input> inputs = it->ptr->get_inputs();
            for (const node::INode::Input& input : inputs)
            {
                if (input.stream_path.empty())
                {
                    continue;
                }
                for (auto nit = items.begin(); nit != items.end();)
                {
                    if (input.stream_path[0] == nit->name)
                    {
                        sorted.push_back(*nit);
                        items.erase(nit);
                    }
                    else
                    {
                        ++nit;
                    }
                }
            }
        }

        //now find all the nodes that use this node as an input
        for (auto nit = items.begin(); nit != items.end();)
        {
            bool found = false;
            std::vector<node::INode::Input> inputs = nit->ptr->get_inputs();
            for (const node::INode::Input& input : inputs)
            {
                if (!input.stream_path.empty() && input.stream_path[0] == node_name)
                {
                    sorted.push_back(*nit);
                    items.erase(nit);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                ++nit;
            }
        }
    }

    //add all the remainding nodes
    for (Item const& item: items)
    {
        sorted.push_back(item);
    }

    m_nodes.set_all(sorted);
}


auto HAL::init(Comms& comms) -> bool
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

    m_bus_factory.add<bus::UART_Linux>("UART Linux");
    m_bus_factory.add<bus::UART_BBang>("UART BBang");
    m_bus_factory.add<bus::I2C_Linux>("I2C Linux");
    m_bus_factory.add<bus::SPI_Linux>("SPI Linux");
    m_bus_factory.add<bus::I2C_BCM>("I2C BCM");
    m_bus_factory.add<bus::SPI_BCM>("SPI BCM");

#if !defined RASPBERRY_PI
    m_node_factory.add<Multirotor_Simulator>("Multirotor Simulator", *this);
#endif

    //m_node_factory.add<EHealth>("EHealth", *this);

    m_node_factory.add<MPU9250>("MPU9250", *this);
    m_node_factory.add<MS5611>("MS5611", *this);
    m_node_factory.add<SRF01>("SRF01", *this);
    m_node_factory.add<SRF02>("SRF02", *this);
    m_node_factory.add<MaxSonar>("MaxSonar", *this);
    m_node_factory.add<Raspicam>("Raspicam", *this);
    m_node_factory.add<RC5T619>("RC5T619", *this);
    m_node_factory.add<ADS1115>("ADS1115", *this);
    m_node_factory.add<AVRADC>("AVRADC", *this);
    m_node_factory.add<UBLOX>("UBLOX", *this);

    m_node_factory.add<PIGPIO>("PIGPIO", *this);
    m_node_factory.add<PCA9685>("PCA9685", *this);

    m_node_factory.add<Multirotor_Brain>("Multirotor Brain", *this);
    m_node_factory.add<Multirotor_Pilot>("Multirotor Pilot", *this, comms);

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
    m_node_factory.add<Resampler<stream::IUAV_Frame>>("UAV Frame RS", *this);
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
    m_node_factory.add<Transformer<stream::IENU_Acceleration, stream::IAcceleration, stream::IUAV_Frame>>("Acceleration (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Acceleration, stream::IECEF_Acceleration, stream::IENU_Frame>>("Acceleration (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::IAcceleration, stream::IENU_Acceleration, stream::IUAV_Frame>>("Acceleration (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Angular_Velocity, stream::IENU_Angular_Velocity, stream::IENU_Frame>>("Angular Velocity (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Angular_Velocity, stream::IAngular_Velocity, stream::IUAV_Frame>>("Angular Velocity (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Angular_Velocity, stream::IECEF_Angular_Velocity, stream::IENU_Frame>>("Angular Velocity (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::IAngular_Velocity, stream::IENU_Angular_Velocity, stream::IUAV_Frame>>("Angular Velocity (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Magnetic_Field, stream::IENU_Magnetic_Field, stream::IENU_Frame>>("Magnetic Field (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Magnetic_Field, stream::IMagnetic_Field, stream::IUAV_Frame>>("Magnetic Field (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Magnetic_Field, stream::IECEF_Magnetic_Field, stream::IENU_Frame>>("Magnetic Field (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::IMagnetic_Field, stream::IENU_Magnetic_Field, stream::IUAV_Frame>>("Magnetic Field (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Linear_Acceleration, stream::IENU_Linear_Acceleration, stream::IENU_Frame>>("Linear Acceleration (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Linear_Acceleration, stream::ILinear_Acceleration, stream::IUAV_Frame>>("Linear Acceleration (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Linear_Acceleration, stream::IECEF_Linear_Acceleration, stream::IENU_Frame>>("Linear Acceleration (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::ILinear_Acceleration, stream::IENU_Linear_Acceleration, stream::IUAV_Frame>>("Linear Acceleration (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Distance, stream::IENU_Distance, stream::IENU_Frame>>("Distance (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Distance, stream::IDistance, stream::IUAV_Frame>>("Distance (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Distance, stream::IECEF_Distance, stream::IENU_Frame>>("Distance (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::IDistance, stream::IENU_Distance, stream::IUAV_Frame>>("Distance (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Force, stream::IENU_Force, stream::IENU_Frame>>("Force (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Force, stream::IForce, stream::IUAV_Frame>>("Force (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Force, stream::IECEF_Force, stream::IENU_Frame>>("Force (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::IForce, stream::IENU_Force, stream::IUAV_Frame>>("Force (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Torque, stream::IENU_Torque, stream::IENU_Frame>>("Torque (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Torque, stream::ITorque, stream::IUAV_Frame>>("Torque (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Torque, stream::IECEF_Torque, stream::IENU_Frame>>("Torque (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::ITorque, stream::IENU_Torque, stream::IUAV_Frame>>("Torque (Local->ENU)", *this);

    m_node_factory.add<Transformer<stream::IECEF_Velocity, stream::IENU_Velocity, stream::IENU_Frame>>("Velocity (ECEF->ENU)", *this);
    m_node_factory.add<Transformer<stream::IENU_Velocity, stream::IVelocity, stream::IUAV_Frame>>("Velocity (ENU->Local)", *this);
    m_node_factory.add<Transformer_Inv<stream::IENU_Velocity, stream::IECEF_Velocity, stream::IENU_Frame>>("Velocity (ENU->ECEF)", *this);
    m_node_factory.add<Transformer_Inv<stream::IVelocity, stream::IENU_Velocity, stream::IUAV_Frame>>("Velocity (Local->ENU)", *this);

    m_node_factory.add<Motor_Mixer>("Motor Mixer", *this);
    m_node_factory.add<Servo_Gimbal>("Servo Gimbal", *this);

    m_node_factory.add<Rate_Controller>("Rate Controller", *this);


    //clear
    m_streams.remove_all();
    m_nodes.remove_all();

    //read the data
    q::data::File_Source fs(k_settings_path);
    if (!fs.is_open())
    {
        QLOGW("Failed to load '{}'", k_settings_path);
        generate_settings_file();
        return false;
    }

    std::string data = q::data::read_whole_source_as_string<std::string>(fs);

    ts::Result<ts::sz::Value> json_result = ts::sz::from_json(data);
    if (json_result != ts::success)
    {
        QLOGE("Failed to load '{}': {}", k_settings_path, json_result.error().what());
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
        if (!set_uav_descriptor(settings.get_uav_descriptor().get_shared_ptr()))
        {
            return false;
        }
    }

    for (hal::Settings::Bus_Data const& data: settings.get_buses())
    {
        if (!create_bus(data.get_type(), data.get_name(), *data.get_descriptor()))
        {
            QLOGE("Failed to create bus {} of type '{}'", data.get_name(), data.get_type());
            return false;
        }
    }
    for (hal::Settings::Node_Data const& data: settings.get_nodes())
    {
        if (!create_node(data.get_type(), data.get_name(), *data.get_descriptor()))
        {
            QLOGE("Failed to create node {} of type '{}'", data.get_name(), data.get_type());
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
            node->set_input_stream_path(idx++, q::Path(input_path));
        }

        if (!node->set_config(*data.get_config()))
        {
            QLOGE("Failed to set config for node {} of type '{}'", data.get_name(), data.get_type());
            return false;
        }
    }

    //start the system
    auto now = q::Clock::now();
    for (auto const& n: m_nodes.get_all())
    {
        if (!n.ptr->start(now))
        {
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

    auto now = q::Clock::now();
    auto dt = now - m_last_process_tp;
    m_telemetry_data.rate = dt.count() > 0 ? 1.f / std::chrono::duration<float>(dt).count() : 0.f;


    auto total_start = now;
    auto node_start = total_start;

    for (auto const& n: m_nodes.get_all())
    {
        n.ptr->process();

        auto now = q::Clock::now();
        m_telemetry_data.nodes[n.name].process_duration = now - node_start;
        node_start = now;
    }

    m_telemetry_data.total_duration = q::Clock::now() - total_start;
    //calculate percentages
    for (auto& nt: m_telemetry_data.nodes)
    {
        float p = std::chrono::duration<float>(nt.second.process_duration).count() / std::chrono::duration<float>(m_telemetry_data.total_duration).count();
        nt.second.process_percentage = math::lerp(nt.second.process_percentage, p, 0.1f);
    }

    //display time
//    for (auto const& n: m_nodes.get_all())
//    {
//        auto& nt = m_telemetry_data.nodes[n.name];
//        QLOGI("{.2}%, {}/{} -> {}", nt.process_percentage*100.f, nt.process_duration, m_telemetry_data.total_duration, n.name);
//    }

}



}

