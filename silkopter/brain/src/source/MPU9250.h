#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
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
struct Init_Params;
struct Config;
}
}



namespace silk
{
namespace node
{

class MPU9250 : public ISource
{
public:
    MPU9250(HAL& hal);
    ~MPU9250();

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document const&;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void process();

    auto get_outputs() const -> std::vector<Output>;

private:
    auto init() -> bool;

private:
    HAL& m_hal;
    bus::II2C_wptr m_i2c;
    bus::ISPI_wptr m_spi;

    struct Buses
    {
        bus::II2C_ptr i2c;
        bus::ISPI_ptr spi;
    };

    auto mpu_read(Buses& buses, uint8_t reg, uint8_t* data, uint32_t size) -> bool;
    auto mpu_read_u8(Buses& buses, uint8_t reg, uint8_t& dst) -> bool;
    auto mpu_read_u16(Buses& buses, uint8_t reg, uint16_t& dst) -> bool;
    auto mpu_write_u8(Buses& buses, uint8_t reg, uint8_t t) -> bool;
    auto mpu_write_u16(Buses& buses, uint8_t reg, uint16_t t) -> bool;

    auto akm_read(Buses& buses, uint8_t reg, uint8_t* data, uint32_t size) -> bool;
    auto akm_read_u8(Buses& buses, uint8_t reg, uint8_t& dst) -> bool;
    auto akm_read_u16(Buses& buses, uint8_t reg, uint16_t& dst) -> bool;
    auto akm_write_u8(Buses& buses, uint8_t reg, uint8_t t) -> bool;

    auto lock(Buses& buses) -> bool;
    void unlock(Buses& buses);

    void reset_fifo(Buses& buses);

    auto setup_compass(Buses& buses) -> bool;
    auto setup_compass_i2c(Buses& buses) -> bool;
    auto setup_compass_spi(Buses& buses) -> bool;

//    void set_bypass(Buses& buses, bool on);
    void process_compass(Buses& buses);

    rapidjson::Document m_init_paramsj;
    std::shared_ptr<sz::MPU9250::Init_Params> m_init_params;
    std::shared_ptr<sz::MPU9250::Config> m_config;

    mutable std::vector<uint8_t> m_fifo_buffer;
    size_t m_fifo_sample_size = 999999;
    q::Clock::time_point m_last_fifo_tp = q::Clock::now();

    uint8_t m_user_ctrl_value = 0;

    struct Common
    {
        uint32_t rate = 0;
        q::Clock::duration dt;
    };

    struct Acceleration : public stream::IAcceleration, public Common
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        Sample last_sample;
        std::vector<Sample> samples;
        float scale_inv = 1.f;
    };
    mutable std::shared_ptr<Acceleration> m_acceleration;

    struct Angular_Velocity : public stream::IAngular_Velocity, public Common
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        std::vector<Sample> samples;
        Sample last_sample;
        std::string name;
        float scale_inv = 1.f;
    };
    mutable std::shared_ptr<Angular_Velocity> m_angular_velocity;

    struct Magnetic_Field : public stream::IMagnetic_Field, public Common
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        std::vector<Sample> samples;
        Sample last_sample;
        uint8_t akm_address = 0;
        q::Clock::time_point last_tp = q::Clock::now();
        float magnetic_adj[3];
    };
    mutable std::shared_ptr<Magnetic_Field> m_magnetic_field;

    struct Temperature : public stream::ITemperature, public Common
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        std::vector<Sample> samples;
        Sample last_sample;
    };
    mutable std::shared_ptr<Temperature> m_temperature;
};


}
}
