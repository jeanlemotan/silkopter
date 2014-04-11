#if BOARD_TYPE == CRIUS_AIOP_2

#include <avr/interrupt.h>
#include <_qmath.h>

#include "util/chrono.h"
#include "util/storage.h"
#include "util/murmurhash.h"
#include "debug/debug.h"
#include "board/board.h"

#include "board/boards/AVR_gpio.h"
#include "board/boards/AVR_i2c.h"
#include "board/boards/AVR_UART.h"
#include "board/boards/AVR_EEPROM.h"
#include "board/boards/Crius_AIOP2/Barometer_MS5611_i2c.h"
#include "board/boards/Crius_AIOP2/IMU_MPU6000_i2c.h"
#include "board/boards/Crius_AIOP2/Sonar_SR04.h"
#include "board/boards/Crius_AIOP2/Compass_HMC5843_i2c.h"
#include "board/boards/Crius_AIOP2/AIOP_RC_In.h"
#include "board/boards/Crius_AIOP2/AIOP_PWM_Out.h"

using namespace board;

#define UART_COUNT 4

UART_ISR(0);
UART_ISR(1);
UART_ISR(2);
UART_ISR(3);

namespace board
{
	static Init_Params s_init_params;
	
	//////////////////////////////////////////////////////////////////////////
	
	static volatile uint8_t* s_ubrrh[UART_COUNT] = { &UBRR0H, &UBRR1H, &UBRR2H, &UBRR3H };
	static volatile uint8_t* s_ubrrl[UART_COUNT] = { &UBRR0L, &UBRR1L, &UBRR2L, &UBRR3L };
	static volatile uint8_t* s_ucsra[UART_COUNT] = { &UCSR0A, &UCSR1A, &UCSR2A, &UCSR3A };
	static volatile uint8_t* s_ucsrb[UART_COUNT] = { &UCSR0B, &UCSR1B, &UCSR2B, &UCSR3B };
	static volatile uint8_t* s_ucsrc[UART_COUNT] = { &UCSR0C, &UCSR1C, &UCSR2C, &UCSR3C };
	static volatile uint8_t* s_udr[UART_COUNT]	 = { &UDR0, &UDR1, &UDR2, &UDR3 };

	static AVR_UART s_uarts[UART_COUNT] =
	{
		{0, s_ubrrh[0], s_ubrrl[0], s_ucsra[0], s_ucsrb[0], s_ucsrc[0], s_udr[0]},
		{1, s_ubrrh[1], s_ubrrl[1], s_ucsra[1], s_ucsrb[1], s_ucsrc[1], s_udr[1]},
		{2, s_ubrrh[2], s_ubrrl[2], s_ucsra[2], s_ucsrb[2], s_ucsrc[2], s_udr[2]},
		{3, s_ubrrh[3], s_ubrrl[3], s_ucsra[3], s_ucsrb[3], s_ucsrc[3], s_udr[3]},
	};
	
	static AVR_UART* s_gs_full_uart = &s_uarts[0];
	static AVR_UART* s_gs_compact_uart = nullptr;
	static AVR_UART* s_gps_uart = nullptr;
	
	static IMU_MPU6000_i2c s_imu;
 	static AIOP_RC_In s_rc_in;
 	static AIOP_PWM_Out s_pwm_out;
	static Barometer_MS5611_i2c s_barometer;
	static Compass_HMC5843 s_compass;
	static AVR_EEPROM s_eeprom;

	//////////////////////////////////////////////////////////////////////////

	void init(Init_Params const& params)
	{
		s_init_params = params;
		
		scheduler::init();
		i2c::init();
		gpio::init();

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
		
		util::storage::init();
		
		IMU::Calibration_Data data = util::storage::get_record(util::storage::Id(static_murmurhash("imu_calibration_data")), s_imu.get_calibration_data());
		s_imu.set_calibration_data(data);
		
		//s_imu.set_accelerometer_bias_scale(
		//	math::vec3f(2.77505,-3.72759,0.48773),
		//	math::vec3f(7.12171,1.69732,0.96036));
		//s_imu.set_gyroscope_bias(math::vec3f(0.00081f, -0.00008f, 0.00002f));
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
		return 2;
	}
	Thermometer const& get_thermometer(uint8_t idx)
	{
		return idx == 0 ? s_imu.get_thermometer() : s_barometer.get_thermometer();
	}
	Thermometer const* get_main_thermometer()
	{
		if (s_init_params.main_thermometer_idx == 0)
		{
			return &s_imu.get_thermometer();
		}
		else if (s_init_params.main_thermometer_idx == 1)
		{
			return &s_barometer.get_thermometer();
		}
		else
		{
			return nullptr;
		}
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
