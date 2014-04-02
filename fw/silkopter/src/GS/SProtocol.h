#pragma once

#include <stdint.h>
#include <stddef.h>
#include "util/Noncopyable.h"
#include "qmath.h"
#include "util/FString.h"
#include "util/chrono.h"
#include "UAV.h"

namespace silk
{
	
class SProtocol : public util::Noncopyable
{
public:

	//each has a corresponsing send method
	enum class Message
	{
		HELLO_WORLD = 253,
		START_FRAME = 254,
		END_FRAME = 255,

		//////////////////////////////////////////////////////////////////////////
		//BOARD

		BOARD_CPU_USAGE = 0,
		BOARD_TIME_MS = 1,
		BOARD_GYROSCOPE = 10,
		BOARD_ACCELEROMETER = 11,
		BOARD_TEMPERATURE = 12,
		BOARD_BARO_PRESSURE = 13,
		BOARD_SONAR_ALTITUDE = 14,
		BOARD_GPS_ALTITUDE = 15,
		BOARD_RC_IN = 16,
		BOARD_PWM_OUT = 17,

		//////////////////////////////////////////////////////////////////////////
		//UAV

		UAV_ACCELERATION = 30,
		UAV_VELOCITY = 31,
		UAV_POSITION = 32,
		UAV_ATTITUDE = 33,
		UAV_PHASE = 34,
		UAV_CONTROL_MODE = 35,
		UAV_CONTROL_REFERENCE_FRAME = 36,
	};
	
	//each has a corresponding receive method
	enum class Command
	{
		//control messages
		SET_MESSAGE_ENABLED,
		SET_ALL_MESSAGES_ENABLED,
		
		//calibration
		SET_BOARD_ACCELEROMETER_BIAS_SCALE,
		SET_BOARD_GYROSCOPE_BIAS_SCALE,
	};

	
	//////////////////////////////////////////////////////////////////////////
	//handshake
	
	typedef util::String<64> Message_String;
	virtual void send_hello_world(Message_String const& msg, uint16_t version) = 0;
	virtual bool is_connected() const = 0;
	
	virtual void send_start_frame() = 0;
	virtual void send_end_frame() = 0;
	
	virtual bool is_message_enabled(Message msg) const = 0;

	//////////////////////////////////////////////////////////////////////////
	
	virtual void send_board_cpu_usage(uint8_t cpu_usage_percent) = 0;
	virtual void send_board_time_ms(chrono::time_ms time) = 0;
	
	//////////////////////////////////////////////////////////////////////////
	//board
	
	virtual void send_board_gyroscope(bool is_valid, math::vec3f const& gyro) = 0;
	virtual void send_board_accelerometer(bool is_valid, math::vec3f const& accel) = 0;
	virtual void send_board_temperature(bool is_valid, float temp) = 0;
	
	virtual void send_board_baro_pressure(bool is_valid, float pressure) = 0;
	virtual void send_board_sonar_altitude(bool is_valid, float altitude) = 0;
	virtual void send_board_gps_altitude(bool is_valid, float altitude) = 0;

	virtual void send_board_rc_in(uint8_t count, int16_t const* values) = 0;
	virtual void send_board_pwm_out(uint8_t count, int16_t const* values) = 0;

	//////////////////////////////////////////////////////////////////////////
	//uav
		
	virtual void send_uav_acceleration(math::vec3f const& accel) = 0;	
	virtual void send_uav_velocity(math::vec3f const& accel) = 0;
	virtual void send_uav_position(math::vec3f const& accel) = 0;

	virtual void send_uav_attitude(math::vec3f const& euler) = 0;

	virtual void send_uav_phase(UAV::Phase phase) = 0;
	virtual void send_uav_control_mode(UAV::Control_Mode mode) = 0;
	virtual void send_uav_control_reference_frame(UAV::Control_Reference_Frame frame) = 0;
};
	
}