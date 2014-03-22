#pragma once

#include "stdint.h"

namespace silk
{
	class Motor_Mixer
	{
	public:
		//type of the uav.
		//The PLUS version has an arm pointing forward while on the X version the front is between 2 arms
		enum class Type : uint8_t
		{
			X,
			PLUS
		};

		Motor_Mixer(Type type, uint8_t motor_count);
		
		void init_x(uint8_t motor_count);
		void init_plus(uint8_t motor_count);
		
	private:
		Type m_type;
		uint8_t m_motor_count;
	};
}