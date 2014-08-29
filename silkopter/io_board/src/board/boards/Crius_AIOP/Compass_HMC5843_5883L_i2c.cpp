#include "Config.h"

#if BOARD_TYPE == BOARD_CRIUS_AIOP_2

#include "board/boards/Crius_AIOP/Compass_HMC5843_5883L_i2c.h"
#include "QBase.h"
#include "board/boards/AVR_gpio.h"
#include "board/boards/AVR_i2c.h"
#include "board/board.h"
#include "utils/Timed_Scope.h"

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

    static const int QUANTIZATION_FACTOR = 10;

	struct Raw_Compass_Data
	{
		int16_t v[3];
	};

	// Read Sensor data
	void Compass_HMC5843_5883L_i2c::poll_data(void* ptr)
	{
		if (i2c::try_lock())
		{
			auto* compass = reinterpret_cast<Compass_HMC5843_5883L_i2c*>(ptr);
			compass->poll_data_locked(true);
		}
	}
	
	void Compass_HMC5843_5883L_i2c::poll_data_locked(bool unlock)
	{
		//this assumes the i2c is locked already
		
		auto& buffer = m_buffer.get();

		Raw_Compass_Data raw_data;
		bool data_ok = i2c::read_registers_uint16_le(COMPASS_ADDRESS, 0x03, raw_data);
		if (unlock)
		{
			i2c::unlock();
		}
			
		if (data_ok)
		{
            buffer.sum.x += raw_data.v[0];
			if (m_model == Compass_Model::HMC5883L)
			{
                buffer.sum.y += raw_data.v[2];
                buffer.sum.z += raw_data.v[1];
			}
			else
			{
				PANIC(); //check the correct orientation
                buffer.sum.y += raw_data.v[1];
                buffer.sum.z += raw_data.v[2];
			}

			buffer.sample_count++;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	Compass_HMC5843_5883L_i2c::Compass_HMC5843_5883L_i2c()
	{
        m_data_config.bias.set(0, 0, 0);
        m_data_config.scale = math::vec3f(1.f/QUANTIZATION_FACTOR);
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
                q::quick_logf(F_STR("Failed to boot HHMC5843 5 times"));
				break;
			}
		} while (1);
	}

	//////////////////////////////////////////////////////////////////////////

	bool Compass_HMC5843_5883L_i2c::init_hardware()
	{
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
        math::vec3f calibration_scale;

		// determine if we are using 5843 or 5883L
		uint8_t base_config = 0;
		uint8_t HMC_5883L_config = (SAMPLE_AVERAGING_8 << 5) | (DATA_OUTPUT_RATE_75HZ << 2) | NORMAL_OPERATION;
		if (!i2c::write_registers(COMPASS_ADDRESS, CONFIG_REG_A, HMC_5883L_config) || !i2c::read_registers(COMPASS_ADDRESS, CONFIG_REG_A, base_config))
		{
			i2c::unlock();
			return false;
		}
		
		if (base_config == HMC_5883L_config)
		{
			// a 5883L supports the sample averaging config
			m_model = Compass_Model::HMC5883L;
			calibration_gain = 0x60;
			expected.set(766, 766, 713);
		}
		else if (base_config == (NORMAL_OPERATION | (DATA_OUTPUT_RATE_75HZ << 2)))
		{
// 			m_model = Compass_Model::HMC5843;
// 			calibration_gain = 0x20;
// 			expected.set(715, 715, 715);

			//THIS NEEDS TOE CORRECT ORIENTATION CHECKED IN THE poll_data
			i2c::unlock();
			return false;
		}
		else
		{
			i2c::unlock();
			return false;
		}

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
			
			auto& buffer = m_buffer.get();
			
			//clear the buffer
            buffer.sum.x = buffer.sum.y = buffer.sum.z = 0;
			buffer.sample_count = 0;
			
			poll_data_locked(false);
			if (buffer.sample_count == 0)
			{
				continue;      // we didn't read valid values
			}

			clock::delay(chrono::millis(10));

            math::vec3s32 read(buffer.sum.x, buffer.sum.y, buffer.sum.z);
			read = math::max(math::abs(read), math::vec3s32(1)); //to avoid division by zero
			math::vec3f cal = expected / math::vec3f(read);

			//clear the buffer
            buffer.sum.x = buffer.sum.y = buffer.sum.z = 0;
			buffer.sample_count = 0;

            q::quick_logf(F_STR("{} CPS: {} / {}"), test_count, cal, read);
			
			if (cal.x > 0.7f && cal.x < 1.3f &&
				cal.y > 0.7f && cal.y < 1.3f &&
				cal.z > 0.7f && cal.z < 1.3f)
			{
				good_count++;
                calibration_scale += cal;
			}
		}

		if (good_count >= k_min_good_count)
		{
            calibration_scale = calibration_scale / float(good_count);
		}
		else
		{
			i2c::unlock();
			return false;
		}

        m_calibration_scale = math::vec3s32(calibration_scale * 256.f * QUANTIZATION_FACTOR);

        q::quick_logf(F_STR("CPS calib: {} / {}"), calibration_scale, m_calibration_scale);

		// leave test mode
		if (!i2c::write_registers(COMPASS_ADDRESS, CONFIG_REG_A, base_config) ||
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
		
		if (m_buffer.get().sample_count > 0)
		{
			//////////////////////////////////////////////////////////////////////////
			//copy the data

			auto& buffer = m_buffer.get_and_swap();

			QASSERT(buffer.sample_count > 0);
            uint8_t sc = buffer.sample_count;
			buffer.sample_count = 0;

            auto raw = buffer.sum;
            buffer.sum.x = 0;
            buffer.sum.y = 0;
            buffer.sum.z = 0;
			
			//////////////////////////////////////////////////////////////////////////

            //this is already included in the calibration scale
            //raw *= 10; //compensated by Data_Config.scale

            switch (sc)
            {
            case 1:
                //nothing to do
                break;
            case 2:
                raw.x >>= 1; raw.y >>= 1; raw.z >>= 1;
                break;
            case 4:
                raw.x >>= 2; raw.y >>= 2; raw.z >>= 2;
                break;
            case 8:
                raw.x >>= 3; raw.y >>= 3; raw.z >>= 3;
                break;
            default:
                raw = raw / sc;
                break;
            }

            raw *= m_calibration_scale;
            raw.x >>= 8;
            raw.y >>= 8;
            raw.z >>= 8;

            m_out_data.direction.set(raw.x, raw.y, raw.z);
            m_out_data.id++;
		}
		
        if (m_out_data.id != data.id)
        {
            data = m_out_data;
            return true;
        }
        return false;
    }
}


#endif
