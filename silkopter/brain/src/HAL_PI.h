#pragma once

#include "IHAL.h"
#include "rapidjson/document.h"

namespace silk
{

class HAL_Pi : public IHAL
{
public:
    HAL_Pi();
    ~HAL_Pi();

    auto init() -> bool;
    void process();
    void shutdown();

    auto get_settings(q::Path const& path) -> rapidjson::Value&;
    void save_settings() const;

private:
    auto is_bus_name_unique(q::String const& name) const -> bool;
    auto is_sensor_name_unique(q::String const& name) const -> bool;
    auto is_actuator_name_unique(q::String const& name) const -> bool;

    bool m_is_initialized = false;

    rapidjson::Value m_emptyValue;
    rapidjson::Document m_settings;

    struct Hardware;
    std::shared_ptr<Hardware> m_hw;

    auto load_settings() -> bool;
};

}
