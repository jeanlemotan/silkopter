#pragma once

#include "board/Thermometer.h"

namespace board
{
	class Barometer_MS5611_i2c;
	class IMU_MPU6000_i2c;

	//simple implementation that has its data filled 
	// by someone else (the imu and barometer for example)	
	class Thermometer_Data : Thermometer
	{
		friend class Barometer_MS5611_i2c;
		friend class IMU_MPU6000_i2c;
	public:
		bool get_data(Data& data) const { data = m_data; return m_is_valid; }
		
	protected:
		Data m_data;
		bool m_is_valid = false;
	};
	
}
