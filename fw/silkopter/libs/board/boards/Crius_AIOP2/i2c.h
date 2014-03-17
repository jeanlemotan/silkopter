#pragma once

namespace board
{
namespace i2c
{
    extern void init();

//     void begin();
//     void end();
//     void setTimeout(uint16_t ms) { _timeoutDelay = ms; }
    extern void set_high_speed(bool active);

    extern uint8_t write(uint8_t addr, const uint8_t* data, uint8_t size);
    extern uint8_t write_register(uint8_t addr, uint8_t reg, uint8_t val);
    extern uint8_t write_registers(uint8_t addr, uint8_t reg, const uint8_t* data, uint8_t size);

    extern uint8_t read(uint8_t addr, uint8_t* data, uint8_t size);
    extern uint8_t read_register(uint8_t addr, uint8_t reg, uint8_t& data);
    extern uint8_t read_registers(uint8_t addr, uint8_t reg, uint8_t* data, uint8_t size);

    extern uint8_t get_lockup_count();

	//exclusive access	
	extern bool lock();
	extern void unlock();
};

}


