#pragma once

#include "board/clock.h"

namespace board
{
namespace i2c
{
    extern void init();

//     void begin();
//     void end();
//     void setTimeout(uint16_t ms) { _timeoutDelay = ms; }
    extern void set_high_speed(bool active);

    extern bool write(uint8_t addr, const uint8_t* data, uint8_t size);
    extern bool write_register(uint8_t addr, uint8_t reg, uint8_t val);
    extern bool write_registers(uint8_t addr, uint8_t reg, const uint8_t* data, uint8_t size);

    extern bool read(uint8_t addr, uint8_t* data, uint8_t size);
    extern bool read_register(uint8_t addr, uint8_t reg, uint8_t& data);
    extern bool read_registers(uint8_t addr, uint8_t reg, uint8_t* data, uint8_t size);
	extern bool read_registers_le(uint8_t addr, uint8_t reg, uint16_t* data, uint8_t size); //this converts to little endian

    extern uint16_t get_lockup_count();

	//exclusive access	
	extern bool lock(chrono::micros timeout = chrono::micros(0));
    extern bool try_lock();
	extern void unlock();
};

}


