#pragma once

namespace board
{

class Barometer_None : public Barometer
{
public:
	bool get_data(Data& data) const
	{
		data.pressure = 0.f;
		return false;
	}
};

}
