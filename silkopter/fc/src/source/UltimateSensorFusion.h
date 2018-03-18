#pragma once

#include "HAL.h"
#include "common/node/ISource.h"
#include "common/stream/IAcceleration.h"
#include "common/stream/IAngular_Velocity.h"
#include "common/stream/IMagnetic_Field.h"
#include "common/stream/IFrame.h"
#include "common/stream/IPressure.h"
#include "common/stream/ITemperature.h"
#include "common/bus/II2C.h"
#include "common/bus/ISPI.h"

#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
{
struct UltimateSensorFusion_Descriptor;
struct UltimateSensorFusion_Config;
}
}


namespace silk
{
namespace node
{

class UltimateSensorFusion : public ISource
{
public:
    UltimateSensorFusion(HAL& hal);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    ts::Result<std::shared_ptr<messages::INode_Message>> send_message(messages::INode_Message const& message) override;

    ts::Result<void> start(Clock::time_point tp) override;

    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    ts::Result<void> init();

    HAL& m_hal;

    std::weak_ptr<bus::II2C_Bus> m_i2c_bus;

    struct Buses
    {
        std::shared_ptr<bus::II2C_Bus> i2c;
    };

    std::shared_ptr<hal::UltimateSensorFusion_Descriptor> m_descriptor;
    std::shared_ptr<hal::UltimateSensorFusion_Config> m_config;

    Clock::time_point m_last_process_tp = Clock::now();

    template<class Base>
    struct Common : public Base
    {
        Common() = default;
    };

    typedef Basic_Output_Stream<Common<stream::IAcceleration>> Acceleration_Stream;
    mutable std::shared_ptr<Acceleration_Stream> m_acceleration;

    typedef Basic_Output_Stream<Common<stream::IAngular_Velocity>> Angular_Velocity_Stream;
    mutable std::shared_ptr<Angular_Velocity_Stream> m_angular_velocity;

    typedef Basic_Output_Stream<Common<stream::IMagnetic_Field>> Magnetic_Field_Stream;
    mutable std::shared_ptr<Magnetic_Field_Stream> m_magnetic_field;

    typedef Basic_Output_Stream<Common<stream::IFrame>> Frame_Stream;
    mutable std::shared_ptr<Frame_Stream> m_frame;

    typedef Basic_Output_Stream<Common<stream::IPressure>> Pressure_Stream;
    mutable std::shared_ptr<Pressure_Stream> m_pressure;

    typedef Basic_Output_Stream<Common<stream::ITemperature>> Temperature_Stream;
    mutable std::shared_ptr<Temperature_Stream> m_temperature;

    struct Stats
    {
        Clock::time_point last_report_tp = Clock::now();
//        struct Pressure
//        {
//            bool operator==(Pressure const& o) const { return memcmp(this, &o, sizeof(*this)) == 0; }
//            bool operator!=(Pressure const& o) const { return !operator==(o); }
//            size_t added = 0;
//        } pres;
//        struct Temperature
//        {
//            bool operator==(Temperature const& o) const { return memcmp(this, &o, sizeof(*this)) == 0; }
//            bool operator!=(Temperature const& o) const { return !operator==(o); }
//            size_t added = 0;
//        } temp;

//        size_t bus_failures = 0;

//        bool operator==(Stats const& o) const { return pres == o.pres && temp == o.temp && bus_failures == o.bus_failures; }
//        bool operator!=(Stats const& o) const { return !operator==(o); }
    } m_stats;


    bool EM7180_set_gyro_FS(util::hw::II2C& i2c, uint16_t gyro_fs);
    bool EM7180_set_mag_acc_FS(util::hw::II2C& i2c, uint16_t mag_fs, uint16_t acc_fs);
    bool EM7180_set_integer_param(util::hw::II2C& i2c, uint8_t param, uint32_t param_val);
    bool EM7180_set_float_param(util::hw::II2C& i2c, uint8_t param, float param_val);
};


}
}
