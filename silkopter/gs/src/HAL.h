#pragma once

#include "common/stream/IAcceleration.h"
#include "common/stream/IADC.h"
#include "common/stream/IFloat.h"
#include "common/stream/IBool.h"
#include "common/stream/IAngular_Velocity.h"
#include "common/stream/IBattery_State.h"
#include "common/stream/ICurrent.h"
#include "common/stream/IDistance.h"
#include "common/stream/IForce.h"
#include "common/stream/IFrame.h"
#include "common/stream/IGPS_Info.h"
#include "common/stream/ILinear_Acceleration.h"
#include "common/stream/IPosition.h"
#include "common/stream/IMagnetic_Field.h"
#include "common/stream/IPressure.h"
#include "common/stream/IPWM.h"
#include "common/stream/ITemperature.h"
#include "common/stream/IThrottle.h"
#include "common/stream/ITorque.h"
#include "common/stream/IVelocity.h"
#include "common/stream/IVideo.h"
#include "common/stream/IVoltage.h"
#include "common/stream/IProximity.h"
#include "common/stream/IMulti_Input.h"
#include "common/stream/IMulti_State.h"

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
namespace gs
{
struct Node;
DECLARE_CLASS_PTR(Node);
}
}


namespace stream
{
namespace gs
{

struct Stream : public IStream
{
    node::gs::Node_wptr node;
    std::string name;
    uint32_t rate = 0;
    int telemetry_active_req = 0;
    bool is_telemetry_active = false;
};
DECLARE_CLASS_PTR(Stream);


template<class Base>
struct Typed_Stream : public Stream
{
    static constexpr Type TYPE = Base::TYPE;

    typedef typename Base::Value Value;
    typedef typename Base::Sample Sample;
    typedef std::vector<Sample> Samples;

    Samples samples;
    q::util::Signal<void(Samples const&)> samples_available_signal;

    virtual auto get_rate() const -> uint32_t override { return rate; }
    virtual auto get_type() const -> Type override { return TYPE; }
};

using Acceleration = Typed_Stream<IAcceleration>;
DECLARE_CLASS_PTR(Acceleration);
using ENU_Acceleration = Typed_Stream<IENU_Acceleration>;
DECLARE_CLASS_PTR(ENU_Acceleration);
//using ECEF_Acceleration = Typed_Stream<IECEF_Acceleration>;
//DECLARE_CLASS_PTR(ECEF_Acceleration);
using ADC = Typed_Stream<IADC>;
DECLARE_CLASS_PTR(ADC);
using Angular_Velocity = Typed_Stream<IAngular_Velocity>;
DECLARE_CLASS_PTR(Angular_Velocity);
using ENU_Angular_Velocity = Typed_Stream<IENU_Angular_Velocity>;
DECLARE_CLASS_PTR(ENU_Angular_Velocity);
using ECEF_Angular_Velocity = Typed_Stream<IECEF_Angular_Velocity>;
DECLARE_CLASS_PTR(ECEF_Angular_Velocity);
using Battery_State = Typed_Stream<IBattery_State>;
DECLARE_CLASS_PTR(Battery_State);
using Current = Typed_Stream<ICurrent>;
DECLARE_CLASS_PTR(Current);
using Distance = Typed_Stream<IDistance>;
DECLARE_CLASS_PTR(Distance);
using ENU_Distance = Typed_Stream<IENU_Distance>;
DECLARE_CLASS_PTR(ENU_Distance);
using ECEF_Distance = Typed_Stream<IECEF_Distance>;
DECLARE_CLASS_PTR(ECEF_Distance);
using Float = Typed_Stream<IFloat>;
DECLARE_CLASS_PTR(Float);
using Bool = Typed_Stream<IBool>;
DECLARE_CLASS_PTR(Float);
using Force = Typed_Stream<IForce>;
DECLARE_CLASS_PTR(Force);
using ENU_Force = Typed_Stream<IENU_Force>;
DECLARE_CLASS_PTR(ENU_Force);
using ECEF_Force = Typed_Stream<IECEF_Force>;
DECLARE_CLASS_PTR(ECEF_Force);
using Frame = Typed_Stream<IFrame>;
DECLARE_CLASS_PTR(Frame);
//using ENU_Frame = GS_Stream<IENU_Frame>;
//DECLARE_CLASS_PTR(ENU_Frame);
using GPS_Info = Typed_Stream<IGPS_Info>;
DECLARE_CLASS_PTR(GPS_Info);
using Linear_Acceleration = Typed_Stream<ILinear_Acceleration>;
DECLARE_CLASS_PTR(Linear_Acceleration);
using ENU_Linear_Acceleration = Typed_Stream<IENU_Linear_Acceleration>;
DECLARE_CLASS_PTR(ENU_Linear_Acceleration);
using ECEF_Linear_Acceleration = Typed_Stream<IECEF_Linear_Acceleration>;
DECLARE_CLASS_PTR(ECEF_Linear_Acceleration);
using ECEF_Position = Typed_Stream<IECEF_Position>;
DECLARE_CLASS_PTR(ECEF_Position);
using Magnetic_Field = Typed_Stream<IMagnetic_Field>;
DECLARE_CLASS_PTR(Magnetic_Field);
using ENU_Magnetic_Field = Typed_Stream<IENU_Magnetic_Field>;
DECLARE_CLASS_PTR(ENU_Magnetic_Field);
using ECEF_Magnetic_Field = Typed_Stream<IECEF_Magnetic_Field>;
DECLARE_CLASS_PTR(ECEF_Magnetic_Field);
using Pressure = Typed_Stream<IPressure>;
DECLARE_CLASS_PTR(Pressure);
using PWM = Typed_Stream<IPWM>;
DECLARE_CLASS_PTR(PWM);
using Temperature = Typed_Stream<ITemperature>;
DECLARE_CLASS_PTR(Temperature);
using Throttle = Typed_Stream<IThrottle>;
DECLARE_CLASS_PTR(Throttle);
using Torque = Typed_Stream<ITorque>;
DECLARE_CLASS_PTR(Torque);
using ENU_Torque = Typed_Stream<IENU_Torque>;
DECLARE_CLASS_PTR(ENU_Torque);
using ECEF_Torque = Typed_Stream<IECEF_Torque>;
DECLARE_CLASS_PTR(ECEF_Torque);
using Velocity = Typed_Stream<IVelocity>;
DECLARE_CLASS_PTR(Velocity);
using ENU_Velocity = Typed_Stream<IENU_Velocity>;
DECLARE_CLASS_PTR(ENU_Velocity);
using ECEF_Velocity = Typed_Stream<IECEF_Velocity>;
DECLARE_CLASS_PTR(ECEF_Velocity);
using Voltage = Typed_Stream<IVoltage>;
DECLARE_CLASS_PTR(Voltage);
using Video = Typed_Stream<IVideo>;
DECLARE_CLASS_PTR(Video);
using Multi_State = Typed_Stream<IMulti_State>;
DECLARE_CLASS_PTR(Multi_State);
using Multi_Input = Typed_Stream<IMulti_Input>;
DECLARE_CLASS_PTR(Multi_Input);
using Proximity = Typed_Stream<IProximity>;
DECLARE_CLASS_PTR(Proximity);

}
}



