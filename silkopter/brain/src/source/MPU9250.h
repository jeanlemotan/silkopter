#pragma once

#include "HAL.h"
#include "common/node/source/ISource.h"
#include "common/node/bus/II2C.h"
#include "common/node/bus/ISPI.h"
#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/ITemperature.h"

namespace silk
{
namespace node
{
namespace source
{

class MPU9250 : public ISource
{
public:
    MPU9250(HAL& hal);
    ~MPU9250();

    struct Init_Params
    {
        std::string name;
        bus::IBus* bus = nullptr;
        uint32_t imu_rate = 1000;
        uint32_t magnetometer_rate = 100;
        uint32_t thermometer_rate = 10;
        uint32_t gyroscope_range = 500; //degrees per second
        uint32_t accelerometer_range = 4; //gees
    };

    auto init(rapidjson::Value const& json) -> bool;
    auto init(Init_Params const& params) -> bool;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> boost::optional<rapidjson::Value const&>;

    void process();

    auto get_name() const -> std::string const&;
    auto get_output_stream_count() const -> size_t;
    auto get_output_stream(size_t idx) -> stream::IStream&;

    void lock();
    void unlock();

private:
    auto init() -> bool;

    auto mpu_read(uint8_t reg, uint8_t* data, uint32_t size) -> bool;
    auto mpu_read_u8(uint8_t reg, uint8_t& dst) -> bool;
    auto mpu_read_u16(uint8_t reg, uint16_t& dst) -> bool;
    auto mpu_write_u8(uint8_t reg, uint8_t t) -> bool;
    auto mpu_write_u16(uint8_t reg, uint16_t t) -> bool;

    auto akm_read(uint8_t reg, uint8_t* data, uint32_t size) -> bool;
    auto akm_read_u8(uint8_t reg, uint8_t& dst) -> bool;
    auto akm_read_u16(uint8_t reg, uint16_t& dst) -> bool;
    auto akm_write_u8(uint8_t reg, uint8_t t) -> bool;
    auto akm_write_u16(uint8_t reg, uint16_t t) -> bool;

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

    struct Acceleraton : public stream::IAcceleration
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        uint32_t rate = 0;
        Sample last_sample;
        std::vector<Sample> samples;
        std::string name;
        float scale_inv = 1.f;
    } m_acceleration;

    struct Angular_Velocity : public stream::IAngular_Velocity
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
        Sample last_sample;
        std::string name;
        float scale_inv = 1.f;
    } m_angular_velocity;

    struct Magnetic_Field : public stream::IMagnetic_Field
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
        Sample last_sample;
        std::string name;

        uint8_t akm_address = 0;
        q::Clock::duration dt;
        q::Clock::time_point last_time_point;
        float magnetic_adj[3];
    } m_magnetic_field;

    struct Temperature : public stream::ITemperature
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        uint32_t rate = 0;
        std::vector<Sample> samples;
        Sample last_sample;
        std::string name;
        q::Clock::duration dt;
    } m_temperature;

    q::Clock::duration m_imu_dt;
};


}
}
}
