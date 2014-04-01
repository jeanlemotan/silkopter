#pragma once

#include "util/Noncopyable.h"

namespace board
{

	class Barometer : util::Noncopyable
	{
	public:
		struct Data
		{
			float pressure;
		};
		
		//returns true if data is valid.
		//if the return value is false, the data is undefined
		virtual bool get_data(Data& data) const = 0;
	};

}
