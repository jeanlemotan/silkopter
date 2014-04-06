#include "stdafx.h"
#include "SFull_Protocol.h"

//async_read doesn't work with std::bind
#include "boost/bind.hpp"


SFull_Protocol::SFull_Protocol()
	: m_is_listening(false)
	, m_is_connected(false)
	, m_protocol_version(0)
	, m_uav_version(0)
	, m_stop_thread(false)
	, m_io()
	, m_port(m_io)
{
}

SFull_Protocol::~SFull_Protocol()
{
	m_stop_thread = true;
	if (m_io_thread.joinable())
	{
		m_io_thread.join();
	}
	m_port.close();
	m_io.stop();
}

void SFull_Protocol::listen_for_connection(std::string const& com_port, uint32_t baud)
{
	stop();
	if (m_io_thread.joinable())
	{
		m_io_thread.join();
	}
	m_stop_thread = false;

	try
	{
		m_port.open(com_port);
		m_port.set_option(boost::asio::serial_port_base::baud_rate(baud));
		m_port.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
		m_port.set_option(boost::asio::serial_port_base::character_size(8));
		m_port.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
		m_port.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));

		m_is_listening = true;

		m_io_thread = std::thread([&]()
		{
			while (!m_stop_thread)
			{
				boost::asio::read(m_port, boost::asio::buffer((char*)m_serial_buffer.data(), m_serial_buffer.size()));
				for (size_t i = 0; i < m_serial_buffer.size(); i++)
				{
					putchar(m_serial_buffer[i]);
				}

				{
					std::lock_guard<std::mutex> lg(m_rx_buffer_mutex);
					m_rx_buffer.insert(m_rx_buffer.end(), m_serial_buffer.begin(), m_serial_buffer.end());
				}
			}
			m_is_listening = false;
		});
	}
	catch(...)
	{
		return;
	}
}
bool SFull_Protocol::is_connected() const
{
	return m_is_connected;
}

bool SFull_Protocol::is_listening() const
{
	return m_is_listening;
}

void SFull_Protocol::stop()
{
	m_stop_thread = true;
	m_is_connected = false;
	m_is_listening = false;
}

//////////////////////////////////////////////////////////////////////////

static void pop_front(std::vector<uint8_t>& t, size_t x = 1)
{
	t.erase(t.begin(), t.begin() + x);
}
template<class T>
static T get_value(const std::vector<uint8_t>& t, size_t& off)
{
	T val = *reinterpret_cast<const T*>(t.data() + off);
	off += sizeof(T);
	return val;
}
template<class T>
static void set_value(std::vector<uint8_t>& t, T const& val, size_t off)
{
	if (off + sizeof(T) > t.size())
	{
		t.resize(off + sizeof(T));
	}
	*reinterpret_cast<T*>(t.data() + off) = val;
}
template<class T>
static void add_value(std::vector<uint8_t>& t, T const& val)
{
	size_t off = t.size();
	t.resize(off + sizeof(T));
	*reinterpret_cast<T*>(t.data() + off) = val;
}
template<class T>
static void read_optional_data(SFull_Protocol::Data<T>& dst, const std::vector<uint8_t>& t, size_t& off)
{
	dst.is_valid = get_value<bool>(t, off);
	if (dst.is_valid)
	{
		dst.value = get_value<T>(t, off);
	}
}

//////////////////////////////////////////////////////////////////////////

