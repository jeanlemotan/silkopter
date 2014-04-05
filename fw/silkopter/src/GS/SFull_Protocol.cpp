#include "GS/SFull_Protocol.h"
#include "debug/debug.h"

using namespace silk;

SFull_Protocol::SFull_Protocol()
	: m_last_frame_idx(0)
	, m_is_frame_started(false)
	, m_is_connected(false)
{
	m_enabled_messages.set();
}

SFull_Protocol::SFull_Protocol(board::UART& uart)
	: m_uart(&uart)
	, m_last_frame_idx(0)
	, m_is_frame_started(false)
	, m_is_connected(false)
{
	m_enabled_messages.set();
	//m_enabled_messages.reset();
		
 	m_enabled_messages.set(static_cast<int>(Message::HELLO_WORLD), 1);
 	m_enabled_messages.set(static_cast<int>(Message::START_FRAME), 1);
 	m_enabled_messages.set(static_cast<int>(Message::END_FRAME), 1);
	m_enabled_messages.set(static_cast<int>(Message::BOARD_TIME_MS), 1);
	 
	 
// 	m_enabled_messages.set(static_cast<int>(Message::BOARD_GYROSCOPE), 1);
// 	m_enabled_messages.set(static_cast<int>(Message::BOARD_ACCELEROMETER), 1);
// 	m_enabled_messages.set(static_cast<int>(Message::UAV_ATTITUDE), 1);
}

//////////////////////////////////////////////////////////////////////////

bool SFull_Protocol::is_message_enabled(Message msg) const
{
	return m_enabled_messages.test(static_cast<uint8_t>(msg));
}

//////////////////////////////////////////////////////////////////////////

static const uint8_t k_version = 1;

//message structure:
//offset : size : description
//0	: 1 : msg
//1 : 1 : size
//2 : 2 : crc of everything excluding these 2 bytes (they are zeroed before)
//>4 : x : data
bool SFull_Protocol::start_message(Message msg)
{
	if (!m_enabled_messages.test(static_cast<uint8_t>(msg)))
	{
		return false;
	}
	m_buffer.clear();
	m_buffer.write(static_cast<uint8_t>(msg));
	m_buffer.write(uint8_t(0)); //size
	m_buffer.write(uint16_t(0)); //crc
	return true;
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
	
void SFull_Protocol::send_hello_world(Message_String const& msg, uint16_t version)
{
	if (!m_uart) return;
	ASSERT(!m_is_frame_started);
	if (m_is_frame_started) return;

	m_is_connected = false;

	start_message(Message::HELLO_WORLD);
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
	
void SFull_Protocol::send_start_frame()
{
	if (!m_uart) return;
	ASSERT(!m_is_frame_started);
	if (m_is_frame_started) return;
	if (!m_is_connected) return;
	
	m_last_frame_idx++;
	start_message(Message::START_FRAME);
	m_buffer.write(m_last_frame_idx);
	flush_message();
	m_is_frame_started = true;
}
void SFull_Protocol::send_end_frame()
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
	if (!m_is_connected) return;
	
	start_message(Message::END_FRAME);
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
	
	if (!start_message(Message::BOARD_CPU_USAGE))
	{
		return;
	}
	m_buffer.write(cpu_usage_percent);
	flush_message();
}

void SFull_Protocol::send_board_time_ms(chrono::time_ms time)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;
	
	if (!start_message(Message::BOARD_TIME_MS))
	{
		return;
	}
	m_buffer.write(uint32_t(time.time_since_epoch().count));
	flush_message();
}
	
void SFull_Protocol::send_board_gyroscope(bool is_valid, math::vec3f const& gyro)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	if (!start_message(Message::BOARD_GYROSCOPE))
	{
		return;
	}
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

	if (!start_message(Message::BOARD_ACCELEROMETER))
	{
		return;
	}
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

	if (!start_message(Message::BOARD_TEMPERATURE))
	{
		return;
	}

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

	if (!start_message(Message::BOARD_BARO_PRESSURE))
	{
		return;
	}
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(pressure);
	}
	flush_message();
}
void SFull_Protocol::send_board_sonar_distance(bool is_valid, float distance)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	if (!start_message(Message::BOARD_SONAR_DISTANCE))
	{
		return;
	}
	m_buffer.write(is_valid);
	if (is_valid)
	{
		m_buffer.write(distance);
	}
	flush_message();
}
void SFull_Protocol::send_board_gps_altitude(bool is_valid, float altitude)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	if (!start_message(Message::BOARD_GPS_ALTITUDE))
	{
		return;
	}
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

	if (!start_message(Message::BOARD_RC_IN))
	{
		return;
	}
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

	if (!start_message(Message::BOARD_PWM_OUT))
	{
		return;
	}
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

	if (!start_message(Message::UAV_ACCELERATION))
	{
		return;
	}
	m_buffer.write(accel);
	flush_message();
}
void SFull_Protocol::send_uav_velocity(math::vec3f const& velocity)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	if (!start_message(Message::UAV_VELOCITY))
	{
		return;
	}
	m_buffer.write(velocity);
	flush_message();
}
void SFull_Protocol::send_uav_position(math::vec3f const& position)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	if (!start_message(Message::UAV_POSITION))
	{
		return;
	}
	m_buffer.write(position);
	flush_message();
}

void SFull_Protocol::send_uav_attitude(math::vec3f const& euler)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	if (!start_message(Message::UAV_ATTITUDE))
	{
		return;
	}
	m_buffer.write(euler);
	flush_message();
}

void SFull_Protocol::send_uav_phase(UAV::Phase phase)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	if (!start_message(Message::UAV_PHASE))
	{
		return;
	}
	m_buffer.write(phase);
	flush_message();
}
void SFull_Protocol::send_uav_control_mode(UAV::Control_Mode mode)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	if (!start_message(Message::UAV_CONTROL_MODE))
	{
		return;
	}
	m_buffer.write(mode);
	flush_message();
}
void SFull_Protocol::send_uav_control_reference_frame(UAV::Control_Reference_Frame frame)
{
	if (!m_uart) return;
	ASSERT(m_is_frame_started);
	if (!m_is_frame_started) return;

	if (!start_message(Message::UAV_CONTROL_REFERENCE_FRAME))
	{
		return;
	}
	m_buffer.write(frame);
	flush_message();
}

