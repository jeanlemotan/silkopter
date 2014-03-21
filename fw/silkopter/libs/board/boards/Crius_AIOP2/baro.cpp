#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2

#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug/debug.h"
#include "board/clock.h"
#include "board/baro.h"
#include "board/scheduler.h"
#include "board/boards/avr_i2c.h"

namespace board
{
namespace baro
{

#define MS5611_ADDR 0x77
#define CMD_MS5611_RESET 0x1E
#define CMD_MS5611_PROM_Setup 0xA0
#define CMD_MS5611_PROM_C1 0xA2
#define CMD_MS5611_PROM_C2 0xA4
#define CMD_MS5611_PROM_C3 0xA6
#define CMD_MS5611_PROM_C4 0xA8
#define CMD_MS5611_PROM_C5 0xAA
#define CMD_MS5611_PROM_C6 0xAC
#define CMD_MS5611_PROM_CRC 0xAE
#define CMD_CONVERT_D1_OSR4096 0x48   // Maximum resolution (oversampling)
#define CMD_CONVERT_D2_OSR4096 0x58   // Maximum resolution (oversampling)

static const uint32_t			k_update_frequency = 50;
static const uint32_t			k_delay_us = 1000000 / k_update_frequency;

static bool						s_is_healthy = false;
static volatile bool            s_has_data = false;
static volatile uint8_t         s_d1_count;
static volatile uint8_t         s_d2_count;
static volatile uint32_t        s_s_d1, s_s_d2;
static uint8_t                  s_state = 0;
static uint32_t                 s_last_update_us = 0;
/* Gates access to asynchronous state: */

static float					s_temperature;
static float					s_pressure;

static int32_t					s_raw_press;
static int32_t					s_raw_temp;
// Internal calibration registers
static float					C1f, C2f, C3f, C4f, C5f, C6f;
static float					D1, D2;

static bool						s_is_initialized = false;

static uint16_t _read_uint16(uint8_t reg)
{
	uint16_t buf[1];
	if (i2c::read_registers_le(MS5611_ADDR, reg, buf, 1) == 0)
	{
		return buf[0];
	}
	ASSERT(0);
	return 0;
}

static uint32_t _read_adc()
{
	uint8_t buf[3];
	if (i2c::read_registers(MS5611_ADDR, 0x00, buf, sizeof(buf)) == 0)
	{
		return (((uint32_t)buf[0]) << 16) | (((uint32_t)buf[1]) << 8) | buf[2];
	}
	ASSERT(0);
	return 0;
}

static void _write(uint8_t reg)
{
	if (i2c::write(MS5611_ADDR, &reg, 1) == 0)
	{
		return;
	}
	ASSERT(0);
}

// Read the sensor. This is a state machine
// We read one time Temperature (state=1) and then 4 times Pressure (states 2-5)
// temperature does not change so quickly...
static void _update(uint32_t micros)
{
	// Throttle read rate to 100hz maximum.
	if (micros - s_last_update_us < k_delay_us)
	{
		return;
	}

	if (!i2c::lock())
	{
		return;
	}

//	TRACE();
			
	s_last_update_us = micros;

	if (s_state == 0)
	{
		s_s_d2 += _read_adc();// On state 0 we read temp
		s_d2_count++;
		if (s_d2_count == 32)
		{
			// we have summed 32 values. This only happens
			// when we stop reading the barometer for a long time
			// (more than 1.2 seconds)
			s_s_d2 >>= 1;
			s_d2_count = 16;
		}
		s_state++;
		_write(CMD_CONVERT_D1_OSR4096);      // Command to read pressure
	}
	else
	{
		s_s_d1 += _read_adc();
		s_d1_count++;
		if (s_d1_count == 128)
		{
			// we have summed 128 values. This only happens
			// when we stop reading the barometer for a long time
			// (more than 1.2 seconds)
			s_s_d1 >>= 1;
			s_d1_count = 64;
		}
		s_state++;
		// Now a new reading exists
		s_has_data = true;
		if (s_state == 5)
		{
			_write(CMD_CONVERT_D2_OSR4096); // Command to read temperature
			s_state = 0;
		}
		else
		{
			_write(CMD_CONVERT_D1_OSR4096); // Command to read pressure
		}
	}

	i2c::unlock();
}


static bool _init_hardware()
{
//	TRACE();
	int lock_tries = 1000;
	while (!i2c::lock() && lock_tries >= 0)
	{
		lock_tries--;
		clock::delay_micros(10);
	}
	if (lock_tries < 0)
	{
		return false;
	}

	_write(CMD_MS5611_RESET);
	clock::delay_millis(50);

	// We read the factory calibration
	// The on-chip CRC is not used
	uint32_t C1 = _read_uint16(CMD_MS5611_PROM_C1);
	uint32_t C2 = _read_uint16(CMD_MS5611_PROM_C2);
	uint32_t C3 = _read_uint16(CMD_MS5611_PROM_C3);
	uint32_t C4 = _read_uint16(CMD_MS5611_PROM_C4);
	uint32_t C5 = _read_uint16(CMD_MS5611_PROM_C5);
	uint32_t C6 = _read_uint16(CMD_MS5611_PROM_C6);
	C1f = C1;
	C2f = C2;
	C3f = C3;
	C4f = C4;
	C5f = (C5 << 8);
	C6f = C6;
//	PRINT(":{0} {1} {2} {3} {4} {5}:", C1, C2, C3, C4, C5, C6);

	//Send a command to read Temp first
	_write(CMD_CONVERT_D2_OSR4096);
	s_last_update_us = clock::micros();
	s_state = 0;
	s_temperature=0;
	s_pressure=0;

	s_s_d1 = 0;
	s_s_d2 = 0;
	s_d1_count = 0;
	s_d2_count = 0;

	scheduler::register_callback(_update);
			
	i2c::unlock();

	// wait for at least one value to be read
	auto start = clock::millis();
	while (!s_has_data)
	{
		clock::delay_millis(10);
		if (clock::millis() - start > 1000)
		{
			ASSERT("PANIC: Baro took more than 1000ms to initialize");
			s_is_healthy = false;
			return false;
		}
	}
	s_is_healthy = true;
	return true;
}

// Calculate Temperature and compensated Pressure in real units (Celsius degrees*100, mbar*100).
static void _calculate()
{
	// Formulas from manufacturer datasheet
	// sub -20c temperature compensation is not included

	// we do the calculations using floating point
	// as this is much faster on an AVR2560, and also allows
	// us to take advantage of the averaging of D1 and D1 over
	// multiple samples, giving us more precision
	float dT = D2 - C5f;
	float TEMP = (dT * C6f)/8388608.f;
	float OFF = C2f * 65536.0f + (C4f * dT) / 128.f;
	float SENS = C1f * 32768.0f + (C3f * dT) / 256.f;
	//PRINT(":{0} {1} {2} {3}:", dT, TEMP, OFF, SENS);

	if (TEMP < 0)
	{
		// second order temperature compensation when under 20 degrees C
		float T2 = (dT*dT) / 0x80000000;
		float Aux = TEMP*TEMP;
		float OFF2 = 2.5f*Aux;
		float SENS2 = 1.25f*Aux;
		TEMP = TEMP - T2;
		OFF = OFF - OFF2;
		SENS = SENS - SENS2;
	}

	float P = (D1*SENS/2097152.f - OFF)/32768.f;
	s_temperature = (TEMP + 2000.f) * 0.01f;
	s_pressure = P;
}

//////////////////////////////////////////////////////////////////////////
// PUBLIC API

void init()
{
	if (s_is_initialized)
	{
		return;
	}
	s_is_initialized = true;

	scheduler::init();

	uint8_t tries = 0;
	do
	{
		bool success = _init_hardware();
		if (success)
		{
			break;
		}
		else
		{
			clock::delay_millis(50); // delay for 50ms
		}
				
		if (tries++ > 5)
		{
			PANIC_MSG("Failed to boot Baro 5 times");
		}
	} while (1);
}

void get_data(Data& data)
{
	ASSERT(s_is_initialized);
	bool updated = s_has_data;
	if (updated)
	{
		// Suspend timer procs because these variables are written to
		// in "_update".
		scheduler::suspend();
		uint32_t sD1 = s_s_d1; 
		s_s_d1 = 0;
		uint32_t sD2 = s_s_d2; 
		s_s_d2 = 0;
		uint8_t d1count = s_d1_count; 
		s_d1_count = 0;
		uint8_t d2count = s_d2_count; 
		s_d2_count = 0;
		s_has_data = false;
		scheduler::resume();

		if (d1count != 0)
		{
			D1 = ((float)sD1) / d1count;
		}
		if (d2count != 0)
		{
			D2 = ((float)sD2) / d2count;
		}
		//_pressure_samples = d1count;
		s_raw_press = D1;
		s_raw_temp = D2;
	}
	_calculate();
// 	if (updated)
// 	{
// 		_last_update = hal.scheduler->millis();
// 	}

	data.pressure.is_valid = s_is_healthy;
	data.pressure.value = s_pressure;
	data.temperature.is_valid = s_is_healthy;
	data.temperature.value = s_temperature;
}


}
}
#endif
