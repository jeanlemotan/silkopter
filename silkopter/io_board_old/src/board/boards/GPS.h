#pragma once

#include "QBase.h"

namespace board
{
	
class GPS : q::util::Noncopyable
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
