#pragma once

#include "util/Noncopyable.h"

namespace board
{

	class PWM_Out : util::Noncopyable
	{
	public:
		struct Data
		{
			float value; //0 .. 1
		};
		
		virtual uint8_t get_count() const = 0;
		virtual void set_data(uint8_t channel, Data const& data) = 0;
		virtual void set_data_for_all(Data const& data) = 0;
		virtual void set_enabled(uint8_t channel, bool enabled) = 0;
		virtual bool is_enabled(uint8_t channel) const = 0;
		virtual void set_enabled_for_all(bool enabled) = 0;
		
		//sets the frequency of a channel in hertz
		//note - some channels are coupled so changing the frequency of one will change for others as well
		//This should be documented in the board
		virtual void set_frequency(uint8_t ch, uint16_t hz) = 0;
		virtual void set_frequency_for_all(uint16_t hz) = 0;
	};
	
}

