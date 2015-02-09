#pragma once

#include "buses/II2C.h"
#include "buses/ISPI.h"
#include "IAccelerometer.h"
#include "IGyroscope.h"
#include "ICompass.h"
#include "IThermometer.h"

namespace silk
{
namespace sensors
{


class MPU9250 : public IAccelerometer, public IGyroscope, public ICompass, public IThermometer, q::util::Noncopyable
{
public:
    MPU9250(q::String const& name);
    ~MPU9250();

    struct Params
    {
        size_t imu_rate = 1000;
        size_t compass_rate = 100;
        size_t thermometer_rate = 10;
        size_t gyroscope_range = 500; //degrees per second
        size_t accelerometer_range = 4; //gees
    };

    auto init(buses::II2C* bus, Params const& params) -> bool;
    auto init(buses::ISPI* bus, Params const& params) -> bool;

    void process();

    auto get_accelerometer_name() const -> q::String const&;
    auto get_gyroscope_name() const -> q::String const&;
    auto get_compass_name() const -> q::String const&;
    auto get_thermometer_name() const -> q::String const&;

    auto get_accelerometer_samples() const -> std::vector<Accelerometer_Sample> const&;
    auto get_gyroscope_samples() const -> std::vector<Gyroscope_Sample> const&;
    auto get_compass_samples() const -> std::vector<Compass_Sample> const&;
    auto get_thermometer_samples() const -> std::vector<Thermometer_Sample> const&;

    void lock();
    void unlock();

private:
    auto init(Params const& params) -> bool;

    auto mpu_read(uint8_t reg, uint8_t* data, uint32_t size) -> bool;
    auto mpu_read_u8(uint8_t reg, uint8_t& dst) -> bool;
    auto mpu_read_u16(uint8_t reg, uint16_t& dst) -> bool;
    auto mpu_write_u8(uint8_t reg, uint8_t const& t) -> bool;
    auto mpu_write_u16(uint8_t reg, uint16_t const& t) -> bool;

    auto akm_read(uint8_t reg, uint8_t* data, uint32_t size) -> bool;
    auto akm_read_u8(uint8_t reg, uint8_t& dst) -> bool;
    auto akm_read_u16(uint8_t reg, uint16_t& dst) -> bool;
    auto akm_write_u8(uint8_t reg, uint8_t const& t) -> bool;
    auto akm_write_u16(uint8_t reg, uint16_t const& t) -> bool;

    void reset_fifo();

    auto setup_compass() -> bool;
    void set_bypass(bool on);
    void process_compass();

    uint8_t m_akm_address = 0;

private:
    buses::II2C* m_i2c = nullptr;
    buses::ISPI* m_spi = nullptr;

    Params m_params;

    q::String m_accelerometer_name;
    q::String m_gyroscope_name;
    q::String m_compass_name;
    q::String m_thermometer_name;

    mutable std::vector<uint8_t> m_fifo_buffer;

    size_t m_fifo_sample_size = 999999;

    struct Samples
    {
        std::vector<Gyroscope_Sample> gyroscope;
        std::vector<Accelerometer_Sample> accelerometer;
        std::vector<Compass_Sample> compass;
        std::vector<Thermometer_Sample> thermometer;
    } m_samples;

    float m_accelerometer_scale_inv = 1.f;
    float m_gyroscope_scale_inv = 1.f;
    float m_magnetic_adj[3];

    uint32_t m_accelerometer_sample_idx = 0;
    uint32_t m_gyroscope_sample_idx = 0;
    uint32_t m_compass_sample_idx = 0;

    q::Clock::duration m_imu_dt;
    q::Clock::duration m_compass_dt;
    q::Clock::duration m_thermometer_dt;

    q::Clock::time_point m_last_compass_timestamp;
};


DECLARE_CLASS_PTR(MPU9250);

}
}
