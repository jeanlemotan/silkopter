#pragma once

#include "rapidjson/document.h"

namespace silk
{

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

    //interfaces
    template<class T> auto get_all_interfaces() const -> std::vector<T*> const&;
    template<class T> auto find_interface_by_name(q::String const& name) const -> T*;

private:
    template<class T> auto add_interface(T* interface) -> bool;

    struct Registry_Base {};
    template <class T> struct Registry : public Registry_Base
    {
        std::vector<T*> interfaces;
    };
    mutable std::unordered_map<size_t, std::unique_ptr<Registry_Base>> m_registry;

    template<class T> auto get_registry() const -> Registry<T>&;

    bool m_is_initialized = false;

    rapidjson::Value m_emptyValue;
    rapidjson::Document m_settings;

    struct Hardware;
    std::shared_ptr<Hardware> m_hw;

    auto load_settings() -> bool;
};






/////////////////////////////////////////////////////////////////////////////////////////////////





template<class T>
auto HAL::get_all_interfaces() const -> std::vector<T*> const&
{
    return get_registry<T>().interfaces;
}
template<class T>
auto HAL::find_interface_by_name(q::String const& name) const -> T*
{
    auto& interfaces = get_registry<T>().interfaces;
    auto it = std::find_if(interfaces.begin(), interfaces.end(),
                           [&](T* s) { return s->get_name() == name; });
    return it != interfaces.end() ? *it : nullptr;
}

template<class T>
auto HAL::get_registry() const -> Registry<T>&
{
    size_t hash = typeid(T).hash_code();
    auto it = m_registry.find(hash);
    Registry<T>* registry = nullptr;
    if (it != m_registry.end())
    {
        registry = reinterpret_cast<Registry<T>*>(it->second.get());
    }
    else
    {
        registry = new Registry<T>();
        m_registry[hash].reset(registry);
    }
    QASSERT(registry);
    return *registry;
}

template<class T>
auto HAL::add_interface(T* interface) -> bool
{
    QASSERT(interface);
    if (!interface)
    {
        return false;
    }
    if (find_interface_by_name<T>(interface->get_name()))
    {
        QLOGE("Duplicated name in interface {}", interface->get_name());
        return false;
    }
    auto& registry = get_registry<T>();
    registry.interfaces.push_back(interface);
    return true;
}

}
