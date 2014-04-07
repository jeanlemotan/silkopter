#include "Config.h"

#if BOARD_TYPE == SIM

#include "debug/debug.h"
#include "board/board.h"
#include "Sim_Barometer.h"

namespace board
{

//////////////////////////////////////////////////////////////////////////

Sim_Barometer::Sim_Barometer()
{
}

void Sim_Barometer::init()
{
	if (m_is_initialized)
	{
		return;
	}
	m_is_initialized = true;
}

bool Sim_Barometer::get_data(Data& data) const
{
	ASSERT(m_is_initialized);

	data.pressure = 0.f;

	return true;
}

}
#endif
