#pragma once

#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2
#   include "boards/Crius_AIOP2/UART.h"
#else 
#	error Define for your board
#endif

