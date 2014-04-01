#pragma once

#include "util/Noncopyable.h"

namespace board
{
	
	class GPS : util::Noncopyable
	{
	public:
		struct Data
		{
			
		};
		
		virtual bool get_data(Data& data) const = 0;
	};
	
}
