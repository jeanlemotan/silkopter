#pragma once

#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2
#   include "boards/Crius_AIOP2/inertial_mpu6000_i2c.h"
#else
#	error Define for your board
#endif

