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


class MPU9250 : q::util::Noncopyable
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

    auto get_accelerometer() -> IAccelerometer&;
    auto get_gyroscope() -> IGyroscope&;
    auto get_compass() -> ICompass&;
    auto get_thermometer() -> IThermometer&;

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

private:
    buses::II2C* m_i2c = nullptr;
    buses::ISPI* m_spi = nullptr;

    Params m_params;

    mutable std::vector<uint8_t> m_fifo_buffer;
    size_t m_fifo_sample_size = 999999;

    struct Accelerometer : public IAccelerometer
    {
        auto get_name() const -> q::String const& { return name; }
        auto get_samples() const -> std::vector<Accelerometer_Sample> const& { return samples; }

        uint32_t sample_idx = 0;
        float scale_inv = 1.f;
        std::vector<Accelerometer_Sample> samples;
        q::String name;
    } m_accelerometer;

    struct Gyroscope : public IGyroscope
    {
        auto get_name() const -> q::String const& { return name; }
        auto get_samples() const -> std::vector<Gyroscope_Sample> const& { return samples; }

        uint32_t sample_idx = 0;
        float scale_inv = 1.f;
        std::vector<Gyroscope_Sample> samples;
        q::String name;
    } m_gyroscope;

    struct Compass : public ICompass
    {
        auto get_name() const -> q::String const& { return name; }
        auto get_samples() const -> std::vector<Compass_Sample> const& { return samples; }

        uint8_t akm_address = 0;
        q::Clock::duration dt;
        q::Clock::time_point last_time_point;
        uint32_t sample_idx = 0;
        float magnetic_adj[3];
        std::vector<Compass_Sample> samples;
        q::String name;
    } m_compass;

    struct Thermometer : public IThermometer
    {
        auto get_name() const -> q::String const& { return name; }
        auto get_samples() const -> std::vector<Thermometer_Sample> const& { return samples; }

        q::Clock::duration dt;
        uint32_t sample_idx = 0;
        std::vector<Thermometer_Sample> samples;
        q::String name;
    } m_thermometer;

    q::Clock::duration m_imu_dt;
};


DECLARE_CLASS_PTR(MPU9250);

}
}
