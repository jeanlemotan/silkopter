//#pragma once

//#include "common/stream/IAcceleration.h"
//#include "common/stream/IADC.h"
//#include "common/stream/IFloat.h"
//#include "common/stream/IBool.h"
//#include "common/stream/IAngular_Velocity.h"
//#include "common/stream/IBattery_State.h"
//#include "common/stream/ICurrent.h"
//#include "common/stream/IDistance.h"
//#include "common/stream/IForce.h"
//#include "common/stream/IFrame.h"
//#include "common/stream/IGPS_Info.h"
//#include "common/stream/ILinear_Acceleration.h"
//#include "common/stream/IPosition.h"
//#include "common/stream/IMagnetic_Field.h"
//#include "common/stream/IPressure.h"
//#include "common/stream/IPWM.h"
//#include "common/stream/ITemperature.h"
//#include "common/stream/IThrottle.h"
//#include "common/stream/ITorque.h"
//#include "common/stream/IVelocity.h"
//#include "common/stream/IVideo.h"
//#include "common/stream/IVoltage.h"
//#include "common/stream/IProximity.h"
//#include "common/stream/IMulti_Commands.h"
//#include "common/stream/IMulti_State.h"

//#include "common/node/INode.h"
//#include "common/node/IPilot.h"

//#include "common/config/Multi.h"
//#include "common/Comm_Data.h"
//#include "common/Manual_Clock.h"

//#include "utils/Json_Util.h"

//namespace silk
//{

//class HAL;
//class Comms;


//class HAL : q::util::Noncopyable
//{
//    friend class Comms;
//public:
//    HAL();
//    ~HAL();

//    void init(Comms& comms);

////    auto get_multi_config() const   -> boost::optional<config::Multi>;
////    void set_multi_config(config::Multi const& config);

////    q::util::Signal<void()> multi_config_refreshed_signal;


////    auto get_node_defs() const      -> Registry<node::gs::Node_Def> const&;
////    auto get_nodes() const          -> Registry<node::gs::Node> const&;
////    auto get_streams() const        -> Registry<stream::gs::Stream> const&;

//    enum class Result
//    {
//        OK,
//        FAILED,
//        TIMEOUT
//    };

////    void add_node(std::string const& def_name, std::string const& name, rapidjson::Document const& init_params);
////    void remove_node(node::gs::Node_ptr node);
////    void set_node_input_stream_path(node::gs::Node_ptr node, std::string const& input_name, q::Path const& stream_path);
////    void set_node_config(node::gs::Node_ptr node, rapidjson::Document const& config);
////    void set_stream_telemetry_active(std::string const& stream_name, bool active);
////    void send_node_message(node::gs::Node_ptr node, rapidjson::Document const& json);

////    auto get_remote_clock() const -> Manual_Clock const&;

//protected:
////    Manual_Clock m_remote_clock;
////    Comms* m_comms = nullptr;

////    struct Configs
////    {
////        boost::optional<config::Multi> multi;
////    } m_configs;

////    Registry<node::gs::Node_Def> m_node_defs;
////    Registry<node::gs::Node> m_nodes;
////    Registry<stream::gs::Stream> m_streams;

//private:

//};


///////////////////////////////////////////////////////////////////////////////////////////////////


////template<class Base>
////auto Registry<Base>::get_all() const -> std::vector<std::shared_ptr<Base>> const&
////{
////    return m_nodes;
////}
////template<class Base>
////template<class T>
////auto Registry<Base>::get_all_of_type() const -> std::vector<std::shared_ptr<T>>
////{
////    std::vector<std::shared_ptr<T>> res;
////    for (auto const& s: m_nodes)
////    {
////        if (s->get_type() == T::TYPE)
////        {
////            res.push_back(std::static_pointer_cast<T>(s));
////        }
////    }
////    return res;
////}
////template<class Base>
////auto Registry<Base>::find_by_name(std::string const& name) const -> std::shared_ptr<Base>
////{
////    auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](std::shared_ptr<Base> const& s) { return s->name == name; });
////    return it != m_nodes.end() ? *it : std::shared_ptr<Base>();
////}
////template<class Base>
////template<class T>
////auto Registry<Base>::add(std::shared_ptr<T> node) -> bool
////{
////    QASSERT(node);
////    if (find_by_name(node->name))
////    {
////        QLOGE("Duplicated name in node {}", node->name);
////        return false;
////    }
////    m_nodes.push_back(node);
////    item_added_signal.execute(node);
////    return true;
////}
////template<class Base>
////void Registry<Base>::remove(std::shared_ptr<Base> node)
////{
////    auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](std::shared_ptr<Base> const& s) { return s == node; });
////    if (it == m_nodes.end())
////    {
////        QLOGE("Cannot find node {}", node->name);
////        return;
////    }
////    item_will_be_removed_signal.execute(node);
////    m_nodes.erase(it);
////}
////template<class Base>
////void Registry<Base>::remove_all()
////{
////    for (auto& node: m_nodes)
////    {
////        item_will_be_removed_signal.execute(node);
////    }
////    m_nodes.clear();
////}




//}
