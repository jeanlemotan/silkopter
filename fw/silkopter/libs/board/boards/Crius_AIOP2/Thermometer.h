#pragma once

#include "util/Noncopyable.h"

namespace board
{
	
	class Thermometer : util::Noncopyable
	{
	public:
		struct Data
		{
			bool is_valid;
			float value; //degrees celsius
		};
		
		void get_data(Data& data) const;
		
	protected:
		Data m_data;
	};
	
}
