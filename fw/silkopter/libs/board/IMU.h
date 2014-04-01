#pragma once

#include <stdint.h>
#include <stddef.h>
#include <qmath.h>

namespace board
{
	class IMU : util::Noncopyable
	{
	public:
		enum class Sample_Rate
		{
			_50_HZ		= 50,
			_100_HZ		= 100,
			_250_HZ		= 250,
			_500_HZ		= 500
		};
		
		//sets calibration data
		virtual void set_gyroscope_bias(math::vec3f const& bias) = 0;
		virtual void set_accelerometer_bias_scale(math::vec3f const& bias, math::vec3f const& scale) = 0;

		struct Gyroscope_Data
		{
			chrono::secondsf dt; //delta time for this data
			math::vec3f delta; //radians/s!!! - it's a delta
		};
		struct Accelerometer_Data
		{
			chrono::secondsf dt; //delta time for this data
			math::vec3f m; //m/s/s
		};

		//returns true if data is valid.
		//if the return value is false, the data is undefined
		virtual bool get_data(Gyroscope_Data& gdata, Accelerometer_Data& adata) const = 0;
	};

}
