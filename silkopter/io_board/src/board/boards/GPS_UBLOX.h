#pragma once

#include "QBase.h"

namespace board
{
	
	class GPS : q::util::Noncopyable
	{
	public:
		struct Data
		{
			
		};
		
		virtual bool get_data(Data& data) const = 0;
	};
	
}
