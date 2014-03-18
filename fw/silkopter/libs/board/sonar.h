#pragma once

#include "Config.h"

#if SONAR_TYPE == SONAR_NONE
#	include "boards/Crius_AIOP2/sonar_none.h"
#elif SONAR_TYPE == SONAR_SR04
#	if BOARD_TYPE == CRIUS_AIOP2
#   	include "boards/Crius_AIOP2/sonar_sr04.h"
#	endif
#endif

