#pragma once

#include <vector>
#include "board/board.h"
#include "util/FString.h"

class Console
{
public:
	Console(board::UART& uart);
	void process();

private:	
	void show_help();

	typedef util::String<64> String;
	
	bool get_command(String& command) const;
	
	mutable String m_command;
	board::UART& m_uart;
};
