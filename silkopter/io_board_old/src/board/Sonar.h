#pragma once

#include "QBase.h"

namespace board
{

	class Sonar : q::util::Noncopyable
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
            uint16_t distance;
            uint8_t id = 0;
		};
		
        //returns true if the data was written to
        virtual bool get_data(Data& data) const = 0;

        //start measuring
        virtual void start() = 0;
        virtual bool is_started() const = 0;
    };

}
