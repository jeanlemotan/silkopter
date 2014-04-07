#pragma once

#include "util/Noncopyable.h"

namespace board
{
	
class GPS : util::Noncopyable
{
	public:
	struct Data
	{
		bool is_valid;
	};
		
	void get_data(Data& data) const;
		
	protected:
};
	
}
