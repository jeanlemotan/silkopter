#pragma once

#include "common/bus/II2C.h"

namespace silk
{
namespace hal
{
struct I2C_BCM_Descriptor;
}
}

namespace silk
{
namespace bus
{

class I2C_BCM : public II2C
{
public:
    I2C_BCM();
    ~I2C_BCM();

    bool init(hal::IBus_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::IBus_Descriptor> get_descriptor() const override;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t address, uint8_t* data, size_t size) -> bool;
    auto write(uint8_t address, uint8_t const* data, size_t size) -> bool;

    auto read_register(uint8_t address, uint8_t reg, uint8_t* data, size_t size) -> bool;
    auto write_register(uint8_t address, uint8_t reg, uint8_t const* data, size_t size) -> bool;

private:
    bool init(uint32_t dev, uint32_t baud);

    std::shared_ptr<hal::I2C_BCM_Descriptor> m_descriptor;

    std::recursive_mutex m_mutex;
    std::vector<uint8_t> m_buffer;

    static std::mutex s_mutex;
};

}
}
