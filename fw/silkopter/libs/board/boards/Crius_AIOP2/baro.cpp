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
static const chrono::micros		k_delay(1000000 / k_update_frequency);

static bool						s_is_healthy = false;
static chrono::time_us			s_last_update_time;
/* Gates access to asynchronous state: */

static float					s_temperature;
static float					s_pressure;

static int32_t					s_raw_press;
static int32_t					s_raw_temp;
// Internal calibration registers
static float					C1f, C2f, C3f, C4f, C5f, C6f;
static float					D1, D2;

static bool						s_is_initialized = false;

//////////////////////////////////////////////////////////////////////////


struct Buffer
{
	Buffer() : has_data(false), d1_count(0), d2_count(0), d1(0), d2(0) {}
	volatile bool			has_data;
	volatile uint8_t		d1_count;
	volatile uint8_t		d2_count;
	volatile uint32_t		d1, d2;
};

static volatile uint8_t s_stage = 0;

static volatile Buffer s_buffers[2];
static volatile uint8_t s_buffer_idx = 0;

static uint16_t _read_uint16(uint8_t reg)
{
	uint16_t buf[1];
	if (i2c::read_registers_le(MS5611_ADDR, reg, buf, 1))
	{
		return buf[0];
	}
	ASSERT(0);
	return 0;
}

static uint32_t _read_adc()
{
	uint8_t buf[3];
	if (i2c::read_registers(MS5611_ADDR, 0x00, buf, sizeof(buf)))
	{
		return (((uint32_t)buf[0]) << 16) | (((uint32_t)buf[1]) << 8) | buf[2];
	}
	TRACE_MSG("i2c failed");
	return 0;
}

static void _write(uint8_t reg)
{
	if (i2c::write(MS5611_ADDR, &reg, 1))
	{
		return;
	}
	TRACE_MSG("i2c failed");
}

// Read the sensor. This is a state machine
// We read one time Temperature (state=1) and then 4 times Pressure (states 2-5)
// temperature does not change so quickly...
static void _update()
{
	auto now = clock::now_us();
	// Throttle read rate to 100hz maximum.
	if (now - s_last_update_time < k_delay)
	{
		return;
	}

	if (!i2c::try_lock())
	{
		return;
	}

	s_last_update_time = now;

	auto& buffer = s_buffers[s_buffer_idx];

	s_stage++;
	if ((s_stage & 7) == 0)
	{
		_write(CMD_CONVERT_D2_OSR4096); // Command to read temperature
		buffer.d2 += _read_adc();// On state 0 we read temp
		buffer.d2_count++;
		if (buffer.d2_count == 32)
		{
			// we have summed 32 values. This only happens
			// when we stop reading the barometer for a long time
			// (more than 1.2 seconds)
			buffer.d2 >>= 1;
			buffer.d2_count = 16;
		}
	}

	_write(CMD_CONVERT_D1_OSR4096);      // Command to read pressure
	buffer.d1 += _read_adc();
	buffer.d1_count++;
	if (buffer.d1_count == 128)
	{
		// we have summed 128 values. This only happens
		// when we stop reading the barometer for a long time
		// (more than 1.2 seconds)
		buffer.d1 >>= 1;
		buffer.d1_count = 64;
	}
	buffer.has_data = true;

	i2c::unlock();
}


static bool _init_hardware()
{
//	TRACE();
	int lock_tries = 1000;
	while (!i2c::lock() && lock_tries >= 0)
	{
		lock_tries--;
		clock::delay(chrono::micros(10));
	}
	if (lock_tries < 0)
	{
		return false;
	}

	_write(CMD_MS5611_RESET);
	clock::delay(chrono::millis(50));

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
	s_last_update_time = clock::now_us();
	s_stage = 0;
	s_temperature = 0;
	s_pressure = 0;

	scheduler::register_callback(_update);
			
	i2c::unlock();

	// wait for at least one value to be read
	auto start = clock::now_ms();
	while (!s_buffers[s_buffer_idx].has_data)
	{
		clock::delay(chrono::millis(10));
		if (clock::now_ms() - start > chrono::millis(1000))
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
			clock::delay(chrono::millis(50));
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
	if (s_buffers[s_buffer_idx].has_data)
	{
		auto last_buffer_idx = s_buffer_idx;
		s_buffer_idx = !s_buffer_idx; //now the interrupt will write in the other buffer.
		auto& buffer = s_buffers[last_buffer_idx];

		uint32_t sD1 = buffer.d1; 
		buffer.d1 = 0;
		uint32_t sD2 = buffer.d2; 
		buffer.d2 = 0;
		uint8_t d1count = buffer.d1_count; 
		buffer.d1_count = 0;
		uint8_t d2count = buffer.d2_count; 
		buffer.d2_count = 0;
		buffer.has_data = false;

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
