#pragma once

#include "board/Barometer.h"

namespace board
{

class Sim_Barometer : public Barometer
{
public:
	Sim_Barometer();

	void init();
	
	bool get_data(Data& data) const;
	
private:
	bool m_is_initialized = false;
};

}
