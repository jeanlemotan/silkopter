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

// MPU 6000 I2C Address
static const uint8_t MPU6000_ADDR						= 0x68;

// MPU 6000 registers
static const uint8_t MPUREG_XG_OFFS_TC					= 0x00;
static const uint8_t MPUREG_YG_OFFS_TC					= 0x01;
static const uint8_t MPUREG_ZG_OFFS_TC					= 0x02;
static const uint8_t MPUREG_X_FINE_GAIN					= 0x03;
static const uint8_t MPUREG_Y_FINE_GAIN					= 0x04;
static const uint8_t MPUREG_Z_FINE_GAIN					= 0x05;
static const uint8_t MPUREG_XA_OFFS_H					= 0x06;    // X axis accelerometer offset (high byte)
static const uint8_t MPUREG_XA_OFFS_L					= 0x07;    // X axis accelerometer offset (low byte)
static const uint8_t MPUREG_YA_OFFS_H					= 0x08;    // Y axis accelerometer offset (high byte)
static const uint8_t MPUREG_YA_OFFS_L					= 0x09;    // Y axis accelerometer offset (low byte)
static const uint8_t MPUREG_ZA_OFFS_H					= 0x0A;    // Z axis accelerometer offset (high byte)
static const uint8_t MPUREG_ZA_OFFS_L					= 0x0B;    // Z axis accelerometer offset (low byte)
static const uint8_t MPUREG_PRODUCT_ID					= 0x0C;    // Product ID Register
static const uint8_t MPUREG_XG_OFFS_USRH				= 0x13;    // X axis gyro offset (high byte)
static const uint8_t MPUREG_XG_OFFS_USRL				= 0x14;    // X axis gyro offset (low byte)
static const uint8_t MPUREG_YG_OFFS_USRH				= 0x15;    // Y axis gyro offset (high byte)
static const uint8_t MPUREG_YG_OFFS_USRL				= 0x16;    // Y axis gyro offset (low byte)
static const uint8_t MPUREG_ZG_OFFS_USRH				= 0x17;    // Z axis gyro offset (high byte)
static const uint8_t MPUREG_ZG_OFFS_USRL				= 0x18;    // Z axis gyro offset (low byte)
static const uint8_t MPUREG_SMPLRT_DIV					= 0x19;    // sample rate.  Fsample= 1Khz/(<this value>+1) = 200Hz
	static const uint8_t MPUREG_SMPLRT_1000HZ			= 0x00;
	static const uint8_t MPUREG_SMPLRT_500HZ			= 0x01;
	static const uint8_t MPUREG_SMPLRT_250HZ			= 0x03;
	static const uint8_t MPUREG_SMPLRT_200HZ			= 0x04;
	static const uint8_t MPUREG_SMPLRT_100HZ			= 0x09;
	static const uint8_t MPUREG_SMPLRT_50HZ				= 0x13;
static const uint8_t MPUREG_CONFIG						= 0x1A;
static const uint8_t MPUREG_GYRO_CONFIG					= 0x1B;
// bit definitions for MPUREG_GYRO_CONFIG
	static const uint8_t BITS_GYRO_FS_250DPS			= 0x00;
	static const uint8_t BITS_GYRO_FS_500DPS			= 0x08;
	static const uint8_t BITS_GYRO_FS_1000DPS			= 0x10;
	static const uint8_t BITS_GYRO_FS_2000DPS			= 0x18;
	static const uint8_t BITS_GYRO_FS_MASK				= 0x18;    // only bits 3 and 4 are used for gyro full scale so use this to mask off other bits
	static const uint8_t BITS_GYRO_ZGYRO_SELFTEST		= 0x20;
	static const uint8_t BITS_GYRO_YGYRO_SELFTEST		= 0x40;
	static const uint8_t BITS_GYRO_XGYRO_SELFTEST		= 0x80;
