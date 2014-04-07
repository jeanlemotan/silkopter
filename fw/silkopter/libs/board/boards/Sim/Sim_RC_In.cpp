#include "Config.h"

#if BOARD_TYPE == SIM

#include "_qmath.h"
#include "debug/debug.h"
#include "board/board.h"
#include "board/boards/Sim/Sim_RC_In.h"

namespace board
{

Sim_RC_In::Sim_RC_In()
{
}

void Sim_RC_In::init()
{
	if (m_is_initialized)
	{
		return;
	}
	m_is_initialized = true;
}

bool Sim_RC_In::get_data(uint8_t ch, Data& data) const
{
	ASSERT(m_is_initialized);
    if (ch >= MAX_CHANNEL_COUNT) 
	{
		return false;
	}
	data.value = 0;
	return true;
}

uint8_t Sim_RC_In::get_count() const
{
	ASSERT(m_is_initialized);
	return MAX_CHANNEL_COUNT;
}



}

#endif
