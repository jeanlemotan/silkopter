#pragma once

#include <cstdint>
#include <memory>

namespace util
{
namespace hw
{

class II2C
{
public:
    virtual ~II2C() = default;

    virtual bool read(uint8_t address, uint8_t* data, size_t size) = 0;
    virtual bool write(uint8_t address, uint8_t const* data, size_t size) = 0;

    virtual bool read_register(uint8_t address, uint8_t reg, uint8_t* data, size_t size) = 0;
    virtual bool write_register(uint8_t address, uint8_t reg, uint8_t const* data, size_t size) = 0;

    //-----------------------------------

    bool read_register_u16(uint8_t address, uint8_t reg, uint16_t& dst);
    bool read_register_u8(uint8_t address, uint8_t reg, uint8_t& dst);
    bool write_register_u8(uint8_t address, uint8_t reg, uint8_t t);
    bool write_register_u16(uint8_t address, uint8_t reg, uint16_t t);
};


//-----------------------------------

inline bool II2C::read_register_u16(uint8_t address, uint8_t reg, uint16_t& dst)
{
    uint8_t val[2];
    if (read_register(address, reg, val, 2))
    {
        reinterpret_cast<uint8_t*>(&dst)[1] = val[0];
        reinterpret_cast<uint8_t*>(&dst)[0] = val[1];
        return true;
    }
    return false;
}
inline bool II2C::read_register_u8(uint8_t address, uint8_t reg, uint8_t& dst)
{
    return read_register(address, reg, &dst, 1);
}
inline bool II2C::write_register_u8(uint8_t address, uint8_t reg, uint8_t t)
{
    return write_register(address, reg, &t, 1);
}
inline bool II2C::write_register_u16(uint8_t address, uint8_t reg, uint16_t t)
{
    uint8_t val[2];
    val[0] = reinterpret_cast<uint8_t const*>(&t)[1];
    val[1] = reinterpret_cast<uint8_t const*>(&t)[0];
    return write_register(address, reg, val, 2);
}

}
}