static const uint8_t MPUREG_ACCEL_CONFIG				= 0x1C;
static const uint8_t MPUREG_MOT_THR						= 0x1F;    // detection threshold for Motion interrupt generation.  Motion is detected when the absolute value of any of the accelerometer measurements exceeds this
static const uint8_t MPUREG_MOT_DUR						= 0x20;    // duration counter threshold for Motion interrupt generation. The duration counter ticks at 1 kHz, therefore MOT_DUR has a unit of 1 LSB = 1 ms
static const uint8_t MPUREG_ZRMOT_THR					= 0x21;    // detection threshold for Zero Motion interrupt generation.
static const uint8_t MPUREG_ZRMOT_DUR					= 0x22;    // duration counter threshold for Zero Motion interrupt generation. The duration counter ticks at 16 Hz, therefore ZRMOT_DUR has a unit of 1 LSB = 64 ms.
static const uint8_t MPUREG_FIFO_EN						= 0x23;
static const uint8_t MPUREG_INT_PIN_CFG					= 0x37;
	static const uint8_t BIT_I2C_BYPASS_EN				= 0x02; 
	static const uint8_t BIT_INT_RD_CLEAR				= 0x10;    // clear the interrupt when any read occurs
	static const uint8_t BIT_LATCH_INT_EN				= 0x20;    // latch data ready pin 
static const uint8_t MPUREG_INT_ENABLE					= 0x38;
// bit definitions for MPUREG_INT_ENABLE				= 	
	static const uint8_t BIT_RAW_RDY_EN					= 0x01;
	static const uint8_t BIT_DMP_INT_EN					= 0x02;    // enabling this bit (DMP_INT_EN) also enables RAW_RDY_EN it seems
	static const uint8_t BIT_UNKNOWN_INT_EN				= 0x04;
	static const uint8_t BIT_I2C_MST_INT_EN				= 0x08;
	static const uint8_t BIT_FIFO_OFLOW_EN				= 0x10;
	static const uint8_t BIT_ZMOT_EN					= 0x20;
	static const uint8_t BIT_MOT_EN						= 0x40;
	static const uint8_t BIT_FF_EN						= 0x80;
static const uint8_t MPUREG_INT_STATUS					= 0x3A;
// bit definitions for MPUREG_INT_STATUS (same bit patte= rn s ab;ove because this register shows what interrupt actually fired)
	static const uint8_t BIT_RAW_RDY_INT				= 0x01;
	static const uint8_t BIT_DMP_INT					= 0x02;
	static const uint8_t BIT_UNKNOWN_INT				= 0x04;
	static const uint8_t BIT_I2C_MST_INT				= 0x08;
	static const uint8_t BIT_FIFO_OFLOW_INT				= 0x10;
	static const uint8_t BIT_ZMOT_INT					= 0x20;
	static const uint8_t BIT_MOT_INT					= 0x40;
	static const uint8_t BIT_FF_INT						= 0x80;
static const uint8_t MPUREG_ACCEL_XOUT_H				= 0x3B;
static const uint8_t MPUREG_ACCEL_XOUT_L				= 0x3C;
static const uint8_t MPUREG_ACCEL_YOUT_H				= 0x3D;
static const uint8_t MPUREG_ACCEL_YOUT_L				= 0x3E;
static const uint8_t MPUREG_ACCEL_ZOUT_H				= 0x3F;
static const uint8_t MPUREG_ACCEL_ZOUT_L				= 0x40;
static const uint8_t MPUREG_TEMP_OUT_H					= 0x41;
static const uint8_t MPUREG_TEMP_OUT_L					= 0x42;
static const uint8_t MPUREG_GYRO_XOUT_H					= 0x43;
static const uint8_t MPUREG_GYRO_XOUT_L					= 0x44;
static const uint8_t MPUREG_GYRO_YOUT_H					= 0x45;
static const uint8_t MPUREG_GYRO_YOUT_L					= 0x46;
static const uint8_t MPUREG_GYRO_ZOUT_H					= 0x47;
static const uint8_t MPUREG_GYRO_ZOUT_L					= 0x48;
static const uint8_t MPUREG_USER_CTRL					= 0x6A;
// bit definitions for MPUREG_USER_CTRL					= 	
	static const uint8_t BIT_USER_CTRL_SIG_COND_RESET	= 0x01;            // resets signal paths and results registers for all sensors (gyros, accel, temp)
	static const uint8_t BIT_USER_CTRL_I2C_MST_RESET	= 0x02;            // reset I2C Master (only applicable if I2C_MST_EN bit is set)
	static const uint8_t BIT_USER_CTRL_FIFO_RESET		= 0x04;            // Reset (i.e. clear) FIFO buffer
	static const uint8_t BIT_USER_CTRL_DMP_RESET		= 0x08;            // Reset DMP
	static const uint8_t BIT_USER_CTRL_I2C_IF_DIS		= 0x10;            // Disable primary I2C interface and enable hal.spi->interface
	static const uint8_t BIT_USER_CTRL_I2C_MST_EN		= 0x20;            // Enable MPU to act as the I2C Master to external slave sensors
	static const uint8_t BIT_USER_CTRL_FIFO_EN			= 0x40;            // Enable FIFO operations
	static const uint8_t BIT_USER_CTRL_DMP_EN			= 0x80;            // Enable DMP operations
