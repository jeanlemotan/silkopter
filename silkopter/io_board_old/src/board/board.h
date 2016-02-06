#pragma once

#ifdef __AVR__
#	include "Config.h"
#endif

#include "utils/chrono.h"

#include "board/Accelerometer.h"
#include "board/Gyroscope.h"
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
		extern void init();
		extern void stop();

		extern uint16_t get_frequency();

		static const uint8_t MAX_CALLBACK_COUNT = 8;
		typedef void(*Callback)(void* user_data);
        extern void register_callback(chrono::micros period, Callback cb, void* user_data = 0);
	}

    extern void init();

    struct Config
    {
        uint8_t uart_count = 0;

        bool has_barometer = false;
        bool has_sonar = false;
        bool has_gps = false;
        bool has_voltage_sensor = false;
        bool has_current_sensor = false;
    };

    extern Config const& get_config();

    extern size_t compute_stack_usage();

	//UART
	extern UART& get_uart(uint8_t idx);
    extern UART& get_debug_uart();
    extern UART& get_comm_uart();
	extern UART* get_gps_uart();

    extern Accelerometer& get_accelerometer();
    extern Gyroscope& get_gyroscope();
    extern Compass& get_compass();
    extern Thermometer const& get_thermometer();
    extern Barometer* get_barometer();
    extern RC_In* get_rc_in();
    extern PWM_Out& get_pwm_out();
    extern EEPROM& get_eeprom();
    extern GPS* get_gps();
    extern Sonar* get_sonar();
}


