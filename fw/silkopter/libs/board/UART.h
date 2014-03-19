#pragma once

#include "Config.h"

#if CPU_TYPE == ATMEGA_2560
#   include "boards/avr_UART.h"
#else 
#	error Define for your cpu
#endif

