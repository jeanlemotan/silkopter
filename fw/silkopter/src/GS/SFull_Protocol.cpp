#include "GS/SFull_Protocol.h"
#include "debug/debug.h"

using namespace silk;

SFull_Protocol::SFull_Protocol()
	: m_last_frame_idx(0)
	, m_is_frame_started(false)
	, m_is_connected(false)
{
		
}

SFull_Protocol::SFull_Protocol(board::UART& uart)
	: m_uart(&uart)
	, m_last_frame_idx(0)
	, m_is_frame_started(false)
	, m_is_connected(false)
{
		
}

//////////////////////////////////////////////////////////////////////////

static const uint8_t MSG_HELLO_WORLD = 253;
static const uint8_t MSG_START_FRAME = 254;
static const uint8_t MSG_END_FRAME = 255;

//////////////////////////////////////////////////////////////////////////
//BOARD

static const uint8_t MSG_BOARD_CPU_USAGE = 0;
static const uint8_t MSG_BOARD_TIME_MS = 1;
static const uint8_t MSG_BOARD_GYROSCOPE = 10;
static const uint8_t MSG_BOARD_ACCELEROMETER = 11;
static const uint8_t MSG_BOARD_TEMPERATURE = 12;
static const uint8_t MSG_BOARD_BARO_PRESSURE = 13;
static const uint8_t MSG_BOARD_SONAR_ALTITUDE = 14;
static const uint8_t MSG_BOARD_GPS_ALTITUDE = 15;
static const uint8_t MSG_BOARD_RC_IN = 16;
static const uint8_t MSG_BOARD_PWM_OUT = 17;

//////////////////////////////////////////////////////////////////////////
//UAV

static const uint8_t MSG_UAV_ACCELERATION = 30;
static const uint8_t MSG_UAV_VELOCITY = 31;
static const uint8_t MSG_UAV_POSITION = 32;
static const uint8_t MSG_UAV_ATTITUDE = 33;
static const uint8_t MSG_UAV_PHASE = 34;
static const uint8_t MSG_UAV_CONTROL_MODE = 35;
static const uint8_t MSG_UAV_CONTROL_REFERENCE_FRAME = 36;
	
//////////////////////////////////////////////////////////////////////////

static const uint8_t k_version = 1;

//message structure:
//offset : size : description
//0	: 1 : msg
//1 : 1 : size
//2 : 2 : crc of everything excluding these 2 bytes (they are zeroed before)
//>4 : x : data
void SFull_Protocol::start_message(uint8_t msg)
{
	m_buffer.clear();
	m_buffer.write(msg);
	m_buffer.write(uint8_t(0)); //size
	m_buffer.write(uint16_t(0)); //crc
}
void SFull_Protocol::flush_message()
{
	m_buffer.write_at(1, uint8_t(m_buffer.get_size()));
	m_buffer.write_at(2, uint16_t(m_buffer.compute_crc()));
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
}

//////////////////////////////////////////////////////////////////////////

bool SFull_Protocol::is_connected() const
{
	return m_is_connected;
}
	
void SFull_Protocol::hello_world(Message_String const& msg, uint16_t version)
{
	if (!m_uart) return;
	ASSERT(!m_is_frame_started);
	if (m_is_frame_started) return;

	m_is_connected = false;

	start_message(MSG_HELLO_WORLD);
	m_buffer.write(k_version);
	m_buffer.write(version);
	m_buffer.write(uint8_t(msg.size()));
	m_buffer.write(reinterpret_cast<uint8_t const*>(msg.c_str()), msg.size());
	flush_message();
	
	uint8_t const expected_response[] = { 'H', 'O', 'L', 'A' };
	if (m_uart->get_data_size() >= sizeof(expected_response))
	{
		uint8_t response[4];
		if (m_uart->read(response, sizeof(response)))
		{
			m_is_connected = !memcmp(response, expected_response, sizeof(response));
		}
	}
}
	
