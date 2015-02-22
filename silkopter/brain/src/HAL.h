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
class Registry : q::util::Noncopyable
{
    friend class HAL;
public:
    auto get_all() const -> std::vector<Base*> const&;
    template<class T> auto find_by_name(std::string const& name) const -> T*;

    template<class T> auto add(T& node) -> bool;

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
    Registry<node::bus::IBus> m_buses;
    Registry<node::source::ISource> m_sources;
    Registry<node::sink::ISink> m_sinks;
    Registry<node::processor::IProcessor> m_processors;
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
auto Registry<Base>::get_all() const -> std::vector<Base*> const&
{
    return m_nodes;
}
template<class Base>
template<class T>
auto Registry<Base>::find_by_name(std::string const& name) const -> T*
{
    auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](Base* s) { return s->get_name() == name; });
    return it != m_nodes.end() ? dynamic_cast<T*>(*it) : nullptr;
}
template<class Base>
template<class T>
auto Registry<Base>::add(T& node) -> bool
{
    if (find_by_name<Base>(node.get_name()))
    {
        QLOGE("Duplicated name in node {}", node.get_name());
        return false;
    }
    m_nodes.push_back(static_cast<Base*>(&node));
    return true;
}

}
