#include "Config.h"

#if BOARD_TYPE == BOARD_CRIUS_AIOP_2

#include <avr/io.h>
#include <avr/interrupt.h>
#include "board/board.h"
#include "Barometer_MS5611_i2c.h"
#include "board/boards/AVR_i2c.h"
#include "utils/Timed_Scope.h"

namespace board
{

static const uint8_t MS5611_ADDR = 0x77;
static const uint8_t CMD_MS5611_RESET = 0x1E;
static const uint8_t CMD_MS5611_PROM_Setup = 0xA0;
static const uint8_t CMD_MS5611_PROM_C1 = 0xA2;
static const uint8_t CMD_MS5611_PROM_C2 = 0xA4;
static const uint8_t CMD_MS5611_PROM_C3 = 0xA6;
static const uint8_t CMD_MS5611_PROM_C4 = 0xA8;
static const uint8_t CMD_MS5611_PROM_C5 = 0xAA;
static const uint8_t CMD_MS5611_PROM_C6 = 0xAC;
static const uint8_t CMD_MS5611_PROM_CRC = 0xAE;
static const uint8_t CMD_CONVERT_D1_OSR4096 = 0x48;   // Maximum resolution (oversampling)
static const uint8_t CMD_CONVERT_D2_OSR4096 = 0x58;   // Maximum resolution (oversampling)

//////////////////////////////////////////////////////////////////////////

Barometer_MS5611_i2c::Barometer_MS5611_i2c()
{
    m_data_config.bias = 0.f;
    m_data_config.scale = 1.f;
}

void Barometer_MS5611_i2c::init()
{
	if (m_is_initialized)
	{
		return;
	}
	m_is_initialized = true;

	i2c::init();
	
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
	QASSERT(baro);
	
	uint8_t pressure_buf[3];
	bool pressure_ok = false;
	
	uint8_t temperature_buf[3];
	bool temperature_ok = false;

	if (!i2c::try_lock())
	{
		return;
	}
	
	baro->m_last_update_time = clock::now_ms();
	auto& buffer = baro->m_buffer.get();

	if (baro->m_stage == 0)
	{
		temperature_ok = i2c::read_registers(MS5611_ADDR, 0x00, temperature_buf, sizeof(temperature_buf));
		i2c::write(MS5611_ADDR, CMD_CONVERT_D1_OSR4096); //read pressure next
		baro->m_stage++;
	}
	else
	{
		pressure_ok = i2c::read_registers(MS5611_ADDR, 0x00, pressure_buf, sizeof(pressure_buf));
		if (baro->m_stage == 5)
		{
			i2c::write(MS5611_ADDR, CMD_CONVERT_D2_OSR4096); //read temp next
			baro->m_stage = 0;
		}
		else
		{
			i2c::write(MS5611_ADDR, CMD_CONVERT_D1_OSR4096); //read pressure next
			baro->m_stage++;
		}
	}
	i2c::unlock(); //unlock as quick as possible

	if (pressure_ok)
	{
		uint32_t val = (((uint32_t)pressure_buf[0]) << 16) | (((uint32_t)pressure_buf[1]) << 8) | pressure_buf[2];
		buffer.pressure_data += val;// On state 0 we read temp

		buffer.pressure_data_count++;
		if (buffer.pressure_data_count == 128)
		{
			// we have summed 128 values. This only happens
			// when we stop reading the barometer for a long time
			// (more than 1.2 seconds)
			buffer.pressure_data >>= 1;
			buffer.pressure_data_count = 64;
		}
		buffer.has_data = true;
	}

	if (temperature_ok)
	{
		uint32_t val = (((uint32_t)temperature_buf[0]) << 16) | (((uint32_t)temperature_buf[1]) << 8) | temperature_buf[2];
		buffer.temperature_data += val;// On state 0 we read temp
				
		buffer.temperature_count++;
		if (buffer.temperature_count == 32)
		{
			// we have summed 32 values. This only happens
			// when we stop reading the barometer for a long time
			// (more than 1.2 seconds)
			buffer.temperature_data >>= 1;
			buffer.temperature_count = 16;
		}
	}
}


bool Barometer_MS5611_i2c::init_hardware()
{
	QASSERT(m_is_initialized);
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

	if (!i2c::write(MS5611_ADDR, CMD_MS5611_RESET))
	{
		return false;
	}
	clock::delay(chrono::millis(50));

	// We read the factory calibration
	// The on-chip CRC is not used
	uint16_t C1, C2, C3, C4, C5, C6;
	if (!i2c::read_registers_uint16_le(MS5611_ADDR, CMD_MS5611_PROM_C1, C1) ||
		!i2c::read_registers_uint16_le(MS5611_ADDR, CMD_MS5611_PROM_C2, C2) ||
		!i2c::read_registers_uint16_le(MS5611_ADDR, CMD_MS5611_PROM_C3, C3) ||
		!i2c::read_registers_uint16_le(MS5611_ADDR, CMD_MS5611_PROM_C4, C4) ||
		!i2c::read_registers_uint16_le(MS5611_ADDR, CMD_MS5611_PROM_C5, C5) ||
		!i2c::read_registers_uint16_le(MS5611_ADDR, CMD_MS5611_PROM_C6, C6))
	{
		return false;
	}
    //q::quick_logf(F_STR("PROM: {} {} {} {} {} {}"), C1, C2, C3, C4, C5, C6);
	
	m_c1f = C1;
	m_c2f = C2;
	m_c3f = C3;
	m_c4f = C4;
	m_c5f = (uint32_t(C5) << 8);
	m_c6f = C6;

	//Send a command to read Temp first
	if (!i2c::write(MS5611_ADDR, CMD_CONVERT_D2_OSR4096))
	{
		return false;
	}
	m_last_update_time = clock::now_ms();

	scheduler::register_callback(chrono::millis(20), poll_data, this);
			
	i2c::unlock();

	// wait for at least one value to be read
	auto start = clock::now_ms();
	while (!m_buffer.get().has_data)
	{
		clock::delay(chrono::millis(10));
		if (clock::now_ms() - start > chrono::millis(1000))
		{
			QASSERT("PANIC: Baro took more than 1000ms to initialize");
			m_is_healthy = false;
			return false;
		}
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
	float dT = m_temperature_data - m_c5f;
	//float TEMP = (dT * m_c6f)/8388608.f;
	float TEMP = (dT * m_c6f)*0.00000011920928955078125f;
	//float OFF = m_c2f * 65536.0f + (m_c4f * dT) / 128.f;
	float OFF = m_c2f * 65536.0f + (m_c4f * dT) * 0.0078125f;
	//float SENS = m_c1f * 32768.0f + (m_c3f * dT) / 256.f;
	float SENS = m_c1f * 32768.0f + (m_c3f * dT) * 0.00390625f;
    //q::quick_logf(F_STR(":{} {} {} {}:"), dT, TEMP, OFF, SENS);

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

	m_temperature = (TEMP + 2000.f) * 0.01f;
	//m_pressure = (m_pressure_data*SENS/2097152.f - OFF)/32768.f;
	m_pressure = (m_pressure_data*SENS*0.000000476837158203125f - OFF)*0.000030517578125f;
	
//	q::quick_logf(F_STR("pressure: {}, temp: {}"), m_pressure, m_temperature);
}

//////////////////////////////////////////////////////////////////////////
// PUBLIC API

bool Barometer_MS5611_i2c::get_data(Data& data) const
{
	QASSERT(m_is_initialized);

    if (m_is_healthy && m_buffer.get().has_data)
	{
		auto& buffer = m_buffer.get_and_swap();

		uint32_t pressure_data = buffer.pressure_data; 
		buffer.pressure_data = 0;
		
		uint32_t temperature_data = buffer.temperature_data; 
		buffer.temperature_data = 0;
		
		uint8_t pressure_data_count = buffer.pressure_data_count; 
		buffer.pressure_data_count = 0;
		
		uint8_t temperature_data_count = buffer.temperature_count; 
		buffer.temperature_count = 0;
		
		buffer.has_data = false;

		bool calc = false;
		if (pressure_data_count != 0)
		{
			m_pressure_data = ((float)pressure_data) / pressure_data_count;
			calc = true;
		}
		if (temperature_data_count != 0)
		{
			m_temperature_data = ((float)temperature_data) / temperature_data_count;
			calc = true;
		}
		if (calc)
		{
			calculate();
            m_out_data.pressure = m_pressure;
            m_out_data.id++;

//            m_thermometer.m_is_valid = m_is_healthy;
//            m_thermometer.m_data.degrees = m_temperature;
//            m_thermometer.m_data.id++;
        }
	}

    if (m_out_data.id != data.id)
    {
        data = m_out_data;
        return true;
    }
    return false;
}

//Thermometer const& Barometer_MS5611_i2c::get_thermometer() const
//{
//	return m_thermometer;
//}

}
#endif
