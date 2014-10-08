#pragma once


namespace silk
{

//this class is used to initialize and shutdown the pigpio library

class PiGPIO : q::util::Noncopyable
{
public:
	~PiGPIO();

	enum class Result
	{
		OK,
		FAILED,
	};

	auto init() -> Result;
};


}
