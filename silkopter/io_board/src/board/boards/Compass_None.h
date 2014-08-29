#pragma once

namespace board
{

class Compass_None : public Compass
{
public:
	bool get_data(Data& data) const
	{
		return false;
	}
};

}