static const uint8_t MPUREG_PWR_MGMT_1					= 0x6B;
	static const uint8_t BIT_PWR_MGMT_1_CLK_INTERNAL	= 0x00;            // clock set to internal 8Mhz oscillator
	static const uint8_t BIT_PWR_MGMT_1_CLK_XGYRO		= 0x01;            // PLL with X axis gyroscope reference
	static const uint8_t BIT_PWR_MGMT_1_CLK_YGYRO		= 0x02;            // PLL with Y axis gyroscope reference
	static const uint8_t BIT_PWR_MGMT_1_CLK_ZGYRO		= 0x03;            // PLL with Z axis gyroscope reference
	static const uint8_t BIT_PWR_MGMT_1_CLK_EXT32KHZ	= 0x04;            // PLL with external 32.768kHz reference
	static const uint8_t BIT_PWR_MGMT_1_CLK_EXT19MHZ	= 0x05;            // PLL with external 19.2MHz reference
	static const uint8_t BIT_PWR_MGMT_1_CLK_STOP		= 0x07;            // Stops the clock and keeps the timing generator in reset
	static const uint8_t BIT_PWR_MGMT_1_TEMP_DIS		= 0x08;            // disable temperature sensor
	static const uint8_t BIT_PWR_MGMT_1_CYCLE			= 0x20;            // put sensor into cycle mode.  cycles between sleep mode and waking up to take a single sample of data from active sensors at a rate determined by LP_WAKE_CTRL
	static const uint8_t BIT_PWR_MGMT_1_SLEEP			= 0x40;            // put sensor into low power sleep mode
	static const uint8_t BIT_PWR_MGMT_1_DEVICE_RESET	= 0x80;            // reset entire device
static const uint8_t MPUREG_PWR_MGMT_2					= 0x6C;            // allows the user to configure the frequency of wake-ups in Accelerometer Only Low Power Mode
static const uint8_t MPUREG_BANK_SEL					= 0x6D;            // DMP bank selection register (used to indirectly access DMP registers)
static const uint8_t MPUREG_MEM_START_ADDR				= 0x6E;            // DMP memory start address (used to indirectly write to dmp memory)
static const uint8_t MPUREG_MEM_R_W						= 0x6F;            // DMP related register
static const uint8_t MPUREG_DMP_CFG_1					= 0x70;            // DMP related register
static const uint8_t MPUREG_DMP_CFG_2					= 0x71;            // DMP related register
static const uint8_t MPUREG_FIFO_COUNTH					= 0x72;
static const uint8_t MPUREG_FIFO_COUNTL					= 0x73;
static const uint8_t MPUREG_FIFO_R_W					= 0x74;
static const uint8_t MPUREG_WHOAMI						= 0x75;


