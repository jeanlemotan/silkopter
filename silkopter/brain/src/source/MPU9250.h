#pragma once

#include "HAL.h"
#include "common/node/source/ISource.h"
#include "common/node/bus/II2C.h"
#include "common/node/bus/ISPI.h"
#include "common/node/stream/IAcceleration.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "common/node/stream/IMagnetic_Field.h"
#include "common/node/stream/ITemperature.h"

namespace sz
{
namespace MPU9250
{
class Init_Params;
class Config;
}
}



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

    auto init(rapidjson::Value const& json) -> bool;
    auto get_init_params() -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() -> rapidjson::Document;

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

    std::shared_ptr<sz::MPU9250::Init_Params> m_init_params;
    std::shared_ptr<sz::MPU9250::Config> m_config;

    mutable std::vector<uint8_t> m_fifo_buffer;
    size_t m_fifo_sample_size = 999999;

    struct Common
    {
        uint32_t rate = 0;
        std::string name;
        q::Clock::duration dt;
    };

    struct Acceleraton : public stream::IAcceleration, public Common
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        Sample last_sample;
        std::vector<Sample> samples;
        float scale_inv = 1.f;
    } m_acceleration;

    struct Angular_Velocity : public stream::IAngular_Velocity, public Common
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        std::vector<Sample> samples;
        Sample last_sample;
        std::string name;
        float scale_inv = 1.f;
    } m_angular_velocity;

    struct Magnetic_Field : public stream::IMagnetic_Field, public Common
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        std::vector<Sample> samples;
        Sample last_sample;
        uint8_t akm_address = 0;
        q::Clock::time_point last_time_point;
        float magnetic_adj[3];
    } m_magnetic_field;

    struct Temperature : public stream::ITemperature, public Common
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }
        auto get_name() const -> std::string const& { return name; }

        std::vector<Sample> samples;
        Sample last_sample;
    } m_temperature;
};


}
}
}
