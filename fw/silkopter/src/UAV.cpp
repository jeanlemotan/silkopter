#include "UAV.h"
#include "Motor_Mixer.h"
#include "debug/debug.h"

using namespace silk;

UAV::UAV(Motor_Mixer::Type type, uint8_t motor_count, float radius)
	: m_radius(radius)
	, m_motor_mixer(type, motor_count)
	, m_control_frame_reference(Control_Reference_Frame::UAV)
	, m_control_mode(Control_Mode::SILKY)
{
	if (radius < 0.01f)
	{
		PANIC();
	}
}

void UAV::set_assists(Assists const& assists)
{
	m_assists = assists;
}
void UAV::set_failsafes(Failsafes const& failsafes)
{
	m_failsafes = failsafes;
}

void UAV::set_control_reference_frame(Control_Reference_Frame frame)
{
	m_control_frame_reference = frame;
}
void UAV::set_control_mode(Control_Mode mode)
{
	m_control_mode = mode;
}

const UAV::Status& UAV::get_status()
{
	return m_status;
}

void UAV::process()
{
}

	
