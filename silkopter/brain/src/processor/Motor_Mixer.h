#pragma once

#include "common/node/IProcessor.h"
#include "common/stream/ITorque.h"
#include "common/stream/IFloat.h"
#include "common/stream/IThrottle.h"
#include "uav_properties/IMultirotor_Properties.h"

#include "HAL.h"

#include "Sample_Accumulator.h"


namespace silk
{
namespace hal
{
struct Motor_Mixer_Descriptor;
struct Motor_Mixer_Config;
}
}


namespace silk
{
namespace node
{

class Motor_Mixer : public IProcessor
{
public:
    Motor_Mixer(HAL& hal);

    ts::Result<void> init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    ts::Result<void> set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    ts::Result<std::shared_ptr<hal::INode_Message>> send_message(hal::INode_Message const& message) override;

    ts::Result<void> start(Clock::time_point tp) override;

    ts::Result<void> set_input_stream_path(size_t idx, std::string const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    auto get_cell_count() const -> boost::optional<uint8_t>;

    void process();

private:
    ts::Result<void> init();

    void compute_throttles(IMultirotor_Properties const& multirotor_properties, stream::IFloat::Value const& collective_thrust, stream::ITorque::Value const& torque);


    HAL& m_hal;

    std::shared_ptr<hal::Motor_Mixer_Descriptor> m_descriptor;
    std::shared_ptr<hal::Motor_Mixer_Config> m_config;

    Sample_Accumulator<stream::ITorque, stream::IFloat> m_accumulator;

    struct Stream : public stream::IThrottle
    {
        auto get_samples() const -> std::vector<Sample> const& { return samples; }
        auto get_rate() const -> uint32_t { return rate; }

        uint32_t rate = 0;
        Sample last_sample;
        std::vector<Sample> samples;

        struct Config
        {
            math::vec3f position;
            math::vec3f max_torque;
            math::vec3f torque_vector;
        } config;

        float throttle = 0;
        float thrust = 0;
        math::vec3f torque;
    };

    mutable std::vector<std::shared_ptr<Stream>> m_outputs;
};


}
}
