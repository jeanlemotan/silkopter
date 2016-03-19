#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/bus/II2C.h"
#include "common/bus/ISPI.h"
#include "common/stream/IAcceleration.h"
#include "common/stream/IAngular_Velocity.h"
#include "common/stream/IMagnetic_Field.h"
#include "common/stream/ITemperature.h"

#include "Basic_Output_Stream.h"

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
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void process();

    auto get_outputs() const -> std::vector<Output>;

private:
    auto init() -> bool;

private:
    HAL& m_hal;
    std::weak_ptr<bus::II2C> m_i2c;
    std::weak_ptr<bus::ISPI> m_spi;

    struct Buses
    {
        std::shared_ptr<bus::II2C> i2c;
        std::shared_ptr<bus::ISPI> spi;
    };
    std::vector<uint8_t> m_dummy_tx_data;

    auto mpu_read(Buses& buses, uint8_t reg, uint8_t* data, uint32_t size, size_t speed) -> bool;
    auto mpu_read_u8(Buses& buses, uint8_t reg, uint8_t& dst, size_t speed) -> bool;
    auto mpu_read_u16(Buses& buses, uint8_t reg, uint16_t& dst, size_t speed) -> bool;
    auto mpu_write_u8(Buses& buses, uint8_t reg, uint8_t t, size_t speed) -> bool;
    auto mpu_write_u16(Buses& buses, uint8_t reg, uint16_t t, size_t speed) -> bool;

    auto akm_read(Buses& buses, uint8_t reg, uint8_t* data, uint32_t size, size_t speed) -> bool;
    auto akm_read_u8(Buses& buses, uint8_t reg, uint8_t& dst, size_t speed) -> bool;
    auto akm_read_u16(Buses& buses, uint8_t reg, uint16_t& dst, size_t speed) -> bool;
    auto akm_write_u8(Buses& buses, uint8_t reg, uint8_t t, size_t speed) -> bool;

    auto lock(Buses& buses) -> bool;
    void unlock(Buses& buses);

    void reset_fifo(Buses& buses);

    void process_thermometer(Buses& buses);

    auto setup_magnetometer(Buses& buses) -> bool;
    auto setup_magnetometer_i2c(Buses& buses) -> bool;
    auto setup_magnetometer_spi(Buses& buses) -> bool;

//    void set_bypass(Buses& buses, bool on);
    void process_magnetometer(Buses& buses);

    std::shared_ptr<sz::MPU9250::Init_Params> m_init_params;
    std::shared_ptr<sz::MPU9250::Config> m_config;

    mutable std::vector<uint8_t> m_fifo_buffer;
    size_t m_fifo_sample_size = 999999;

    uint8_t m_user_ctrl_value = 0;

    typedef Basic_Output_Stream<stream::IAcceleration> Acceleration_Stream;
    mutable std::shared_ptr<Acceleration_Stream> m_acceleration;
    float m_acceleration_sensor_scale = 1.0f;
    math::vec3f m_acceleration_scale = math::vec3f::one;
    math::vec3f m_acceleration_bias;

    typedef Basic_Output_Stream<stream::IAngular_Velocity> Angular_Velocity_Stream;
    mutable std::shared_ptr<Angular_Velocity_Stream> m_angular_velocity;
    float m_angular_velocity_sensor_scale = 1.0f;
    math::vec3f m_angular_velocity_bias;

    math::mat3f m_imu_rotation;

    typedef Basic_Output_Stream<stream::IMagnetic_Field> Magnetic_Field_Stream;
    mutable std::shared_ptr<Magnetic_Field_Stream> m_magnetic_field;
    uint8_t m_akm_address = 0;
    math::vec3f m_magnetic_field_sensor_scale;
    math::vec3f m_magnetic_field_scale = math::vec3f::one;
    math::vec3f m_magnetic_field_bias;

    math::mat3f m_magnetometer_rotation;

    math::vec3f m_last_magnetic_field_value;
    q::Clock::time_point m_last_magnetic_field_tp = q::Clock::now();

    typedef Basic_Output_Stream<stream::ITemperature> Temperature_Stream;
    mutable std::shared_ptr<Temperature_Stream> m_temperature;

    float m_last_temperature_value = 0;
    q::Clock::time_point m_last_temperature_tp = q::Clock::now();


    struct Stats
    {
        q::Clock::time_point last_report_tp = q::Clock::now();
        struct Acceleration
        {
            bool operator==(Acceleration const& o) const { return memcmp(this, &o, sizeof(*this)) == 0; }
            bool operator!=(Acceleration const& o) const { return !operator==(o); }
            size_t added = 0;
            size_t skipped = 0;
        } acc;
        struct Angular_Velocity
        {
            bool operator==(Angular_Velocity const& o) const { return memcmp(this, &o, sizeof(*this)) == 0; }
            bool operator!=(Angular_Velocity const& o) const { return !operator==(o); }
            size_t added = 0;
            size_t skipped = 0;
        } av;
        struct Magnetic_Field
        {
            bool operator==(Magnetic_Field const& o) const { return memcmp(this, &o, sizeof(*this)) == 0; }
            bool operator!=(Magnetic_Field const& o) const { return !operator==(o); }
            size_t reset = 0;
            size_t added = 0;
            size_t overflow = 0;
            size_t bad_values = 0;
        } mf;
        struct Temperature
        {
            bool operator==(Temperature const& o) const { return memcmp(this, &o, sizeof(*this)) == 0; }
            bool operator!=(Temperature const& o) const { return !operator==(o); }
            size_t added = 0;
            size_t bad_values = 0;
        } temp;

        size_t bus_failures = 0;

        bool operator==(Stats const& o) const { return acc == o.acc && av == o.av && mf == o.mf && temp == o.temp && bus_failures == o.bus_failures; }
        bool operator!=(Stats const& o) const { return !operator==(o); }
    } m_stats;

};


}
}
