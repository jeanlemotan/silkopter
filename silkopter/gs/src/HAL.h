#pragma once

#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IADC_Value.h"
#include "common/node/stream/IBattery_State.h"
#include "common/node/stream/ICardinal_Points.h"
#include "common/node/stream/ICurrent.h"
#include "common/node/stream/IDistance.h"
#include "common/node/stream/ILinear_Acceleration.h"
#include "common/node/stream/ILocation.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/IPressure.h"
#include "common/node/stream/IPWM_Value.h"
#include "common/node/stream/IReference_Frame.h"
#include "common/node/stream/ITemperature.h"
#include "common/node/stream/IVideo.h"
#include "common/node/stream/IVoltage.h"

#include "common/node/INode.h"
#include "common/node/processor/IMultirotor_Pilot.h"

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
    q::rtti::class_id class_id;
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
struct Angular_Velocity : public Stream
{
    typedef IAngular_Velocity::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Angular_Velocity&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Angular_Velocity);
struct ADC_Value : public Stream
{
    typedef IADC_Value::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(ADC_Value&)> samples_available_signal;
};
DECLARE_CLASS_PTR(ADC_Value);
struct Battery_State : public Stream
{
    typedef IBattery_State::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Battery_State&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Battery_State);
struct Cardinal_Points : public Stream
{
    typedef ICardinal_Points::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Cardinal_Points&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Cardinal_Points);
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
struct Linear_Acceleration : public Stream
{
    typedef ILinear_Acceleration::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Linear_Acceleration&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Linear_Acceleration);
struct Location : public Stream
{
    typedef ILocation::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Location&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Location);
struct Magnetic_Field : public Stream
{
    typedef IMagnetic_Field::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Magnetic_Field&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Magnetic_Field);
struct Pressure : public Stream
{
    typedef IPressure::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Pressure&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Pressure);
struct PWM_Value : public Stream
{
    typedef IPWM_Value::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(PWM_Value&)> samples_available_signal;
};
DECLARE_CLASS_PTR(PWM_Value);
struct Reference_Frame : public Stream
{
    typedef IReference_Frame::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Reference_Frame&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Reference_Frame);
struct Temperature : public Stream
{
    typedef IADC_Value::Sample Sample;
    std::vector<Sample> samples;
    q::util::Signal<void(Temperature&)> samples_available_signal;
};
DECLARE_CLASS_PTR(Temperature);
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
    q::rtti::class_id class_id;
    rapidjson::Document default_init_params;
    rapidjson::Document default_config;

    struct Input
    {
        q::rtti::class_id class_id;
        std::string name;
        uint32_t rate = 0;
    };
    std::vector<Input> inputs;

    struct Output
    {
        q::rtti::class_id class_id;
        std::string name;
        uint32_t rate = 0;
    };
    std::vector<Output> outputs;
};
DECLARE_CLASS_PTR(Node_Def);

struct Node
{
    std::string name;
    q::rtti::class_id class_id;

    rapidjson::Document init_params;
    rapidjson::Document config;

    struct Input
    {
        stream::Stream_wptr stream;
        q::rtti::class_id class_id;
        std::string name;
        uint32_t rate = 0;
    };
    std::vector<Input> inputs;

    struct Output
    {
        stream::Stream_ptr stream;
        q::rtti::class_id class_id;
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
