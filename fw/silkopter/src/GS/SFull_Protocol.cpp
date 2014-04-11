#include "GS/SFull_Protocol.h"
#include "debug/debug.h"

using namespace silk;

SFull_Protocol::SFull_Protocol()
	: m_is_connected(false)
{
}

SFull_Protocol::SFull_Protocol(board::UART& uart)
	: m_uart(&uart)
	, m_is_connected(false)
{
}

//////////////////////////////////////////////////////////////////////////

static const uint8_t k_version = 1;

//message structure:
//offset : size : description
//0	: 1 : msg
//1 : 1 : size
//2 : 2 : crc of everything excluding these 2 bytes (they are zeroed before)
//>4 : x : data
void SFull_Protocol::start_tx_message(TX_Message msg)
{
	m_tx_buffer.clear();
	m_tx_buffer.append(static_cast<uint8_t>(msg));
	m_tx_buffer.append(uint8_t(0)); //size
	m_tx_buffer.append(uint16_t(0)); //crc
}
void SFull_Protocol::flush_tx_message()
{
	m_tx_buffer[1] = uint8_t(m_tx_buffer.size());
	auto crc = util::compute_crc(m_tx_buffer.data(), m_tx_buffer.size());
	m_tx_buffer.write_at(2, crc);
	m_uart->write(m_tx_buffer.data(), m_tx_buffer.size());
}

//////////////////////////////////////////////////////////////////////////

bool SFull_Protocol::is_connected() const
{
	return m_is_connected;
}
	