namespace node
{
namespace gs
{

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
        stream::gs::Stream_wptr stream;
        stream::Type type;
        std::string name;
        uint32_t rate = 0;
    };
    std::vector<Input> inputs;
    struct Output
    {
        stream::gs::Stream_ptr stream;
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
}



template<class Base>
class Registry : q::util::Noncopyable
{
public:
    auto get_all() const -> std::vector<std::shared_ptr<Base>> const&;
    template<class T> auto get_all_of_type() const -> std::vector<std::shared_ptr<T>>;
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


    auto get_node_defs() const      -> Registry<node::gs::Node_Def> const&;
    auto get_nodes() const          -> Registry<node::gs::Node> const&;
    auto get_streams() const        -> Registry<stream::gs::Stream> const&;

    enum class Result
    {
        OK,
        FAILED,
        TIMEOUT
    };

    void add_node(std::string const& def_name, std::string const& name, rapidjson::Document const& init_params);
    void remove_node(node::gs::Node_ptr node);
    void set_node_input_stream_path(node::gs::Node_ptr node, std::string const& input_name, q::Path const& stream_path);
    void set_node_config(node::gs::Node_ptr node, rapidjson::Document const& config);
    void set_stream_telemetry_active(std::string const& stream_name, bool active);
    void send_node_message(node::gs::Node_ptr node, rapidjson::Document const& json);

    auto get_remote_clock() const -> Manual_Clock const&;

protected:
    Manual_Clock m_remote_clock;
    Comms& m_comms;

    struct Configs
    {
        boost::optional<config::Multi> multi;
    } m_configs;

    Registry<node::gs::Node_Def> m_node_defs;
    Registry<node::gs::Node> m_nodes;
    Registry<stream::gs::Stream> m_streams;

private:

};


/////////////////////////////////////////////////////////////////////////////////////////////////


template<class Base>
auto Registry<Base>::get_all() const -> std::vector<std::shared_ptr<Base>> const&
{
    return m_nodes;
}
template<class Base>
template<class T>
auto Registry<Base>::get_all_of_type() const -> std::vector<std::shared_ptr<T>>
{
    std::vector<std::shared_ptr<T>> res;
    for (auto const& s: m_nodes)
    {
        if (s->get_type() == T::TYPE)
        {
            res.push_back(std::static_pointer_cast<T>(s));
        }
    }
    return res;
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
