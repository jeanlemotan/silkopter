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

static const uint8_t MSG_START_FRAME = 254;
static const uint8_t MSG_END_FRAME = 255;

//////////////////////////////////////////////////////////////////////////
//BOARD

static const uint8_t MSG_BOARD_CPU_USAGE = 0;
static const uint8_t MSG_BOARD_GYROSCOPE = 1;
static const uint8_t MSG_BOARD_ACCELEROMETER = 2;
static const uint8_t MSG_BOARD_TEMPERATURE = 3;
static const uint8_t MSG_BOARD_BARO_PRESSURE = 4;
static const uint8_t MSG_BOARD_SONAR_ALTITUDE = 5;
static const uint8_t MSG_BOARD_GPS_ALTITUDE = 6;
static const uint8_t MSG_BOARD_RC_IN = 7;
static const uint8_t MSG_BOARD_PWM_OUT = 8;

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
	m_buffer.clear();
	m_buffer.write(MSG_START_FRAME);	//header: +1
	m_buffer.write(uint8_t(4 + sizeof(m_last_frame_idx)));		//size: +1
	m_buffer.write(m_last_frame_idx);//version: +2
	
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());//crc: +2
	m_is_frame_started = true;
}
void SFull_Protocol::end_frame()
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
	
	m_buffer.clear();
	m_buffer.write(MSG_END_FRAME);	//header: +1
	m_buffer.write(uint8_t(4 + sizeof(m_last_frame_idx)));		//size: +1
	m_buffer.write(m_last_frame_idx);//version: +2
	
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());//crc: +2
	m_is_frame_started = false;
}

//////////////////////////////////////////////////////////////////////////
	
void SFull_Protocol::send_board_cpu_usage(uint8_t cpu_usage_percent)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
	
	m_buffer.clear();
	m_buffer.write(MSG_BOARD_CPU_USAGE);
	m_buffer.write(uint8_t(4 + sizeof(cpu_usage_percent)));
	m_buffer.write(cpu_usage_percent);
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());//crc: +2
}
	
void SFull_Protocol::send_board_gyroscope(bool is_valid, math::vec3f const& gyro)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();	
	m_buffer.write(MSG_BOARD_GYROSCOPE);
	m_buffer.write(uint8_t(4 + sizeof(is_valid) + is_valid ? sizeof(gyro) : 0));
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(gyro);
	}
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}
void SFull_Protocol::send_board_accelerometer(bool is_valid, math::vec3f const& accel)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_BOARD_ACCELEROMETER);
	m_buffer.write(uint8_t(4 + sizeof(is_valid) + is_valid ? sizeof(accel) : 0));
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(accel);
	}
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}
void SFull_Protocol::send_board_temperature(bool is_valid, float temp)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_BOARD_TEMPERATURE);
	m_buffer.write(uint8_t(4 + sizeof(is_valid) + is_valid ? sizeof(temp) : 0));
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(temp);
	}
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}
void SFull_Protocol::send_board_baro_pressure(bool is_valid, float pressure)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_BOARD_BARO_PRESSURE);
	m_buffer.write(uint8_t(4 + sizeof(is_valid) + is_valid ? sizeof(pressure) : 0));
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(pressure);
	}
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}
void SFull_Protocol::send_board_sonar_altitude(bool is_valid, float altitude)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_BOARD_SONAR_ALTITUDE);
	m_buffer.write(uint8_t(4 + sizeof(is_valid) + is_valid ? sizeof(altitude) : 0));
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(altitude);
	}
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}
void SFull_Protocol::send_board_gps_altitude(bool is_valid, float altitude)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_BOARD_GPS_ALTITUDE);
	m_buffer.write(uint8_t(4 + sizeof(is_valid) + is_valid ? sizeof(altitude) : 0));
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(altitude);
	}
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}
void SFull_Protocol::send_board_rc_in(uint8_t count, int16_t const* values)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_BOARD_RC_IN);
	m_buffer.write(uint8_t(4 + sizeof(count) + count));
	m_buffer.write(count);
	if (count)
	{
		m_buffer.write(reinterpret_cast<uint8_t const*>(values), count*sizeof(int16_t));
	}
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}
void SFull_Protocol::send_board_pwm_out(uint8_t count, int16_t const* values)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_BOARD_PWM_OUT);
	m_buffer.write(uint8_t(4 + sizeof(count) + count));
	m_buffer.write(count);
	if (count)
	{
		m_buffer.write(reinterpret_cast<uint8_t const*>(values), count*sizeof(int16_t));
	}
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}

//////////////////////////////////////////////////////////////////////////

void SFull_Protocol::send_uav_acceleration(math::vec3f const& accel)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_UAV_ACCELERATION);
	m_buffer.write(uint8_t(4 + sizeof(accel)));
	m_buffer.write(accel);
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}
void SFull_Protocol::send_uav_speed(math::vec3f const& speed)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_UAV_SPEED);
	m_buffer.write(uint8_t(4 + sizeof(speed)));
	m_buffer.write(speed);
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}
void SFull_Protocol::send_uav_position(math::vec3f const& position)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_UAV_POSITION);
	m_buffer.write(uint8_t(4 + sizeof(position)));
	m_buffer.write(position);
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}

void SFull_Protocol::send_uav_attitude(math::vec3f const& euler)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_UAV_ATTITUDE);
	m_buffer.write(uint8_t(4 + sizeof(euler)));
	m_buffer.write(euler);
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}

void SFull_Protocol::send_uav_phase(UAV::Phase phase)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_UAV_PHASE);
	m_buffer.write(uint8_t(4 + sizeof(phase)));
	m_buffer.write(phase);
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}
void SFull_Protocol::send_uav_control_mode(UAV::Control_Mode mode)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_UAV_CONTROL_MODE);
	m_buffer.write(uint8_t(4 + sizeof(mode)));
	m_buffer.write(mode);
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}
void SFull_Protocol::send_uav_control_reference_frame(UAV::Control_Reference_Frame frame)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	m_buffer.clear();
	m_buffer.write(MSG_UAV_CONTROL_REFERENCE_FRAME);
	m_buffer.write(uint8_t(4 + sizeof(frame)));
	m_buffer.write(frame);
	m_uart->write(m_buffer.get_data_ptr(), m_buffer.get_size());
	m_uart->write(m_buffer.get_crc());
}

