#pragma once

#include "common/node/IController.h"
#include "common/stream/IAngular_Velocity.h"
#include "common/stream/ITorque.h"
#include "HAL.h"
#include "uav_properties/IMultirotor_Properties.h"

#include "utils/PID.h"
#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace silk
{
namespace hal
{
struct Rate_Controller_Descriptor;
struct Rate_Controller_Config;
}
}



namespace silk
{
namespace node
{

class Rate_Controller : public IController
{
public:
    Rate_Controller(HAL& hal);

    bool init(hal::INode_Descriptor const& descriptor) override;
    std::shared_ptr<const hal::INode_Descriptor> get_descriptor() const override;

    bool set_config(hal::INode_Config const& config) override;
    std::shared_ptr<const hal::INode_Config> get_config() const override;

    //auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    auto start(q::Clock::time_point tp) -> bool override;

    void set_input_stream_path(size_t idx, q::Path const& path);
    auto get_inputs() const -> std::vector<Input>;
    auto get_outputs() const -> std::vector<Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<hal::Rate_Controller_Descriptor> m_descriptor;
    std::shared_ptr<hal::Rate_Controller_Config> m_config;

    Sample_Accumulator<stream::IAngular_Velocity, stream::IAngular_Velocity> m_accumulator;

    math::vec3f compute_feedforward(const IMultirotor_Properties& multirotor_properties, stream::IAngular_Velocity::Value const& input, stream::IAngular_Velocity::Value const& target);
    math::vec3f compute_feedback(stream::IAngular_Velocity::Value const& input, stream::IAngular_Velocity::Value const& target);

    typedef util::PID<float, float, float> PID;
    PID m_x_pid;
    PID m_y_pid;
    PID m_z_pid;

    typedef Basic_Output_Stream<stream::ITorque> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};



}
}
