#pragma once

#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IADC.h"
#include "common/node/stream/IFloat.h"
#include "common/node/stream/IBool.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IBattery_State.h"
#include "common/node/stream/ICurrent.h"
#include "common/node/stream/IDistance.h"
#include "common/node/stream/IForce.h"
#include "common/node/stream/IFrame.h"
#include "common/node/stream/IGPS_Info.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/IPosition.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/IPressure.h"
#include "common/node/stream/IPWM.h"
#include "common/node/stream/ITemperature.h"
#include "common/node/stream/IThrottle.h"
#include "common/node/stream/ITorque.h"
#include "common/node/stream/IVelocity.h"
#include "common/node/stream/IVideo.h"
#include "common/node/stream/IVoltage.h"
#include "common/node/stream/IProximity.h"
#include "common/node/stream/IMulti_Input.h"
#include "common/node/stream/IMulti_State.h"

#include "common/node/INode.h"
#include "common/node/IPilot.h"

#include "common/config/Multi.h"
#include "common/Comm_Data.h"
#include "common/Manual_Clock.h"

#include "utils/Json_Util.h"

namespace silk
{

class HAL;
class Comms;




namespace node
{
struct Node;
DECLARE_CLASS_PTR(Node);


namespace stream
{

struct Stream
{
    Node_wptr node;
    std::string name;
    stream::Type type;
    uint32_t rate = 0;
    int telemetry_active_req = 0;
    bool is_telemetry_active = false;
};
DECLARE_CLASS_PTR(Stream);

struct Acceleration : public Stream
{
    typedef IAcceleration::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Acceleration&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Acceleration);
struct ENU_Acceleration : public Stream
{
    typedef IENU_Acceleration::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ENU_Acceleration&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ENU_Acceleration);
struct ECEF_Acceleration : public Stream
{
    typedef IECEF_Acceleration::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ECEF_Acceleration&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ECEF_Acceleration);

struct ADC : public Stream
{
    typedef IADC::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ADC&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ADC);

struct Angular_Velocity : public Stream
{
    typedef IAngular_Velocity::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Angular_Velocity&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Angular_Velocity);
struct ENU_Angular_Velocity : public Stream
{
    typedef IENU_Angular_Velocity::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ENU_Angular_Velocity&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ENU_Angular_Velocity);
struct ECEF_Angular_Velocity : public Stream
{
    typedef IECEF_Angular_Velocity::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ECEF_Angular_Velocity&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ECEF_Angular_Velocity);

struct Battery_State : public Stream
{
    typedef IBattery_State::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Battery_State&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Battery_State);

struct Current : public Stream
{
    typedef ICurrent::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Current&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Current);

struct Distance : public Stream
{
    typedef IDistance::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Distance&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Distance);
struct ENU_Distance : public Stream
{
    typedef IENU_Distance::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ENU_Distance&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ENU_Distance);
struct ECEF_Distance : public Stream
{
    typedef IECEF_Distance::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ECEF_Distance&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ECEF_Distance);

struct Float : public Stream
{
    typedef IFloat::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Float&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Float);

struct Bool : public Stream
{
    typedef IBool::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Bool&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Float);

struct Force : public Stream
{
    typedef IForce::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Force&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Force);
struct ENU_Force : public Stream
{
    typedef IENU_Force::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ENU_Force&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ENU_Force);
struct ECEF_Force : public Stream
{
    typedef IECEF_Force::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ECEF_Force&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ECEF_Force);

struct Frame : public Stream
{
    typedef IFrame::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Frame&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Frame);
struct ENU_Frame : public Stream
{
    typedef IENU_Frame::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ENU_Frame&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ENU_Frame);

struct GPS_Info : public Stream
{
    typedef IGPS_Info::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(GPS_Info&)> samples_available_signal;
};
DECLARE_CLASS_PTR(GPS_Info);

struct Linear_Acceleration : public Stream
{
    typedef ILinear_Acceleration::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Linear_Acceleration&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Linear_Acceleration);
struct ENU_Linear_Acceleration : public Stream
{
    typedef IENU_Linear_Acceleration::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ENU_Linear_Acceleration&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ENU_Linear_Acceleration);
struct ECEF_Linear_Acceleration : public Stream
{
    typedef IECEF_Linear_Acceleration::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ECEF_Linear_Acceleration&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ECEF_Linear_Acceleration);

struct ECEF_Position : public Stream
{
    typedef IECEF_Position::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ECEF_Position&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ECEF_Position);

struct Magnetic_Field : public Stream
{
    typedef IMagnetic_Field::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Magnetic_Field&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Magnetic_Field);
struct ENU_Magnetic_Field : public Stream
{
    typedef IENU_Magnetic_Field::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ENU_Magnetic_Field&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ENU_Magnetic_Field);
struct ECEF_Magnetic_Field : public Stream
{
    typedef IECEF_Magnetic_Field::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ECEF_Magnetic_Field&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ECEF_Magnetic_Field);

struct Pressure : public Stream
{
    typedef IPressure::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Pressure&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Pressure);

struct PWM : public Stream
{
    typedef IPWM::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(PWM&)> samples_available_signal;
};
DECLARE_CLASS_PTR(PWM);

struct Temperature : public Stream
{
    typedef ITemperature::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Temperature&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Temperature);

struct Throttle : public Stream
{
    typedef IThrottle::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Throttle&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Throttle);

struct Torque : public Stream
{
    typedef ITorque::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Torque&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Torque);
struct ENU_Torque : public Stream
{
    typedef IENU_Torque::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ENU_Torque&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ENU_Torque);
struct ECEF_Torque : public Stream
{
    typedef IECEF_Torque::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ECEF_Torque&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ECEF_Torque);

struct Velocity : public Stream
{
    typedef IVelocity::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Velocity&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Velocity);
struct ENU_Velocity : public Stream
{
    typedef IENU_Velocity::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ENU_Velocity&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ENU_Velocity);
struct ECEF_Velocity : public Stream
{
    typedef IECEF_Velocity::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ECEF_Velocity&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ECEF_Velocity);

struct Voltage : public Stream
{
    typedef IVoltage::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Voltage&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Voltage);

struct Video : public Stream
{
    typedef IVideo::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Video&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Video);

struct Multi_State : public Stream
{
    typedef IMulti_State::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Multi_State&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Multi_State);

struct Multi_Input : public Stream
{
    typedef IMulti_Input::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Multi_Input&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Multi_Input);

struct Proximity : public Stream
{
    typedef IProximity::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Proximity&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Proximity);

}




struct Node_Def
{
    std::string name;
    node::Type type;
    rapidjson::Document default_init_params;
    //rapidjson::Document default_config; this is not relevant as nodes have to initialize forst in order to have a valid config

