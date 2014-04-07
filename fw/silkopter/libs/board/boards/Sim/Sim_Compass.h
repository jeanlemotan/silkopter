#pragma once

#include "board/Compass.h"

namespace board
{
	
class Sim_Compass : public Compass
{
public:
	bool get_data(Data& data) const { return false; }
		
protected:
};
	
}
