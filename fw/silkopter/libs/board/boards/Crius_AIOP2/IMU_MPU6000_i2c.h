#pragma once

#include <stdint.h>
#include <stddef.h>
#include <qmath.h>
#include "util/chrono.h"
#include "board/IMU.h"
#include "Thermometer_Data.h"

namespace board
{

class IMU_MPU6000_i2c : public IMU
{
public:
	IMU_MPU6000_i2c();
	
    void init(Sample_Rate rate);
	
	//sets calibration data
	void set_gyroscope_bias(math::vec3f const& bias);
	void set_accelerometer_bias_scale(math::vec3f const& bias, math::vec3f const& scale);

 	bool get_data(Gyroscope_Data& gdata, Accelerometer_Data& adata) const;
	 
	Thermometer const& get_thermometer() const;
	 
private:
	 mutable chrono::time_us m_last_refresh_time;
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
	
	mutable Thermometer_Data m_thermometer;
	
	mutable math::vec3f m_last_accel_data;
	mutable float m_last_temp_data;
	
	static void poll_data(void* ptr);
	bool refresh_data(Gyroscope_Data& gdata, Accelerometer_Data& adata) const;
	//chrono::time_ms s_last_refresh_time;

	void set_filter_register(uint8_t filter_hz, uint8_t default_filter);
	bool init_hardware(Sample_Rate sample_rate);
};

}
