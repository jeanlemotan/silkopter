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
    double m_acceleration_sensor_scale = 1.0;

    typedef Basic_Output_Stream<stream::IAngular_Velocity> Angular_Velocity_Stream;
    mutable std::shared_ptr<Angular_Velocity_Stream> m_angular_velocity;
    double m_angular_velocity_sensor_scale = 1.0;

    typedef Basic_Output_Stream<stream::IMagnetic_Field> Magnetic_Field_Stream;
    mutable std::shared_ptr<Magnetic_Field_Stream> m_magnetic_field;
    uint8_t m_akm_address = 0;
    math::vec3d m_magnetic_field_sensor_scale;

    math::vec3d m_last_magnetic_field_value;

    typedef Basic_Output_Stream<stream::ITemperature> Temperature_Stream;
    mutable std::shared_ptr<Temperature_Stream> m_temperature;

};


}
}
