#include "GS/GS.h"
#include "board/board.h"
#include "debug/debug.h"
#include "util/murmurhash.h"
#include "util/storage.h"

using namespace silk;

static const uint16_t k_version = 100;
static const SProtocol::Message_String k_hello_world("Silkopter");


GS::GS(UAV& uav, board::UART& full_uart)
	: m_uav(uav)
	, m_full_uart(full_uart)
	, m_compact_uart(nullptr)
	, m_full_protocol(full_uart)
	, m_step(0)
{
	full_uart.set_blocking(false);
	m_full_protocol.tx_hello_world(k_hello_world, k_version);
}
GS::GS(UAV& uav, board::UART& full_uart, board::UART& compact_uart)
	: m_uav(uav)
	, m_full_uart(full_uart)
	, m_compact_uart(&compact_uart)
	, m_full_protocol(full_uart)
	//, m_compact_protocol(compact_uart)
	, m_step(0)
{
	full_uart.set_blocking(false);
	compact_uart.set_blocking(false);
	m_full_protocol.tx_hello_world(k_hello_world, k_version);
	//m_compact_protocol.hello_world(k_hello_world, k_version);
}

void GS::process(chrono::micros max_duration)
{
	chrono::micros duration;

	//calculate frame time
	auto start = board::clock::now_us();
// 	if (start - m_last_time < chrono::micros(10000))
// 	{
// 		return;
// 	}
	m_frame_duration = start - m_last_time;
	m_last_time = start;
	
	if (!m_full_protocol.is_connected())
	{
		if (start - m_last_hello_time > chrono::micros(100000))
		{
			m_last_hello_time = start;
			m_full_protocol.tx_hello_world(k_hello_world, k_version);
		}
		return;
	}
	
	receive_data(m_full_protocol.get_next_rx_message());

	start = board::clock::now_us();	
	
	bool done = false;
	do
	{
		done = send_data(m_step++);
		auto now = board::clock::now_us();
		duration += now - start;
		start = now;
	} while (!done && duration <= max_duration);

	//start over	
	if (done)
	{
		m_step = 0;
	}
}

void GS::receive_data(SProtocol::RX_Message message)
{
	switch (message)
	{
		case SProtocol::RX_Message::NONE:
			return;
		break;
		case SProtocol::RX_Message::STREAM_ALL_MESSAGES:
		{
			bool enabled;
			m_full_protocol.decode_stream_all_messages(enabled);
			m_full_protocol.tx_printf(F_STR("Stream all messages {}"), enabled ? F_STR("enabled") : F_STR("disabled"));
			std::fill(m_message_send_option, m_message_send_option + 256, enabled ? Send_Option::STREAM : Send_Option::DISABLED);
		}
		break;
		case SProtocol::RX_Message::STREAM_MESSAGE:
		{
			bool enabled;
			SProtocol::TX_Message msg;
			m_full_protocol.decode_stream_message(msg, enabled);
			uint8_t msg_idx = static_cast<uint8_t>(msg);
			m_full_protocol.tx_printf(F_STR("Stream message {} {}"), msg_idx, enabled ? F_STR("enabled") : F_STR("disabled"));
			m_message_send_option[msg_idx] = enabled ? Send_Option::STREAM : Send_Option::DISABLED;
		}
		break;
		case SProtocol::RX_Message::SEND_ALL_MESSAGES_ONCE:
		{
			m_full_protocol.decode_send_all_messages_once();
			m_full_protocol.tx_printf(F_STR("Send all messages once"));
			std::fill(m_message_send_option, m_message_send_option + 256, Send_Option::ONCE);
		}
		break;
		case SProtocol::RX_Message::SEND_MESSAGE_ONCE:
		{
			SProtocol::TX_Message msg;
			m_full_protocol.decode_send_message_once(msg);
			uint8_t msg_idx = static_cast<uint8_t>(msg);
			m_full_protocol.tx_printf(F_STR("Send message {} once"), msg_idx);
			m_message_send_option[msg_idx] = Send_Option::ONCE;
		}
		break;
		case SProtocol::RX_Message::SET_BOARD_ACCELEROMETER_BIAS_SCALE:
		{
			auto data = board::get_main_imu().get_calibration_data();
			m_full_protocol.decode_board_accelerometer_bias_scale(data.accelerometer_bias, data.accelerometer_scale);
			board::get_main_imu().set_calibration_data(data);
			m_full_protocol.tx_printf(F_STR("Received accelerometer bias {.9} / scale {.9}"), data.accelerometer_bias, data.accelerometer_scale);

			util::storage::set_record(util::storage::Id(static_murmurhash("imu_calibration_data")), data);
		}
		break;
		case SProtocol::RX_Message::SET_BOARD_GYROSCOPE_BIAS:
		{
			auto data = board::get_main_imu().get_calibration_data();
			m_full_protocol.decode_board_gyroscope_bias(data.gyroscope_bias);
			board::get_main_imu().set_calibration_data(data);
			m_full_protocol.tx_printf(F_STR("Received gyroscope bias {.9}"), data.gyroscope_bias);

			util::storage::set_record(util::storage::Id(static_murmurhash("imu_calibration_data")), data);
		}
		break;
		default:
			m_full_protocol.discard_rx_message();
		break;
	}
}

