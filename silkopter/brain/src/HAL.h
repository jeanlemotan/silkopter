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

class Factory : q::util::Noncopyable
{
public:
    template <class T> void register_node(HAL& hal, std::string const& class_name);
    template <class Base> auto create_node(HAL& hal, std::string const& class_name) -> std::unique_ptr<Base>;
    auto get_node_default_config(std::string const& class_name) -> boost::optional<rapidjson::Document const&>;
    auto get_node_default_init_params(std::string const& class_name) -> boost::optional<rapidjson::Document const&>;
private:
    typedef void* (*create_node_function)(HAL& hal);
    struct Data
    {
        create_node_function ctor;
        rapidjson::Document init_params;
        rapidjson::Document config;
    };
    std::map<std::string, Data> m_name_registry;
    template <class T> static auto create_node_func(HAL& hal) -> void*;
    void _register_node(std::string const& class_name, create_node_function ctor, rapidjson::Document&& init_params, rapidjson::Document&& config);
};


template<class Base>
class Registry : q::util::Noncopyable
{
public:
    auto get_all() const -> std::vector<Base*> const&;
    void remove_all();
    template<class T> auto find_by_name(std::string const& name) const -> T*;
    auto add(Base& node) -> bool;
private:
    std::vector<Base*> m_nodes;
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

    auto get_buses()        -> Registry<node::bus::IBus>&;
    auto get_sources()      -> Registry<node::source::ISource>&;
    auto get_sinks()        -> Registry<node::sink::ISink>&;
    auto get_processors()   -> Registry<node::processor::IProcessor>&;
    auto get_streams()      -> Registry<node::stream::IStream>&;

private:
    template<class Base> auto create_nodes(rapidjson::Value& json) -> bool;
    template<class Base> auto create_node(std::string const& type, rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool;

    Registry<node::bus::IBus> m_buses;
    Registry<node::source::ISource> m_sources;
    Registry<node::sink::ISink> m_sinks;
    Registry<node::processor::IProcessor> m_processors;
    Registry<node::stream::IStream> m_streams;

    Factory m_factory;

    //bool m_is_initialized = false;

    rapidjson::Value m_emptyValue;
    rapidjson::Document m_settings;

    struct Hardware;
    std::shared_ptr<Hardware> m_hw;

    auto load_settings() -> bool;
};


/////////////////////////////////////////////////////////////////////////////////////////////////


template<class Base>
auto Registry<Base>::get_all() const -> std::vector<Base*> const&
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
auto Registry<Base>::find_by_name(std::string const& name) const -> T*
{
    auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](Base* s) { return s->get_name() == name; });
    return it != m_nodes.end() ? dynamic_cast<T*>(*it) : nullptr;
}
template<class Base>
auto Registry<Base>::add(Base& node) -> bool
{
    if (find_by_name<Base>(node.get_name()))
    {
        QLOGE("Duplicated name in node {}", node.get_name());
        return false;
    }
    m_nodes.push_back(static_cast<Base*>(&node));
    return true;
}



template <class T> void Factory::register_node(HAL& hal, std::string const& class_name)
{
    auto* ctor = &create_node_func<T>;
    T instance(hal);

    //write the jsons for testing - to see the structure
    {
        rapidjson::StringBuffer s;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
        instance.get_init_params().Accept(writer);    // Accept() traverses the DOM and generates Handler events.
        q::data::File_Sink fs((q::Path(class_name + "_init_params.json")));
        std::string data = s.GetString();
        fs.write((uint8_t const* )data.data(), data.size());
    }
    {
        rapidjson::StringBuffer s;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
        instance.get_config().Accept(writer);    // Accept() traverses the DOM and generates Handler events.
        q::data::File_Sink fs((q::Path(class_name + "_config.json")));
        std::string data = s.GetString();
        fs.write((uint8_t const* )data.data(), data.size());
    }

    _register_node(class_name, ctor, instance.get_init_params(), instance.get_config());
}

template <class Base> auto Factory::create_node(HAL& hal, std::string const& class_name) -> std::unique_ptr<Base>
{
    auto it = m_name_registry.find(class_name);
    return std::unique_ptr<Base>(it == m_name_registry.end() ? nullptr : reinterpret_cast<Base*>((*it->second.ctor)(hal)));
}
inline auto Factory::get_node_default_config(std::string const& class_name) -> boost::optional<rapidjson::Document const&>
{
    auto it = m_name_registry.find(class_name);
    if (it != m_name_registry.end())
    {
        return it->second.config;
    }
    return boost::none;
}
inline auto Factory::get_node_default_init_params(std::string const& class_name) -> boost::optional<rapidjson::Document const&>
{
    auto it = m_name_registry.find(class_name);
    if (it != m_name_registry.end())
    {
        return it->second.init_params;
    }
    return boost::none;
}
template <class T> auto Factory::create_node_func(HAL& hal) -> void*
{
    return reinterpret_cast<void*>(new T(hal));
}
inline void Factory::_register_node(std::string const& class_name, create_node_function ctor, rapidjson::Document&& init_params, rapidjson::Document&& config)
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
    m_name_registry[class_name] = { ctor, std::move(init_params), std::move(config) };
}


}
