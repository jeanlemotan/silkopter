#pragma once

namespace board
{

class Sonar_None : Sonar
{
public:
	bool get_data(Data& data) const
	{ 
		data.distance = 0.f;
		return false;
	} 
};

}