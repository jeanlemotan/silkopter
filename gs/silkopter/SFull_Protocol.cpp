#include "stdafx.h"
#include "SFull_Protocol.h"

//async_read doesn't work with std::bind
#include "boost/bind.hpp"
#include <thread>
#include "util/crc.h"


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

void SFull_Protocol::handle_read(const boost::system::error_code& e, size_t bytes_transferred)
{
	//THIS CAUSES SERIAL BUFFER OVERFLOWS!!!
// 	for (size_t i = 0; i < bytes_transferred; i++)
// 	{
// 		putchar(m_serial_buffer[i]);// 	}

	{
		std::lock_guard<std::mutex> lg(m_rx_buffer_mutex);
		m_rx_buffer.insert(m_rx_buffer.end(), m_serial_buffer.begin(), m_serial_buffer.begin() + bytes_transferred);
	}

	m_port.async_read_some(boost::asio::buffer(m_serial_buffer.data(), m_serial_buffer.size()),
		boost::bind(
		&SFull_Protocol::handle_read, this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred
		));
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

		m_port.async_read_some(boost::asio::buffer(m_serial_buffer.data(), m_serial_buffer.size()), 
			boost::bind(
			&SFull_Protocol::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
			));

		m_is_listening = true;

		m_io_thread = std::thread([&]()
		{
			while (!m_stop_thread)
			{
				m_io.run();
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

	bool needs_more_data = false;
	do
	{
		auto message = decode_message(needs_more_data);
		if (message)
		{
			process_rx_message(*message);
		}
	} while (!needs_more_data);
//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

//////////////////////////////////////////////////////////////////////////

boost::optional<SFull_Protocol::Message> SFull_Protocol::decode_message(bool& needs_more_data)
{
	needs_more_data = false;

	std::lock_guard<std::mutex> lg(m_rx_buffer_mutex);
	if (m_rx_buffer.size() < 4)
	{
		needs_more_data = true;
		return boost::optional<Message>();
	}

	size_t off = 0;
	auto msg = static_cast<RX_Message>(get_value<uint8_t>(m_rx_buffer, off));
	auto size = get_value<uint8_t>(m_rx_buffer, off);
	auto crc = get_value<uint16_t>(m_rx_buffer, off);
	if (size <= 4)
	{
		int a = 0;
		pop_front(m_rx_buffer); //msg
		return boost::optional<Message>();
	}
	if (m_rx_buffer.size() < size)
	{
		needs_more_data = true;
		return boost::optional<Message>();
	}
	//clear crc bytes and compute crc
	auto crc2 = m_rx_buffer[2];
	auto crc3 = m_rx_buffer[3];
	m_rx_buffer[2] = 0;
	m_rx_buffer[3] = 0;

	auto computed_crc = util::compute_crc(m_rx_buffer.data(), size);
	if (crc != computed_crc)
	{
		m_rx_buffer[2] = crc2;
		m_rx_buffer[3] = crc3;

		printf("\n crc failed for msg: %d, size: %d", (int)msg, (int)size);
		//std::string str(m_buffer.begin(), m_buffer.end());
		int a = 0;
		//put back the crcs (as they were not actually crcs apparently)
		pop_front(m_rx_buffer); //msg
		return boost::optional<Message>();
	}

	Message message;
	message.msg = msg;
	message.crc = crc;
	message.payload.resize(size - 4);
	std::copy(m_rx_buffer.begin() + 4, m_rx_buffer.begin() + 4 + message.payload.size(), message.payload.begin());
	pop_front(m_rx_buffer, size);
//	printf("\nMessage %d, size %d", (int)msg, (int)size);
	return boost::optional<Message>(std::move(message));
}

void SFull_Protocol::process_rx_message(Message const& message)
{
	size_t off = 0;
	if (m_is_connected || message.msg == RX_Message::HELLO_WORLD)
	{
		switch (message.msg)
		{
		case RX_Message::HELLO_WORLD:
			{
				if (!m_is_connected)
				{
					m_protocol_version = get_value<uint8_t>(message.payload, off);
					if (m_protocol_version != 1)
					{
						break;
					}
					m_uav_version = get_value<uint16_t>(message.payload, off);
					auto msg_size = get_value<uint8_t>(message.payload, off);
					if (msg_size > 0)
					{
						m_uav_message.resize(msg_size);
						std::copy(message.payload.begin() + off, message.payload.begin() + off + msg_size, m_uav_message.begin());
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
		case RX_Message::PRINT:
			{
				if (!message.payload.empty())
				{
					std::string str(">");
					str.append(message.payload.begin(), message.payload.end());
					puts(str.c_str());
				}
			}
			break;
		case RX_Message::BOARD_CPU_USAGE:
			{
				data_board_cpu_usage.is_valid = true;
				data_board_cpu_usage.value = get_value<uint8_t>(message.payload, off);
			}
			break;
		case RX_Message::BOARD_TIME:
			{
				data_board_time.is_valid = true;
				data_board_time.value = get_value<uint32_t>(message.payload, off);
			}
			break;
		case RX_Message::BOARD_GYROSCOPE:
			{
			read_optional_data(data_board_gyroscope, message.payload, off);
			}
			break;
		case RX_Message::BOARD_ACCELEROMETER:
			{
			read_optional_data(data_board_accelerometer, message.payload, off);
			}
			break;
		case RX_Message::BOARD_TEMPERATURE:
			{
			read_optional_data(data_board_temperature, message.payload, off);
			}
			break;
		case RX_Message::BOARD_BARO_PRESSURE:
			{
			read_optional_data(data_board_baro_pressure, message.payload, off);
			}
			break;
		case RX_Message::BOARD_SONAR_DISTANCE:
			{
			read_optional_data(data_board_sonar_distance, message.payload, off);
			}
			break;
		case RX_Message::BOARD_GPS_ALTITUDE:
			{
			read_optional_data(data_board_gps_altitude, message.payload, off);
			}
			break;
		case RX_Message::UAV_ACCELERATION:
			{
			data_uav_acceleration.value = get_value<math::vec3f>(message.payload, off);
				data_uav_acceleration.is_valid = true;
			}
			break;
		case RX_Message::UAV_VELOCITY:
			{
			data_uav_velocity.value = get_value<math::vec3f>(message.payload, off);
				data_uav_velocity.is_valid = true;
			}
			break;
		case RX_Message::UAV_POSITION:
			{
			data_uav_position.value = get_value<math::vec3f>(message.payload, off);
				data_uav_position.is_valid = true;
			}
			break;
		case RX_Message::UAV_ATTITUDE:
			{
			data_uav_attitude.value = get_value<math::vec3f>(message.payload, off);
				data_uav_attitude.is_valid = true;
			}
			break;
		default:
			//__dbg_break(0);
			int a = 0;
			break;

		}
	}
}

//tx_message structure:
//offset : size : description
//0	: 1 : msg
//1 : 1 : size
//2 : 2 : crc of everything excluding these 2 bytes (they are zeroed before)
//>4 : x : data
void SFull_Protocol::start_tx_message(TX_Message msg)
{
	m_tx_buffer.clear();
	add_value(m_tx_buffer, static_cast<uint8_t>(msg));
	add_value(m_tx_buffer, uint8_t(0));
	add_value(m_tx_buffer, uint16_t(0));
}
uint16_t SFull_Protocol::flush_tx_message()
{
	set_value(m_tx_buffer, uint8_t(m_tx_buffer.size()), 1);
	uint16_t crc = util::compute_crc(m_tx_buffer.data(), m_tx_buffer.size());
	set_value(m_tx_buffer, crc, 2);
	boost::asio::write(m_port, boost::asio::buffer(m_tx_buffer.data(), m_tx_buffer.size()));
	return crc;
}

bool SFull_Protocol::set_board_gyroscope_bias(math::vec3f const& bias)
{
	start_tx_message(TX_Message::SET_BOARD_GYROSCOPE_BIAS);
	add_value(m_tx_buffer, bias);
	auto crc = flush_tx_message();
	return wait_for_response(crc, std::chrono::seconds(1));
}

bool SFull_Protocol::set_board_accelerometer_bias_scale(math::vec3f const& bias, math::vec3f const& scale)
{
	start_tx_message(TX_Message::SET_BOARD_ACCELEROMETER_BIAS_SCALE);
	add_value(m_tx_buffer, bias);
	add_value(m_tx_buffer, scale);
	auto crc = flush_tx_message();
	return wait_for_response(crc, std::chrono::seconds(1));
}

bool SFull_Protocol::set_stream_all_messages(bool enabled)
{
	start_tx_message(TX_Message::STREAM_ALL_MESSAGES);
	add_value(m_tx_buffer, enabled);
	auto crc = flush_tx_message();
	return wait_for_response(crc, std::chrono::seconds(1));
}
bool SFull_Protocol::set_stream_message(RX_Message message, bool enabled)
{
	start_tx_message(TX_Message::STREAM_MESSAGE);
	add_value(m_tx_buffer, message);
	add_value(m_tx_buffer, enabled);
	auto crc = flush_tx_message();
	return wait_for_response(crc, std::chrono::seconds(1));
}
bool SFull_Protocol::set_send_all_message_once()
{
	start_tx_message(TX_Message::SEND_ALL_MESSAGES_ONCE);
	auto crc = flush_tx_message();
	return wait_for_response(crc, std::chrono::seconds(1));
}
bool SFull_Protocol::set_send_message_once(RX_Message message)
{
	start_tx_message(TX_Message::SEND_MESSAGE_ONCE);
	add_value(m_tx_buffer, message);
	auto crc = flush_tx_message();
	return wait_for_response(crc, std::chrono::seconds(1));
}

//////////////////////////////////////////////////////////////////////////

bool SFull_Protocol::wait_for_response(uint16_t expected_crc, std::chrono::high_resolution_clock::duration timeout)
{
	auto start = std::chrono::high_resolution_clock::now();

	do
	{
		bool needs_more_data = false;
		auto message = decode_message(needs_more_data);
		if (message)
		{
			process_rx_message(*message);
			if (message->msg == RX_Message::ACKNOLEDGE)
			{
				size_t off = 0;
				uint16_t crc = get_value<uint16_t>(message->payload, off);
				if (expected_crc == crc)
				{
					return true;
				}
			}
		}
		std::this_thread::yield(); //to allow the serial thread to get more data
	} while (std::chrono::high_resolution_clock::now() - start < timeout);

	return false;
}



