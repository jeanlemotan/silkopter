#pragma once

namespace board
{
namespace baro
{
	extern void init();
	extern bool is_enabled();
	
	struct Data
	{
		struct
		{	
			bool is_valid;
			int8_t value; //degrees
		} temperature;

		struct
		{
			bool is_valid;
			float value;
		} pressure;
	};
	
	extern void get_data(Data& data);
}
}
