#pragma once

#include <stdint.h>
#include <stddef.h>
#include <qmath.h>
#include "util/chrono.h"
#include "Thermometer.h"

namespace board
{
	void _poll_data(void*);
	
class IMU
{
	friend void _poll_data(void*);
public:
	IMU();
	
	enum class Sample_Rate
	{
		_50_HZ		= 50,
		_100_HZ		= 100,
		_250_HZ		= 250,
		_500_HZ		= 500
	};
	//initialize the sensor to a sample rate
    void init(Sample_Rate rate);
	
	//sets calibration data
	void set_gyroscope_bias(math::vec3f const& bias);
	void set_accelerometer_bias_scale(math::vec3f const& bias, math::vec3f const& scale);

	struct Data
	{
		struct 
		{
			bool is_valid;
			chrono::secondsf dt;
			math::vec3f value; //radians/s!!! - it's a delta
		} gyroscope;	
		struct
		{
			bool is_valid;
			math::vec3f value; //m/s/s
		} accelerometer;
	};

 	void get_data(Data& data) const;
	 
	Thermometer const& get_thermometer() const;
	 
private:
	 volatile chrono::time_us m_last_refresh_time;
	 uint8_t m_mpu_addr;
	 int8_t m_sample_freq_div;
	 volatile int8_t m_sample_freq_counter;
	 bool m_is_initialised;

	struct Buffer
	{
		Buffer() : temp_sum(0), sample_count(0) {}
	
		volatile math::vec3s32 accel_sum;
		volatile math::vec3s32 gyro_sum;
		volatile uint16_t temp_sum;
		volatile chrono::micros delta_time_sum;
		volatile uint8_t sample_count;
	};
	mutable volatile Buffer m_buffers[2];
	mutable volatile uint8_t m_buffer_idx;

	math::vec3s16 m_gyro_calibration_bias;
	math::vec3s16 m_accel_calibration_bias;
	math::vec3f m_accel_calibration_scale;

	int16_t m_raw_mpu[7];
	
	Thermometer m_thermometer;
	
	void poll_data();
	void refresh_data(Data& data) const;
	//chrono::time_ms s_last_refresh_time;

	void set_filter_register(uint8_t filter_hz, uint8_t default_filter);
	bool init_hardware(Sample_Rate sample_rate);
};

}
