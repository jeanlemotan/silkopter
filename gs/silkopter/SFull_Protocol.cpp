#include "stdafx.h"
#include "SFull_Protocol.h"

//async_read doesn't work with std::bind
#include "boost/bind.hpp"

SFull_Protocol::SFull_Protocol()
	: m_is_listening(false)
	, m_is_connected(false)
	, m_protocol_version(0)
	, m_uav_version(0)
	, m_is_frame_started(false)
	, m_stop_thread(false)
	, m_last_frame_idx(0)
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

void SFull_Protocol::serial_callback(const boost::system::error_code& error, size_t bytes_transferred)
{
	{
		std::lock_guard<std::mutex> lg(m_buffer_mutex);
		m_buffer.insert(m_buffer.end(), m_serial_buffer.begin(), m_serial_buffer.begin() + bytes_transferred);
// 		for (size_t i = 0; i < bytes_transferred; i++)
// 		{
// 			putchar(m_serial_buffer[i]);
// 		}
	}

	//m_serial_buffer[bytes_transferred] = 0;
	//std::cout << bytes_transferred << " bytes: " << m_serial_buffer.data() << std::endl;

	read_async();
}

void SFull_Protocol::read_async()
{
// 	m_port.async_read_some(boost::asio::buffer((char*)m_serial_buffer.data(), m_serial_buffer.size() - 1),
// 		boost::bind(&SFull_Protocol::serial_callback, 
// 		this,
// 		boost::asio::placeholders::error, 
// 		boost::asio::placeholders::bytes_transferred));

//	boost::asio::read(m_port, boost::asio::buffer((char*)m_serial_buffer.data(), m_serial_buffer.size() - 1));
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

		read_async();

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
					std::lock_guard<std::mutex> lg(m_buffer_mutex);
					m_buffer.insert(m_buffer.end(), m_serial_buffer.begin(), m_serial_buffer.end());
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
static void read_optional_data(SFull_Protocol::Data<T>& dst, const std::vector<uint8_t>& t, size_t& off)
{
	dst.is_valid = get_value<bool>(t, off);
	if (dst.is_valid)
	{
		dst.value = get_value<T>(t, off);
	}
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
static const uint8_t MSG_BOARD_SONAR_DISTANCE = 14;
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

void SFull_Protocol::process()
{
	if (!m_is_listening || m_stop_thread)
	{
		return;
	}

	std::lock_guard<std::mutex> lg(m_buffer_mutex);

	while (!process_message())
	{
	}
//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}


bool SFull_Protocol::process_message()
{
//	printf("\nsize = %d", m_buffer.size());
	if (m_buffer.size() < 4)
	{
		return true;
	}

	size_t off = 0;
	auto msg = get_value<uint8_t>(m_buffer, off);
	auto size = get_value<uint8_t>(m_buffer, off);
	auto crc = get_value<uint16_t>(m_buffer, off);
	if (size < 4)
	{
		int a = 0;
		pop_front(m_buffer); //msg
		return false;
	}
	if (m_buffer.size() < size)
	{
		return true;
	}
	//clear crc bytes and compute crc
	auto crc2 = m_buffer[2];
	auto crc3 = m_buffer[3];
	m_buffer[2] = 0;
	m_buffer[3] = 0;

	auto computed_crc = compute_crc(m_buffer.data(), size);
	if (crc != computed_crc)
	{
		//std::string str(m_buffer.begin(), m_buffer.end());
		int a = 0;
		//put back the crcs (as they were not actually crcs apparently)
		m_buffer[2] = crc2;
		m_buffer[3] = crc3;
		pop_front(m_buffer); //msg
		return false;
	}

	if (m_is_connected || msg == MSG_HELLO_WORLD)
	{
		switch (msg)
		{
		case MSG_HELLO_WORLD:
			{
				if (!m_is_connected)
				{
					m_protocol_version = get_value<uint8_t>(m_buffer, off);
					if (m_protocol_version != 1)
					{
						break;
					}
					m_uav_version = get_value<uint16_t>(m_buffer, off);
					auto msg_size = get_value<uint8_t>(m_buffer, off);
					if (msg_size > 0)
					{
						m_uav_message.resize(msg_size);
						std::copy(m_buffer.begin() + off, m_buffer.begin() + off + msg_size, m_uav_message.begin());
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
		case MSG_START_FRAME:
			{
				auto frame_idx = get_value<uint32_t>(m_buffer, off);
				if (frame_idx <= m_last_frame_idx)
				{
					break;
				}
				m_last_frame_idx = frame_idx;
				m_is_frame_started = true;
			}
			break;
		case MSG_END_FRAME:
			{
				auto frame_idx = get_value<uint32_t>(m_buffer, off);
				if (frame_idx != m_last_frame_idx)
				{
					break;
				}
				m_is_frame_started = false;
			}
			break;
		case MSG_BOARD_CPU_USAGE:
			{
				data_board_cpu_usage.is_valid = true;
				data_board_cpu_usage.received_frame_idx = m_last_frame_idx;
				data_board_cpu_usage.value = get_value<uint8_t>(m_buffer, off);
			}
			break;
		case MSG_BOARD_TIME_MS:
			{
				data_board_time_ms.is_valid = true;
				data_board_time_ms.received_frame_idx = m_last_frame_idx;
				data_board_time_ms.value = get_value<uint32_t>(m_buffer, off);
			}
			break;
		case MSG_BOARD_GYROSCOPE:
			{
				read_optional_data(data_board_gyroscope, m_buffer, off);
				data_board_gyroscope.received_frame_idx = m_last_frame_idx;
			}
			break;
		case MSG_BOARD_ACCELEROMETER:
			{
				read_optional_data(data_board_accelerometer, m_buffer, off);
				data_board_accelerometer.received_frame_idx = m_last_frame_idx;
			}
			break;
		case MSG_BOARD_TEMPERATURE:
			{
				read_optional_data(data_board_temperature, m_buffer, off);
				data_board_temperature.received_frame_idx = m_last_frame_idx;
			}
			break;
		case MSG_BOARD_BARO_PRESSURE:
			{
				read_optional_data(data_board_baro_pressure, m_buffer, off);
				data_board_baro_pressure.received_frame_idx = m_last_frame_idx;
			}
			break;
		case MSG_BOARD_SONAR_DISTANCE:
			{
				read_optional_data(data_board_sonar_distance, m_buffer, off);
				data_board_sonar_distance.received_frame_idx = m_last_frame_idx;
			}
			break;
		case MSG_BOARD_GPS_ALTITUDE:
			{
				read_optional_data(data_board_gps_altitude, m_buffer, off);
				data_board_gps_altitude.received_frame_idx = m_last_frame_idx;
			}
			break;
		case MSG_UAV_ACCELERATION:
			{
				data_uav_acceleration.value = get_value<math::vec3f>(m_buffer, off);
				data_uav_acceleration.is_valid = true;
				data_uav_acceleration.received_frame_idx = m_last_frame_idx;
			}
			break;
		case MSG_UAV_VELOCITY:
			{
				data_uav_velocity.value = get_value<math::vec3f>(m_buffer, off);
				data_uav_velocity.is_valid = true;
				data_uav_velocity.received_frame_idx = m_last_frame_idx;
			}
			break;
		case MSG_UAV_POSITION:
			{
				data_uav_position.value = get_value<math::vec3f>(m_buffer, off);
				data_uav_position.is_valid = true;
				data_uav_position.received_frame_idx = m_last_frame_idx;
			}
			break;
		case MSG_UAV_ATTITUDE:
			{
				data_uav_attitude.value = get_value<math::vec3f>(m_buffer, off);
				data_uav_attitude.is_valid = true;
				data_uav_attitude.received_frame_idx = m_last_frame_idx;
			}
			break;
		default:
			//__dbg_break(0);
			int a = 0;
			break;

		}
	}

	pop_front(m_buffer, size);
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

