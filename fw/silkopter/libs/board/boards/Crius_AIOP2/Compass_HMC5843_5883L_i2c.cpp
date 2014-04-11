#include "Config.h"

#if BOARD_TYPE == CRIUS_AIOP_2

#include "board/boards/Crius_AIOP2/Compass_HMC5843_5883L_i2c.h"
#include "debug/debug.h"
#include "util/format.h"
#include "board/boards/avr_gpio.h"
#include "board/boards/avr_i2c.h"
#include "board/board.h"

namespace board
{
	static const uint8_t COMPASS_ADDRESS		= 0x1E;
	static const uint8_t CONFIG_REG_A			= 0x00;
	static const uint8_t CONFIG_REG_B			= 0x01;
	static const uint8_t MAG_GAIN				= 0x20;
	static const uint8_t POSITIVE_BIAS_CONFIG	= 0x11;
	static const uint8_t NEGATIVE_BIAS_CONFIG	= 0x12;
	static const uint8_t NORMAL_OPERATION		= 0x10;
	static const uint8_t MODE_REGISTER			= 0x02;
	static const uint8_t CONTINUOUS_CONVERSION	= 0x00;
	static const uint8_t SINGLE_CONVERSION		= 0x01;

	// ConfigRegA valid sample averaging for 5883L
	static const uint8_t SAMPLE_AVERAGING_1		= 0x00;
	static const uint8_t SAMPLE_AVERAGING_2		= 0x01;
	static const uint8_t SAMPLE_AVERAGING_4		= 0x02;
	static const uint8_t SAMPLE_AVERAGING_8		= 0x03;

	// ConfigRegA valid data output rates for 5883L
	static const uint8_t DATA_OUTPUT_RATE_0_75HZ= 0x00;
	static const uint8_t DATA_OUTPUT_RATE_1_5HZ	= 0x01;
	static const uint8_t DATA_OUTPUT_RATE_3HZ	= 0x02;
	static const uint8_t DATA_OUTPUT_RATE_7_5HZ	= 0x03;
	static const uint8_t DATA_OUTPUT_RATE_15HZ	= 0x04;
	static const uint8_t DATA_OUTPUT_RATE_30HZ	= 0x05;
	static const uint8_t DATA_OUTPUT_RATE_75HZ	= 0x06;

	struct Raw_Compass_Data
	{
		int16_t x;
		int16_t y;
		int16_t z;
	};

	// Read Sensor data
	void Compass_HMC5843_5883L_i2c::poll_data(void* ptr)
	{
		if (i2c::try_lock())
		{
			auto* compass = reinterpret_cast<Compass_HMC5843_5883L_i2c*>(ptr);
			compass->poll_data_locked();
		}
	}
	
