#if BOARD_TYPE == SIM

#include <_qmath.h>

#include "util/chrono.h"
#include "debug/debug.h"
#include "board/board.h"

#include "board/boards/Sim/Sim_UART.h"
#include "board/boards/Sim/Sim_EEPROM.h"
#include "board/boards/Sim/Sim_Barometer.h"
#include "board/boards/Sim/Sim_IMU.h"
#include "board/boards/Sim/Sim_Sonar.h"
#include "board/boards/Sim/Sim_Compass.h"
#include "board/boards/Sim/Sim_RC_In.h"
#include "board/boards/Sim/Sim_PWM_Out.h"

namespace board
{
	static Init_Params s_init_params;

#define UART_COUNT 1
	
	//////////////////////////////////////////////////////////////////////////
	
	static Sim_UART s_uarts[UART_COUNT] =
	{
		{0},
	};
	
	static Sim_UART* s_gs_full_uart = &s_uarts[0];
	static Sim_UART* s_gs_compact_uart = nullptr;
	static Sim_UART* s_gps_uart = nullptr;
	
	static Sim_IMU s_imu;
 	static Sim_RC_In s_rc_in;
 	static Sim_PWM_Out s_pwm_out;
	static Sim_Barometer s_barometer;
	static Sim_Compass s_compass;
	static Sim_EEPROM s_eeprom("eeprom");

	//////////////////////////////////////////////////////////////////////////

	void init(Init_Params const& params)
	{
		s_init_params = params;
		
		scheduler::init(s_init_params.scheduler_frequency);

		//initialize debug first to get asserts early
		s_uarts[0].begin(115200);
		debug::init(&s_uarts[0]);
		
		if (s_init_params.gs_full_uart_idx >= UART_COUNT)
		{
			PANIC_MSG("UART out of bounds");
		}
		s_gs_full_uart = &s_uarts[s_init_params.gs_full_uart_idx];
		s_gs_full_uart->begin(s_init_params.gs_full_uart_baud);
		
		if (s_init_params.gs_compact_uart_idx >= 0 && s_init_params.gs_compact_uart_idx < UART_COUNT)
		{
			s_gs_compact_uart = &s_uarts[s_init_params.gs_compact_uart_idx];
			s_gs_compact_uart->begin(s_init_params.gs_compact_uart_baud);
		}
		if (s_init_params.gps_uart_idx >= 0 && s_init_params.gps_uart_idx < UART_COUNT)
		{
			s_gps_uart = &s_uarts[s_init_params.gps_uart_idx];
			s_gps_uart->begin(s_init_params.gps_uart_baud);
		}
		
		s_init_params.main_imu_idx = 0;		
		s_imu.init(s_init_params.main_imu_sample_rate);
		
		s_rc_in.init();
		s_pwm_out.init();
		s_barometer.init();
		//s_compass.init();

		s_init_params.main_thermometer_idx = math::clamp<int8_t>(s_init_params.main_thermometer_idx, 0, 2);
		
		//rc_in::init();
		
		//pwm_out::init();
		//pwm_out::set_frequencies(50);
		
		s_imu.set_accelerometer_bias_scale(
			math::vec3f(2.77505,-3.72759,0.48773),
			math::vec3f(7.12171,1.69732,0.96036));
		s_imu.set_gyroscope_bias(math::vec3f(0.00081f, -0.00008f, 0.00002f));
	}
	Init_Params const& get_init_params()
	{
		return s_init_params;
	}

	//IMU
	uint8_t get_imu_count()
	{
		return 1;
	}
	IMU& get_imu(uint8_t idx)
	{
		return s_imu;
	}
	IMU& get_main_imu()
	{
		return s_imu;
	}

	//UART
	uint8_t get_uart_count()
	{
		return UART_COUNT;
	}
	UART& get_uart(uint8_t idx)
	{
		ASSERT(idx < UART_COUNT);
		return s_uarts[idx];
	}
	UART& get_gs_full_uart()
	{
		return *s_gs_full_uart;
	}
	UART* get_gs_compact_uart()
	{
		return s_gs_compact_uart;
	}
	UART* get_gps_uart()
	{
		return s_gps_uart;		
	}

	//GPS
	uint8_t get_gps_count()
	{
		return 0;
	}
	GPS const& get_gps(uint8_t idx)
	{
		return *get_main_gps();
	}
	GPS const* get_main_gps()
	{
		return nullptr;
	}

	//SONAR
	uint8_t get_sonar_count()
	{
		return 0;
	}
	Sonar const& get_sonar(uint8_t idx)
	{
		return *get_altitude_sonar();
	}
	Sonar const* get_altitude_sonar()
	{
		return nullptr;
	}
	
	//THERMOMETER
	uint8_t get_thermometer_count()
	{
		return 0;
	}
// 	Thermometer const& get_thermometer(uint8_t idx)
// 	{
// 		return *nullptr;
// 	}
	Thermometer const* get_main_thermometer()
	{
		return nullptr;
	}

	//BAROMETER
	uint8_t get_barometer_count()
	{
		return 1;
	}
	Barometer const& get_barometer(uint8_t idx)
	{
		return s_barometer;	
	}
	Barometer const* get_main_barometer()
	{
		if (s_init_params.main_barometer_idx == 0)
		{
			return &s_barometer;
		}
		else
		{
			return nullptr;
		}
	}

	//COMPASS
	uint8_t get_compass_count()
	{
		return 1;
	}
	Compass const& get_compass(uint8_t idx)
	{
		return s_compass;
	}
	Compass const* get_main_compass()
	{
		if (s_init_params.main_compass_idx == 0)
		{
			return &s_compass;
		}
		else
		{
			return nullptr;
		}
	}
	
	RC_In const& get_rc_in()
	{
		return s_rc_in;
	}
	PWM_Out& get_pwm_out()
	{
		return s_pwm_out;
	}

	EEPROM& get_eeprom()
	{
		return s_eeprom;
	}

}

#endif
