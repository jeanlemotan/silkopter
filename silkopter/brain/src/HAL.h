#pragma once

#include "rapidjson/document.h"
#include "common/node/bus/IBus.h"
#include "common/node/source/ISource.h"
#include "common/node/sink/ISink.h"
#include "common/node/processor/IProcessor.h"
#include "common/node/stream/IStream.h"

namespace silk
{

class HAL;

template<class Base>
class Factory : q::util::Noncopyable
{
public:
    Factory(HAL& hal) : m_hal(hal) {}
    template <class T> void register_node(std::string const& class_name);
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
    typedef Base* (*create_node_function)(HAL& hal);
    struct Data
    {
        create_node_function ctor;
    };
    std::map<std::string, Data> m_name_registry;
    HAL& m_hal;
    template <class T> static auto create_node_func(HAL& hal) -> Base*;
    void _register_node(std::string const& class_name, create_node_function ctor);
};


template<class Base>
class Registry : q::util::Noncopyable
{
public:
    struct Item
    {
        std::string name;
        std::shared_ptr<Base> node;
    };

    auto get_all() const -> std::vector<Item> const&;
    void remove_all();
    template<class T> auto find_by_name(std::string const& name) const -> std::shared_ptr<T>;
    auto add(std::string const& name, std::shared_ptr<Base> const& node) -> bool;
private:
    std::vector<Item> m_nodes;
};


class HAL : q::util::Noncopyable
{
public:
    HAL();
    ~HAL();

    auto init() -> bool;
    void process();
    void shutdown();

    auto get_settings(q::Path const& path) -> rapidjson::Value&;
    void save_settings();

    auto get_bus_factory()         -> Factory<node::bus::IBus>&;
    auto get_source_factory()      -> Factory<node::source::ISource>&;
    auto get_sink_factory()        -> Factory<node::sink::ISink>&;
    auto get_processor_factory()   -> Factory<node::processor::IProcessor>&;

    auto get_buses()        -> Registry<node::bus::IBus>&;
    auto get_sources()      -> Registry<node::source::ISource>&;
    auto get_sinks()        -> Registry<node::sink::ISink>&;
    auto get_processors()   -> Registry<node::processor::IProcessor>&;
    auto get_streams()      -> Registry<node::stream::IStream>&;

    template<class Base>
    auto create_node(std::string const& type,
                     std::string const& name,
                     rapidjson::Value const& init_params,
                     rapidjson::Value const& config) -> std::shared_ptr<Base>;

    auto create_bus(std::string const& type,
                    std::string const& name,
                    rapidjson::Value const& init_params) -> std::shared_ptr<node::bus::IBus>;

private:
    template<class Base> auto create_nodes(rapidjson::Value& json) -> bool;

    Registry<node::bus::IBus> m_buses;
    Registry<node::source::ISource> m_sources;
    Registry<node::sink::ISink> m_sinks;
    Registry<node::processor::IProcessor> m_processors;
    Registry<node::stream::IStream> m_streams;

    Factory<node::bus::IBus> m_bus_factory;
    Factory<node::source::ISource> m_source_factory;
    Factory<node::sink::ISink> m_sink_factory;
    Factory<node::processor::IProcessor> m_processor_factory;

    //bool m_is_initialized = false;

    rapidjson::Value m_emptyValue;
    rapidjson::Document m_settings;