// Configuration bits MPU 3000 and MPU 6000 (not revised)?
static const uint8_t BITS_DLPF_CFG_256HZ_NOLPF2			= 0x00;
static const uint8_t BITS_DLPF_CFG_188HZ				= 0x01;
static const uint8_t BITS_DLPF_CFG_98HZ					= 0x02;
static const uint8_t BITS_DLPF_CFG_42HZ					= 0x03;
static const uint8_t BITS_DLPF_CFG_20HZ					= 0x04;
static const uint8_t BITS_DLPF_CFG_10HZ					= 0x05;
static const uint8_t BITS_DLPF_CFG_5HZ					= 0x06;
static const uint8_t BITS_DLPF_CFG_2100HZ_NOLPF			= 0x07;
static const uint8_t BITS_DLPF_CFG_MASK					= 0x07;

											// Product ID Description for MPU6000
											// high 4 bits 	low 4 bits
											// Product Name	Product Revision
static const uint8_t MPU6000_REV_A4						= 0x04; 	// 0000			0100
static const uint8_t MPU6000ES_REV_C4 					= 0x14; 	// 0001			0100
static const uint8_t MPU6000ES_REV_C5 					= 0x15; 	// 0001			0101
static const uint8_t MPU6000ES_REV_D6 					= 0x16;	// 0001			0110
static const uint8_t MPU6000ES_REV_D7 					= 0x17;	// 0001			0111
static const uint8_t MPU6000ES_REV_D8 					= 0x18;	// 0001			1000	
static const uint8_t MPU6000_REV_C4 					= 0x54;	// 0101			0100 
static const uint8_t MPU6000_REV_C5 					= 0x55;	// 0101			0101
static const uint8_t MPU6000_REV_D6 					= 0x56;	// 0101			0110	
static const uint8_t MPU6000_REV_D7 					= 0x57;	// 0101			0111
static const uint8_t MPU6000_REV_D8 					= 0x58;	// 0101			1000
static const uint8_t MPU6000_REV_D9 					= 0x59;	// 0101			1001

/* 
 *  RM-MPU-6000A-00.pdf, page 33, section 4.25 lists LSB sensitivity of
 *  gyro as 16.4 LSB/DPS at scale factor of +/- 2000dps (FS_SEL==3)
 */
//static const float s_gyro_scale = (0.0174532f / 16.4f); //2000dps
static const float s_gyro_scale_inv = (0.0174532f / 32.8f); //1000dps
//static const float s_gyro_scale = (0.0174532f / 65.5f); //500dps

static const float s_fp_g = physics::constants::g / 4096.f;

IMU_MPU6000_i2c::IMU_MPU6000_i2c()
	: m_mpu_addr(MPU6000_ADDR)
{
}

//================ HARDWARE FUNCTIONS ==================== 

struct Raw_MPU_Data
{
	int16_t ax;
	int16_t ay;
	int16_t az;
	int16_t temp;
	int16_t gx;
	int16_t gy;
	int16_t gz;
};

void IMU_MPU6000_i2c::poll_data(void* ptr)
{
	auto* imu = reinterpret_cast<IMU_MPU6000_i2c*>(ptr);
	
	if (i2c::try_lock())
	{
		auto& buffer = imu->m_buffers[imu->m_buffer_idx];

		Raw_MPU_Data raw_data;
		bool data_ok = i2c::read_registers_uint16_le(imu->m_mpu_addr, MPUREG_ACCEL_XOUT_H, raw_data);
		i2c::unlock(); //unlock immediately
		
		if (data_ok)
		{
			buffer.accel_sum.x	+= raw_data.ax;
			buffer.accel_sum.y	+= raw_data.ay;
			buffer.accel_sum.z	+= raw_data.az;
				
			buffer.temp_sum		+= raw_data.temp;
				
			//if saturated, use the last sample
			if (raw_data.gx < -32000 || raw_data.gx > 32000)
			{
				raw_data.gx = imu->m_raw_gyro_sample.x;
			}
			else
			{
				imu->m_raw_gyro_sample.x = raw_data.gx;
			}
			if (raw_data.gy < -32000 || raw_data.gy > 32000)
			{
				raw_data.gy = imu->m_raw_gyro_sample.y;
			}
			else
			{
				imu->m_raw_gyro_sample.y = raw_data.gy;
			}
			if (raw_data.gz < -32000 || raw_data.gz > 32000)
			{
				raw_data.gz = imu->m_raw_gyro_sample.z;
			}
			else
			{
				imu->m_raw_gyro_sample.z = raw_data.gz;
			}
				
			buffer.gyro_sum.x	+= raw_data.gx;
			buffer.gyro_sum.y	+= raw_data.gy;
			buffer.gyro_sum.z	+= raw_data.gz;
				
			//PRINT("\nGYRO: {}", (const math::vec3s32&)buffer.gyro_sum);
				
			buffer.sample_count++;
		}
		else
		{
			//TRACE_MSG("i2c failed");
		}
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
        i2c::write_registers(m_mpu_addr, MPUREG_CONFIG, filter);
    }
}

