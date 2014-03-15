#pragma once

#include <stdint.h>

class UART;

#if BOARD_TYPE == CRIUS_AIOP2
#   include "Crius_AIOP2_UART.h"
#else 
#	error Define for your board
#endif