void SFull_Protocol::process()
{
	if (!m_is_listening || m_stop_thread)
	{
		return;
	}

	std::lock_guard<std::mutex> lg(m_rx_buffer_mutex);

	while (!process_message())
	{
	}
//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}


bool SFull_Protocol::process_message()
{
//	printf("\nsize = %d", m_buffer.size());
	if (m_rx_buffer.size() < 4)
	{
		return true;
	}

	size_t off = 0;
	auto msg = static_cast<Message>(get_value<uint8_t>(m_rx_buffer, off));
	auto size = get_value<uint8_t>(m_rx_buffer, off);
	auto crc = get_value<uint16_t>(m_rx_buffer, off);
	if (size < 4)
	{
		int a = 0;
		pop_front(m_rx_buffer); //msg
		return false;
	}
	if (m_rx_buffer.size() < size)
	{
		return true;
	}
	//clear crc bytes and compute crc
	auto crc2 = m_rx_buffer[2];
	auto crc3 = m_rx_buffer[3];
	m_rx_buffer[2] = 0;
	m_rx_buffer[3] = 0;

	auto computed_crc = compute_crc(m_rx_buffer.data(), size);
	if (crc != computed_crc)
	{
		//std::string str(m_buffer.begin(), m_buffer.end());
		int a = 0;
		//put back the crcs (as they were not actually crcs apparently)
		m_rx_buffer[2] = crc2;
		m_rx_buffer[3] = crc3;
		pop_front(m_rx_buffer); //msg
		return false;
	}

	if (m_is_connected || msg == Message::HELLO_WORLD)
	{
		switch (msg)
		{
		case Message::HELLO_WORLD:
			{
				if (!m_is_connected)
				{
					m_protocol_version = get_value<uint8_t>(m_rx_buffer, off);
					if (m_protocol_version != 1)
					{
						break;
					}
					m_uav_version = get_value<uint16_t>(m_rx_buffer, off);
					auto msg_size = get_value<uint8_t>(m_rx_buffer, off);
					if (msg_size > 0)
					{
						m_uav_message.resize(msg_size);
						std::copy(m_rx_buffer.begin() + off, m_rx_buffer.begin() + off + msg_size, m_uav_message.begin());
						off += msg_size;
					}

					uint8_t response[] = { 'H', 'O', 'L', 'A' };
					boost::asio::write(m_port, boost::asio::buffer(response, sizeof(response)));

					m_is_connected = true;
				}
				else
				{
					uint8_t response[] = { 'H', 'O', 'L', 'A' };
					boost::asio::write(m_port, boost::asio::buffer(response, sizeof(response)));
				}
			}
			break;
		case Message::BOARD_CPU_USAGE:
			{
				data_board_cpu_usage.is_valid = true;
				data_board_cpu_usage.value = get_value<uint8_t>(m_rx_buffer, off);
			}
			break;
		case Message::BOARD_TIME:
			{
				data_board_time.is_valid = true;
				data_board_time.value = get_value<uint32_t>(m_rx_buffer, off);
			}
			break;
		case Message::BOARD_GYROSCOPE:
			{
				read_optional_data(data_board_gyroscope, m_rx_buffer, off);
			}
			break;
		case Message::BOARD_ACCELEROMETER:
			{
				read_optional_data(data_board_accelerometer, m_rx_buffer, off);
			}
			break;
		case Message::BOARD_TEMPERATURE:
			{
				read_optional_data(data_board_temperature, m_rx_buffer, off);
			}
			break;
		case Message::BOARD_BARO_PRESSURE:
			{
				read_optional_data(data_board_baro_pressure, m_rx_buffer, off);
			}
			break;
		case Message::BOARD_SONAR_DISTANCE:
			{
				read_optional_data(data_board_sonar_distance, m_rx_buffer, off);
			}
			break;
		case Message::BOARD_GPS_ALTITUDE:
			{
				read_optional_data(data_board_gps_altitude, m_rx_buffer, off);
			}
			break;
		case Message::UAV_ACCELERATION:
			{
				data_uav_acceleration.value = get_value<math::vec3f>(m_rx_buffer, off);
				data_uav_acceleration.is_valid = true;
			}
			break;
		case Message::UAV_VELOCITY:
			{
				data_uav_velocity.value = get_value<math::vec3f>(m_rx_buffer, off);
				data_uav_velocity.is_valid = true;
			}
			break;
		case Message::UAV_POSITION:
			{
				data_uav_position.value = get_value<math::vec3f>(m_rx_buffer, off);
				data_uav_position.is_valid = true;
			}
			break;
		case Message::UAV_ATTITUDE:
			{
				data_uav_attitude.value = get_value<math::vec3f>(m_rx_buffer, off);
				data_uav_attitude.is_valid = true;
			}
			break;
		default:
			//__dbg_break(0);
			int a = 0;
			break;

		}
	}

	pop_front(m_rx_buffer, size);
	return false;
}

static uint16_t crc16_update(uint16_t crc, uint8_t a)
{
	crc ^= a;
	for (int i = 0; i < 8; ++i)
	{
		if (crc & 1)
		{
			crc = (crc >> 1) ^ 0xA001;
		}
		else
		{
			crc = (crc >> 1);
		}
	}
	return crc;
}

uint16_t SFull_Protocol::compute_crc(uint8_t const* data, size_t size) const
{
	uint16_t crc = 0;
	if (data)
	{
		for (size_t i = 0; i < size; i++)
		{
			crc = crc16_update(crc, *data++);
		}
	}
	return crc;
}

//command structure:
//offset : size : description
//0	: 1 : msg
//1 : 1 : size
//2 : 2 : crc of everything excluding these 2 bytes (they are zeroed before)
//>4 : x : data
bool SFull_Protocol::start_command(Command cmd)
{
	m_tx_buffer.clear();
	add_value(m_tx_buffer, static_cast<uint8_t>(cmd));
	add_value(m_tx_buffer, uint8_t(0));
	add_value(m_tx_buffer, uint16_t(0));
	return true;
}
void SFull_Protocol::flush_command()
{
	set_value(m_tx_buffer, uint8_t(m_tx_buffer.size()), 1);
	uint16_t crc = compute_crc(m_tx_buffer.data(), m_tx_buffer.size());
	set_value(m_tx_buffer, crc, 2);
	boost::asio::write(m_port, boost::asio::buffer(m_tx_buffer.data(), m_tx_buffer.size()));
}

void SFull_Protocol::set_board_gyroscope_bias(math::vec3f const& bias)
{
	start_command(Command::SET_BOARD_GYROSCOPE_BIAS);
	add_value(m_tx_buffer, bias);
	flush_command();
}

void SFull_Protocol::set_board_accelerometer_bias_scale(math::vec3f const& bias, math::vec3f const& scale)
{
	start_command(Command::SET_BOARD_ACCELEROMETER_BIAS_SCALE);
	add_value(m_tx_buffer, bias);
	add_value(m_tx_buffer, scale);
	flush_command();
}


