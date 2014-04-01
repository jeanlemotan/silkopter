#pragma once

namespace board
{

class Sonar : util::Noncopyable
{
public:
	struct Data
	{
		bool is_valid;
		float distance;
	};
		
	void get_data(Data& data) const
	{ 
		data.is_valid = false; 
		data.distance = 0.f;
	} 
};

}