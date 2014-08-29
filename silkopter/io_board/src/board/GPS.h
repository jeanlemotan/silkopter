#pragma once

#include "QBase.h"

namespace board
{
	
	class GPS : q::util::Noncopyable
	{
	public:
		struct Data
		{
            uint8_t id = 0;
		};
		
        virtual void get_data(Data& data) const = 0;
    };
	
}
