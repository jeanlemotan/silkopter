#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP2

#include "board/boards/Crius_AIOP2/IMU_MPU6000_i2c.h"
#include "debug/debug.h"
#include "util/format.h"
#include "board/boards/avr_gpio.h"
#include "board/boards/avr_i2c.h"
#include "board/board.h"
#include "physics/constants.h"

namespace board
{

//#define DISABLE_INTERNAL_MAG

// MPU 6000 I2C Address
#define MPU6000_ADDR 0x68

// MPU 6000 registers
#define MPUREG_XG_OFFS_TC                               0x00
#define MPUREG_YG_OFFS_TC                               0x01
#define MPUREG_ZG_OFFS_TC                               0x02
#define MPUREG_X_FINE_GAIN                              0x03
#define MPUREG_Y_FINE_GAIN                              0x04
#define MPUREG_Z_FINE_GAIN                              0x05
#define MPUREG_XA_OFFS_H                                0x06    // X axis accelerometer offset (high byte)
#define MPUREG_XA_OFFS_L                                0x07    // X axis accelerometer offset (low byte)
#define MPUREG_YA_OFFS_H                                0x08    // Y axis accelerometer offset (high byte)
#define MPUREG_YA_OFFS_L                                0x09    // Y axis accelerometer offset (low byte)
#define MPUREG_ZA_OFFS_H                                0x0A    // Z axis accelerometer offset (high byte)
#define MPUREG_ZA_OFFS_L                                0x0B    // Z axis accelerometer offset (low byte)
#define MPUREG_PRODUCT_ID                               0x0C    // Product ID Register
#define MPUREG_XG_OFFS_USRH                     0x13    // X axis gyro offset (high byte)
#define MPUREG_XG_OFFS_USRL                     0x14    // X axis gyro offset (low byte)
#define MPUREG_YG_OFFS_USRH                     0x15    // Y axis gyro offset (high byte)
#define MPUREG_YG_OFFS_USRL                     0x16    // Y axis gyro offset (low byte)
#define MPUREG_ZG_OFFS_USRH                     0x17    // Z axis gyro offset (high byte)
#define MPUREG_ZG_OFFS_USRL                     0x18    // Z axis gyro offset (low byte)
#define MPUREG_SMPLRT_DIV                               0x19    // sample rate.  Fsample= 1Khz/(<this value>+1) = 200Hz
	#define MPUREG_SMPLRT_1000HZ                             0x00
	#define MPUREG_SMPLRT_500HZ                              0x01
	#define MPUREG_SMPLRT_250HZ                              0x03
	#define MPUREG_SMPLRT_200HZ                              0x04
	#define MPUREG_SMPLRT_100HZ                              0x09
	#define MPUREG_SMPLRT_50HZ                               0x13
#define MPUREG_CONFIG                                           0x1A
#define MPUREG_GYRO_CONFIG 0x1B
// bit definitions for MPUREG_GYRO_CONFIG
#       define BITS_GYRO_FS_250DPS                              0x00
#       define BITS_GYRO_FS_500DPS                              0x08
#       define BITS_GYRO_FS_1000DPS                             0x10
#       define BITS_GYRO_FS_2000DPS                             0x18
#       define BITS_GYRO_FS_MASK                                0x18    // only bits 3 and 4 are used for gyro full scale so use this to mask off other bits
#       define BITS_GYRO_ZGYRO_SELFTEST                 0x20
#       define BITS_GYRO_YGYRO_SELFTEST                 0x40
#       define BITS_GYRO_XGYRO_SELFTEST                 0x80
#define MPUREG_ACCEL_CONFIG 0x1C
#define MPUREG_MOT_THR                                  0x1F    // detection threshold for Motion interrupt generation.  Motion is detected when the absolute value of any of the accelerometer measurements exceeds this
#define MPUREG_MOT_DUR                                  0x20    // duration counter threshold for Motion interrupt generation. The duration counter ticks at 1 kHz, therefore MOT_DUR has a unit of 1 LSB = 1 ms
#define MPUREG_ZRMOT_THR                                0x21    // detection threshold for Zero Motion interrupt generation.
#define MPUREG_ZRMOT_DUR                                0x22    // duration counter threshold for Zero Motion interrupt generation. The duration counter ticks at 16 Hz, therefore ZRMOT_DUR has a unit of 1 LSB = 64 ms.
#define MPUREG_FIFO_EN 0x23
#define MPUREG_INT_PIN_CFG 0x37
#       define BIT_I2C_BYPASS_EN                                0x02 
#       define BIT_INT_RD_CLEAR                                 0x10    // clear the interrupt when any read occurs
#       define BIT_LATCH_INT_EN                                 0x20    // latch data ready pin 
#define MPUREG_INT_ENABLE 0x38
// bit definitions for MPUREG_INT_ENABLE
#       define BIT_RAW_RDY_EN                                   0x01
#       define BIT_DMP_INT_EN                                   0x02    // enabling this bit (DMP_INT_EN) also enables RAW_RDY_EN it seems
#       define BIT_UNKNOWN_INT_EN                               0x04
#       define BIT_I2C_MST_INT_EN                               0x08
#       define BIT_FIFO_OFLOW_EN                                0x10
#       define BIT_ZMOT_EN                                              0x20
#       define BIT_MOT_EN                                               0x40
#       define BIT_FF_EN                                                0x80
#define MPUREG_INT_STATUS 0x3A
// bit definitions for MPUREG_INT_STATUS (same bit pattern as above because this register shows what interrupt actually fired)
#       define BIT_RAW_RDY_INT                                  0x01
#       define BIT_DMP_INT                                              0x02
#       define BIT_UNKNOWN_INT                                  0x04
#       define BIT_I2C_MST_INT                                  0x08
#       define BIT_FIFO_OFLOW_INT                               0x10
#       define BIT_ZMOT_INT                                             0x20
#       define BIT_MOT_INT                                              0x40
#       define BIT_FF_INT                                               0x80
#define MPUREG_ACCEL_XOUT_H                             0x3B
#define MPUREG_ACCEL_XOUT_L                             0x3C
#define MPUREG_ACCEL_YOUT_H                             0x3D
#define MPUREG_ACCEL_YOUT_L                             0x3E
#define MPUREG_ACCEL_ZOUT_H                             0x3F
#define MPUREG_ACCEL_ZOUT_L                             0x40
#define MPUREG_TEMP_OUT_H                               0x41
#define MPUREG_TEMP_OUT_L                               0x42
#define MPUREG_GYRO_XOUT_H                              0x43
#define MPUREG_GYRO_XOUT_L                              0x44
#define MPUREG_GYRO_YOUT_H                              0x45
#define MPUREG_GYRO_YOUT_L                              0x46
#define MPUREG_GYRO_ZOUT_H                              0x47
#define MPUREG_GYRO_ZOUT_L                              0x48
#define MPUREG_USER_CTRL                                0x6A
// bit definitions for MPUREG_USER_CTRL
#       define BIT_USER_CTRL_SIG_COND_RESET             0x01            // resets signal paths and results registers for all sensors (gyros, accel, temp)
#       define BIT_USER_CTRL_I2C_MST_RESET              0x02            // reset I2C Master (only applicable if I2C_MST_EN bit is set)
#       define BIT_USER_CTRL_FIFO_RESET                 0x04            // Reset (i.e. clear) FIFO buffer
#       define BIT_USER_CTRL_DMP_RESET                  0x08            // Reset DMP
#       define BIT_USER_CTRL_I2C_IF_DIS                 0x10            // Disable primary I2C interface and enable hal.spi->interface
#       define BIT_USER_CTRL_I2C_MST_EN                 0x20            // Enable MPU to act as the I2C Master to external slave sensors
#       define BIT_USER_CTRL_FIFO_EN                    0x40            // Enable FIFO operations
#       define BIT_USER_CTRL_DMP_EN                             0x80            // Enable DMP operations
#define MPUREG_PWR_MGMT_1                               0x6B
#       define BIT_PWR_MGMT_1_CLK_INTERNAL              0x00            // clock set to internal 8Mhz oscillator
#       define BIT_PWR_MGMT_1_CLK_XGYRO                 0x01            // PLL with X axis gyroscope reference
#       define BIT_PWR_MGMT_1_CLK_YGYRO                 0x02            // PLL with Y axis gyroscope reference
#       define BIT_PWR_MGMT_1_CLK_ZGYRO                 0x03            // PLL with Z axis gyroscope reference
#       define BIT_PWR_MGMT_1_CLK_EXT32KHZ              0x04            // PLL with external 32.768kHz reference
#       define BIT_PWR_MGMT_1_CLK_EXT19MHZ              0x05            // PLL with external 19.2MHz reference
#       define BIT_PWR_MGMT_1_CLK_STOP                  0x07            // Stops the clock and keeps the timing generator in reset
#       define BIT_PWR_MGMT_1_TEMP_DIS                  0x08            // disable temperature sensor
#       define BIT_PWR_MGMT_1_CYCLE                             0x20            // put sensor into cycle mode.  cycles between sleep mode and waking up to take a single sample of data from active sensors at a rate determined by LP_WAKE_CTRL
#       define BIT_PWR_MGMT_1_SLEEP                             0x40            // put sensor into low power sleep mode
#       define BIT_PWR_MGMT_1_DEVICE_RESET              0x80            // reset entire device
#define MPUREG_PWR_MGMT_2                               0x6C            // allows the user to configure the frequency of wake-ups in Accelerometer Only Low Power Mode
#define MPUREG_BANK_SEL                                 0x6D            // DMP bank selection register (used to indirectly access DMP registers)
#define MPUREG_MEM_START_ADDR                   0x6E            // DMP memory start address (used to indirectly write to dmp memory)
#define MPUREG_MEM_R_W                                  0x6F            // DMP related register
#define MPUREG_DMP_CFG_1                                0x70            // DMP related register
#define MPUREG_DMP_CFG_2                                0x71            // DMP related register
#define MPUREG_FIFO_COUNTH 0x72
#define MPUREG_FIFO_COUNTL 0x73
#define MPUREG_FIFO_R_W 0x74
#define MPUREG_WHOAMI                                   0x75


// Configuration bits MPU 3000 and MPU 6000 (not revised)?
#define BITS_DLPF_CFG_256HZ_NOLPF2  0x00
#define BITS_DLPF_CFG_188HZ         0x01
#define BITS_DLPF_CFG_98HZ          0x02
#define BITS_DLPF_CFG_42HZ          0x03
#define BITS_DLPF_CFG_20HZ          0x04
#define BITS_DLPF_CFG_10HZ          0x05
#define BITS_DLPF_CFG_5HZ           0x06
#define BITS_DLPF_CFG_2100HZ_NOLPF  0x07
#define BITS_DLPF_CFG_MASK          0x07

