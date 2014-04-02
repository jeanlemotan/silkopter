#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2

#include <avr/io.h>
#include <avr/interrupt.h>
#include "debug/debug.h"
#include "board/board.h"
#include "Barometer_MS5611_i2c.h"
#include "board/boards/avr_i2c.h"

namespace board
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

//////////////////////////////////////////////////////////////////////////

static uint16_t _read_uint16(uint8_t reg)
{
	uint16_t buf[1];
	if (i2c::read_registers_le(MS5611_ADDR, reg, buf, 1))
	{
		return buf[0];
	}
	TRACE_MSG("i2c failed");
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

Barometer_MS5611_i2c::Barometer_MS5611_i2c()
	: m_is_healthy(false)
	, m_temperature(0)
	, m_pressure(0)
// 	, m_raw_pressure(0)
// 	, m_raw_temperature(0)
	, m_c1f(0)
	, m_c2f(0)
	, m_c3f(0)
	, m_c4f(0)
	, m_c5f(0)
	, m_c6f(0)
	, m_d1(0)
	, m_d2(0)
	, m_stage(0)
	, m_buffer_idx(0)
{
	uint8_t tries = 0;
	do
	{
		bool success = init_hardware();
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

// Read the sensor. This is a state machine
// We read one time Temperature (state=1) and then 4 times Pressure (states 2-5)
// temperature does not change so quickly...
void Barometer_MS5611_i2c::poll_data(void* ptr)
{
	auto* baro = reinterpret_cast<Barometer_MS5611_i2c*>(ptr);
	ASSERT(baro);
	
	auto now = clock::now_us();
	// Throttle read rate to 100hz maximum.
	if (now - baro->m_last_update_time < k_delay)
	{
		return;
	}

	if (!i2c::try_lock())
	{
		return;
	}

	baro->m_last_update_time = now;

	auto& buffer = baro->m_buffers[baro->m_buffer_idx];

	baro->m_stage++;
	if ((baro->m_stage & 7) == 0)
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


bool Barometer_MS5611_i2c::init_hardware()
{
#ifdef SIMULATOR
	return true;
#endif
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
	m_c1f = C1;
	m_c2f = C2;
	m_c3f = C3;
	m_c4f = C4;
	m_c5f = (C5 << 8);
	m_c6f = C6;
//	PRINT(":{0} {1} {2} {3} {4} {5}:", C1, C2, C3, C4, C5, C6);

	//Send a command to read Temp first
	_write(CMD_CONVERT_D2_OSR4096);
	m_last_update_time = clock::now_us();
	m_stage = 0;
	m_temperature = 0;
	m_pressure = 0;

	scheduler::register_callback(poll_data, this);
			
	i2c::unlock();

	// wait for at least one value to be read
	auto start = clock::now_ms();
	while (!m_buffers[m_buffer_idx].has_data)
	{
		clock::delay(chrono::millis(10));
		if (clock::now_ms() - start > chrono::millis(1000))
		{
			ASSERT("PANIC: Baro took more than 1000ms to initialize");
			m_is_healthy = false;
			return false;
		}
	#ifdef SIMULATOR
		break;
	#endif
	}
	m_is_healthy = true;
	return true;
}

// Calculate Temperature and compensated Pressure in real units (Celsius degrees*100, mbar*100).
void Barometer_MS5611_i2c::calculate() const
{
	// Formulas from manufacturer datasheet
	// sub -20c temperature compensation is not included

	// we do the calculations using floating point
	// as this is much faster on an AVR2560, and also allows
	// us to take advantage of the averaging of D1 and D1 over
	// multiple samples, giving us more precision
	float dT = m_d2 - m_c5f;
	float TEMP = (dT * m_c6f)/8388608.f;
	float OFF = m_c2f * 65536.0f + (m_c4f * dT) / 128.f;
	float SENS = m_c1f * 32768.0f + (m_c3f * dT) / 256.f;
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

	float P = (m_d1*SENS/2097152.f - OFF)/32768.f;
	m_temperature = (TEMP + 2000.f) * 0.01f;
	m_pressure = P;
}

//////////////////////////////////////////////////////////////////////////
// PUBLIC API

bool Barometer_MS5611_i2c::get_data(Data& data) const
{
	if (m_buffers[m_buffer_idx].has_data)
	{
		auto last_buffer_idx = m_buffer_idx;
		m_buffer_idx = m_buffer_idx ^ uint8_t(1); //now the interrupt will write in the other buffer.
		auto& buffer = m_buffers[last_buffer_idx];

		uint32_t d1 = buffer.d1; 
		buffer.d1 = 0;
		
		uint32_t d2 = buffer.d2; 
		buffer.d2 = 0;
		
		uint8_t d1count = buffer.d1_count; 
		buffer.d1_count = 0;
		
		uint8_t d2count = buffer.d2_count; 
		buffer.d2_count = 0;
		
		buffer.has_data = false;

		bool calc = false;
		if (d1count != 0)
		{
			m_d1 = ((float)d1) / d1count;
			calc = true;
		}
		if (d2count != 0)
		{
			m_d2 = ((float)d2) / d2count;
			calc = true;
		}
		if (calc)
		{
			calculate();
		}
	}

	data.pressure = m_pressure;
	
	m_thermometer.m_is_valid = m_is_healthy;
	m_thermometer.m_data.degrees = m_temperature;
	
	return m_is_healthy;
}

}
#endif
