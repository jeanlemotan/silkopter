#include "GS/SFull_Protocol.h"
#include "debug/debug.h"

using namespace silk;

SFull_Protocol::SFull_Protocol()
	: m_last_frame_idx(0)
	, m_is_frame_started(false)
{
		
}

SFull_Protocol::SFull_Protocol(board::UART& uart)
	: m_uart(&uart)
	, m_last_frame_idx(0)
	, m_is_frame_started(false)
{
		
}

//////////////////////////////////////////////////////////////////////////
//BOARD

static const uint16_t MSG_BOARD_GYROSCOPE = 1;
static const uint16_t MSG_BOARD_ACCELEROMETER = 2;
static const uint16_t MSG_BOARD_TEMPERATURE = 3;
static const uint16_t MSG_BOARD_BARO_PRESSURE = 4;
static const uint16_t MSG_BOARD_SONAR_ALTITUDE = 5;
static const uint16_t MSG_BOARD_GPS_ALTITUDE = 6;

//////////////////////////////////////////////////////////////////////////
//UAV

static const uint16_t MSG_UAV_ACCELERATION = 7;
static const uint16_t MSG_UAV_SPEED = 8;
static const uint16_t MSG_UAV_POSITION = 9;
static const uint16_t MSG_UAV_ATTITUDE = 10;
static const uint16_t MSG_UAV_PHASE = 11;
static const uint16_t MSG_UAV_CONTROL_MODE = 12;
static const uint16_t MSG_UAV_CONTROL_REFERENCE_FRAME = 13;
	
//////////////////////////////////////////////////////////////////////////

static const uint8_t k_version = 1;
	
void SFull_Protocol::hello_world(Message_String const& msg, uint16_t version)
{
	if (!m_uart) return;
	ASSERT(!m_is_frame_started);
	if (m_is_frame_started) return;

	uint8_t header[] = { 0x89, 0x0D, 0x0A, 0x1A, 0x0A, 'H', 'E', 'L', 'L', 'O', k_version, 0x89 };
	m_uart->write(header, sizeof(header));
	m_uart->write(msg.size());
	m_uart->write(reinterpret_cast<uint8_t const*>(msg.c_str()), msg.size());
	m_uart->write(version);
	m_uart->write(header, sizeof(header));
}
	
void SFull_Protocol::start_frame()
{
	if (!m_uart) return;
	ASSERT(!m_is_frame_started);
	if (m_is_frame_started) return;
	
	m_last_frame_idx++;
	uint8_t vl = *reinterpret_cast<uint8_t*>(&m_last_frame_idx);
	uint8_t vh = *(reinterpret_cast<uint8_t*>(&m_last_frame_idx) + 1);
	uint8_t buf[] = { 0x0D, vl, vh, 0x89 };
	m_uart->write(buf, sizeof(buf));
	m_is_frame_started = true;
}
void SFull_Protocol::end_frame()
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
	
	m_last_frame_idx++;
	uint8_t vl = *reinterpret_cast<uint8_t*>(&m_last_frame_idx);
	uint8_t vh = *(reinterpret_cast<uint8_t*>(&m_last_frame_idx) + 1);
	uint8_t buf[] = { 0x89, vl, vh, 0x0C };
	m_uart->write(buf, sizeof(buf));
	m_is_frame_started = false;
}
	
void SFull_Protocol::send_board_gyroscope(bool is_valid, math::vec3f const& gyro)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
	
	m_uart->write(MSG_BOARD_GYROSCOPE);
	m_uart->write(is_valid);
	if (is_valid)
	{
		m_uart->write(gyro);
	}
}
void SFull_Protocol::send_board_accelerometer(bool is_valid, math::vec3f const& accel)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_uart->write(MSG_BOARD_ACCELEROMETER);
	m_uart->write(is_valid);
	if (is_valid)
	{
		m_uart->write(accel);
	}
}
void SFull_Protocol::send_board_temperature(bool is_valid, float temp)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_uart->write(MSG_BOARD_TEMPERATURE);
	m_uart->write(is_valid);
	if (is_valid)
	{
		m_uart->write(temp);
	}
}

void SFull_Protocol::send_board_baro_pressure(bool is_valid, float pressure)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_uart->write(MSG_BOARD_BARO_PRESSURE);
	m_uart->write(is_valid);
	if (is_valid)
	{
		m_uart->write(pressure);
	}
}
void SFull_Protocol::send_board_sonar_altitude(bool is_valid, float altitude)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_uart->write(MSG_BOARD_SONAR_ALTITUDE);
	m_uart->write(is_valid);
	if (is_valid)
	{
		m_uart->write(altitude);
	}
}
void SFull_Protocol::send_board_gps_altitude(bool is_valid, float altitude)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_uart->write(MSG_BOARD_GPS_ALTITUDE);
	m_uart->write(is_valid);
	if (is_valid)
	{
		m_uart->write(altitude);
	}
}

void SFull_Protocol::send_uav_acceleration(math::vec3f const& accel)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_uart->write(MSG_UAV_ACCELERATION);
	m_uart->write(accel);
}
void SFull_Protocol::send_uav_speed(math::vec3f const& speed)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_uart->write(MSG_UAV_SPEED);
	m_uart->write(speed);
}
void SFull_Protocol::send_uav_position(math::vec3f const& position)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_uart->write(MSG_UAV_POSITION);
	m_uart->write(position);
}

void SFull_Protocol::send_uav_attitude(math::vec3f const& euler)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_uart->write(MSG_UAV_ATTITUDE);
	m_uart->write(euler);
}

void SFull_Protocol::send_uav_phase(UAV::Phase phase)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_uart->write(MSG_UAV_PHASE);
	m_uart->write(phase);
}
void SFull_Protocol::send_uav_control_mode(UAV::Control_Mode mode)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_uart->write(MSG_UAV_CONTROL_MODE);
	m_uart->write(mode);
}
void SFull_Protocol::send_uav_control_reference_frame(UAV::Control_Reference_Frame frame)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_uart->write(MSG_UAV_CONTROL_REFERENCE_FRAME);
	m_uart->write(frame);
}

