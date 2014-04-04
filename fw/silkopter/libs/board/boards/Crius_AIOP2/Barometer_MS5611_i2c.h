#pragma once

#include "board/Barometer.h"
#include "board/boards/Crius_AIOP2/Thermometer_Data.h"

namespace board
{

class Barometer_MS5611_i2c : public Barometer
{
public:
	Barometer_MS5611_i2c();

	void init();
	
	bool get_data(Data& data) const;
	
	Thermometer const& get_thermometer() const;
	
private:
	mutable Thermometer_Data m_thermometer;
	
	bool					m_is_initialized;
	bool					m_is_healthy;
	chrono::time_us			m_last_update_time;
	/* Gates access to asynchronous state: */

	mutable float			m_temperature;
	mutable float			m_pressure;

	//mutable int32_t			m_raw_pressure;
	//mutable int32_t			m_raw_temperature;
	// Internal calibration registers
	float					m_c1f, m_c2f, m_c3f, m_c4f, m_c5f, m_c6f;
	mutable float			m_d1, m_d2;

	struct Buffer
	{
		Buffer() : has_data(false), d1_count(0), d2_count(0), d1(0), d2(0) {}
		volatile bool			has_data;
		volatile uint8_t		d1_count;
		volatile uint8_t		d2_count;
		volatile uint32_t		d1, d2;
	};

	volatile uint8_t m_stage;

	mutable volatile Buffer m_buffers[2];
	mutable volatile uint8_t m_buffer_idx;
	
	static void poll_data(void*);
	bool init_hardware();
	void calculate() const;

};

}
