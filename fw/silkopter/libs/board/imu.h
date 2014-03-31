#pragma once

#include <stdint.h>
#include <stddef.h>
#include <qmath.h>
#include "util/chrono.h"
#include "board/UART.h"

namespace board
{
namespace imu
{
	enum class Sample_Rate
	{
		_50_HZ		= 50,
		_100_HZ		= 100,
		_250_HZ		= 250,
		_500_HZ		= 500
	};
    extern void init(Sample_Rate rate);
	
	extern void set_gyroscope_bias(math::vec3f const& bias);
	extern void set_accelerometer_bias_scale(math::vec3f const& bias, math::vec3f const& scale);

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

 	extern void get_data(Data& data);
}
}
