#pragma once

#include "common/bus/II2C.h"

namespace silk
{
namespace uav
{
struct I2C_Linux_Descriptor;
}
}

namespace silk
{
namespace bus
{

class I2C_Linux : public II2C
{
public:
    I2C_Linux();
    ~I2C_Linux();

    bool init(std::shared_ptr<uav::IBus_Descriptor> descriptor) override;
    std::shared_ptr<uav::IBus_Descriptor> get_descriptor() const override;

    void close();

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t address, uint8_t* data, size_t size) -> bool;
    auto write(uint8_t address, uint8_t const* data, size_t size) -> bool;

    auto read_register(uint8_t address, uint8_t reg, uint8_t* data, size_t size) -> bool;
    auto write_register(uint8_t address, uint8_t reg, uint8_t const* data, size_t size) -> bool;

private:
    bool init(std::string const& dev);

    std::shared_ptr<uav::I2C_Linux_Descriptor> m_descriptor;

    int m_fd = -1;
    std::recursive_mutex m_mutex;
    std::vector<uint8_t> m_buffer;
};

}
}