auto GS::get_send_option(SProtocol::TX_Message msg) -> Send_Option
{
	auto& so = m_message_send_option[static_cast<uint8_t>(msg)];
	if (so == Send_Option::ONCE)
	{
		so = Send_Option::DISABLED;
		return Send_Option::ONCE;
	}
	return so;
}

bool GS::send_data(uint32_t step)
{

	switch (step)
	{
	case 5:
	{
		if (get_send_option(SProtocol::TX_Message::BOARD_CPU_USAGE) != Send_Option::DISABLED)
		{
			uint8_t cpu_usage = m_frame_duration.count / 1000;// * 100 / 5000;
			m_full_protocol.tx_board_cpu_usage(cpu_usage);
		}
	}
	case 6:
	{
		if (get_send_option(SProtocol::TX_Message::BOARD_TIME) != Send_Option::DISABLED)
		{
			m_full_protocol.tx_board_time(board::clock::now_us());
		}
		return false;
	}
	case 10:
	{
		if (get_send_option(SProtocol::TX_Message::BOARD_ACCELEROMETER) != Send_Option::DISABLED)
		{
			board::IMU::Data data;
			bool is_valid = board::get_main_imu().get_data(data);
			m_full_protocol.tx_board_accelerometer(is_valid, data.acceleration);
		}
		return false;
	}
	case 13:
	{
		if (get_send_option(SProtocol::TX_Message::BOARD_GYROSCOPE) != Send_Option::DISABLED)
		{
			board::IMU::Data data;
			bool is_valid = board::get_main_imu().get_data(data);
			m_full_protocol.tx_board_gyroscope(is_valid, data.angular_velocity);
		}
		return false;
	}
	case 18:
	{
		if (get_send_option(SProtocol::TX_Message::BOARD_SONAR_DISTANCE) != Send_Option::DISABLED)
		{
			// 		board::sonar::Data data;
			// 		board::sonar::get_data(data);
			// 		m_full_protocol.tx_board_sonar_altitude(data.is_valid, data.altitude);
		}
		return false;
	}
	case 20:
	{
		if (get_send_option(SProtocol::TX_Message::BOARD_BARO_PRESSURE) != Send_Option::DISABLED)
		{
			board::Barometer::Data data;
			if (board::get_main_barometer())
			{
				bool is_valid = board::get_main_barometer()->get_data(data);
				m_full_protocol.tx_board_baro_pressure(is_valid, data.pressure);
			}
		}
		return false;
	}
	case 22:
	{
		if (get_send_option(SProtocol::TX_Message::BOARD_RC_IN) != Send_Option::DISABLED)
		{
			//		int16_t data[32];
			//		auto count = math::min(board::get_rc_in().get_count(), uint8_t(32));
			// 		board::rc_in::get_channels(data, count);
			// 		m_full_protocol.tx_board_rc_in(count, data);
		}
		return false;
	}
	case 24:
	{
		if (get_send_option(SProtocol::TX_Message::BOARD_PWM_OUT) != Send_Option::DISABLED)
		{
			//		int16_t data[32];
			//		auto count = math::min(board::get_pwm_out().get_count(), uint8_t(32));
			// 		board::pwm_out::get_channels(data, count);
			// 		m_full_protocol.tx_board_pwm_out(count, data);
		}
		return false;
	}

	case 30:
	{
		if (get_send_option(SProtocol::TX_Message::UAV_ACCELERATION) != Send_Option::DISABLED)
		{
			m_full_protocol.tx_uav_acceleration(m_uav.get_status().acceleration);
		}
		return false;
	}
	case 32:
	{
		if (get_send_option(SProtocol::TX_Message::UAV_VELOCITY) != Send_Option::DISABLED)
		{
			m_full_protocol.tx_uav_velocity(m_uav.get_status().velocity);
		}
		return false;
	}
	case 34:
	{
		if (get_send_option(SProtocol::TX_Message::UAV_POSITION) != Send_Option::DISABLED)
		{
			m_full_protocol.tx_uav_position(m_uav.get_status().position);
		}
		return false;
	}
	case 36:
	{
		if (get_send_option(SProtocol::TX_Message::UAV_ATTITUDE) != Send_Option::DISABLED)
		{
			m_full_protocol.tx_uav_attitude(m_uav.get_status().attitude.get_euler());
		}
		return false;
	}
		
	//LAST ONE!!! change this as well if needed
	case 100: return true;
	}
	
	return false;
}
