#include "Config.h"

#if BOARD_TYPE == BOARD_CRIUS_AIOP_2

#include <avr/interrupt.h>
#include <_qmath.h>

#include "QBase.h"
#include "utils/chrono.h"
#include "utils/storage.h"
#include "board/board.h"

#include "board/boards/AVR_gpio.h"
#include "board/boards/AVR_i2c.h"
#include "board/boards/AVR_UART.h"
#include "board/boards/AVR_EEPROM.h"
#include "board/boards/Crius_AIOP/Barometer_MS5611_i2c.h"
#include "board/boards/Crius_AIOP/IMU_MPU6000_i2c.h"
#include "board/boards/Crius_AIOP/Sonar_SR04.h"
#include "board/boards/Crius_AIOP/Compass_HMC5843_5883L_i2c.h"
#include "board/boards/Crius_AIOP/AIOP_RC_In.h"
#include "board/boards/Crius_AIOP/AIOP_PWM_Out.h"
#include "new"

using namespace board;

#define UART_COUNT 4

UART_ISR(0);
UART_ISR(1);
UART_ISR(2);
UART_ISR(3);

namespace board
{
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
	
    static Config s_config;

    static AVR_UART* s_debug_uart = &s_uarts[0];
    static AVR_UART* s_comm_uart = &s_uarts[2];
	static AVR_UART* s_gps_uart = nullptr;
	
	static IMU_MPU6000_i2c s_imu;
 	static AIOP_RC_In s_rc_in;
 	static AIOP_PWM_Out s_pwm_out;
	static Barometer_MS5611_i2c s_barometer;
	static Compass_HMC5843_5883L_i2c s_compass;
	static AVR_EEPROM s_eeprom;
    static Sonar* s_sonar = nullptr;
	//////////////////////////////////////////////////////////////////////////
    static int _uart_putchar(char c, FILE* stream);

    static FILE _mystdout, _mystderr;

    static int _uart_putchar(char c, FILE* stream)
    {
		UART* uart = reinterpret_cast<UART*>(fdev_get_udata(stream));
		if (uart)
		{
			uart->write(c);
		}
	    return 0;
    }


    void init()
	{
		scheduler::init();
		i2c::init();
		gpio::init();

        s_config.uart_count = UART_COUNT;

        {
            uint8_t uart_idx = 0;//util::storage::get_record(util::storage::Id(static_murmurhash("debug_uart_idx")), uint8_t(0));
            uint32_t uart_baud = 115200;//util::storage::get_record(util::storage::Id(static_murmurhash("debug_uart_baud")), uint32_t(115200));
            QASSERT(uart_idx >= 0 && uart_idx < UART_COUNT);
            s_debug_uart = &s_uarts[uart_idx];
            s_debug_uart->begin(uart_baud, 32, 16);

            fdev_setup_stream(&_mystdout, _uart_putchar, NULL, _FDEV_SETUP_WRITE);
            fdev_setup_stream(&_mystderr, _uart_putchar, NULL, _FDEV_SETUP_WRITE);
            fdev_set_udata(&_mystdout, s_debug_uart);
            fdev_set_udata(&_mystderr, s_debug_uart);
            stdout = &_mystdout;
            stderr = &_mystderr;
        }

        util::storage::init();

        q::quick_logf(F_STR("...Crius AIOP Board..."));

        {
            uint8_t uart_idx = util::storage::get_record(util::storage::Id(static_murmurhash("comm_uart_idx")), uint8_t(3));
            uint32_t uart_baud = 500000;//util::storage::get_record(util::storage::Id(static_murmurhash("comm_uart_baud")), uint32_t(115200));
            QASSERT(uart_idx >= 0 && uart_idx < UART_COUNT);
            s_comm_uart = &s_uarts[uart_idx];
            s_comm_uart->begin(uart_baud, 128, 256);
        }

        {
            uint8_t uart_idx = util::storage::get_record(util::storage::Id(static_murmurhash("gps_uart_idx")), uint8_t(2));
            uint32_t uart_baud = util::storage::get_record(util::storage::Id(static_murmurhash("gps_uart_baud")), uint32_t(115200));
            if (uart_idx >= 0 && uart_idx < UART_COUNT)
            {
                s_gps_uart = &s_uarts[uart_idx];
                s_gps_uart->begin(uart_baud, 16, 64);
            }
        }

        {
            auto rate = util::storage::get_record(util::storage::Id(static_murmurhash("imu_sample_rate")), IMU_MPU6000_i2c::Sample_Rate::_500_HZ);
            s_imu.init(rate);
        }
		
		s_rc_in.init();
		s_pwm_out.init();

        s_config.has_barometer = true;
        s_barometer.init();

        s_compass.init();

        enum class Sonar_Type
        {
            NONE  = 0,
            SR04  = 1,
        };

        {
            auto sonar_type = util::storage::get_record(util::storage::Id(static_murmurhash("sonar_type")), Sonar_Type::NONE);
            sonar_type = Sonar_Type::SR04;
            if (sonar_type == Sonar_Type::SR04)
            {
                s_sonar = new Sonar_SR04();
                s_sonar->start();
            }
            else if (sonar_type != Sonar_Type::NONE)
            {
                PANIC();
            }
        }

        s_config.has_sonar = s_sonar != nullptr;

        q::quick_logf(F_STR("Stack usage: {}"), compute_stack_usage());
	}
    Config const& get_config()
	{
        return s_config;
	}

    Accelerometer& get_accelerometer()
    {
        return s_imu.get_accelerometer();
    }

    Gyroscope& get_gyroscope()
    {
        return s_imu.get_gyroscope();
    }

    Compass& get_compass()
    {
        return s_compass;
    }

    Thermometer const& get_thermometer()
    {
        return s_imu.get_thermometer();
    }

    Barometer* get_barometer()
    {
        return &s_barometer;
    }

    RC_In* get_rc_in()
    {
        return &s_rc_in;
    }

    PWM_Out& get_pwm_out()
    {
        return s_pwm_out;
    }

    EEPROM& get_eeprom()
    {
        return s_eeprom;
    }

    GPS* get_gps()
    {
        return nullptr;
    }

    Sonar* get_sonar()
    {
        return s_config.has_sonar ? s_sonar : nullptr;
    }

	//UART
	uint8_t get_uart_count()
	{
		return UART_COUNT;
	}
	UART& get_uart(uint8_t idx)
	{
		QASSERT(idx < UART_COUNT);
		return s_uarts[idx];
	}
    UART& get_debug_uart()
	{
        return *s_debug_uart;
	}
    UART& get_comm_uart()
	{
        return *s_comm_uart;
	}
	UART* get_gps_uart()
	{
		return s_gps_uart;		
	}

    size_t compute_stack_usage()
    {
        return 0;
    }

}

#endif
