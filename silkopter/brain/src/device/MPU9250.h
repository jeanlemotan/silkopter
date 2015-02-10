#pragma once

#include "interface/II2C.h"
#include "interface/ISPI.h"
#include "interface/IAccelerometer.h"
#include "interface/IGyroscope.h"
#include "interface/ICompass.h"
#include "interface/IThermometer.h"

namespace silk
{
namespace device
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

    auto init(bus::II2C* bus, Params const& params) -> bool;
    auto init(bus::ISPI* bus, Params const& params) -> bool;

    void process();

    auto get_accelerometer() -> sensor::IAccelerometer&;
    auto get_gyroscope() -> sensor::IGyroscope&;
    auto get_compass() -> sensor::ICompass&;
    auto get_thermometer() -> sensor::IThermometer&;

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
    bus::II2C* m_i2c = nullptr;
    bus::ISPI* m_spi = nullptr;

    Params m_params;

    mutable std::vector<uint8_t> m_fifo_buffer;
    size_t m_fifo_sample_size = 999999;

    struct Accelerometer : public sensor::IAccelerometer
    {
        auto get_name() const -> q::String const& { return name; }
        auto get_samples() const -> std::vector<sensor::Accelerometer_Sample> const& { return samples; }

        uint32_t sample_idx = 0;
        float scale_inv = 1.f;
        std::vector<sensor::Accelerometer_Sample> samples;
        q::String name;
    } m_accelerometer;

    struct Gyroscope : public sensor::IGyroscope
    {
        auto get_name() const -> q::String const& { return name; }
        auto get_samples() const -> std::vector<sensor::Gyroscope_Sample> const& { return samples; }

        uint32_t sample_idx = 0;
        float scale_inv = 1.f;
        std::vector<sensor::Gyroscope_Sample> samples;
        q::String name;
    } m_gyroscope;

    struct Compass : public sensor::ICompass
    {
        auto get_name() const -> q::String const& { return name; }
        auto get_samples() const -> std::vector<sensor::Compass_Sample> const& { return samples; }

        uint8_t akm_address = 0;
        q::Clock::duration dt;
        q::Clock::time_point last_time_point;
        uint32_t sample_idx = 0;
        float magnetic_adj[3];
        std::vector<sensor::Compass_Sample> samples;
        q::String name;
    } m_compass;

    struct Thermometer : public sensor::IThermometer
    {
        auto get_name() const -> q::String const& { return name; }
        auto get_samples() const -> std::vector<sensor::Thermometer_Sample> const& { return samples; }

        q::Clock::duration dt;
        uint32_t sample_idx = 0;
        std::vector<sensor::Thermometer_Sample> samples;
        q::String name;
    } m_thermometer;

    q::Clock::duration m_imu_dt;
};


DECLARE_CLASS_PTR(MPU9250);

}
}
