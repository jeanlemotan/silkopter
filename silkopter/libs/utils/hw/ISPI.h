#pragma once

#include "utils/Clock.h"

namespace util
{
namespace hw
{

class ISPI
{
public:
    virtual ~ISPI() = default;

    virtual bool transfer(void const* tx_data, void* rx_data, size_t size, uint32_t speed = 0) = 0;
    virtual bool transfer_register(uint8_t reg, void const* tx_data, void* rx_data, size_t size, uint32_t speed = 0) = 0;

    struct Transfer
    {
        void const* tx_data = nullptr;
        void* rx_data = nullptr;
        size_t size = 0;
        Clock::duration delay = Clock::duration::zero();
    };

    virtual bool transfers(Transfer const* transfers, size_t transfer_count, uint32_t speed = 0) = 0;

    //-----------------------------------
    //convenience method
    bool transfer_register_u16(uint8_t reg, uint16_t tx_data, uint16_t& rx_data, uint32_t speed = 0);
    bool transfer_register_u8(uint8_t reg, uint8_t tx_data, uint8_t& rx_data, uint32_t speed = 0);
};

//-----------------------------------

inline bool ISPI::transfer_register_u16(uint8_t reg, uint16_t tx_data, uint16_t& rx_data, uint32_t speed)
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
inline bool ISPI::transfer_register_u8(uint8_t reg, uint8_t tx_data, uint8_t& rx_data, uint32_t speed)
{
    return transfer_register(reg, &tx_data, &rx_data, 1, speed);
}

}
}
