#pragma once

#include "HAL.h"
#include "common/node/bus/II2C.h"
#include "common/node/bus/ISPI.h"
#include "common/node/source/IAccelerometer.h"
#include "common/node/source/IGyroscope.h"
#include "common/node/source/ICompass.h"
#include "common/node/source/IThermometer.h"

namespace silk
{
namespace node
{
namespace source
{

class MPU9250 : q::util::Noncopyable
{
public:
    MPU9250(HAL& hal);
    ~MPU9250();

    struct Init_Params
    {
        std::string name;
        std::string bus;
        uint32_t imu_rate = 1000;
        uint32_t compass_rate = 100;
        uint32_t thermometer_rate = 10;
        uint32_t gyroscope_range = 500; //degrees per second
        uint32_t accelerometer_range = 4; //gees
    };

    auto init(Init_Params const& params) -> bool;

    void process();

    auto get_accelerometer()    -> source::IAccelerometer&;
    auto get_gyroscope()        -> source::IGyroscope&;
    auto get_compass()          -> source::ICompass&;
    auto get_thermometer()      -> source::IThermometer&;

    void lock();
    void unlock();

private:
    auto init() -> bool;

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
    HAL& m_hal;
    bus::II2C* m_i2c = nullptr;
    bus::ISPI* m_spi = nullptr;

    Init_Params m_params;

    mutable std::vector<uint8_t> m_fifo_buffer;
    size_t m_fifo_sample_size = 999999;

    struct Accelerometer : public source::IAccelerometer, public stream::IAcceleration
    {
        auto get_stream() -> stream::IAcceleration& { return *this; }
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        uint32_t sample_idx = 0;
        float scale_inv = 1.f;
        std::vector<Sample> samples;
    } m_accelerometer;

    struct Gyroscope : public source::IGyroscope, public stream::IAngular_Velocity
    {
        auto get_stream() -> stream::IAngular_Velocity& { return *this; }
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        uint32_t sample_idx = 0;
        float scale_inv = 1.f;
        std::vector<Sample> samples;
    } m_gyroscope;

    struct Compass : public source::ICompass, public stream::IMagnetic_Field
    {
        auto get_stream() -> stream::IMagnetic_Field& { return *this; }
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        uint8_t akm_address = 0;
        q::Clock::duration dt;
        q::Clock::time_point last_time_point;
        uint32_t sample_idx = 0;
        float magnetic_adj[3];
        std::vector<Sample> samples;
    } m_compass;

    struct Thermometer : public source::IThermometer, public stream::ITemperature
    {
        auto get_stream() -> stream::ITemperature& { return *this; }
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        q::Clock::duration dt;
        uint32_t sample_idx = 0;
        std::vector<Sample> samples;
    } m_thermometer;

    q::Clock::duration m_imu_dt;
};


DECLARE_CLASS_PTR(MPU9250);

}
}
}
