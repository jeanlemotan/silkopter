#pragma once

namespace board
{
namespace sonar
{

	extern void init();
	extern bool is_enabled();
	
	struct Data
	{
		bool is_valid;
		float altitude;
	};
	
	extern void get_data(Data& data);
		
}
}

