#pragma once

#include "II2C.h"
#include <mutex>
#include <vector>
#include <string>

namespace util
{
namespace hw
{

class I2C_Dev : public II2C
{
public:
    I2C_Dev();
    ~I2C_Dev();

    ts::Result<void> init(std::string const& device);

    void lock() override;
    bool try_lock() override;
    void unlock() override;

    bool read(uint8_t address, uint8_t* data, size_t size) override;
    bool write(uint8_t address, uint8_t const* data, size_t size) override;

    bool read_register(uint8_t address, uint8_t reg, uint8_t* data, size_t size) override;
    bool write_register(uint8_t address, uint8_t reg, uint8_t const* data, size_t size) override;

private:
    void close();

    std::string m_device;
    int m_fd = -1;
    std::vector<uint8_t> m_buffer;
    mutable std::recursive_mutex m_mutex;
};

}
}
