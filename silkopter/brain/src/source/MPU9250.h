#pragma once

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
    MPU9250();
    ~MPU9250();

    struct Init_Params
    {
        size_t imu_rate = 1000;
        size_t compass_rate = 100;
        size_t thermometer_rate = 10;
        size_t gyroscope_range = 500; //degrees per second
        size_t accelerometer_range = 4; //gees
    };

    auto init(bus::II2C* bus, Init_Params const& params) -> bool;
    auto init(bus::ISPI* bus, Init_Params const& params) -> bool;

    void process();

    auto get_accelerometer()    -> source::IAccelerometer&;
    auto get_gyroscope()        -> source::IGyroscope&;
    auto get_compass()          -> source::ICompass&;
    auto get_thermometer()      -> source::IThermometer&;

    void lock();
    void unlock();

private:
    auto init(Init_Params const& params) -> bool;

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

    Init_Params m_params;

    mutable std::vector<uint8_t> m_fifo_buffer;
    size_t m_fifo_sample_size = 999999;

    struct Accelerometer : public source::IAccelerometer, public stream::IAcceleration
    {
        auto get_stream() const -> stream::IAcceleration const& { return *this; }
        auto get_samples() const -> std::vector<Sample> const& { return samples; }

        uint32_t sample_idx = 0;
        float scale_inv = 1.f;
        std::vector<Sample> samples;
    } m_accelerometer;

    struct Gyroscope : public source::IGyroscope, public stream::IAngular_Velocity
    {
        auto get_stream() const -> stream::IAngular_Velocity const& { return *this; }
        auto get_samples() const -> std::vector<Sample> const& { return samples; }

        uint32_t sample_idx = 0;
        float scale_inv = 1.f;
        std::vector<Sample> samples;
    } m_gyroscope;

    struct Compass : public source::ICompass, public stream::IMagnetic_Field
    {
        auto get_stream() const -> stream::IMagnetic_Field const& { return *this; }
        auto get_samples() const -> std::vector<Sample> const& { return samples; }

        uint8_t akm_address = 0;
        q::Clock::duration dt;
        q::Clock::time_point last_time_point;
        uint32_t sample_idx = 0;
        float magnetic_adj[3];
        std::vector<Sample> samples;
    } m_compass;

    struct Thermometer : public source::IThermometer, public stream::ITemperature
    {
        auto get_stream() const -> stream::ITemperature const& { return *this; }
        auto get_samples() const -> std::vector<Sample> const& { return samples; }

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
