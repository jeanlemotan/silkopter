#pragma once

namespace util
{
namespace hw
{

class ISPI
{
public:
    virtual ~ISPI() = default;

    virtual void lock() = 0;
    virtual bool try_lock() = 0;
    virtual void unlock() = 0;

    virtual bool transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed = 0) = 0;
    virtual bool transfer_register(uint8_t reg, void const* tx_data, void* rx_data, size_t size, uint32_t speed = 0) = 0;
};

}
}
