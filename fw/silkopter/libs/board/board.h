#pragma once

#ifdef __AVR__
#	include "Config.h"
#endif

#include "util/chrono.h"

#include "board/IMU.h"
#include "board/Thermometer.h"
#include "board/Barometer.h"
#include "board/Compass.h"
#include "board/Sonar.h"
#include "board/RC_In.h"
#include "board/PWM_Out.h"
#include "board/UART.h"
#include "board/GPS.h"
#include "board/EEPROM.h"


namespace board
{
	namespace clock
	{
		extern void init();
		
		extern chrono::time_ms now_ms();
		extern chrono::time_us now_us();
		
		extern void delay(chrono::micros us);
		extern void delay(chrono::millis ms);
	}
	namespace scheduler
	{
		enum class Frequency
		{
			_250_HZ		= 250,
			_500_HZ		= 500,
			_1000_HZ	= 1000,
			_2000_HZ	= 2000,
		};
		extern Frequency get_frequency();
		extern uint16_t get_frequency_hz();
		
		extern void init(Frequency freq);

		extern void stop();

		static const uint8_t MAX_CALLBACK_COUNT = 4;
		typedef void(*Callback)(void* user_data);
		extern void register_callback(Callback cb, void* user_data = 0);
	}

	//////////////////////////////////////////////////////////////////////////
	
	struct Init_Params
	{
		Init_Params()
			: scheduler_frequency(scheduler::Frequency::_500_HZ)
			, main_imu_idx(0)
			, main_imu_sample_rate(IMU::Sample_Rate::_500_HZ)
			, gs_full_uart_idx(0)
			, gs_full_uart_baud(115200)
			, gs_compact_uart_idx(-1)
			, gs_compact_uart_baud(9600)
			, gps_uart_idx(-1)
			, gps_uart_baud(115200)
			, altitude_sonar_idx(-1)
			, main_thermometer_idx(-1)
			, main_barometer_idx(-1)
			, main_compass_idx(-1)
		{
		}
			
		scheduler::Frequency scheduler_frequency;
		
		//IMU
		uint8_t main_imu_idx; //required!!!
		IMU::Sample_Rate main_imu_sample_rate;
		
		//UART
		uint8_t gs_full_uart_idx; //required!!!
		uint32_t gs_full_uart_baud;
		
		int8_t gs_compact_uart_idx; //-1 if not present
		uint32_t gs_compact_uart_baud;
		
		//GPS
		int8_t gps_uart_idx; //-1 if not present
		uint32_t gps_uart_baud;

		//SONAR		
		int8_t altitude_sonar_idx; //-1 if not present
		
		//THERMOMETER
		int8_t main_thermometer_idx; //-1 if not present
		
		//BAROMETER
		int8_t main_barometer_idx; //-1 if not present
		
		//COMPASS
		int8_t main_compass_idx; //-1 if not present
	};
	extern void init(Init_Params const& params);
	extern Init_Params const& get_init_params();

	//IMU	
	extern uint8_t get_imu_count();
	extern IMU const& get_imu(uint8_t idx);
	extern IMU const& get_main_imu();

	//UART
	extern uint8_t get_uart_count();
	extern UART& get_uart(uint8_t idx);
	extern UART& get_gs_full_uart();
	extern UART* get_gs_compact_uart();
	extern UART* get_gps_uart();

	//GPS	
	extern uint8_t get_gps_count();
	extern GPS const& get_gps(uint8_t idx);
	extern GPS const* get_main_gps();

	//SONAR
	extern uint8_t get_sonar_count();	
	extern Sonar const& get_sonar(uint8_t idx);
	extern Sonar const* get_altitude_sonar();
	
	//THERMOMETER
	extern uint8_t get_thermometer_count();
 	extern Thermometer const& get_thermomether(uint8_t idx);
 	extern Thermometer const* get_main_thermomether();

	//BAROMETER
	extern uint8_t get_barometer_count();
	extern Barometer const& get_barometer(uint8_t idx);
	extern Barometer const* get_main_barometer();

	//COMPASS
	extern uint8_t get_compass_count();
	extern Compass const& get_compass(uint8_t idx);
	extern Compass const* get_main_compass();
	
	//IN/OUT
	extern RC_In const& get_rc_in();
	extern PWM_Out& get_pwm_out();
	
	//eeprom
	extern EEPROM& get_eeprom();

}


