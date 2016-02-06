#pragma once

#include "board/board.h"

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
    extern bool write_registers(uint8_t addr, uint8_t reg, const uint8_t* data, uint8_t size);
	
	template<class T>
	bool write(uint8_t addr, T const& data)
	{
		auto const* __restrict x = reinterpret_cast<uint8_t const*>(&data);
		return write(addr, x, sizeof(T));
	}
	template<class T>
	bool write_registers(uint8_t addr, uint8_t reg, T const& data)
	{
		auto const* __restrict x = reinterpret_cast<uint8_t const*>(&data);
		return write_registers(addr, reg, x, sizeof(T));
	}

    extern bool read(uint8_t addr, uint8_t* data, uint8_t size);
    extern bool read_registers(uint8_t addr, uint8_t reg, uint8_t* data, uint8_t size);
	extern bool read_registers_uint16_le(uint8_t addr, uint8_t reg, uint16_t* data, uint8_t size); //this converts to little endian

	template<class T>
	bool read(uint8_t addr, T& data)
	{
		auto* __restrict x = reinterpret_cast<uint8_t*>(&data);
		return read(addr, x, sizeof(T));
	}
	template<class T>
	bool read_registers(uint8_t addr, uint8_t reg, T& data)
	{
		auto* __restrict x = reinterpret_cast<uint8_t*>(&data);
		return read_registers(addr, reg, x, sizeof(T));
	}
	template<class T>
	bool read_registers_uint16_le(uint8_t addr, uint8_t reg, T& data)
	{
		auto* __restrict x = reinterpret_cast<uint16_t*>(&data);
		return read_registers_uint16_le(addr, reg, x, sizeof(T)/sizeof(uint16_t));
	}

    extern uint16_t get_lockup_count();

	//exclusive access	
	extern bool lock(chrono::micros timeout = chrono::micros(0));
    extern bool try_lock();
	extern void unlock();
};

}


