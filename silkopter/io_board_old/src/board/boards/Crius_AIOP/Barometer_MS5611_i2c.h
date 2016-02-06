#pragma once

#include "board/Barometer.h"
#include "utils/Double_Buffer.h"

namespace board
{

class Barometer_MS5611_i2c : public Barometer
{
public:
	Barometer_MS5611_i2c();

	void init();
	
    Data_Config get_data_config() const { return m_data_config; }
    bool get_data(Data& data) const;

private:
	bool					m_is_initialized = false;
	bool					m_is_healthy = false;
	chrono::time_ms			m_last_update_time;

	mutable float			m_temperature = 0;
	mutable float			m_pressure = 0;

	// Internal calibration registers
	float					m_c1f = 0;
	float					m_c2f = 0;
	float					m_c3f = 0;
	float					m_c4f = 0;
	float					m_c5f = 0;
	float					m_c6f = 0;;
	mutable float			m_pressure_data = 0;
	mutable float			m_temperature_data = 0;;

	struct Buffer
	{
		volatile bool			has_data = false;
		volatile uint8_t		pressure_data_count = 0;
		volatile uint8_t		temperature_count = 0;
		volatile uint32_t		pressure_data = 0;
		volatile uint32_t		temperature_data = 0;
	};

	volatile uint8_t m_stage = 0;

	mutable util::Double_Buffer<Buffer> m_buffer;

    Data_Config m_data_config;
    mutable Data m_out_data;
	
	static void poll_data(void*);
	bool init_hardware();
	void calculate() const;
};

}
