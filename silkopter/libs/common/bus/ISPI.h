#pragma once

#include "IBus.h"

namespace silk
{
namespace bus
{

class ISPI: public IBus
{
public:
    virtual ~ISPI() = default;

    virtual void lock() = 0;
    virtual auto try_lock() -> bool = 0;
    virtual void unlock() = 0;

    virtual auto transfer(uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed = 0) -> bool = 0;
    virtual auto transfer_register(uint8_t reg, uint8_t const* tx_data, uint8_t* rx_data, size_t size, uint32_t speed = 0) -> bool = 0;

    //convenience method
    auto transfer_register_u16(uint8_t reg, uint16_t tx_data, uint16_t& rx_data, uint32_t speed = 0) -> bool
    {
        uint8_t tx_val[2] = { reinterpret_cast<uint8_t*>(&tx_data)[1], reinterpret_cast<uint8_t*>(&tx_data)[0] };
        uint8_t rx_val[2] = {  0 };
        if (transfer_register(reg, tx_val, rx_val, 2, speed))
        {
            reinterpret_cast<uint8_t*>(&rx_data)[1] = rx_val[0];
            reinterpret_cast<uint8_t*>(&rx_data)[0] = rx_val[1];
            return true;
        }
        return false;
    }
    auto transfer_register_u8(uint8_t reg, uint8_t tx_data, uint8_t& rx_data, uint32_t speed = 0) -> bool
    {
        return transfer_register(reg, &tx_data, &rx_data, 1, speed);
    }
};


}
}
