#pragma once

#include <stdint.h>
#include <stddef.h>
#include <_qmath.h>
#include "utils/chrono.h"
#include "board/boards/Dummy_Accelerometer.h"
#include "board/boards/Dummy_Gyroscope.h"
#include "board/boards/Dummy_Thermometer.h"
#include "utils/Double_Buffer.h"

namespace board
{

class IMU_MPU6000_i2c : q::util::Noncopyable
{
public:
	IMU_MPU6000_i2c();

    enum class Sample_Rate
    {
        _50_HZ		= 50,
        _100_HZ		= 100,
        _250_HZ		= 250,
        _500_HZ		= 500,
        _1000_HZ	= 1000
    };
	
    void init(Sample_Rate rate);
	
    Thermometer& get_thermometer() const { return m_thermometer; }
    Accelerometer& get_accelerometer() const { return m_accelerometer; }
    Gyroscope& get_gyroscope() const { return m_gyroscope; }

private:
    bool refresh_accelerometer_data() const;
    bool refresh_gyroscope_data() const;
    bool refresh_thermometer_data() const;

    class IMU_Accelerometer : public Dummy_Accelerometer
    {
    public:
        IMU_Accelerometer(IMU_MPU6000_i2c& imu) : m_imu(imu) {}
        bool get_data(Data& data) const
        {
            m_imu.refresh_accelerometer_data();
            if (m_data.id != data.id)
            {
                data = m_data;
                return true;
            }
            return false;
        }
    protected:
        IMU_MPU6000_i2c& m_imu;
    };

    class IMU_Gyroscope : public Dummy_Gyroscope
    {
    public:
        IMU_Gyroscope(IMU_MPU6000_i2c& imu) : m_imu(imu) {}
        bool get_data(Data& data) const
        {
            m_imu.refresh_gyroscope_data();
            if (m_data.id != data.id)
            {
                data = m_data;
                return true;
            }
            return false;
        }
    protected:
        IMU_MPU6000_i2c& m_imu;
    };

    class IMU_Thermometer : public Dummy_Thermometer
    {
    public:
        IMU_Thermometer(IMU_MPU6000_i2c& imu) : m_imu(imu) {}
        bool get_data(Data& data) const
        {
            m_imu.refresh_thermometer_data();
            if (m_data.id != data.id)
            {
                data = m_data;
                return true;
            }
            return false;
        }
    protected:
        IMU_MPU6000_i2c& m_imu;
    };

    mutable IMU_Accelerometer m_accelerometer;
    mutable IMU_Gyroscope m_gyroscope;
    mutable IMU_Thermometer m_thermometer;


	 //mutable chrono::time_us m_last_refresh_time;
	 uint8_t m_mpu_addr;
	 bool m_is_initialised = 0;

    struct Accel_Buffer
	{
        /*volatile*/ math::vec3s32 sum;
        volatile uint8_t sample_count = 0;
	};
    mutable util::Double_Buffer<Accel_Buffer> m_accel_buffer;
    struct Gyro_Buffer
    {
        /*volatile*/ math::vec3s32 sum;
        volatile uint8_t sample_count = 0;
    };
    mutable util::Double_Buffer<Gyro_Buffer> m_gyro_buffer;
    struct Temp_Buffer
    {
        /*volatile*/ uint16_t sum;
        volatile uint8_t sample_count = 0;
    };
    mutable util::Double_Buffer<Temp_Buffer> m_temp_buffer;


    uint16_t m_fifo_count = 0;
	math::vec3s16 m_raw_gyro_sample;
    uint8_t m_sample_time_ms = 0;
    uint8_t m_sample_time_ms_2 = 0;

	static void poll_data(void* ptr);

	bool init_hardware(Sample_Rate sample_rate);
};

}
