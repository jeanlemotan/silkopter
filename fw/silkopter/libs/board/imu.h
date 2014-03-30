#pragma once

#include <stdint.h>
#include <stddef.h>
#include <qmath.h>
#include "util/chrono.h"

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

    // calibrates the offsets for the specified duration
    extern void calibrate(chrono::millis duration);
	extern float get_rest_g();

	struct Data
	{
		struct 
		{
			bool is_valid;
			math::vec3f value;	
		} gyroscope;	
		struct
		{
			bool is_valid;
			math::vec3f value;
		} accelerometer;
	};

 	extern void get_data(Data& data);
}
}
