#pragma once

#include "IBus.h"

namespace silk
{
namespace node
{
namespace bus
{

class ISPI: public IBus
{
public:
    virtual ~ISPI() {}

    virtual void lock() = 0;
    virtual auto try_lock() -> bool = 0;
    virtual void unlock() = 0;

    virtual auto read(uint8_t* data, size_t size) -> bool = 0;
    virtual auto write(uint8_t const* data, size_t size) -> bool = 0;

    virtual auto read_register(uint8_t reg, uint8_t* data, size_t size) -> bool = 0;
    virtual auto write_register(uint8_t reg, uint8_t const* data, size_t size) -> bool = 0;


    //convenience method
    auto read_register_u16(uint8_t reg, uint16_t& dst) -> bool
    {
        uint8_t val[2];
        if (read_register(reg, val, 2))
        {
            reinterpret_cast<uint8_t*>(&dst)[1] = val[0];
            reinterpret_cast<uint8_t*>(&dst)[0] = val[1];
            return true;
        }
        return false;
    }
    auto read_register_u8(uint8_t reg, uint8_t& dst) -> bool
    {
        return read_register(reg, &dst, 1);
    }
    auto write_register_u8(uint8_t reg, uint8_t const& t) -> bool
    {
        return write_register(reg, &t, 1);
    }
    auto write_register_u16(uint8_t reg, uint16_t const& t) -> bool
    {
        uint8_t val[2];
        val[0] = reinterpret_cast<uint8_t const*>(&t)[1];
        val[1] = reinterpret_cast<uint8_t const*>(&t)[0];
        return write_register(reg, val, 2);
    }
};

}
}
}
