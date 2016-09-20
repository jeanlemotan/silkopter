#pragma once

#include <memory>

#include "uav_properties/IUAV_Properties.h"
#include "common/bus/IBus.h"
#include "common/node/INode.h"
#include "common/stream/IStream.h"

#include "MPL_Helper.h"

namespace silk
{
namespace hal
{
struct IUAV_Descriptor;
struct Multirotor_Descriptor;
}

class HAL;

template<class Base>
class Factory : q::util::Noncopyable
{
public:
    template <class T, typename... Params> void add(std::string const& class_name, Params&&... params);
    auto create(std::string const& class_name) -> std::shared_ptr<Base>;
//    auto get_default_config(std::string const& class_name) -> boost::optional<rapidjson::Document const&>;
//    auto get_default_init_params(std::string const& class_name) -> boost::optional<rapidjson::Document const&>;

    struct Info
    {
        std::string name;
        std::unique_ptr<Base> ptr;
    };

    auto create_all() const -> std::vector<Info>;
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
        std::shared_ptr<Base> ptr;
    };

    auto get_all() const -> std::vector<Item> const&;
    void set_all(std::vector<Item> const& items);
    void remove_all();
    template<class T> auto find_by_name(std::string const& name) const -> std::shared_ptr<T>;
    bool add(std::string const& name, std::string const& type, std::shared_ptr<Base> const& ptr);
    void remove(std::shared_ptr<Base> const& ptr);
private:
    std::vector<Item> m_items;
};

class RC_Comms;
class GS_Comms;


class HAL : q::util::Noncopyable
{
    friend class RC_Comms;
    friend class GS_Comms;
public:
    HAL();
    ~HAL();

    auto init(RC_Comms& rc_comms, GS_Comms& gs_comms) -> bool;
    void process();
    void shutdown();

    void save_settings();

    ts::Result<void> set_uav_descriptor(std::shared_ptr<const hal::IUAV_Descriptor> descriptor);
    auto get_uav_descriptor() const   -> std::shared_ptr<const hal::IUAV_Descriptor>;

    auto get_uav_properties() const   -> std::shared_ptr<const IUAV_Properties>;

    template<class Properties>
    auto get_specialized_uav_properties() const   -> std::shared_ptr<const Properties>;

    q::util::Signal<void(HAL&)> uav_properties_changed_signal;

    typedef Factory<bus::IBus> Bus_Factory;
    Bus_Factory const& get_bus_factory() const;

    typedef Factory<node::INode> Node_Factory;
    Node_Factory const& get_node_factory() const;

    typedef Registry<bus::IBus> Bus_Registry;
    Bus_Registry const& get_bus_registry() const;

    typedef Registry<node::INode> Node_Registry;
    Node_Registry const& get_node_registry() const;

    typedef Registry<stream::IStream> Stream_Registry;
    Stream_Registry const& get_stream_registry() const;

    void remove_add_nodes();

protected:

    struct Telemetry_Data
    {
        size_t version = 0;
        q::Clock::duration crt_total_duration;
        q::Clock::duration crt_max_total_duration;

        q::Clock::duration total_duration;
        q::Clock::duration max_total_duration;
        float rate = 0;
        struct Node
        {
            q::Clock::duration crt_process_duration;
            q::Clock::duration crt_max_process_duration;

            q::Clock::duration process_duration;
            q::Clock::duration max_process_duration;
        };
        std::map<std::string, Node> nodes;
    };

    auto get_telemetry_data() const -> Telemetry_Data const&;

private:
    void generate_settings_file();

    ts::Result<std::shared_ptr<bus::IBus>> create_bus(std::string const& type, std::string const& name, hal::IBus_Descriptor const& descriptor);
    ts::Result<std::shared_ptr<node::INode>> create_node(std::string const& type, std::string const& name, hal::INode_Descriptor const& descriptor);

    bool remove_node(std::shared_ptr<node::INode> node);

    void sort_nodes(std::shared_ptr<node::INode> first_node);

    std::shared_ptr<IUAV_Properties> m_uav_properties;
    std::shared_ptr<const hal::IUAV_Descriptor> m_uav_descriptor;

    Bus_Registry m_buses;
    Node_Registry m_nodes;
    Stream_Registry m_streams;

    Bus_Factory m_bus_factory;
    Node_Factory m_node_factory;

    q::Clock::time_point m_last_process_tp = q::Clock::now();

    q::Clock::time_point m_last_telemetry_data_latch_tp = q::Clock::now();
    Telemetry_Data m_telemetry_data;
};


/////////////////////////////////////////////////////////////////////////////////////////////////

template<class Properties>
auto HAL::get_specialized_uav_properties() const   -> std::shared_ptr<const Properties>
{
    std::shared_ptr<const IUAV_Properties> properties = get_uav_properties();
    return std::dynamic_pointer_cast<const Properties>(properties);
}

/////////////////////////////////////////////////////////////////////////////////////////////////


template<class Base>
auto Registry<Base>::get_all() const -> std::vector<Item> const&
{
    return m_items;
}

template<class Base>
void Registry<Base>::set_all(std::vector<Item> const& items)
{
    m_items = items;
}
template<class Base>
void Registry<Base>::remove_all()
{
    m_items.clear();
}
template<class Base>
template<class T>
auto Registry<Base>::find_by_name(std::string const& name) const -> std::shared_ptr<T>
{
    auto it = std::find_if(m_items.begin(), m_items.end(), [&](Item const& s) { return s.name == name; });
    return it != m_items.end() ? std::dynamic_pointer_cast<T>(it->ptr) : nullptr;
}
template<class Base>
auto Registry<Base>::add(std::string const& name, std::string const& type, std::shared_ptr<Base> const& ptr) -> bool
{
    if (find_by_name<Base>(name))
    {
        QLOGE("Duplicated name {}", name);
        return false;
    }
    m_items.push_back({name, type, ptr});
    return true;
}
template<class Base>
void Registry<Base>::remove(std::shared_ptr<Base> const& ptr)
{
    m_items.erase(std::remove_if(m_items.begin(), m_items.end(), [ptr](Item const& item) { return item.ptr == ptr; }), m_items.end());
}


template<class Base>
template <class T, typename... Params>
void Factory<Base>::add(std::string const& class_name, Params&&... params)
{
//    T instance(uav);

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
auto Factory<Base>::create(std::string const& class_name) -> std::shared_ptr<Base>
{
    auto it = m_name_registry.find(class_name);
    return std::shared_ptr<Base>(it == m_name_registry.end() ? nullptr : reinterpret_cast<Base*>(it->second.ctor->create()));
}
//template<class Base>
//auto Factory<Base>::get_default_config(std::string const& class_name) -> boost::optional<rapidjson::Document const&>
//{
//    auto it = m_name_registry.find(class_name);
//    if (it != m_name_registry.end())
//    {
//        return it->second.config;
//    }
//    return boost::none;
//}
//template<class Base>
//auto Factory<Base>::get_default_init_params(std::string const& class_name) -> boost::optional<rapidjson::Document const&>
//{
//    auto it = m_name_registry.find(class_name);
//    if (it != m_name_registry.end())
//    {
//        return it->second.init_params;
//    }
//    return boost::none;
//}

template<class Base>
auto Factory<Base>::create_all() const -> std::vector<Info>
{
    std::vector<Info> info;
    info.reserve(m_name_registry.size());
    for (auto const& n: m_name_registry)
    {
        info.push_back({n.first, std::unique_ptr<Base>(n.second.ctor->create())});
    }
    return info;
}



}
