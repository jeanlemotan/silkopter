#pragma once

#include <stdint.h>
#include <stddef.h>
#include <bitset>
#include "GS/SProtocol.h"
#include "board/UART.h"
#include "util/CRC_Buffer.h"

namespace silk
{
	
class SFull_Protocol : public SProtocol
{
public:
	SFull_Protocol();
	explicit SFull_Protocol(board::UART& uart);

	void send_hello_world(const Message_String& msg, uint16_t version);
	bool is_connected() const;
	
	void send_start_frame();
	void send_end_frame();
	
	bool is_message_enabled(Message msg) const;
	
	void send_board_cpu_usage(uint8_t cpu_usage_percent);
	void send_board_time_ms(chrono::time_ms time);
	
	void send_board_gyroscope(bool is_valid, math::vec3f const& gyro);
	void send_board_accelerometer(bool is_valid, math::vec3f const& accel);
	void send_board_temperature(bool is_valid, float temp);
	void send_board_baro_pressure(bool is_valid, float pressure);
	void send_board_sonar_altitude(bool is_valid, float altitude);
	void send_board_gps_altitude(bool is_valid, float altitude);
	void send_board_rc_in(uint8_t count, int16_t const* values);
	void send_board_pwm_out(uint8_t count, int16_t const* values);

	void send_uav_acceleration(math::vec3f const& accel);
	void send_uav_velocity(math::vec3f const& accel);
	void send_uav_position(math::vec3f const& accel);

	void send_uav_attitude(math::vec3f const& euler);

	void send_uav_phase(UAV::Phase phase);
	void send_uav_control_mode(UAV::Control_Mode mode);
	void send_uav_control_reference_frame(UAV::Control_Reference_Frame frame);
	
private:
	bool start_message(Message msg);
	void flush_message();

	board::UART* m_uart;
	util::CRC_Buffer<255> m_buffer;
	uint32_t m_last_frame_idx;
	bool m_is_frame_started;
	bool m_is_connected;
	
	std::bitset<256> m_enabled_messages;
};
	
}