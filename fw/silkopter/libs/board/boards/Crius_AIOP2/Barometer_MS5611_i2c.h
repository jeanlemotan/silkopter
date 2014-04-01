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
		
	void get_data(Data& data) const;
	Thermometer const& get_thermometer() const;
};

}
