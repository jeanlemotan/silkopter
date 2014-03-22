#include "GS/SCompact_Protocol.h"
#include "debug/debug.h"

using namespace silk;

SCompact_Protocol::SCompact_Protocol()
: m_last_frame_idx(0)
, m_is_frame_started(false)
{
	
}

SCompact_Protocol::SCompact_Protocol(board::UART& uart)
: m_uart(&uart)
, m_last_frame_idx(0)
, m_is_frame_started(false)
{
	
}

//////////////////////////////////////////////////////////////////////////

static const uint8_t k_version = 1;

void SCompact_Protocol::hello_world(Message_String const& msg, uint16_t version)
{
	if (!m_uart) return;
	ASSERT(!m_is_frame_started);
	if (m_is_frame_started) return;
}

void SCompact_Protocol::start_frame()
{
	if (!m_uart) return;
	ASSERT(!m_is_frame_started);
	if (m_is_frame_started) return;
	
	m_last_frame_idx++;
	m_is_frame_started = true;
}
void SCompact_Protocol::end_frame()
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
	
	m_last_frame_idx++;
	m_is_frame_started = false;
}

void SCompact_Protocol::send_board_gyroscope(bool is_valid, math::vec3f const& gyro)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
}
void SCompact_Protocol::send_board_accelerometer(bool is_valid, math::vec3f const& accel)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
}
void SCompact_Protocol::send_board_temperature(bool is_valid, float temp)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
}

void SCompact_Protocol::send_board_baro_pressure(bool is_valid, float pressure)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
}
void SCompact_Protocol::send_board_sonar_altitude(bool is_valid, float altitude)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
}
void SCompact_Protocol::send_board_gps_altitude(bool is_valid, float altitude)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
}

void SCompact_Protocol::send_uav_acceleration(math::vec3f const& accel)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
}
void SCompact_Protocol::send_uav_speed(math::vec3f const& speed)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
}
void SCompact_Protocol::send_uav_position(math::vec3f const& position)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
}

void SCompact_Protocol::send_uav_attitude(math::vec3f const& euler)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
}

void SCompact_Protocol::send_uav_phase(UAV::Phase phase)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
}
void SCompact_Protocol::send_uav_control_mode(UAV::Control_Mode mode)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
}
void SCompact_Protocol::send_uav_control_reference_frame(UAV::Control_Reference_Frame frame)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
}

