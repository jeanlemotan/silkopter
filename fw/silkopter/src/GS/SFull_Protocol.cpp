#include "GS/SFull_Protocol.h"
#include "debug/debug.h"

using namespace silk;

SFull_Protocol::SFull_Protocol()
	: m_is_connected(false)
{
	m_enabled_tx_messages.set();
}

SFull_Protocol::SFull_Protocol(board::UART& uart)
	: m_uart(&uart)
	, m_is_connected(false)
{
	m_enabled_tx_messages.set();
	//m_enabled_messages.reset();
		
 	m_enabled_tx_messages.set(static_cast<int>(TX_Message::HELLO_WORLD), 1);
	m_enabled_tx_messages.set(static_cast<int>(TX_Message::BOARD_TIME), 1);
	 
	 
// 	m_enabled_messages.set(static_cast<int>(Message::BOARD_GYROSCOPE), 1);
// 	m_enabled_messages.set(static_cast<int>(Message::BOARD_ACCELEROMETER), 1);
// 	m_enabled_messages.set(static_cast<int>(Message::UAV_ATTITUDE), 1);
}

//////////////////////////////////////////////////////////////////////////

bool SFull_Protocol::is_tx_message_enabled(TX_Message msg) const
{
	return m_enabled_tx_messages.test(static_cast<uint8_t>(msg));
}

//////////////////////////////////////////////////////////////////////////

static const uint8_t k_version = 1;

