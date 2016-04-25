#pragma once

#include "common/bus/II2C.h"

namespace silk
{
namespace bus
{

class I2C_BCM : public II2C
{
public:
    I2C_BCM(ts::Type_System const& ts);
    ~I2C_BCM();

    bool init(std::shared_ptr<ts::IValue> descriptor) override;
    std::shared_ptr<const ts::IValue> get_descriptor() const override;

    void lock();
    auto try_lock() -> bool;
    void unlock();

    auto read(uint8_t address, uint8_t* data, size_t size) -> bool;
    auto write(uint8_t address, uint8_t const* data, size_t size) -> bool;

    auto read_register(uint8_t address, uint8_t reg, uint8_t* data, size_t size) -> bool;
    auto write_register(uint8_t address, uint8_t reg, uint8_t const* data, size_t size) -> bool;

private:
    bool init(uint32_t dev);

    std::shared_ptr<ts::IValue> m_descriptor;

    std::recursive_mutex m_mutex;
    std::vector<uint8_t> m_buffer;

    static std::mutex s_mutex;
};

}
}