    auto load_settings() -> bool;
};

inline auto HAL::create_bus(
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
        auto res = m_buses.add(name, node); //this has to succeed since we already tested for duplicate names
        QASSERT(res);
        return node;
    }
    return node::bus::IBus_ptr();
}
template<>
inline auto HAL::create_node<node::source::ISource>(
        std::string const& type,
        std::string const& name,
        rapidjson::Value const& init_params,
        rapidjson::Value const& config) -> node::source::ISource_ptr
{
    if (m_sources.find_by_name<node::source::ISource>(name))
    {
        QLOGE("Source '{}' already exist", name);
        return node::source::ISource_ptr();
    }
    auto node = m_source_factory.create_node(type);
    if (node && node->init(init_params) && node->set_config(config))
    {
        auto res = m_sources.add(name, node); //this has to succeed since we already tested for duplicate names
        QASSERT(res);
        auto outputs = node->get_outputs();
        for (auto const& x: outputs)
        {
            std::string stream_name = q::util::format2<std::string>("{}/{}", name, x.name);
            if (!m_streams.add(stream_name, x.stream))
            {
                QLOGE("Cannot add stream '{}'", stream_name);
                return node::source::ISource_ptr();
            }
        }
        return node;
    }
    return node::source::ISource_ptr();
}
template<>
inline auto HAL::create_node<node::sink::ISink>(
        std::string const& type,
        std::string const& name,
        rapidjson::Value const& init_params,
        rapidjson::Value const& config) -> node::sink::ISink_ptr
{
    if (m_sinks.find_by_name<node::sink::ISink>(name))
    {
        QLOGE("Sink '{}' already exist", name);
        return node::sink::ISink_ptr();
    }
    auto node = m_sink_factory.create_node(type);
    if (node && node->init(init_params) && node->set_config(config))
    {
        auto res = m_sinks.add(name, node); //this has to succeed since we already tested for duplicate names
        QASSERT(res);
        return node;
    }
    return node::sink::ISink_ptr();
}
template<>
inline auto HAL::create_node<node::processor::IProcessor>(
        std::string const& type,
        std::string const& name,
        rapidjson::Value const& init_params,
        rapidjson::Value const& config) -> node::processor::IProcessor_ptr
{
    if (m_processors.find_by_name<node::processor::IProcessor>(name))
    {
        QLOGE("Processor '{}' already exist", name);
        return node::processor::IProcessor_ptr();
    }
    auto node = m_processor_factory.create_node(type);
    if (node && node->init(init_params) && node->set_config(config))
    {
        auto res = m_processors.add(name, node); //this has to succeed since we already tested for duplicate names
        QASSERT(res);
        auto outputs = node->get_outputs();
        for (auto const& x: outputs)
        {
            std::string stream_name = q::util::format2<std::string>("{}/{}", name, x.name);
            if (!m_streams.add(stream_name, x.stream))
            {
                QLOGE("Cannot add stream '{}'", stream_name);
                return node::processor::IProcessor_ptr();
            }
        }
        return node;
    }
    return node::processor::IProcessor_ptr();
}


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
auto Registry<Base>::add(std::string const& name, std::shared_ptr<Base> const& node) -> bool
{
    if (find_by_name<Base>(name))
    {
        QLOGE("Duplicated name in node {}", name);
        return false;
    }
    m_nodes.push_back({name, node});
    return true;
}



template<class Base>
template <class T>
void Factory<Base>::register_node(std::string const& class_name)
{
    auto* ctor = &create_node_func<T>;
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

    _register_node(class_name, ctor);
}

template <class Base>
auto Factory<Base>::create_node(std::string const& class_name) -> std::shared_ptr<Base>
{
    auto it = m_name_registry.find(class_name);
    return std::shared_ptr<Base>(it == m_name_registry.end() ? nullptr : reinterpret_cast<Base*>((*it->second.ctor)(m_hal)));
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
template <class T>
auto Factory<Base>::create_node_func(HAL& hal) -> Base*
{
    return new T(hal);
}

template<class Base>
void Factory<Base>::_register_node(std::string const& class_name, create_node_function ctor)
{
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
    m_name_registry[class_name] = { ctor };
}
template<class Base>
auto Factory<Base>::create_all() const -> std::vector<Node_Info>
{
    std::vector<Node_Info> info;
    info.reserve(m_name_registry.size());
    for (auto const& n: m_name_registry)
    {
        info.push_back({n.first, std::unique_ptr<Base>((n.second.ctor)(m_hal))});
    }
    return info;
}



}