	void Compass_HMC5843_5883L_i2c::poll_data_locked()
	{
		//this assumes the i2c is locked already
		
		auto& buffer = m_buffers[m_buffer_idx];

		Raw_Compass_Data raw_data;
		bool data_ok = i2c::read_registers_uint16_le(COMPASS_ADDRESS, 0x03, raw_data);
		i2c::unlock();
			
		if (data_ok)
		{
			buffer.compass_sum.x += raw_data.x;
			if (m_model == Compass_Model::HMC5883L)
			{
				buffer.compass_sum.y += raw_data.z;
				buffer.compass_sum.z += raw_data.y;
			}
			else
			{
				buffer.compass_sum.y += raw_data.y;
				buffer.compass_sum.z += raw_data.z;
			}

			buffer.sample_count++;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	Compass_HMC5843_5883L_i2c::Compass_HMC5843_5883L_i2c()
	{
	}
	
	//////////////////////////////////////////////////////////////////////////
	
	void Compass_HMC5843_5883L_i2c::init()
	{
		i2c::init();

		uint8_t tries = 0;
		do
		{
			bool success = init_hardware();
			if (success)
			{
				m_is_initialized = true;
				break;
			}
			else
			{
				clock::delay(chrono::millis(50));
			}
			
			if (tries++ > 5)
			{
				debug::printf("\nFailed to boot HHMC5843 5 times");
				break;
			}
		} while (1);
	}

	//////////////////////////////////////////////////////////////////////////

	bool Compass_HMC5843_5883L_i2c::init_hardware()
	{
	#ifdef SIMULATOR
		return true;
	#endif

		if (!i2c::lock(chrono::micros(10000)))
		{
			return false;
		}
		
		int test_count = 0;
		const int k_test_count = 20;
		
		int good_count = 0;
		const int k_min_good_count = 5;
		bool success = false;
		uint8_t calibration_gain = 0;
		math::vec3f expected;
		float gain_multiple = 0;


		// determine if we are using 5843 or 5883L
		_base_config = 0;
		uint8_t HMC_5883L_config = (SAMPLE_AVERAGING_8 << 5) | (DATA_OUTPUT_RATE_75HZ << 2) | NORMAL_OPERATION;
		if (!i2c::write_registers(COMPASS_ADDRESS, CONFIG_REG_A, HMC_5883L_config) || !i2c::read_registers(COMPASS_ADDRESS, CONFIG_REG_A, _base_config))
		{
			i2c::unlock();
			return false;
		}
		
		if (_base_config == HMC_5883L_config)
		{
			// a 5883L supports the sample averaging config
			m_model = Compass_Model::HMC5883L;
			calibration_gain = 0x60;
			expected.set(766, 713, 713);
			gain_multiple = 660.0 / 1090;  // adjustment for runtime vs calibration gain
		}
		else if (_base_config == (NORMAL_OPERATION | (DATA_OUTPUT_RATE_75HZ << 2)))
		{
			m_model = Compass_Model::HMC5843;
			calibration_gain = 0x20;
			expected.set(715, 715, 715);
			gain_multiple = 1.0;
		}
		else
		{
			i2c::unlock();
			return false;
		}

		m_calibration.set(0, 0, 0);

		while (test_count < k_test_count && good_count < k_min_good_count)
		{
			// record number of attempts at initialisation
			test_count++;

			// force positiveBias (compass should return 715 for all channels)
			if (!i2c::write_registers(COMPASS_ADDRESS, CONFIG_REG_A, POSITIVE_BIAS_CONFIG))
			{
				continue;      // compass not responding on the bus
			}
			clock::delay(chrono::millis(50));

			// set gains
			if (!i2c::write_registers(COMPASS_ADDRESS, CONFIG_REG_B, calibration_gain) ||
				!i2c::write_registers(COMPASS_ADDRESS, MODE_REGISTER, SINGLE_CONVERSION))
			{
				continue;
			}

			// read values from the compass
			clock::delay(chrono::millis(50));
			
			auto& buffer = m_buffers[m_buffer_idx];
			
			//clear the buffer
			buffer.compass_sum.x = buffer.compass_sum.y = buffer.compass_sum.z = 0;
			buffer.sample_count = 0;
			
			poll_data_locked();
			if (buffer.sample_count == 0)
			{
				continue;      // we didn't read valid values
			}

			clock::delay(chrono::millis(10));

			math::vec3s32 read(buffer.compass_sum.x, buffer.compass_sum.y, buffer.compass_sum.z);
			read = math::max(math::abs(read), math::vec3s32(1)); //to avoid division by zero
			math::vec3f cal = expected / math::vec3f(read);

			//clear the buffer
			buffer.compass_sum.x = buffer.compass_sum.y = buffer.compass_sum.z = 0;
			buffer.sample_count = 0;

			debug::printf("\n{} CPS: {} / {}", test_count, cal, read);
			
			if (cal.x > 0.7f && cal.x < 1.3f &&
				cal.y > 0.7f && cal.y < 1.3f &&
				cal.z > 0.7f && cal.z < 1.3f)
			{
				good_count++;
				m_calibration += cal;
			}
		}

		if (good_count >= k_min_good_count)
		{
			m_calibration = m_calibration * gain_multiple / float(good_count);
		}
		else
		{
			i2c::unlock();
			return false;
		}
		debug::printf("\nCPS calib: {}", m_calibration);

		// leave test mode
		if (!i2c::write_registers(COMPASS_ADDRESS, CONFIG_REG_A, _base_config) ||
			!i2c::write_registers(COMPASS_ADDRESS, CONFIG_REG_B, MAG_GAIN) ||
			!i2c::write_registers(COMPASS_ADDRESS, MODE_REGISTER, CONTINUOUS_CONVERSION))
		{
			i2c::unlock();
			return false;
		}

		i2c::unlock();
		
		//~75hz
		board::scheduler::register_callback(chrono::millis(13), &poll_data, this);

		//read();
		return true;
	}

	bool Compass_HMC5843_5883L_i2c::get_data(Data& data) const
	{
		if (!m_is_initialized)
		{
			return false;
		}
		
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

			m_out_data.direction.set((math::vec3s32&)(buffer.compass_sum));
			buffer.compass_sum.x = 0;
			buffer.compass_sum.y = 0;
			buffer.compass_sum.z = 0;
			
			//////////////////////////////////////////////////////////////////////////
			
			m_out_data.direction = math::normalized<float, math::fast>(m_out_data.direction * m_calibration);
			
			// rotate to the desired orientation
// 			Vector3f rot_mag = Vector3f(mag_x,mag_y,mag_z);
// 			if (product_id == AP_COMPASS_TYPE_HMC5883L) 
// 			{
// 				rot_mag.rotate(ROTATION_YAW_90);
// 			}

		}
		
		data = m_out_data;
		return true;
	}
	
}

#endif
