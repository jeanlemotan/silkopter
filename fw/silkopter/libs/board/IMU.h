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
		
		struct Calibration_Data
		{
			math::vec3f gyroscope_bias;
			math::vec3f accelerometer_bias;
			math::vec3f accelerometer_scale = math::vec3f(1.f);
		};
		virtual void set_calibration_data(Calibration_Data const& data) = 0;
		virtual Calibration_Data const& get_calibration_data() const = 0;

		struct Data
		{
			uint8_t sample_idx = 0; //this is incremented every time there is a new sample
				
			chrono::secondsf dt; //for how long is this measurement
			
			math::vec3f acceleration; //m/s/s
			
			math::vec3f angular_velocity;
		};

		//returns true if data is valid.
		//if the return value is false, the data is undefined
		virtual bool get_data(Data& data) const = 0;
	};

}
