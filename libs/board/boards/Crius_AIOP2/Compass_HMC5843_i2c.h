#pragma once

#include "board/Compass.h"

namespace board
{
	
class Compass_HMC5843 : public Compass
{
public:
	bool get_data(Data& data) const { return false; }
		
protected:
};
	
}