bool IMU_MPU6000_i2c::init_hardware(Sample_Rate sample_rate)
{
#ifdef SIMULATOR
	return true;
#endif

	if (!i2c::lock(chrono::micros(10000)))
	{
		return false;
	}

    // Chip reset
    uint8_t tries;
    uint8_t reg_val;
    for (tries = 0; tries < 5; tries++) 
	{
		i2c::write_registers(m_mpu_addr, MPUREG_PWR_MGMT_1, BIT_PWR_MGMT_1_DEVICE_RESET);
		clock::delay(chrono::millis(100));
			
		// Wake up device and select GyroZ clock. Note that the
		// MPU6000 starts up in sleep mode, and it can take some time
		// for it to come out of sleep
		i2c::write_registers(m_mpu_addr, MPUREG_PWR_MGMT_1, BIT_PWR_MGMT_1_CLK_ZGYRO);
		clock::delay(chrono::millis(5));
			
		// check it has woken up
		i2c::read_registers(m_mpu_addr, MPUREG_PWR_MGMT_1, reg_val);
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
    i2c::write_registers(m_mpu_addr, MPUREG_PWR_MGMT_2, uint8_t(0));
    clock::delay(chrono::millis(1));
    
    uint8_t default_filter = BITS_DLPF_CFG_10HZ;
	uint8_t rate = MPUREG_SMPLRT_50HZ;

    // sample rate and filtering
    // to minimise the effects of aliasing we choose a filter
    // that is less than half of the sample rate  
	chrono::millis period(10);
    switch (sample_rate)
	{
    case Sample_Rate::_50_HZ:
		rate = MPUREG_SMPLRT_50HZ;
		default_filter = BITS_DLPF_CFG_10HZ;
		period = chrono::millis(20);
        break;
    case Sample_Rate::_100_HZ:
    	rate = MPUREG_SMPLRT_100HZ;
        default_filter = BITS_DLPF_CFG_42HZ;
		period = chrono::millis(10);
        break;
    case Sample_Rate::_250_HZ:
		rate = MPUREG_SMPLRT_250HZ;
		default_filter = BITS_DLPF_CFG_98HZ;
		period = chrono::millis(4);
		break;
    case Sample_Rate::_500_HZ:
		rate = MPUREG_SMPLRT_500HZ;
		default_filter = BITS_DLPF_CFG_98HZ;
		period = chrono::millis(2);
	    break;
    default:
		rate = 0;
		PANIC();
    }
	
	m_sample_time = period;

	
    set_filter_register(0, default_filter);

    // set sample rate to 200Hz, and use _sample_divider to give
    // the requested rate to the application
    i2c::write_registers(m_mpu_addr, MPUREG_SMPLRT_DIV, rate);
    clock::delay(chrono::millis(1));

    i2c::write_registers(m_mpu_addr, MPUREG_GYRO_CONFIG, BITS_GYRO_FS_1000DPS);
    clock::delay(chrono::millis(1));

		// Get chip revision
    i2c::read_registers(m_mpu_addr, MPUREG_PRODUCT_ID, reg_val);

		// Select Accel scale
	if ((reg_val == MPU6000_REV_A4) || (reg_val == MPU6000ES_REV_C4) || (reg_val == MPU6000ES_REV_C5) ||
		(reg_val == MPU6000_REV_C4)   || (reg_val == MPU6000_REV_C5))
	{
		// Accel scale 8g (4096 LSB/g)
		// Rev C has different scaling than rev D
		i2c::write_registers(m_mpu_addr, MPUREG_ACCEL_CONFIG, uint8_t(1<<3));
	} 
	else 
	{
		// Accel scale 8g (4096 LSB/g)
		i2c::write_registers(m_mpu_addr, MPUREG_ACCEL_CONFIG, uint8_t(2<<3));
	}
			
    clock::delay(chrono::millis(1));

    // Enable I2C bypass mode, to work with Magnetometer 5883L
    // Disable I2C Master mode
    i2c::write_registers(m_mpu_addr, MPUREG_USER_CTRL, uint8_t(0));
    i2c::write_registers(m_mpu_addr, MPUREG_INT_PIN_CFG, BIT_I2C_BYPASS_EN);
    
    i2c::unlock();
	
	//m_last_refresh_time = clock::now_us();

	// start the timer process to read samples
	board::scheduler::register_callback(period, &poll_data, this);
	
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
}

bool IMU_MPU6000_i2c::get_data(Data& data) const
{
	ASSERT(m_is_initialised);
#ifdef SIMULATOR
	m_buffers[m_buffer_idx].sample_count = 1;
#endif

	if (m_buffers[m_buffer_idx].sample_count > 0)
	{
		//////////////////////////////////////////////////////////////////////////
		//copy the data

		auto last_buffer_idx = m_buffer_idx;
		m_buffer_idx = !m_buffer_idx; //now the interrupt will write in the other buffer.
		auto& buffer = m_buffers[last_buffer_idx];

		ASSERT(buffer.sample_count > 0);
		float sc = float(buffer.sample_count);
		buffer.sample_count = 0;

		m_out_data.angular_velocity.set((math::vec3s32&)(buffer.gyro_sum));
		buffer.gyro_sum.x = 0;
		buffer.gyro_sum.y = 0;
		buffer.gyro_sum.z = 0;

		m_out_data.acceleration.set((math::vec3s32&)(buffer.accel_sum));
		buffer.accel_sum.x = 0;
		buffer.accel_sum.y = 0;
		buffer.accel_sum.z = 0;

		float temp = float(buffer.temp_sum);
		buffer.temp_sum = 0;

		// done copying the data
		//////////////////////////////////////////////////////////////////////////

		float scinv = 1.f / sc;
	
		//auto now = clock::now_us();
		//chrono::secondsf dt = now - m_last_refresh_time;
		//m_last_refresh_time = now;

		//////////////////////////////////////////////////////////////////////////
		//scale the accel and apply the calibration offset
		m_out_data.acceleration = ((m_out_data.acceleration * (scinv * s_fp_g)) - m_calibration_data.accelerometer_bias) * m_calibration_data.accelerometer_scale;

		m_out_data.angular_velocity = m_out_data.angular_velocity * (scinv * s_gyro_scale_inv) - m_calibration_data.gyroscope_bias;
		m_out_data.sample_idx ++;
		m_out_data.dt.count = m_sample_time.count * sc;
	
		//From the specs: Temperature in degrees C = (TEMP_OUT Register Value as a signed quantity)/340 + 36.53
		m_thermometer.m_is_valid = true;
		m_thermometer.m_data.degrees = temp * scinv * 0.002941f + 36.53f;
	}

	data = m_out_data;

	return true;
}

void IMU_MPU6000_i2c::set_calibration_data(Calibration_Data const& data)
{
	m_calibration_data = data;
}
IMU_MPU6000_i2c::Calibration_Data const& IMU_MPU6000_i2c::get_calibration_data() const
{
	return m_calibration_data;
}

Thermometer const& IMU_MPU6000_i2c::get_thermometer() const
{
	return m_thermometer;	
}

}

#endif
