#include "Config.h"

#if BOARD_TYPE == SIM

#include <_qmath.h>
#include "debug/debug.h"
#include "board/board.h"
#include "board/boards/Sim/Sim_Sonar.h"

//////////////////////////////////////////////////////////////////////////

namespace board
{

//////////////////////////////////////////////////////////////////////////

Sim_Sonar::Sim_Sonar()
{
}

bool Sim_Sonar::get_data(Data& data) const
{
	data.distance = 0;
	return true;
}


}

#endif
