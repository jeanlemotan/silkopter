#pragma once

namespace board
{
	class Thermometer : util::Noncopyable
	{
	public:
		struct Data
		{
			float degrees; //celsius
		};
				
		//returns true if data is valid.
		//if the return value is false, the data is undefined
		virtual bool get_data(Data& data) const = 0;
	};
}
