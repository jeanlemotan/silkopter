#pragma once

#include "board/IThermometer.h"

namespace board
{

	//simple implementation that has its data filled 
	// by someone else (the imu and barometer for example)	
	class Thermometer_Data : Thermometer
	{
	public:
		void get_data(Data& data) const { data = m_data; }
		
	protected:
		Data m_data;
	};
	
}
