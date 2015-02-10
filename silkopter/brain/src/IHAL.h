#pragma once

#include "interface/II2C.h"
#include "interface/ISPI.h"
#include "interface/IUART.h"
#include "interface/ICamera.h"
#include "interface/IAccelerometer.h"
#include "interface/IGyroscope.h"
#include "interface/ICompass.h"
#include "interface/IBarometer.h"
#include "interface/IThermometer.h"
#include "interface/IADC.h"
#include "interface/IVoltmeter.h"
#include "interface/IAmmeter.h"
#include "interface/IGPS.h"
#include "interface/ISonar.h"
#include "interface/IPWM.h"

#include "rapidjson/document.h"

namespace silk
{

class IHAL : q::util::Noncopyable
{
public:
    virtual ~IHAL() {}

    virtual auto init() -> bool = 0;
    virtual void process() = 0;
    virtual void shutdown() = 0;

    //settings
    virtual auto get_settings(q::Path const& path) -> rapidjson::Value& = 0;
    virtual void save_settings() const = 0;

    //interfaces
    template<class T> auto get_all_interfaces() const -> std::vector<T*> const&;
    template<class T> auto find_interface_by_name(q::String const& name) const -> T*;

protected:

    template<class T> auto add_interface(T* interface) -> bool;

private:
    struct Registry_Base {};
    template <class T> struct Registry : public Registry_Base
    {
        std::vector<T*> interfaces;
    };
    mutable std::unordered_map<size_t, std::unique_ptr<Registry_Base>> m_registry;

    template<class T> auto get_registry() const -> Registry<T>&;
};


/////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
auto IHAL::get_all_interfaces() const -> std::vector<T*> const&
{
    return get_registry<T>().interfaces;
}
template<class T>
auto IHAL::find_interface_by_name(q::String const& name) const -> T*
{
    auto& interfaces = get_registry<T>().interfaces;
    auto it = std::find_if(interfaces.begin(), interfaces.end(),
                           [&](T* s) { return s->get_name() == name; });
    return it != interfaces.end() ? *it : nullptr;
}

template<class T>
auto IHAL::get_registry() const -> Registry<T>&
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
auto IHAL::add_interface(T* interface) -> bool
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
