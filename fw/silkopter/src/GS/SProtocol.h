#pragma once

#include <stdint.h>
#include <stddef.h>
#include "util/Noncopyable.h"
#include "qmath.h"
#include "util/FString.h"
#include "UAV.h"

namespace silk
{
	
class SProtocol : public util::Noncopyable
{
public:
	
	//////////////////////////////////////////////////////////////////////////
	//handshake
	
	typedef util::FString<64> Message_String;
	virtual void hello_world(Message_String const& msg, uint16_t version) = 0;
	
	virtual void start_frame() = 0;
	virtual void end_frame() = 0;
	
	//////////////////////////////////////////////////////////////////////////
	//board
	
	virtual void send_board_gyroscope(bool is_valid, math::vec3f const& gyro) = 0;
	virtual void send_board_accelerometer(bool is_valid, math::vec3f const& accel) = 0;
	virtual void send_board_temperature(bool is_valid, float temp) = 0;
	
	virtual void send_board_baro_pressure(bool is_valid, float pressure) = 0;
	virtual void send_board_sonar_altitude(bool is_valid, float altitude) = 0;
	virtual void send_board_gps_altitude(bool is_valid, float altitude) = 0;

	//////////////////////////////////////////////////////////////////////////
	//uav
		
	virtual void send_uav_acceleration(math::vec3f const& accel) = 0;	
	virtual void send_uav_speed(math::vec3f const& accel) = 0;
	virtual void send_uav_position(math::vec3f const& accel) = 0;

	virtual void send_uav_attitude(math::vec3f const& euler) = 0;

	virtual void send_uav_phase(UAV::Phase phase) = 0;
	virtual void send_uav_control_mode(UAV::Control_Mode mode) = 0;
	virtual void send_uav_control_reference_frame(UAV::Control_Reference_Frame frame) = 0;
};
	
}