#include "src/GS/Console.h"

Console::Console(board::UART& uart)
	: m_uart(uart)
{
}

void Console::process()
{
}

bool Console::get_command(String& command) const
{
	String buf;
	buf.resize(String::MAX_SIZE);
	bool eoc = false; //end of command
	while (m_uart.has_data())
	{
		uint8_t ch;
		m_uart.read(ch);
		if (ch == '\n')
		{
			eoc = true;
			break;
		}
		buf.append(ch);
	}

	m_command.append(buf);
	if (eoc)
	{
		command = m_command;
		m_command.clear();
	}
	return eoc;
}
