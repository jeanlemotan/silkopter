#include "GS/GS.h"
#include "board/board.h"
#include "debug/debug.h"

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
	full_uart.set_blocking(true);
	m_full_protocol.send_hello_world(k_hello_world, k_version);
}
GS::GS(UAV& uav, board::UART& full_uart, board::UART& compact_uart)
	: m_uav(uav)
	, m_full_uart(full_uart)
	, m_compact_uart(&compact_uart)
	, m_full_protocol(full_uart)
	//, m_compact_protocol(compact_uart)
	, m_step(0)
{
	full_uart.set_blocking(true);
	compact_uart.set_blocking(true);
	m_full_protocol.send_hello_world(k_hello_world, k_version);
	//m_compact_protocol.hello_world(k_hello_world, k_version);
}

void GS::process(chrono::micros max_duration)
{
	chrono::micros duration;

	//return;
	
	//calculate frame time
	auto start = board::clock::now_us();
	if (start - m_last_time < chrono::micros(10000))
	{
		return;
	}
	m_frame_duration = start - m_last_time;
	m_last_time = start;
	
	if (!m_full_protocol.is_connected())
	{
		if (start - m_last_hello_time > chrono::micros(100000))
		{
			m_last_hello_time = start;
			m_full_protocol.send_hello_world(k_hello_world, k_version);
		}
		return;
	}
	
	if (m_step == 0)
	{
		m_full_protocol.send_start_frame();
	}

	//take into account the eventual start_Frame
	duration += board::clock::now_us() - start;
	
	bool done = false;
	do
	{
		start = board::clock::now_us();
		done = send_data(m_step++);
		duration += board::clock::now_us() - start;
	} while (!done && duration <= max_duration);

	//start over	
	if (done)
	{
		m_full_protocol.send_end_frame();
		m_step = 0;
	}
}

bool GS::send_data(uint32_t step)
{
	switch (step)
	{
	case 5:
	{
		uint8_t cpu_usage = m_frame_duration.count / 5000;
		m_full_protocol.send_board_cpu_usage(cpu_usage);
		return false;
	}
	case 6:
	{
		m_full_protocol.send_board_time_ms(board::clock::now_ms());
		return false;
	}
	case 10:
	case 11:
	case 12:
	case 13:
	{
		//if (step - 10 < board::s_imu_count)
		board::IMU::Accelerometer_Data adata;
		board::IMU::Gyroscope_Data gdata;
		bool is_valid = board::get_main_imu().get_data(gdata, adata);
		m_full_protocol.send_board_gyroscope(is_valid, gdata.delta);
		m_full_protocol.send_board_accelerometer(is_valid, adata.acceleration);
		return false;
	}
	case 18:
	{
// 		board::sonar::Data data;
// 		board::sonar::get_data(data);
// 		m_full_protocol.send_board_sonar_altitude(data.is_valid, data.altitude);
		return false;
	}
	case 20:
	{
// 		board::baro::Data data;
// 		board::baro::get_data(data);
// 		m_full_protocol.send_board_baro_pressure(data.pressure.is_valid, data.pressure.value);
// 		m_full_protocol.send_board_temperature(data.temperature.is_valid, data.temperature.value);
		return false;
	}
	case 22:
	{
//		int16_t data[32];
//		auto count = math::min(board::get_rc_in().get_count(), uint8_t(32));
// 		board::rc_in::get_channels(data, count);
// 		m_full_protocol.send_board_rc_in(count, data);
		return false;
	}
	case 24:
	{
//		int16_t data[32];
//		auto count = math::min(board::get_pwm_out().get_count(), uint8_t(32));
// 		board::pwm_out::get_channels(data, count);
// 		m_full_protocol.send_board_pwm_out(count, data);
		return false;
	}

	case 30:
	{
		m_full_protocol.send_uav_acceleration(m_uav.get_status().acceleration);
		return false;
	}
	case 32:
	{
		m_full_protocol.send_uav_velocity(m_uav.get_status().velocity);
		return false;
	}
	case 34:
	{
		m_full_protocol.send_uav_position(m_uav.get_status().position);
		return false;
	}
		
	//LAST ONE!!! change this as well if needed
	case 100: return true;
	}
	
	return false;
}