void SFull_Protocol::start_frame()
{
	if (!m_uart) return;
	ASSERT(!m_is_frame_started);
	if (m_is_frame_started) return;
	if (!m_is_connected) return;
	
	m_last_frame_idx++;
	start_message(MSG_START_FRAME);
	m_buffer.write(m_last_frame_idx);
	flush_message();
	m_is_frame_started = true;
}
void SFull_Protocol::end_frame()
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
	if (!m_is_connected) return;
	
	start_message(MSG_END_FRAME);
	m_buffer.write(m_last_frame_idx);//version: +4
	flush_message();
	m_is_frame_started = false;
}

//////////////////////////////////////////////////////////////////////////
	
void SFull_Protocol::send_board_cpu_usage(uint8_t cpu_usage_percent)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
	
	start_message(MSG_BOARD_CPU_USAGE);
	m_buffer.write(cpu_usage_percent);
	flush_message();
}

void SFull_Protocol::send_board_time_ms(chrono::time_ms time)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
	
	start_message(MSG_BOARD_TIME_MS);
	m_buffer.write(uint32_t(time.time_since_epoch().count));
	flush_message();
}
	
void SFull_Protocol::send_board_gyroscope(bool is_valid, math::vec3f const& gyro)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_BOARD_GYROSCOPE);
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(gyro);
	}
	flush_message();
}
void SFull_Protocol::send_board_accelerometer(bool is_valid, math::vec3f const& accel)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_BOARD_ACCELEROMETER);
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(accel);
	}
	flush_message();
}
void SFull_Protocol::send_board_temperature(bool is_valid, float temp)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_BOARD_TEMPERATURE);
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(temp);
	}
	flush_message();
}
void SFull_Protocol::send_board_baro_pressure(bool is_valid, float pressure)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_BOARD_BARO_PRESSURE);
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(pressure);
	}
	flush_message();
}
void SFull_Protocol::send_board_sonar_altitude(bool is_valid, float altitude)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_BOARD_SONAR_ALTITUDE);
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(altitude);
	}
	flush_message();
}
void SFull_Protocol::send_board_gps_altitude(bool is_valid, float altitude)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_BOARD_GPS_ALTITUDE);
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(altitude);
	}
	flush_message();
}
void SFull_Protocol::send_board_rc_in(uint8_t count, int16_t const* values)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_BOARD_RC_IN);
	m_buffer.write(count);
	if (count)
	{
		m_buffer.write(reinterpret_cast<uint8_t const*>(values), count*sizeof(int16_t));
	}
	flush_message();
}
void SFull_Protocol::send_board_pwm_out(uint8_t count, int16_t const* values)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_BOARD_PWM_OUT);
	m_buffer.write(count);
	if (count)
	{
		m_buffer.write(reinterpret_cast<uint8_t const*>(values), count*sizeof(int16_t));
	}
	flush_message();
}

//////////////////////////////////////////////////////////////////////////

void SFull_Protocol::send_uav_acceleration(math::vec3f const& accel)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_UAV_ACCELERATION);
	m_buffer.write(accel);
	flush_message();
}
void SFull_Protocol::send_uav_velocity(math::vec3f const& velocity)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_UAV_VELOCITY);
	m_buffer.write(velocity);
	flush_message();
}
void SFull_Protocol::send_uav_position(math::vec3f const& position)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_UAV_POSITION);
	m_buffer.write(position);
	flush_message();
}

void SFull_Protocol::send_uav_attitude(math::vec3f const& euler)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_UAV_ATTITUDE);
	m_buffer.write(euler);
	flush_message();
}

void SFull_Protocol::send_uav_phase(UAV::Phase phase)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_UAV_PHASE);
	m_buffer.write(phase);
	flush_message();
}
void SFull_Protocol::send_uav_control_mode(UAV::Control_Mode mode)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_UAV_CONTROL_MODE);
	m_buffer.write(mode);
	flush_message();
}
void SFull_Protocol::send_uav_control_reference_frame(UAV::Control_Reference_Frame frame)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	start_message(MSG_UAV_CONTROL_REFERENCE_FRAME);
	m_buffer.write(frame);
	flush_message();
}

