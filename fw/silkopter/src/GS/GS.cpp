#include "GS/GS.h"
#include "board/clock.h"
#include "board/imu.h"
#include "board/sonar.h"
#include "board/baro.h"
#include "board/rc_in.h"
#include "board/pwm_out.h"
#include "debug/debug.h"

using namespace silk;

static const uint16_t k_version = 100;
static const SProtocol::Message_String k_hello_world("Silkopter");


GS::GS(board::UART& full_uart)
	: m_full_uart(full_uart)
	, m_compact_uart(nullptr)
	, m_full_protocol(full_uart)
	, m_step(0)
{
	full_uart.set_blocking(true);
	m_full_protocol.hello_world(k_hello_world, k_version);
}
GS::GS(board::UART& full_uart, board::UART& compact_uart)
	: m_full_uart(full_uart)
	, m_compact_uart(&compact_uart)
	, m_full_protocol(full_uart)
	, m_compact_protocol(compact_uart)
	, m_step(0)
{
	full_uart.set_blocking(true);
	compact_uart.set_blocking(true);
	m_full_protocol.hello_world(k_hello_world, k_version);
	m_compact_protocol.hello_world(k_hello_world, k_version);
}

void GS::process(chrono::micros max_duration)
{
	chrono::micros duration;

	//return;
	
	//calculate frame time
	auto start = board::clock::now_us();
	m_frame_duration = start - m_last_time;
	m_last_time = start;
	
	if (!m_full_protocol.is_connected())
	{
		if (start - m_last_hello_time > chrono::micros(100000))
		{
			m_last_hello_time = start;
			m_full_protocol.hello_world(k_hello_world, k_version);
		}
		return;
	}
	
	if (m_step == 0)
	{
		m_full_protocol.start_frame();
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
		m_full_protocol.end_frame();
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
		m_compact_protocol.send_board_cpu_usage(cpu_usage);
		return false;
	}
	case 6:
	{
		m_full_protocol.send_board_time_ms(board::clock::now_ms());
		m_compact_protocol.send_board_time_ms(board::clock::now_ms());
		return false;
	}
	case 10:
	{
		board::imu::Data data;
		board::imu::get_data(data);
		m_full_protocol.send_board_gyroscope(data.gyroscope.is_valid, data.gyroscope.value);
		m_compact_protocol.send_board_gyroscope(data.gyroscope.is_valid, data.gyroscope.value);
		m_full_protocol.send_board_accelerometer(data.accelerometer.is_valid, data.accelerometer.value);
		m_compact_protocol.send_board_accelerometer(data.accelerometer.is_valid, data.accelerometer.value);
		return false;
	}
	case 12:
	{
		board::sonar::Data data;
		board::sonar::get_data(data);
		m_full_protocol.send_board_sonar_altitude(data.is_valid, data.altitude);
		m_compact_protocol.send_board_sonar_altitude(data.is_valid, data.altitude);
		return false;
	}
	case 14:
	{
		board::baro::Data data;
		board::baro::get_data(data);
		m_full_protocol.send_board_baro_pressure(data.pressure.is_valid, data.pressure.value);
		m_compact_protocol.send_board_baro_pressure(data.pressure.is_valid, data.pressure.value);
		m_full_protocol.send_board_temperature(data.temperature.is_valid, data.temperature.value);
		m_compact_protocol.send_board_temperature(data.temperature.is_valid, data.temperature.value);
		return false;
	}
	case 16:
	{
		int16_t data[32];
		auto count = math::min(board::rc_in::get_channel_count(), uint8_t(32));
		board::rc_in::get_channels(data, count);
		m_full_protocol.send_board_rc_in(count, data);
		m_compact_protocol.send_board_rc_in(count, data);
		return false;
	}
	case 18:
	{
		int16_t data[32];
		auto count = math::min(board::pwm_out::get_channel_count(), uint8_t(32));
		board::pwm_out::get_channels(data, count);
		m_full_protocol.send_board_pwm_out(count, data);
		m_compact_protocol.send_board_pwm_out(count, data);
		return false;
	}
		
	//LAST ONE!!! change this as well if needed
	case 100: return true;
	}
	
	return false;
}
