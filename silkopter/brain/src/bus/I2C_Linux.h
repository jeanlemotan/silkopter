#pragma once

#include "common/node/bus/II2C.h"

namespace silk
{
namespace node
{
namespace bus
{

class I2C_Linux : public II2C
{
public:
    I2C_Linux();
    ~I2C_Linux();

    auto open(q::String const& device) -> bool;
    void close();

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t address, uint8_t* data, size_t size) -> bool;
    auto write(uint8_t address, uint8_t const* data, size_t size) -> bool;

    auto read_register(uint8_t address, uint8_t reg, uint8_t* data, size_t size) -> bool;
    auto write_register(uint8_t address, uint8_t reg, uint8_t const* data, size_t size) -> bool;

private:
    q::String m_device;
    int m_fd = -1;
    std::recursive_mutex m_mutex;
    std::vector<uint8_t> m_buffer;
};

DECLARE_CLASS_PTR(I2C_Linux);


}
}
}
