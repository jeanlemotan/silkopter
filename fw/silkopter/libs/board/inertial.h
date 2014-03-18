#pragma once

#include <stdint.h>
#include <qmath.h>

namespace board
{
namespace inertial
{
	enum class Sample_Rate
	{
		RATE_50_HZ,
		RATE_100_HZ,
		RATE_200_HZ,
		RATE_500_HZ
	};
    extern void init(Sample_Rate rate);

    // sample_available - true when a new sample is available
    extern bool has_data();

    // get_delta_time returns the time period in seconds overwhich the sensor data was collected
    extern float get_delta_time();

 	extern const math::vec3f& get_gyroscope_data();
 	extern const math::vec3f& get_accelerometer_data();
}
}
