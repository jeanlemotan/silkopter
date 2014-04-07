#include "Config.h"

#if BOARD_TYPE == SIM

#include "board/boards/Sim/Sim_IMU.h"
#include "debug/debug.h"
#include "util/format.h"
#include "board/board.h"
#include "physics/constants.h"

namespace board
{


Sim_IMU::Sim_IMU()
{
}

void Sim_IMU::init(Sample_Rate rate)
{
	if (m_is_initialised)
	{
		return;
	}
	m_is_initialised = true;
}

bool Sim_IMU::get_data(Data& data) const
{
	ASSERT(0);
	ASSERT(m_is_initialised);
	return true;
}

void Sim_IMU::set_gyroscope_bias(math::vec3f const& bias)
{
	ASSERT(0);
}
void Sim_IMU::set_accelerometer_bias_scale(math::vec3f const& bias, math::vec3f const& scale)
{
	ASSERT(0);
}

}

#endif
