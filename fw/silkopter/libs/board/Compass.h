#pragma once

#include "util/Noncopyable.h"

namespace board
{
	
	class Compass : util::Noncopyable
	{
	public:
		struct Data
		{
			math::vec3f radians;
		};
		
		//returns true if data is valid.
		//if the return value is false, the data is undefined
		virtual bool get_data(Data& data) const = 0;
	};
	
}
