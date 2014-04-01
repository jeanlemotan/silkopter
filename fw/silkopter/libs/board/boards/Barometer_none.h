#pragma once

namespace board
{

class Barometer : util::Noncopyable
{
	struct Data
	{
		bool is_valid;
		float pressure;
	};
		
	void get_data(Data& data) const
	{
		data.is_valid = false;
		data.pressure = 0.f;
	}
};
}
