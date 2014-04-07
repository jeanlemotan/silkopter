#pragma once

#include <stdint.h>
#include <stddef.h>
#include "util/Noncopyable.h"
#include "_qmath.h"
#include "util/FString.h"
#include "util/chrono.h"
#include "UAV.h"

namespace silk
{
	
class SProtocol : public util::Noncopyable
{
public:

	//each has a corresponsing send method
	enum class TX_Message
	{
		HELLO_WORLD = 253,
		ACKNOWLEDGE = 254,
		PRINT = 255,

		//////////////////////////////////////////////////////////////////////////
		//BOARD

		BOARD_CPU_USAGE = 0,
		BOARD_TIME = 1,
		BOARD_GYROSCOPE = 10,
		BOARD_ACCELEROMETER = 11,
		BOARD_TEMPERATURE = 12,
		BOARD_BARO_PRESSURE = 13,
		BOARD_SONAR_DISTANCE = 14,
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
	enum class RX_Message
	{
		NONE,
		
		//control messages
		SET_MESSAGE_ENABLED,
		SET_ALL_MESSAGES_ENABLED,
		
		//calibration
		SET_BOARD_ACCELEROMETER_BIAS_SCALE,
		SET_BOARD_GYROSCOPE_BIAS,
	};

	
	//////////////////////////////////////////////////////////////////////////
	//handshake
	
	typedef util::String<64> Message_String;
	virtual void tx_hello_world(Message_String const& msg, uint16_t version) = 0;
	virtual bool is_connected() const = 0;
	
	virtual bool is_tx_message_enabled(TX_Message msg) const = 0;
	
	virtual void tx_print(char const* str, size_t size) = 0;
	
	template<class Fmt, typename... Params>
	void tx_print(Fmt const& fmt, Params... params)
	{
		util::String<255> str;
		util::format(str, fmt, params...);
		tx_print(str.m_data(), str.size());
	}
	
	//////////////////////////////////////////////////////////////////////////
	//commands
	
	virtual RX_Message get_next_rx_message() = 0;
	virtual void rx_board_accelerometer_bias_scale(math::vec3f& bias, math::vec3f& scale) const = 0;
	virtual void rx_board_gyroscope_bias(math::vec3f& bias) const = 0;

	//////////////////////////////////////////////////////////////////////////
	
	virtual void tx_board_cpu_usage(uint8_t cpu_usage_percent) = 0;
	virtual void tx_board_time(chrono::time_us time) = 0;
	
	//////////////////////////////////////////////////////////////////////////
	//board
	
	virtual void tx_board_gyroscope(bool is_valid, math::vec3f const& gyro) = 0;
	virtual void tx_board_accelerometer(bool is_valid, math::vec3f const& accel) = 0;
	virtual void tx_board_temperature(bool is_valid, float temp) = 0;
	
	virtual void tx_board_baro_pressure(bool is_valid, float pressure) = 0;
	virtual void tx_board_sonar_distance(bool is_valid, float distance) = 0;
	virtual void tx_board_gps_altitude(bool is_valid, float altitude) = 0;

	virtual void tx_board_rc_in(uint8_t count, int16_t const* values) = 0;
	virtual void tx_board_pwm_out(uint8_t count, int16_t const* values) = 0;

	//////////////////////////////////////////////////////////////////////////
	//uav
		
	virtual void tx_uav_acceleration(math::vec3f const& accel) = 0;	
	virtual void tx_uav_velocity(math::vec3f const& accel) = 0;
	virtual void tx_uav_position(math::vec3f const& accel) = 0;

	virtual void tx_uav_attitude(math::vec3f const& euler) = 0;

	virtual void tx_uav_phase(UAV::Phase phase) = 0;
	virtual void tx_uav_control_mode(UAV::Control_Mode mode) = 0;
	virtual void tx_uav_control_reference_frame(UAV::Control_Reference_Frame frame) = 0;
};
	
}