#pragma once

#include "board/Thermometer.h"

namespace board
{
	class Sim_Barometer;
	class Sim_IMU;

	//simple implementation that has its data filled 
	// by someone else (the imu and barometer for example)	
	class Sim_Thermometer : Thermometer
	{
		friend class Sim_Barometer;
		friend class Sim_IMU;
	public:
		bool get_data(Data& data) const { data = m_data; return m_is_valid; }
		
	protected:
		Data m_data;
		bool m_is_valid = false;
	};
	
}
