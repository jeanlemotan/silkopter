#pragma once

#include <stdint.h>
#include <stddef.h>
#include <qmath.h>
#include "util/chrono.h"
#include "board/IMU.h"

namespace board
{

class Sim_IMU : public IMU
{
public:
	Sim_IMU();
	
    void init(Sample_Rate rate);
	
	//sets calibration data
	void set_gyroscope_bias(math::vec3f const& bias);
	void set_accelerometer_bias_scale(math::vec3f const& bias, math::vec3f const& scale);

 	bool get_data(Data& data) const;
	 
private:
	 bool m_is_initialised = false;
};

}
