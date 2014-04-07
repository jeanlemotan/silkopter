#include "Config.h"

#if BOARD_TYPE == SIM

#include "_qmath.h"
#include "debug/debug.h"
#include "board/board.h"
#include "board/boards/Sim/Sim_PWM_Out.h"

namespace board
{

static const uint8_t MAX_CHANNEL_COUNT = 8;

Sim_PWM_Out::Sim_PWM_Out()
{
}

void Sim_PWM_Out::init()
{
	if (m_is_initialized)
	{
		return;
	}
	m_is_initialized = true;
}

void Sim_PWM_Out::set_frequency(uint8_t ch, uint16_t hz)
{
	ASSERT(m_is_initialized);
}
void Sim_PWM_Out::set_frequency_for_all(uint16_t hz)
{
	ASSERT(m_is_initialized);

	for (uint8_t i = 0; i < MAX_CHANNEL_COUNT; i++)
	{
		set_frequency(i, hz);
	}
}
void Sim_PWM_Out::set_data(uint8_t ch, Data const& data)
{
	ASSERT(m_is_initialized);
} 
void Sim_PWM_Out::set_data_for_all(Data const& data)
{
	ASSERT(m_is_initialized);
}
void Sim_PWM_Out::set_enabled_for_all(bool enabled)
{
	ASSERT(m_is_initialized);
}
void Sim_PWM_Out::set_enabled(uint8_t ch, bool enabled)
{
	ASSERT(m_is_initialized);
}

uint8_t Sim_PWM_Out::get_count() const
{
	ASSERT(m_is_initialized);
	return MAX_CHANNEL_COUNT;
}

}

#endif
