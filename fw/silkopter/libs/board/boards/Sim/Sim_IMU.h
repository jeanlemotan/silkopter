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
	void set_calibration_data(Calibration_Data const& data) { m_calibration_data = data; }
	Calibration_Data const& get_calibration_data() const { return m_calibration_data;  }

 	bool get_data(Data& data) const;
	 
private:
	 bool m_is_initialised = false;
	 Calibration_Data m_calibration_data;
};

}
