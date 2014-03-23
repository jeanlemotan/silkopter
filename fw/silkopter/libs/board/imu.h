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
		RATE_50_HZ,
		RATE_100_HZ,
		RATE_250_HZ,
		RATE_500_HZ
	};
    extern void init(Sample_Rate rate);

    // calibrates the offsets for the specified duration
    extern void calibrate(chrono::millis duration);

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
