#include "Motor_Mixer.h"
#include "debug/debug.h"

using namespace silk;

Motor_Mixer::Motor_Mixer(Type type, uint8_t motor_count)
	: m_type(type)
	, m_motor_count(motor_count)
{
	if (m_motor_count < 3)
	{
		PANIC();
	}
	//ticopters are always X
	if (m_motor_count == 3)
	{
		m_type = Type::X;
	}
}

