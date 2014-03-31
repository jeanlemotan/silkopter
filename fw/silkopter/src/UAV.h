#pragma once

#include "qmath.h"
#include "util/Flag_Set.h"
#include "util/chrono.h"
#include "Motor_Mixer.h"
#include "board/imu.h"
#include "board/sonar.h"
#include "board/baro.h"
#include "AHRS.h"

namespace silk
{

class UAV
{
public:
		
	//how the user inputs control the UAV in manual modes
	enum class Control_Mode : uint8_t
	{
		SILKY,	//uav is silky smooth and stabilized. Neutral controls always result in hovering
		ACRO,	//uav keeps the same attitude until the user counter-compensates
	};

	//the reference frame for the user controls
	enum class Control_Reference_Frame : uint8_t
	{
		UAV, 	//normal mode - back means back of uav
		USER,	//simple mode - back means towards the user, front away from her. Needs GPS
	};

	enum class Phase : uint8_t
	{
		//MOTORS will NEVER spin in these states
		SYSTEMS_CHECK,	//uav will check it's systems and sensors for problems. From here it will go to IDLE
		IDLE,			//uav is idle and waits for commands. From here we can only go to PRE_ARM_CHECKS
		PRE_ARM_CHECKS,	//uav will do a final check for bad sensor data, uneven ground, noise, battery etc. From here it will go to ARMED

		//MOTORS can spin from here on
		ACTIVATE_ROTORS,	//this is here to indicate that from now on, motors can spin. This goes straight to ARMED
		ARMED,				//now the uav can take off. This can go to TAKING_OFF or IDLE after turning the motors OFF
		AUTO_TAKE_OFF,		//the uav is auto-taking off. This can go to AUTO_FLIGHT, or FAILSAFE
		AUTO_FLIGHT,		//uav is executing it's mission and then will go to either MANUAL_FLIGHT, AUTO_LAND or FAILSAFE
		AUTO_LAND,			//uav is executing a landing manuver and will continue with either MANUAL_FLIGHT or FAILSAFE
		MANUAL_FLIGHT,		//the user controls the UAV. This can continue to AUTO_FLIGH, AUTO_LAND or FAILSAFE
		FAILSAFE 			//something bad happened and the quad is executing the failsafe procedure. This can go to MANUAL_FLIGHT only
	};

	//this represents the current status of the uav
	//to manipulate any of these values you have to call the appropiate methods of the current State
	struct Status
	{
		Status() 
			: is_armed(false)
			, is_airborne(false)
		{}

		bool is_armed;	//shortcut to know if the uav is armed. In this mode throttle always results in prop rotation
		bool is_airborne; //shortcut to know if the uav is in the air. When true, is_armed is ALWAYS true

		//this represents the uavs position.
		//x and y are the cartesian coords in a 2d plane with 0, 0 representing the home position
		//z is the altitude with 0 representing the home altitude
		math::vec3f position;
		math::vec3f velocity;
		math::vec3f acceleration;

		AHRS attitude;

		Control_Mode control_mode; //decides how the user actions are translated into uav movement
		Control_Reference_Frame control_reference_frame; //controls are relative to this reference frame
		//AI ai; //assist the user

		Phase phase;
	};

	struct Assists
	{
		Assists()
			: min_ground_distance(1.5f)
			, home_radius(3.f)
			, max_radio_distance(800.f)
			, min_time_to_ground(3.f) 
		{}
		
		enum class Type : uint16_t
		{
			HOLD_ALTITUDE			= 1 << 0,	//maintain constant altitude. Needs baro/GPS
			HOLD_POSITION 			= 1 << 1,	//maintain constant xy position. Needs GPS
			KEEP_IN_RADIO_RANGE 	= 1 << 2,	//avoid out of radio range situations. Needs GPS. Configured by Params::max_radio_distance
			KEEP_IN_BATTERY_RANGE	= 1 << 3, 	//avoid goind too far considering current battery. Needs GPS
			KEEP_IN_PERIMETER		= 1 << 4,	//stay in a configured perimeter. Needs GPS
			AVOID_ALTITUDE_DROP 	= 1 << 5,	//avoid dropping too much altitude too fast. Needs GPS. Controlled by Params::min_time_to_ground parameter.
			AVOID_THE_USER			= 1 << 6, 	//avoid being too low around the home position. Needs sonar and GPS. Controlled by the Params::home_radius
			AVOID_THE_GROUND 		= 1 << 7,	//maintains a min distance from the ground. Needs sonar. Configured by Params::min_ground_distance parameter
		};
		typedef util::Flag_Set<Type, uint16_t> Types;

		Types type;

		//REPEL_GROUND params
		//The uav will try to keep this minimum distance from the ground. 
		float min_ground_distance;

		//AVOID_THE_USER params
		//This feature is meant to protect the user from getting hit by the uav
		//Avoid this radius around the home position
		float home_radius;

		//KEEP_IN_RADIO_RANGE params
		//The maximum allowed radio range, in meters
		float max_radio_distance;

		//AVOID_ALTITUDE_DROP params
		//the minimum time to ground in seconds, calculated from the current altitude and speed with g acceleration
		//A uav hovering at 100m will take ~4.5 seconds to hit the ground at terminal speed of 44.2m/s
		float min_time_to_ground;

		//KEEP_IN_PERIMETER
		//a list of points that define an enclosed perimeter. The last point is connected to the first point
		//std::array<math::vec2f> perimeter_points;
	};

	//This controls all aspects of the UAV AI.
	struct Failsafes
	{
		Failsafes()
		{
			radio_params.behavior = Behavior::RETURN_HOME;
			battery_params.behavior = Behavior::LAND;
			panic_params.behavior = Behavior::HOLD_POSITION;
			other_params.behavior = Behavior::RETURN_HOME;
		}
		
		enum class Behavior : uint8_t
		{
			HOLD_ALTITUDE,
			HOLD_POSITION,
			RETURN_HOME,
			LAND
		};

		struct Behavior_Params
		{
			Behavior_Params()
				: return_home_speed(2.f)
				, descent_speed(2.f)
			{}

			Behavior behavior;

			//m/s
			//what speed to use when returning home. For radio failsafes you might want a higher speed than a battery failsafe
			float return_home_speed;

			//m/s
			//what speed to use when landing. For battery failsafes you might want a faster descent
			float descent_speed;
		};

		Behavior_Params radio_params;
		Behavior_Params battery_params;
		Behavior_Params panic_params;
		Behavior_Params other_params;
	};


	UAV(Motor_Mixer::Type type, uint8_t motor_count, float radius);

	void set_assists(Assists const& assists);
	void set_failsafes(Failsafes const& failsafes);

	void set_control_reference_frame(Control_Reference_Frame frame);
	void set_control_mode(Control_Mode mode);

	//current status and parameters
	const Status& get_status();

	//this has to be called periodically - as often as possible. Minimum is 100hz
	void process();
	
private:
	float m_radius;
	Motor_Mixer m_motor_mixer;
	Assists m_assists;
	Failsafes m_failsafes;
	Control_Reference_Frame m_control_frame_reference;
	Control_Mode m_control_mode;
	Status m_status;
	
	chrono::time_us m_last_time;
	chrono::micros m_dt;
	float m_dts;
	
	void read_imu_data();
	void read_sonar_data();
	void read_gps_data();
	void read_baro_data();
	void read_compass_data();

	void compute_linear_motion();
	
	board::imu::Data m_imu_data;
	//board::gps::Data m_gps_data;
	board::sonar::Data m_sonar_data;
	board::baro::Data m_baro_data;
	//board::compass::Data m_compass_data;
};

};



