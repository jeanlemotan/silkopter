#pragma once

#include "HAL.h"
#include "common/node/bus/ISPI.h"

namespace sz
{
namespace SPI_Linux
{
class Init_Params;
class Config;
}
}


namespace silk
{
namespace node
{
namespace bus
{

class SPI_Linux : public ISPI
{
public:
    SPI_Linux(HAL& hal);
    ~SPI_Linux();

    auto init(rapidjson::Value const& init_params, rapidjson::Value const& config) -> bool;
    auto get_init_params() -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> rapidjson::Document;

    void close();

    auto get_name() const -> std::string const&;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t* data, size_t size) -> bool;
    auto write(uint8_t const* data, size_t size) -> bool;

    auto read_register(uint8_t reg, uint8_t* data, size_t size) -> bool;
    auto write_register(uint8_t reg, uint8_t const* data, size_t size) -> bool;

private:
    auto init() -> bool;

    HAL& m_hal;
    std::shared_ptr<sz::SPI_Linux::Init_Params> m_init_params;
    std::shared_ptr<sz::SPI_Linux::Config> m_config;

    int m_fd = -1;
    std::recursive_mutex m_mutex;
    std::vector<uint8_t> m_buffer;
};

}
}
}
