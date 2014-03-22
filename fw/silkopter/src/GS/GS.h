#pragma once

#include <stdint.h>
#include <stddef.h>
#include "GS/SFull_Protocol.h"
#include "GS/SCompact_Protocol.h"
#include "util/Noncopyable.h"
#include "board/UART.h"

namespace silk
{

	class GS : util::Noncopyable
	{
	public:
		explicit GS(board::UART& full_uart);
		GS(board::UART& full_uart, board::UART& compact_uart);

		void process();

	private:
		board::UART& m_full_uart;
		board::UART* m_compact_uart;

		SFull_Protocol m_full_protocol;
		SCompact_Protocol m_compact_protocol;

		
	};

}