//message structure:
//offset : size : description
//0	: 1 : msg
//1 : 1 : size
//2 : 2 : crc of everything excluding these 2 bytes (they are zeroed before)
//>4 : x : data
bool SFull_Protocol::start_tx_message(TX_Message msg)
{
	if (!m_enabled_tx_messages.test(static_cast<uint8_t>(msg)))
	{
		return false;
	}
	m_tx_buffer.clear();
	m_tx_buffer.append(static_cast<uint8_t>(msg));
	m_tx_buffer.append(uint8_t(0)); //size
	m_tx_buffer.append(uint16_t(0)); //crc
	return true;
}
void SFull_Protocol::flush_tx_message()
{
	m_tx_buffer[1] = uint8_t(m_tx_buffer.size());
	auto crc = util::compute_crc(m_tx_buffer.data(), m_tx_buffer.size());
	//PRINT("\n msg: {0}, size {1}, crc {2}", m_tx_buffer[0], m_tx_buffer[1], crc);
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
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void SFull_Protocol::tx_print(Print_String const& str)
{
	start_tx_message(TX_Message::PRINT);
	m_tx_buffer.append(reinterpret_cast<uint8_t const*>(str.c_str()), str.size());
	flush_tx_message();
}
	
//////////////////////////////////////////////////////////////////////////
	
void SFull_Protocol::tx_board_cpu_usage(uint8_t cpu_usage_percent)
{
	if (!start_tx_message(TX_Message::BOARD_CPU_USAGE))
	{
		return;
	}
	m_tx_buffer.append(cpu_usage_percent);
	flush_tx_message();
}

void SFull_Protocol::tx_board_time(chrono::time_us time)
{
	if (!start_tx_message(TX_Message::BOARD_TIME))
	{
		return;
	}
	m_tx_buffer.append(uint32_t(time.time_since_epoch().count));
	flush_tx_message();
}
	
void SFull_Protocol::tx_board_gyroscope(bool is_valid, math::vec3f const& gyro)
{
	if (!start_tx_message(TX_Message::BOARD_GYROSCOPE))
	{
		return;
	}
	m_tx_buffer.append(is_valid);
	if (is_valid)
	{
		m_tx_buffer.append(gyro);
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_accelerometer(bool is_valid, math::vec3f const& accel)
{
	if (!start_tx_message(TX_Message::BOARD_ACCELEROMETER))
	{
		return;
	}
	m_tx_buffer.append(is_valid);
	if (is_valid)
	{
		m_tx_buffer.append(accel);
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_temperature(bool is_valid, float temp)
{
	if (!start_tx_message(TX_Message::BOARD_TEMPERATURE))
	{
		return;
	}

	m_tx_buffer.append(is_valid);
	if (is_valid)
	{
		m_tx_buffer.append(temp);
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_baro_pressure(bool is_valid, float pressure)
{
	if (!start_tx_message(TX_Message::BOARD_BARO_PRESSURE))
	{
		return;
	}
	m_tx_buffer.append(is_valid);
	if (is_valid)
	{
		m_tx_buffer.append(pressure);
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_sonar_distance(bool is_valid, float distance)
{
	if (!start_tx_message(TX_Message::BOARD_SONAR_DISTANCE))
	{
		return;
	}
	m_tx_buffer.append(is_valid);
	if (is_valid)
	{
		m_tx_buffer.append(distance);
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_gps_altitude(bool is_valid, float altitude)
{
	if (!start_tx_message(TX_Message::BOARD_GPS_ALTITUDE))
	{
		return;
	}
	m_tx_buffer.append(is_valid);
	if (is_valid)
	{
		m_tx_buffer.append(altitude);
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_rc_in(uint8_t count, int16_t const* values)
{
	if (!start_tx_message(TX_Message::BOARD_RC_IN))
	{
		return;
	}
	m_tx_buffer.append(count);
	if (count)
	{
		m_tx_buffer.append(reinterpret_cast<uint8_t const*>(values), count*sizeof(int16_t));
	}
	flush_tx_message();
}
void SFull_Protocol::tx_board_pwm_out(uint8_t count, int16_t const* values)
{
	if (!start_tx_message(TX_Message::BOARD_PWM_OUT))
	{
		return;
	}
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
	if (!start_tx_message(TX_Message::UAV_ACCELERATION))
	{
		return;
	}
	m_tx_buffer.append(accel);
	flush_tx_message();
}
void SFull_Protocol::tx_uav_velocity(math::vec3f const& velocity)
{
	if (!start_tx_message(TX_Message::UAV_VELOCITY))
	{
		return;
	}
	m_tx_buffer.append(velocity);
	flush_tx_message();
}
void SFull_Protocol::tx_uav_position(math::vec3f const& position)
{
	if (!start_tx_message(TX_Message::UAV_POSITION))
	{
		return;
	}
	m_tx_buffer.append(position);
	flush_tx_message();
}

void SFull_Protocol::tx_uav_attitude(math::vec3f const& euler)
{
	if (!start_tx_message(TX_Message::UAV_ATTITUDE))
	{
		return;
	}
	m_tx_buffer.append(euler);
	flush_tx_message();
}

void SFull_Protocol::tx_uav_phase(UAV::Phase phase)
{
	if (!start_tx_message(TX_Message::UAV_PHASE))
	{
		return;
	}
	m_tx_buffer.append(phase);
	flush_tx_message();
}
void SFull_Protocol::tx_uav_control_mode(UAV::Control_Mode mode)
{
	if (!start_tx_message(TX_Message::UAV_CONTROL_MODE))
	{
		return;
	}
	m_tx_buffer.append(mode);
	flush_tx_message();
}
void SFull_Protocol::tx_uav_control_reference_frame(UAV::Control_Reference_Frame frame)
{
	if (!start_tx_message(TX_Message::UAV_CONTROL_REFERENCE_FRAME))
	{
		return;
	}
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

bool SFull_Protocol::decode_rx_header(Header& header, bool& needs_more_data)
{
	needs_more_data = false;

	if (m_rx_buffer.size() < sizeof(Header))
	{
		needs_more_data = true;
		return false;
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
	header.msg = msg;
	header.size = size - sizeof(Header);
	header.crc = crc;
	return true;
}

SProtocol::RX_Message SFull_Protocol::get_next_rx_message()
{
	auto available = m_uart->get_data_size();
	if (available > 0)
	{
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
		Header header;
		bool needs_more_data;
		if (decode_rx_header(header, needs_more_data))
		{
			PRINT("\nreceived {0}, crc: {1}, size: {2}", static_cast<int>(header.msg), header.crc, header.size);
			return header.msg;
		}
	}
	return RX_Message::NONE;
}

void SFull_Protocol::rx_board_accelerometer_bias_scale(math::vec3f& bias, math::vec3f& scale) const
{
	
}
void SFull_Protocol::rx_board_gyroscope_bias(math::vec3f& bias) const
{
	
}
