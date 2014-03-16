#pragma once

#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2
#   include "boards/Crius_AIOP2/clock.h"
#else
#	error Define for your board
#endif

