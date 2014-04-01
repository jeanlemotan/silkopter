#pragma once

#include <stdint.h>
#include <stddef.h>
#include "GS/SFull_Protocol.h"
#include "GS/SCompact_Protocol.h"
#include "util/Noncopyable.h"
#include "board/board.h"
#include "util/chrono.h"
#include "UAV.h"

namespace silk
{

	class GS : util::Noncopyable
	{
	public:
		explicit GS(UAV& uav, board::UART& full_uart);
		GS(UAV& uav, board::UART& full_uart, board::UART& compact_uart);

		void process(chrono::micros max_duration);

	private:
		UAV& m_uav;
		board::UART& m_full_uart;
		board::UART* m_compact_uart;

		SFull_Protocol m_full_protocol;
		//SCompact_Protocol m_compact_protocol;
		//SFull_Protocol m_compact_protocol;
		
		uint32_t m_step;
		
		chrono::time_us m_last_time;
		chrono::micros m_frame_duration;
		
		chrono::time_us m_last_hello_time;

		
		bool send_data(uint32_t step); //returns true when done
	};
}