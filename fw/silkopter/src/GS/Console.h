#pragma once

#include <vector>
#include "board/UART.h"
#include "util/FString.h"

class Console
{
public:
	Console(board::UART& uart);
	void process();

private:	
	void show_help();

	typedef util::FString<64> String;
	
	bool get_command(String& command) const;
	
	mutable String m_command;
	board::UART& m_uart;
};
