#pragma once

#include "QBase.h"
#include "_qmath.h"
#include <stdint.h>
#include <stddef.h>

namespace board
{
	
	class Compass : q::util::Noncopyable
	{
	public:
        struct Data_Config
        {
            math::vec3f bias;
            math::vec3f scale = math::vec3f::one;
        };
        virtual Data_Config get_data_config() const = 0;

		struct Data
		{
            math::vec3s16 direction;
            uint8_t id = 0;
		};
		
        //returns true if the data was written to
        virtual bool get_data(Data& data) const = 0;
    };
	
}