void SFull_Protocol::tx_hello_world(Message_String const& msg, uint16_t version)
{
	m_is_connected = false;

	start_tx_message(TX_Message::HELLO_WORLD);
	m_tx_buffer.append(k_version);
	m_tx_buffer.append(version);
	m_tx_buffer.append(uint8_t(msg.size()));
	m_tx_buffer.append(reinterpret_cast<uint8_t const*>(msg.c_str()), msg.size());
	flush_tx_message();
	
	uint8_t const expected_response[] = { 'H', 'O', 'L', 'A' };
	if (m_uart->get_data_size() >= sizeof(expected_response))
	{
		uint8_t response[4];
		if (m_uart->read(response, sizeof(response)))
		{
			m_is_connected = !memcmp(response, expected_response, sizeof(response));
			if (m_is_connected)
			{
				tx_printf(F_STR("Connected."));
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void SFull_Protocol::tx_print(char const* str, size_t size)
{
	start_tx_message(TX_Message::PRINT);
	m_tx_buffer.append(reinterpret_cast<uint8_t const*>(str), size);
	flush_tx_message();
}

void SFull_Protocol::tx_acknowledge(util::crc_t crc)
{
	start_tx_message(TX_Message::ACKNOWLEDGE);
	m_tx_buffer.append(crc);
	flush_tx_message();
}
//////////////////////////////////////////////////////////////////////////
	
void SFull_Protocol::tx_board_cpu_usage(uint8_t cpu_usage_percent)
{
	start_tx_message(TX_Message::BOARD_CPU_USAGE);
	m_tx_buffer.append(cpu_usage_percent);
	flush_tx_message();
}

void SFull_Protocol::tx_board_time(chrono::time_us time)
{
	start_tx_message(TX_Message::BOARD_TIME);
	m_tx_buffer.append(uint32_t(time.time_since_epoch().count));
	flush_tx_message();
}
	
void SFull_Protocol::tx_board_gyroscope(bool is_valid, math::vec3f const& gyro)
{
	start_tx_message(TX_Message::BOARD_GYROSCOPE);
	m_tx_buffer.append(is_valid);
	if (is_valid)
	{
		m_tx_buffer.append(gyro);
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_accelerometer(bool is_valid, math::vec3f const& accel)
{
	start_tx_message(TX_Message::BOARD_ACCELEROMETER);
	m_tx_buffer.append(is_valid);
	if (is_valid)
	{
		m_tx_buffer.append(accel);
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_temperature(bool is_valid, float temp)
{
	start_tx_message(TX_Message::BOARD_TEMPERATURE);
	m_tx_buffer.append(is_valid);
	if (is_valid)
	{
		m_tx_buffer.append(temp);
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_baro_pressure(bool is_valid, float pressure)
{
	start_tx_message(TX_Message::BOARD_BARO_PRESSURE);
	m_tx_buffer.append(is_valid);
	if (is_valid)
	{
		m_tx_buffer.append(pressure);
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_sonar_distance(bool is_valid, float distance)
{
	start_tx_message(TX_Message::BOARD_SONAR_DISTANCE);
	m_tx_buffer.append(is_valid);
	if (is_valid)
	{
		m_tx_buffer.append(distance);
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_gps_altitude(bool is_valid, float altitude)
{
	start_tx_message(TX_Message::BOARD_GPS_ALTITUDE);
	m_tx_buffer.append(is_valid);
	if (is_valid)
	{
		m_tx_buffer.append(altitude);
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_rc_in(uint8_t count, int16_t const* values)
{
	start_tx_message(TX_Message::BOARD_RC_IN);
	m_tx_buffer.append(count);
	if (count)
	{
		m_tx_buffer.append(reinterpret_cast<uint8_t const*>(values), count*sizeof(int16_t));
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_pwm_out(uint8_t count, int16_t const* values)
{
	start_tx_message(TX_Message::BOARD_PWM_OUT);
	m_tx_buffer.append(count);
	if (count)
	{
		m_tx_buffer.append(reinterpret_cast<uint8_t const*>(values), count*sizeof(int16_t));
	}
	flush_tx_message();
}

//////////////////////////////////////////////////////////////////////////

void SFull_Protocol::tx_uav_acceleration(math::vec3f const& accel)
{
	start_tx_message(TX_Message::UAV_ACCELERATION);
	m_tx_buffer.append(accel);
	flush_tx_message();
}
void SFull_Protocol::tx_uav_velocity(math::vec3f const& velocity)
{
	start_tx_message(TX_Message::UAV_VELOCITY);
	m_tx_buffer.append(velocity);
	flush_tx_message();
}
void SFull_Protocol::tx_uav_position(math::vec3f const& position)
{
	start_tx_message(TX_Message::UAV_POSITION);
	m_tx_buffer.append(position);
	flush_tx_message();
}
void SFull_Protocol::tx_uav_attitude(math::vec3f const& euler)
{
	start_tx_message(TX_Message::UAV_ATTITUDE);
	m_tx_buffer.append(euler);
	flush_tx_message();
}

//////////////////////////////////////////////////////////////////////////

void SFull_Protocol::tx_uav_phase(UAV::Phase phase)
{
	start_tx_message(TX_Message::UAV_PHASE);
	m_tx_buffer.append(phase);
	flush_tx_message();
}
void SFull_Protocol::tx_uav_control_mode(UAV::Control_Mode mode)
{
	start_tx_message(TX_Message::UAV_CONTROL_MODE);
	m_tx_buffer.append(mode);
	flush_tx_message();
}
void SFull_Protocol::tx_uav_control_reference_frame(UAV::Control_Reference_Frame frame)
{
	start_tx_message(TX_Message::UAV_CONTROL_REFERENCE_FRAME);
	m_tx_buffer.append(frame);
	flush_tx_message();
}

//////////////////////////////////////////////////////////////////////////

template<class Container, class T>
static T get_value(Container const& t, size_t& off)
{
	T val;
	uint8_t* ptr = reinterpret_cast<uint8_t*>(&val);
	for (size_t i = 0; i < sizeof(T); i++)
	{
		*ptr++ = t[off++];
	}
	return val;
}
template<class Container, class T>
static void set_value(Container const& t, T const& val, size_t off)
{
	ASSERT(off + sizeof(T) <= t.size());
	uint8_t const* ptr = reinterpret_cast<uint8_t const*>(&val);
	for (size_t i = 0; i < sizeof(T); i++)
	{
		t[off++] = *ptr++;
	}
}

bool SFull_Protocol::decode_rx_header(bool& needs_more_data)
{
	needs_more_data = false;

	if (m_rx_buffer.size() < sizeof(Header))
	{
		if (board::clock::now_ms() - m_last_rx_time < chrono::millis(500))
		{
			needs_more_data = true;
			return false;
		}
		else
		{
			m_rx_buffer.pop_front(); //msg
			return false;
		}
	}

	size_t off = 0;
	auto msg = static_cast<RX_Message>(get_value<RX_Buffer, uint8_t>(m_rx_buffer, off));
	auto size = get_value<RX_Buffer, uint8_t>(m_rx_buffer, off);
	auto crc = get_value<RX_Buffer, uint16_t>(m_rx_buffer, off);
	if (size <= sizeof(Header))
	{
		m_rx_buffer.pop_front(); //msg
		return false;
	}
	if (m_rx_buffer.size() < size)
	{
		if (board::clock::now_ms() - m_last_rx_time < chrono::millis(500))
		{
			needs_more_data = true;
			return false;
		}
		else
		{
			m_rx_buffer.pop_front(); //msg
			return false;
		}
	}
	//clear crc bytes and compute crc
	auto crc2 = m_rx_buffer[2];
	auto crc3 = m_rx_buffer[3];
	m_rx_buffer[2] = 0;
	m_rx_buffer[3] = 0;

	auto computed_crc = compute_crc(m_rx_buffer, size);
	if (crc != computed_crc)
	{
		//put back the crcs (as they were not actually crcs apparently)
		m_rx_buffer[2] = crc2;
		m_rx_buffer[3] = crc3;
		m_rx_buffer.pop_front(); //msg
		return false;
	}

	m_rx_buffer.pop_front(sizeof(Header)); //whole header
	m_rx_header.msg = msg;
	m_rx_header.size = size - sizeof(Header);
	m_rx_header.crc = crc;
	return true;
}

SProtocol::RX_Message SFull_Protocol::get_next_rx_message()
{
	auto available = m_uart->get_data_size();
	if (available > 0)
	{
		//tx_printf("received {} bytes / {} ({})", available, m_rx_buffer.size(), m_uart->get_rx_data_counter());
		m_last_rx_time = board::clock::now_ms();
		for (size_t i = 0; i < available; i++)
		{
			uint8_t ch;
			m_uart->read(ch);
			m_rx_buffer.push_back(ch);
		}
	}
	
	if (!m_rx_buffer.empty())
	{
		bool needs_more_data;
		if (decode_rx_header(needs_more_data))
		{
			return m_rx_header.msg;
		}
	}
	return RX_Message::NONE;
}

void SFull_Protocol::discard_rx_message()
{
	m_rx_buffer.pop_front(m_rx_header.size);
}
void SFull_Protocol::decode_board_accelerometer_bias_scale(math::vec3f& bias, math::vec3f& scale)
{
	ASSERT(m_rx_header.msg == RX_Message::SET_BOARD_ACCELEROMETER_BIAS_SCALE);
	size_t off = 0;
	bias = get_value<RX_Buffer, math::vec3f>(m_rx_buffer, off);
	scale = get_value<RX_Buffer, math::vec3f>(m_rx_buffer, off);

	m_rx_buffer.pop_front(m_rx_header.size);
	tx_acknowledge(m_rx_header.crc);
}
void SFull_Protocol::decode_board_gyroscope_bias(math::vec3f& bias)
{
	ASSERT(m_rx_header.msg == RX_Message::SET_BOARD_GYROSCOPE_BIAS);
	size_t off = 0;
	bias = get_value<RX_Buffer, math::vec3f>(m_rx_buffer, off);

	m_rx_buffer.pop_front(m_rx_header.size);
	tx_acknowledge(m_rx_header.crc);
}
void SFull_Protocol::decode_stream_all_messages(bool& enabled)
{
	ASSERT(m_rx_header.msg == RX_Message::STREAM_ALL_MESSAGES);
	size_t off = 0;
	enabled = get_value<RX_Buffer, bool>(m_rx_buffer, off);

	m_rx_buffer.pop_front(m_rx_header.size);
	tx_acknowledge(m_rx_header.crc);
}
void SFull_Protocol::decode_stream_message(TX_Message& msg, bool& enabled)
{
	ASSERT(m_rx_header.msg == RX_Message::STREAM_MESSAGE);
	size_t off = 0;
	msg = get_value<RX_Buffer, TX_Message>(m_rx_buffer, off);
	enabled = get_value<RX_Buffer, bool>(m_rx_buffer, off);

	m_rx_buffer.pop_front(m_rx_header.size);
	tx_acknowledge(m_rx_header.crc);
}
void SFull_Protocol::decode_send_all_messages_once()
{
	ASSERT(m_rx_header.msg == RX_Message::SEND_ALL_MESSAGES_ONCE);
	m_rx_buffer.pop_front(m_rx_header.size);
	tx_acknowledge(m_rx_header.crc);
}
void SFull_Protocol::decode_send_message_once(TX_Message& msg)
{
	ASSERT(m_rx_header.msg == RX_Message::SEND_MESSAGE_ONCE);
	size_t off = 0;
	msg = get_value<RX_Buffer, TX_Message>(m_rx_buffer, off);

	m_rx_buffer.pop_front(m_rx_header.size);
	tx_acknowledge(m_rx_header.crc);
}
