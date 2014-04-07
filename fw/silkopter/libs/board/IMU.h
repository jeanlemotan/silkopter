#pragma once

#include "util/Noncopyable.h"
#include <stdint.h>
#include <stddef.h>
#include <_qmath.h>

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

		struct Data
		{
			Data() : sample_idx(0) {}
			uint8_t sample_idx; //this is incremented every time there is a new sample
				
			chrono::secondsf dt; //for how long is this measurement
			
			math::vec3f acceleration; //m/s/s
			
			//accumulated euler/s value. This is a delta that has to be applied once per increment of sample_idx
			math::vec3f gyroscope; 
		};

		//returns true if data is valid.
		//if the return value is false, the data is undefined
		virtual bool get_data(Data& data) const = 0;
	};

}
