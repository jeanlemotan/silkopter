#pragma once

#include "QBase.h"

namespace board
{
	class Thermometer : q::util::Noncopyable
	{
	public:
        struct Data_Config
        {
            float bias = 0.f;
            float scale = 1.f;
        };
        virtual Data_Config get_data_config() const = 0;

		struct Data
		{
            int16_t temperature = 0;
            uint8_t id = 0;
        };
				
        //returns true if the data was written to
        virtual bool get_data(Data& data) const = 0;
	};
}