    struct Input
    {
        stream::Type type;
        std::string name;
        uint32_t rate = 0;
    };
    std::vector<Input> inputs;
    struct Output
    {
        stream::Type type;
        std::string name;
        uint32_t rate = 0;
    };
    std::vector<Output> outputs;
};
DECLARE_CLASS_PTR(Node_Def);

struct Node
{
    std::string name;
    node::Type type;

    rapidjson::Document init_params;
    rapidjson::Document config;

    struct Input
    {
        q::Path stream_path;
        stream::Stream_wptr stream;
        stream::Type type;
        std::string name;
        uint32_t rate = 0;
    };
    std::vector<Input> inputs;
    struct Output
    {
        stream::Stream_ptr stream;
        stream::Type type;
        std::string name;
        uint32_t rate = 0;
    };
    std::vector<Output> outputs;

    q::util::Signal<void()> changed_signal;
    q::util::Signal<void(rapidjson::Document const& json)> message_received_signal;
};
DECLARE_CLASS_PTR(Node);

}



template<class Base>
class Registry : q::util::Noncopyable
{
public:
    auto get_all() const -> std::vector<std::shared_ptr<Base>> const&;
    auto find_by_name(std::string const& name) const -> std::shared_ptr<Base>;
    template<class T> auto add(std::shared_ptr<T> node) -> bool;
    void remove(std::shared_ptr<Base> node);
    void remove_all();

    mutable q::util::Signal<void(std::shared_ptr<Base>)> item_added_signal;
    mutable q::util::Signal<void(std::shared_ptr<Base>)> item_will_be_removed_signal;
private:
    std::vector<std::shared_ptr<Base>> m_nodes;
};


class HAL : q::util::Noncopyable
{
    friend class Comms;
public:
    HAL(Comms& comms);
    ~HAL();

    auto get_multi_config() const   -> boost::optional<config::Multi>;
    void set_multi_config(config::Multi const& config);

    q::util::Signal<void()> multi_config_refreshed_signal;


    auto get_node_defs() const      -> Registry<node::Node_Def> const&;
    auto get_nodes() const          -> Registry<node::Node> const&;
    auto get_streams() const        -> Registry<node::stream::Stream> const&;

    enum class Result
    {
        OK,
        FAILED,
        TIMEOUT
    };

    void add_node(std::string const& def_name, std::string const& name, rapidjson::Document const& init_params);
    void remove_node(node::Node_ptr node);
    void set_node_input_stream_path(node::Node_ptr node, std::string const& input_name, q::Path const& stream_path);
    void set_node_config(node::Node_ptr node, rapidjson::Document const& config);
    void set_stream_telemetry_active(std::string const& stream_name, bool active);
    void send_node_message(node::Node_ptr node, rapidjson::Document const& json);

    auto get_remote_clock() const -> Manual_Clock const&;

protected:
    Manual_Clock m_remote_clock;
    Comms& m_comms;

    struct Configs
    {
        boost::optional<config::Multi> multi;
    } m_configs;

    Registry<node::Node_Def> m_node_defs;
    Registry<node::Node> m_nodes;
    Registry<node::stream::Stream> m_streams;

private:

};


/////////////////////////////////////////////////////////////////////////////////////////////////


template<class Base>
auto Registry<Base>::get_all() const -> std::vector<std::shared_ptr<Base>> const&
{
    return m_nodes;
}
template<class Base>
auto Registry<Base>::find_by_name(std::string const& name) const -> std::shared_ptr<Base>
{
    auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](std::shared_ptr<Base> const& s) { return s->name == name; });
    return it != m_nodes.end() ? *it : std::shared_ptr<Base>();
}
template<class Base>
template<class T>
auto Registry<Base>::add(std::shared_ptr<T> node) -> bool
{
    QASSERT(node);
    if (find_by_name(node->name))
    {
        QLOGE("Duplicated name in node {}", node->name);
        return false;
    }
    m_nodes.push_back(node);
    item_added_signal.execute(node);
    return true;
}
template<class Base>
void Registry<Base>::remove(std::shared_ptr<Base> node)
{
    auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](std::shared_ptr<Base> const& s) { return s == node; });
    if (it == m_nodes.end())
    {
        QLOGE("Cannot find node {}", node->name);
        return;
    }
    item_will_be_removed_signal.execute(node);
    m_nodes.erase(it);
}
template<class Base>
void Registry<Base>::remove_all()
{
    m_nodes.clear();
}




}
