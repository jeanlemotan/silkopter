#pragma once

#include "rapidjson/document.h"
#include "common/node/bus/IBus.h"
#include "common/node/INode.h"
#include "common/node/stream/IStream.h"
#include "common/config/Multi.h"

#include "MPL_Helper.h"

namespace silk
{

class HAL;

template<class Base>
class Factory : q::util::Noncopyable
{
public:
    template <class T, typename... Params> void register_node(std::string const& class_name, Params&&... params);
    auto create_node(std::string const& class_name) -> std::shared_ptr<Base>;
    auto get_node_default_config(std::string const& class_name) -> boost::optional<rapidjson::Document const&>;
    auto get_node_default_init_params(std::string const& class_name) -> boost::optional<rapidjson::Document const&>;

    struct Node_Info
    {
        std::string name;
        std::unique_ptr<Base> node;
    };

    auto create_all() const -> std::vector<Node_Info>;
private:
    struct Data
    {
        std::unique_ptr<detail::Ctor_Helper_Base<Base>> ctor;
    };
    std::map<std::string, Data> m_name_registry;
};


template<class Base>
class Registry : q::util::Noncopyable
{
public:
    struct Item
    {
        std::string name;
        std::string type;
        std::shared_ptr<Base> node;
    };

    auto get_all() const -> std::vector<Item> const&;
    void remove_all();
    template<class T> auto find_by_name(std::string const& name) const -> std::shared_ptr<T>;
    auto add(std::string const& name, std::string const& type, std::shared_ptr<Base> const& node) -> bool;
    void remove(std::shared_ptr<Base> const& node);
private:
    std::vector<Item> m_nodes;
};

class Comms;


class HAL : q::util::Noncopyable
{
    friend class Comms;
public:
    HAL();
    ~HAL();

    auto init(Comms& comms) -> bool;
    void process();
    void shutdown();

    void save_settings();

    auto get_multi_config() const   -> boost::optional<config::Multi>;
//    auto get_plane_config() const   -> boost::optional<config::Plane const&>;
//    auto get_copter_config() const  -> boost::optional<config::Copter const&>;
//    auto get_rover_config() const   -> boost::optional<config::Rover const&>;
//    auto get_boat_config() const    -> boost::optional<config::Boat const&>;

    auto get_bus_factory()          -> Factory<node::bus::IBus>&;
    auto get_node_factory()         -> Factory<node::INode>&;

    auto get_buses()        -> Registry<node::bus::IBus>&;
    auto get_nodes()        -> Registry<node::INode>&;
    auto get_streams()      -> Registry<node::stream::IStream>&;

protected:
    auto set_multi_config(config::Multi const& config) -> bool;

    struct Telemetry_Data
    {
        q::Clock::duration total_duration;
        float rate = 0;
        struct Node
        {
            q::Clock::duration process_duration;
            float process_percentage = 0;
        };
        std::map<std::string, Node> nodes;
    };

    auto get_telemetry_data() const -> Telemetry_Data const&;

private:
    void generate_settings_file();

    auto create_bus(std::string const& type,
                    std::string const& name,
                    rapidjson::Value const& init_params) -> node::bus::IBus_ptr;
    auto create_buses(rapidjson::Value& json) -> bool;

    auto create_node(std::string const& type,
                     std::string const& name,
                     rapidjson::Value const& init_params) -> node::INode_ptr;
    auto create_nodes(rapidjson::Value& json) -> bool;

    struct Configs
    {
        boost::optional<config::Multi> multi;
    } m_configs;

    Registry<node::bus::IBus> m_buses;
    Registry<node::INode> m_nodes;
    Registry<node::stream::IStream> m_streams;

    Factory<node::bus::IBus> m_bus_factory;
    Factory<node::INode> m_node_factory;

    q::Clock::time_point m_last_process_tp = q::Clock::now();


    Telemetry_Data m_telemetry_data;
};


/////////////////////////////////////////////////////////////////////////////////////////////////


template<class Base>
auto Registry<Base>::get_all() const -> std::vector<Item> const&
{
    return m_nodes;
}
template<class Base>
void Registry<Base>::remove_all()
{
    m_nodes.clear();
}
template<class Base>
template<class T>
auto Registry<Base>::find_by_name(std::string const& name) const -> std::shared_ptr<T>
{
    auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](Item const& s) { return s.name == name; });
    return it != m_nodes.end() ? std::dynamic_pointer_cast<T>(it->node) : nullptr;
}
template<class Base>
auto Registry<Base>::add(std::string const& name, std::string const& type, std::shared_ptr<Base> const& node) -> bool
{
    if (find_by_name<Base>(name))
    {
        QLOGE("Duplicated name in node {}", name);
        return false;
    }
    m_nodes.push_back({name, type, node});
    return true;
}
template<class Base>
void Registry<Base>::remove(std::shared_ptr<Base> const& node)
{
    m_nodes.erase(std::remove_if(m_nodes.begin(), m_nodes.end(), [node](Item const& item) { return item.node == node; }), m_nodes.end());
}


template<class Base>
template <class T, typename... Params>
void Factory<Base>::register_node(std::string const& class_name, Params&&... params)
{
//    T instance(hal);

//    //write the jsons for testing - to see the structure
//    {
//        rapidjson::StringBuffer s;
//        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
//        instance.get_init_params().Accept(writer);    // Accept() traverses the DOM and generates Handler events.
//        q::data::File_Sink fs((q::Path(class_name + "_init_params.json")));
//        std::string data = s.GetString();
//        fs.write((uint8_t const* )data.data(), data.size());
//    }
//    {
//        rapidjson::StringBuffer s;
//        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
//        instance.get_config().Accept(writer);    // Accept() traverses the DOM and generates Handler events.
//        q::data::File_Sink fs((q::Path(class_name + "_config.json")));
//        std::string data = s.GetString();
//        fs.write((uint8_t const* )data.data(), data.size());
//    }

    if (class_name.empty())
    {
        return;
    }
    auto it = m_name_registry.find(class_name);
    if (it != m_name_registry.end())
    {
        QLOGE("Error: class '{}' already defined.", class_name);
        return;
    }
    m_name_registry[class_name].ctor.reset(new detail::Ctor_Helper<Base, T, Params...>(params...));
}

template <class Base>
auto Factory<Base>::create_node(std::string const& class_name) -> std::shared_ptr<Base>
{
    auto it = m_name_registry.find(class_name);
    return std::shared_ptr<Base>(it == m_name_registry.end() ? nullptr : reinterpret_cast<Base*>(it->second.ctor->create()));
}
template<class Base>
auto Factory<Base>::get_node_default_config(std::string const& class_name) -> boost::optional<rapidjson::Document const&>
{
    auto it = m_name_registry.find(class_name);
    if (it != m_name_registry.end())
    {
        return it->second.config;
    }
    return boost::none;
}
template<class Base>
auto Factory<Base>::get_node_default_init_params(std::string const& class_name) -> boost::optional<rapidjson::Document const&>
{
    auto it = m_name_registry.find(class_name);
    if (it != m_name_registry.end())
    {
        return it->second.init_params;
    }
    return boost::none;
}

template<class Base>
auto Factory<Base>::create_all() const -> std::vector<Node_Info>
{
    std::vector<Node_Info> info;
    info.reserve(m_name_registry.size());
    for (auto const& n: m_name_registry)
    {
        info.push_back({n.first, std::unique_ptr<Base>(n.second.ctor->create())});
    }
    return info;
}



}
