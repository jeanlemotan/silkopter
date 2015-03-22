#pragma once

#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IADC.h"
#include "common/node/stream/IFactor.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IBattery_State.h"
#include "common/node/stream/ICommands.h"
#include "common/node/stream/ICurrent.h"
#include "common/node/stream/IDistance.h"
#include "common/node/stream/IForce.h"
#include "common/node/stream/IFrame.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/ILocation.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/IPressure.h"
#include "common/node/stream/IPWM.h"
#include "common/node/stream/ITemperature.h"
#include "common/node/stream/IThrottle.h"
#include "common/node/stream/ITorque.h"
#include "common/node/stream/IVelocity.h"
#include "common/node/stream/IVideo.h"
#include "common/node/stream/IVoltage.h"

#include "common/node/INode.h"
#include "common/node/processor/IPilot.h"

#include "common/Comm_Data.h"
#include "common/Manual_Clock.h"

#include "utils/Json_Helpers.h"

namespace silk
{

class HAL;




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

struct Commands : public Stream
{
    typedef ICommands::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Commands&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Commands);

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

struct Factor : public Stream
{
    typedef IFactor::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Factor&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Factor);

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

struct ECEF_Location : public Stream
{
    typedef IECEF_Location::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ECEF_Location&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ECEF_Location);

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

}


struct Node_Def
{
    std::string name;
    node::Type type;
    rapidjson::Document default_init_params;
    rapidjson::Document default_config;

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

    q::util::Signal<void(Node&)> changed_signal;
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
private:
    std::vector<std::shared_ptr<Base>> m_nodes;
};


class HAL : q::util::Noncopyable
{
    friend class Comms;
public:
    HAL();
    ~HAL();

    auto get_node_defs() const      -> Registry<node::Node_Def> const&;
    auto get_nodes() const          -> Registry<node::Node> const&;
    auto get_streams() const        -> Registry<node::stream::Stream> const&;

    enum class Result
    {
        OK,
        FAILED,
        TIMEOUT
    };

    typedef std::function<void(Result, node::Node_ptr)> Add_Node_Callback;
    void add_node(std::string const& def_name, std::string const& name, rapidjson::Document&& init_params, Add_Node_Callback callback);

    typedef std::function<void(Result)> Remove_Node_Callback;
    void remove_node(node::Node_ptr node, Remove_Node_Callback callback);

    void connect_node_input(node::Node_ptr node, std::string const& input_name, std::string const& stream_name);
    void set_node_config(node::Node_ptr node, rapidjson::Document const& config);

    typedef std::function<void(Result)> Stream_Telemetry_Callback;
    void set_stream_telemetry_active(std::string const& stream_name, bool active, Stream_Telemetry_Callback callback);

    q::util::Signal<void()> node_defs_refreshed_signal;
    q::util::Signal<void()> nodes_refreshed_signal;

    auto get_remote_clock() const -> Manual_Clock const&;

protected:
    Manual_Clock m_remote_clock;

    Registry<node::Node_Def> m_node_defs;
    Registry<node::Node> m_nodes;
    Registry<node::stream::Stream> m_streams;

    struct Queue_Item
    {
        bool was_sent = false;
        q::Clock::time_point sent_time_point;
        uint32_t req_id = 0;
    };

    struct Add_Queue_Item : public Queue_Item
    {
        std::string def_name;
        std::string name;
        rapidjson::Document init_params;
        Add_Node_Callback callback;
    };
    std::vector<Add_Queue_Item> m_add_queue;

    struct Remove_Queue_Item : public Queue_Item
    {
        node::Node_ptr node;
        Remove_Node_Callback callback;
    };
    std::vector<Remove_Queue_Item> m_remove_queue;

    struct Set_Config_Queue_Item : public Queue_Item
    {
        silk::comms::Setup_Message message;
        std::string name;
        rapidjson::Document config;
    };
    std::vector<Set_Config_Queue_Item> m_set_config_queue;

    struct Stream_Telemetry_Queue_Item : public Queue_Item
    {
        std::string stream_name;
        bool is_active = false;
        Stream_Telemetry_Callback callback;
    };
    std::vector<Stream_Telemetry_Queue_Item> m_stream_telemetry_queue;

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
    m_nodes.push_back(std::move(node));
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
    m_nodes.erase(it);
}
template<class Base>
void Registry<Base>::remove_all()
{
    m_nodes.clear();
}




}
