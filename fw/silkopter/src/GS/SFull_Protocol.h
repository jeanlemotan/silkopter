#pragma once

#include <stdint.h>
#include <stddef.h>
#include <bitset>
#include "GS/SProtocol.h"
#include "board/board.h"
#include "util/Buffer.h"
#include "util/Circular_Buffer.h"
#include "util/crc.h"

namespace silk
{
	
class SFull_Protocol : public SProtocol
{
public:
	SFull_Protocol();
	explicit SFull_Protocol(board::UART& uart);

	void tx_hello_world(const Message_String& msg, uint16_t version);
	bool is_connected() const;
	
	void tx_print(char const* str, size_t size);
	void tx_acknowledge(util::crc_t crc);
	
	void tx_board_cpu_usage(uint8_t cpu_usage_percent);
	void tx_board_time(chrono::time_us time);
	
	void tx_board_gyroscope(bool is_valid, math::vec3f const& gyro);
	void tx_board_accelerometer(bool is_valid, math::vec3f const& accel);
	void tx_board_temperature(bool is_valid, float temp);
	void tx_board_baro_pressure(bool is_valid, float pressure);
	void tx_board_sonar_distance(bool is_valid, float distance);
	void tx_board_gps_altitude(bool is_valid, float altitude);
	void tx_board_rc_in(uint8_t count, int16_t const* values);
	void tx_board_pwm_out(uint8_t count, int16_t const* values);

	void tx_uav_acceleration(math::vec3f const& accel);
	void tx_uav_velocity(math::vec3f const& accel);
	void tx_uav_position(math::vec3f const& accel);

	void tx_uav_attitude(math::vec3f const& euler);

	void tx_uav_phase(UAV::Phase phase);
	void tx_uav_control_mode(UAV::Control_Mode mode);
	void tx_uav_control_reference_frame(UAV::Control_Reference_Frame frame);

	//////////////////////////////////////////////////////////////////////////
	//rx
	
	RX_Message get_next_rx_message();
	void discard_rx_message();
	void decode_board_accelerometer_bias_scale(math::vec3f& bias, math::vec3f& scale);
	void decode_board_gyroscope_bias(math::vec3f& bias);
	void decode_stream_all_messages(bool& enabled);
	void decode_stream_message(TX_Message& msg, bool& enabled);
	void decode_send_all_messages_once();
	void decode_send_message_once(TX_Message& msg);
	
private:
	void start_tx_message(TX_Message msg);
	void flush_tx_message();
	
	struct Header
	{
		RX_Message msg;
		uint8_t size;
		util::crc_t crc;
	};
	
	bool decode_rx_header(bool& needs_more_data);

	board::UART* m_uart;
	
	typedef util::Buffer<uint8_t, 128> TX_Buffer;
	TX_Buffer m_tx_buffer;
	
	typedef util::Circular_Buffer<uint8_t, 128> RX_Buffer;
	RX_Buffer m_rx_buffer;
	Header m_rx_header;
	chrono::time_ms m_last_rx_time;
	
	bool m_is_connected;
};
	
}