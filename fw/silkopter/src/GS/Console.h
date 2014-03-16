#pragma once

#include <vector>
#include "HAL/UART.h"
#include "Format/FString.h"

class Console
{
public:
	Console(hal::UART& uart);
	void process();

private:	
	void show_help();
	
	bool get_command(FString<64>& command) const;
	
	typedef FString<64> String;

	mutable String m_command;
	hal::UART& m_uart;
};
