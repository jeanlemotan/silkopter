#pragma once

#include "rapidjson/document.h"
#include "common/node/bus/IBus.h"
#include "common/node/source/ISource.h"
#include "common/node/sink/ISink.h"
#include "common/node/stream/IStream.h"

namespace silk
{

class HAL;

template<class Base>
class Registry : q::util::Noncopyable
{
    friend class HAL;
public:
    template<class T> auto get_all() const -> std::vector<std::pair<q::String const&, T*>> const&;
    template<class T> auto find_by_name(q::String const& name) const -> T*;

protected:
    typedef std::vector<std::pair<q::String const&, Base*>> Nodes;
    typedef std::unordered_map<size_t, Nodes> Nodes_Map;

    template<class T> auto add(q::String const& name, T& node) -> bool;

private:
    template<class T> auto get_nodes() const -> Nodes&;
    mutable Nodes_Map m_nodes_map;
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

    auto get_buses()    -> Registry<node::bus::IBus>&;
    auto get_sources()  -> Registry<node::source::ISource>&;
    auto get_sinks()    -> Registry<node::sink::ISink>&;
    auto get_streams()  -> Registry<node::stream::IStream>&;

private:
    Registry<node::bus::IBus> m_buses;
    Registry<node::source::ISource> m_sources;
    Registry<node::sink::ISink> m_sinks;
    Registry<node::stream::IStream> m_streams;

    bool m_is_initialized = false;

    rapidjson::Value m_emptyValue;
    rapidjson::Document m_settings;

    struct Hardware;
    std::shared_ptr<Hardware> m_hw;

    auto load_settings() -> bool;
};


/////////////////////////////////////////////////////////////////////////////////////////////////


template<class Base>
template<class T>
auto Registry<Base>::get_all() const -> std::vector<std::pair<q::String const&, T*>> const&
{
    return get_nodes<T>();
}
template<class Base>
template<class T>
auto Registry<Base>::find_by_name(q::String const& name) const -> T*
{
    auto& nodes = get_nodes<T>();
    auto it = std::find_if(nodes.begin(), nodes.end(),
                           [&](std::pair<q::String const&, Base*> const& s) { return s.first == name; });
    return it != nodes.end() ? reinterpret_cast<T*>(it->second) : nullptr;
}
template<class Base>
template<class T>
auto Registry<Base>::add(q::String const& name, T& node) -> bool
{
    if (find_by_name<T>(name))
    {
        QLOGE("Duplicated name in node {}", name);
        return false;
    }
    auto& nodes = get_nodes<T>();
    nodes.push_back({name, static_cast<Base*>(&node)});
    return true;
}
template<class Base>
template<class T>
auto Registry<Base>::get_nodes() const -> Nodes&
{
    size_t hash = typeid(T).hash_code();
    Nodes& nodes = m_nodes_map[hash];
    return nodes;
}


}
