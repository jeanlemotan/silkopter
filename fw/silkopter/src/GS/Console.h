#pragma once

#include <vector>
#include "board/UART.h"
#include "Format/FString.h"

class Console
{
public:
	Console(board::UART& uart);
	void process();

private:	
	void show_help();
	
	bool get_command(FString<64>& command) const;
	
	typedef FString<64> String;

	mutable String m_command;
	board::UART& m_uart;
};
