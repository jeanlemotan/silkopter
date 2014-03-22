#pragma once

#include <stdint.h>
#include <stddef.h>
#include "GS/SProtocol.h"
#include "board/UART.h"

namespace silk
{
	
class SCompact_Protocol : public SProtocol
{
public:
	SCompact_Protocol();
	explicit SCompact_Protocol(board::UART& uart);

	void hello_world(Message_String const& msg);
		
	void start_frame();
	void end_frame();
		
	void send_board_gyroscope(math::vec3f const& gyro);
	void send_board_accelerometer(math::vec3f const& accel);
	void send_board_temperature(float temp);
		
	void send_board_baro_altitude(float altitude);
	void send_board_sonar_altitude(float altitude);
	void send_board_gps_altitude(float altitude);

	void send_uav_acceleration(math::vec3f const& accel);
	void send_uav_speed(math::vec3f const& accel);
	void send_uav_position(math::vec3f const& accel);

	void send_uav_attitude(math::vec3f const& euler);

	void send_uav_phase(UAV::Phase phase);
	void send_uav_control_mode(UAV::Control_Mode mode);
	void send_uav_control_reference_frame(UAV::Control_Reference_Frame frame);
		
private:
	board::UART* m_uart;
	size_t m_last_frame_idx;
	bool m_is_frame_started;
};
	
}