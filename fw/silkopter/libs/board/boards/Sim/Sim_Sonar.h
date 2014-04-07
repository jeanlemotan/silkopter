#pragma once

#include "board/Sonar.h"

namespace board
{

class Sim_Sonar : public Sonar
{
public:
	Sim_Sonar();
	
	bool get_data(Data& data) const;
	
private:
};

}