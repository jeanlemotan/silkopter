#pragma once

#include "common/bus/II2C.h"

namespace sz
{
namespace I2C_RPI
{
struct Init_Params;
struct Config;
}
}


namespace silk
{
namespace bus
{

class I2C_RPI : public II2C
{
public:
    I2C_RPI();
    ~I2C_RPI();

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t address, uint8_t* data, size_t size) -> bool;
    auto write(uint8_t address, uint8_t const* data, size_t size) -> bool;

    auto read_register(uint8_t address, uint8_t reg, uint8_t* data, size_t size) -> bool;
    auto write_register(uint8_t address, uint8_t reg, uint8_t const* data, size_t size) -> bool;

private:
    auto init() -> bool;

    std::shared_ptr<sz::I2C_RPI::Init_Params> m_init_params;
    std::shared_ptr<sz::I2C_RPI::Config> m_config;

    std::recursive_mutex m_mutex;
    std::vector<uint8_t> m_buffer;

    static std::mutex s_mutex;
};

}
}
