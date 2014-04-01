#pragma once

#include "util/Noncopyable.h"

namespace board
{

	class RC_In : util::Noncopyable
	{
	public:
		struct Data
		{
			float value; //0 .. 1	
		};
		
		virtual uint8_t get_count() const = 0;
		//returns true if data is valid.
		//if the return value is false, the data is undefined
		virtual bool get_data(uint8_t channel, Data& data) const = 0;
	};
	
}