											// Product ID Description for MPU6000
											// high 4 bits 	low 4 bits
											// Product Name	Product Revision
#define MPU6000_REV_A4				0x04 	// 0000			0100
#define MPU6000ES_REV_C4 			0x14 	// 0001			0100
#define MPU6000ES_REV_C5 			0x15 	// 0001			0101
#define MPU6000ES_REV_D6 			0x16	// 0001			0110
#define MPU6000ES_REV_D7 			0x17	// 0001			0111
#define MPU6000ES_REV_D8 			0x18	// 0001			1000	
#define MPU6000_REV_C4 				0x54	// 0101			0100 
#define MPU6000_REV_C5 				0x55	// 0101			0101
#define MPU6000_REV_D6 				0x56	// 0101			0110	
#define MPU6000_REV_D7 				0x57	// 0101			0111
#define MPU6000_REV_D8 				0x58	// 0101			1000
#define MPU6000_REV_D9 				0x59	// 0101			1001

/* 
 *  RM-MPU-6000A-00.pdf, page 33, section 4.25 lists LSB sensitivity of
 *  gyro as 16.4 LSB/DPS at scale factor of +/- 2000dps (FS_SEL==3)
 */
//static const float s_gyro_scale = (0.0174532f / 16.4f); //2000dps
static const float s_gyro_scale = (0.0174532f / 32.8f); //1000dps

IMU_MPU6000_i2c::IMU_MPU6000_i2c()
	: m_mpu_addr(MPU6000_ADDR)
	, m_sample_freq_div(10)
	, m_sample_freq_counter(0)
	, m_is_initialised(false)
	, m_buffer_idx(0)
	, m_accel_calibration_scale(1.f)
{
}

//================ HARDWARE FUNCTIONS ==================== 

void IMU_MPU6000_i2c::poll_data(void* ptr)
{
	auto* imu = reinterpret_cast<IMU_MPU6000_i2c*>(ptr);
	
	if (--imu->m_sample_freq_counter <= 0)
	{	
		imu->m_sample_freq_counter = imu->m_sample_freq_div;

		auto now = clock::now_us();
		if (i2c::try_lock())
		{
			auto d = now - imu->m_last_refresh_time;
			
			auto& buffer = imu->m_buffers[imu->m_buffer_idx];

			if (i2c::read_registers_le(imu->m_mpu_addr, MPUREG_ACCEL_XOUT_H, reinterpret_cast<uint16_t*>(imu->m_raw_mpu), 7))
			{
				buffer.accel_sum.x	+= imu->m_raw_mpu[0] - imu->m_accel_calibration_bias.x;
				buffer.accel_sum.y	+= imu->m_raw_mpu[1] - imu->m_accel_calibration_bias.y;
				buffer.accel_sum.z	+= imu->m_raw_mpu[2] - imu->m_accel_calibration_bias.z;
				
				buffer.temp_sum		+= imu->m_raw_mpu[3];
				buffer.gyro_sum.x	+= imu->m_raw_mpu[4] - imu->m_gyro_calibration_bias.x;
				buffer.gyro_sum.y	+= imu->m_raw_mpu[5] - imu->m_gyro_calibration_bias.y;
				buffer.gyro_sum.z	+= imu->m_raw_mpu[6] - imu->m_gyro_calibration_bias.z;
				
				buffer.sample_count++;
				
				buffer.delta_time_sum.count += d.count;

				imu->m_last_refresh_time.ticks = now.ticks;
			}
			else
			{
				//PRINT("\ni2c failed");
				TRACE_MSG("i2c failed");
			}

			i2c::unlock();
		}
//		PRINT("\n{0}:{1}", now, clock::now_us() - now);
	}
}

//  set the DLPF filter frequency. Assumes caller has taken semaphore
void IMU_MPU6000_i2c::set_filter_register(uint8_t filter_hz, uint8_t default_filter)
{
    uint8_t filter = default_filter;
    // choose filtering frequency
    switch (filter_hz) 
	{
    case 5:
        filter = BITS_DLPF_CFG_5HZ;
        break;
    case 10:
        filter = BITS_DLPF_CFG_10HZ;
        break;
    case 20:
        filter = BITS_DLPF_CFG_20HZ;
        break;
    case 42:
        filter = BITS_DLPF_CFG_42HZ;
        break;
    case 98:
        filter = BITS_DLPF_CFG_98HZ;
        break;
    }

    if (filter != 0) 
	{
        //_last_filter_hz = filter_hz;
        i2c::write_register(m_mpu_addr, MPUREG_CONFIG, filter);
    }
}

bool IMU_MPU6000_i2c::init_hardware(Sample_Rate sample_rate)
{
	if (!i2c::lock(chrono::micros(10000)))
	{
		return false;
	}

    // Chip reset
    uint8_t tries;
    uint8_t reg_val;
    for (tries = 0; tries < 5; tries++) 
	{
		i2c::write_register(m_mpu_addr, MPUREG_PWR_MGMT_1, BIT_PWR_MGMT_1_DEVICE_RESET);
		clock::delay(chrono::millis(100));
			
		// Wake up device and select GyroZ clock. Note that the
		// MPU6000 starts up in sleep mode, and it can take some time
		// for it to come out of sleep
		i2c::write_register(m_mpu_addr, MPUREG_PWR_MGMT_1, BIT_PWR_MGMT_1_CLK_ZGYRO);
		clock::delay(chrono::millis(5));
			
		// check it has woken up
		i2c::read_register(m_mpu_addr, MPUREG_PWR_MGMT_1, reg_val);
		if (reg_val == BIT_PWR_MGMT_1_CLK_ZGYRO) 
		{
			break;
		}
	#ifdef SIMULATOR
		break;
	#endif
    }
    if (tries == 5) 
	{
        PANIC_MSG("Failed to boot MPU6000 5 times");
    }

    // only used for wake-up in accelerometer only low power mode
    i2c::write_register(m_mpu_addr, MPUREG_PWR_MGMT_2, 0);
    clock::delay(chrono::millis(1));
    
    uint8_t default_filter = BITS_DLPF_CFG_10HZ, rate = MPUREG_SMPLRT_50HZ;

	auto scheduler_freq_hz = scheduler::get_frequency_hz();
	
    // sample rate and filtering
    // to minimise the effects of aliasing we choose a filter
    // that is less than half of the sample rate  
	uint16_t sample_rate_hz = static_cast<uint16_t>(sample_rate);
    switch (sample_rate)
	{
    case Sample_Rate::_50_HZ:
		rate = MPUREG_SMPLRT_50HZ;
		default_filter = BITS_DLPF_CFG_10HZ;
        break;
    case Sample_Rate::_100_HZ:
    	rate = MPUREG_SMPLRT_100HZ;
        default_filter = BITS_DLPF_CFG_42HZ;
        break;
    case Sample_Rate::_250_HZ:
		rate = MPUREG_SMPLRT_250HZ;
		default_filter = BITS_DLPF_CFG_98HZ;
		break;
    case Sample_Rate::_500_HZ:
		rate = MPUREG_SMPLRT_500HZ;
		default_filter = BITS_DLPF_CFG_98HZ;
	    break;
    default:
		rate = 0;
		PANIC();
    }

	m_sample_freq_div = scheduler_freq_hz / sample_rate_hz;
	//check the sample rate is compatible with the scheduler frequency
	if (m_sample_freq_div * sample_rate_hz != scheduler_freq_hz)
	{
		PANIC();
	}	
	
	PRINT("\nIMU scheduler frequency divider: {0}", m_sample_freq_div);
	
    set_filter_register(0, default_filter);

    // set sample rate to 200Hz, and use _sample_divider to give
    // the requested rate to the application
    i2c::write_register(m_mpu_addr, MPUREG_SMPLRT_DIV, rate);
    clock::delay(chrono::millis(1));

    i2c::write_register(m_mpu_addr, MPUREG_GYRO_CONFIG, BITS_GYRO_FS_1000DPS); // Gyro scale 2000?/s
    clock::delay(chrono::millis(1));

		// Get chip revision
    i2c::read_register(m_mpu_addr, MPUREG_PRODUCT_ID, reg_val);

		// Select Accel scale
	if ((reg_val == MPU6000_REV_A4) || (reg_val == MPU6000ES_REV_C4) || (reg_val == MPU6000ES_REV_C5) ||
		(reg_val == MPU6000_REV_C4)   || (reg_val == MPU6000_REV_C5))
	{
		// Accel scale 8g (4096 LSB/g)
		// Rev C has different scaling than rev D
		i2c::write_register(m_mpu_addr, MPUREG_ACCEL_CONFIG, 1<<3);
	} 
	else 
	{
		// Accel scale 8g (4096 LSB/g)
		i2c::write_register(m_mpu_addr, MPUREG_ACCEL_CONFIG, 2<<3);
	}
			
    clock::delay(chrono::millis(1));

#ifndef DISABLE_INTERNAL_MAG
    // Enable I2C bypass mode, to work with Magnetometer 5883L
    // Disable I2C Master mode
    i2c::write_register(m_mpu_addr, MPUREG_USER_CTRL, 0);
    i2c::write_register(m_mpu_addr, MPUREG_INT_PIN_CFG, BIT_I2C_BYPASS_EN);
#endif
    
/*  Dump MPU6050 registers  
    hal.console->println_P(PSTR("MPU6000 registers"));
    for (uint8_t reg=MPUREG_PRODUCT_ID; reg<=108; reg++) {
    	  i2c::readRegister(s_mpu_addr, reg, &reg_val);
        hal.console->printf_P(PSTR("%02x:%02x "), (unsigned)reg, (unsigned)reg_val);
        if ((reg - (MPUREG_PRODUCT_ID-1)) % 16 == 0) {
            hal.console->println();
        }
    }
    hal.console->println();*/

	//t_read_data_transaction();
    
    i2c::unlock();
	
    return true;
}

bool IMU_MPU6000_i2c::refresh_data(Gyroscope_Data& gdata, Accelerometer_Data& adata) const
{
#ifdef SIMULATOR
	m_buffers[m_buffer_idx].sample_count = 1;
#else
	if (!m_buffers[m_buffer_idx].sample_count)
	{
		m_thermometer.m_is_valid = false;
		return false;
	}
#endif

	m_thermometer.m_is_valid = true;

//	auto now = clock::now_ms();
//	s_last_refresh_time = now;
	
	//////////////////////////////////////////////////////////////////////////
	//copy the data

	auto last_buffer_idx = m_buffer_idx;
	m_buffer_idx = !m_buffer_idx; //now the interrupt will write in the other buffer.
	auto& buffer = m_buffers[last_buffer_idx];

	ASSERT(buffer.sample_count > 0);
	auto sc = buffer.sample_count;
	buffer.sample_count = 0;

	gdata.delta.set((math::vec3s32&)(buffer.gyro_sum));
	((math::vec3s32&)buffer.gyro_sum).set(0, 0, 0);

	adata.acceleration.set((math::vec3s32&)(buffer.accel_sum));
	((math::vec3s32&)buffer.accel_sum).set(0, 0, 0);

	m_thermometer.m_data.degrees = buffer.temp_sum * 0.000244140625f;
	buffer.temp_sum = 0;

	float delta_time = (float)buffer.delta_time_sum.count * 0.000001f;
	buffer.delta_time_sum.count = 0;

	// done copying the data
	//////////////////////////////////////////////////////////////////////////

	float scinv = 1.f / sc;

	//////////////////////////////////////////////////////////////////////////
	//scale the accel and apply the calibration offset
	adata.acceleration *= (scinv * physics::constants::g / 4096.f); // divided by 4096
	adata.acceleration *= m_accel_calibration_scale;
	adata.dt = delta_time;

	gdata.delta *= (s_gyro_scale * scinv * delta_time);
	gdata.dt = delta_time;
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// PUBLIC API

void IMU_MPU6000_i2c::init(Sample_Rate rate)
{
	if (m_is_initialised)
	{
		return;
	}
	m_is_initialised = true;
	
	i2c::init();

	uint8_t tries = 0;
	do
	{
		bool success = init_hardware(rate);
		if (success)
		{
			break;
		}
		else
		{
			clock::delay(chrono::millis(50));
		}
		
		if (tries++ > 5)
		{
			PANIC_MSG("Failed to boot MPU6000 5 times");
		}
	} while (1);

	(chrono::time_us&)m_last_refresh_time = clock::now_us();

	// start the timer process to read samples
	board::scheduler::register_callback(&poll_data, this);
}

bool IMU_MPU6000_i2c::get_data(Gyroscope_Data& gdata, Accelerometer_Data& adata) const
{
	ASSERT(m_is_initialised);
	return refresh_data(gdata, adata);
}

void IMU_MPU6000_i2c::set_gyroscope_bias(math::vec3f const& bias)
{
	m_gyro_calibration_bias.set(bias * 4096.f);
	//s_gyro_calibration_scale.set(scale);
}
void IMU_MPU6000_i2c::set_accelerometer_bias_scale(math::vec3f const& bias, math::vec3f const& scale)
{
	m_accel_calibration_bias.set(bias * 4096.f / physics::constants::g);
	m_accel_calibration_scale.set(scale);
}

Thermometer const& IMU_MPU6000_i2c::get_thermometer() const
{
	return m_thermometer;	
}

}

#endif
