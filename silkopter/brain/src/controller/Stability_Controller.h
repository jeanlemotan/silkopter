#pragma once

#include "common/node/IController.h"
#include "common/node/stream/IFrame.h"
#include "common/node/stream/IAngular_Velocity.h"
#include "HAL.h"

#include "utils/PID.h"
#include "Sample_Accumulator.h"
#include "Basic_Output_Stream.h"


namespace sz
{
namespace Stability_Controller
{
struct Init_Params;
struct Config;
}
}



namespace silk
{
namespace node
{

class Stability_Controller : public IController
{
public:
    Stability_Controller(HAL& hal);

    auto init(rapidjson::Value const& init_params) -> bool;
    auto get_init_params() const -> rapidjson::Document;

    auto set_config(rapidjson::Value const& json) -> bool;
    auto get_config() const -> rapidjson::Document;

    auto send_message(rapidjson::Value const& json) -> rapidjson::Document;

    void set_stream_input_path(size_t idx, q::Path const& path);
    auto get_stream_inputs() const -> std::vector<Stream_Input>;
    auto get_stream_outputs() const -> std::vector<Stream_Output>;

    void process();

private:
    auto init() -> bool;

    HAL& m_hal;

    std::shared_ptr<sz::Stability_Controller::Init_Params> m_init_params;
    std::shared_ptr<sz::Stability_Controller::Config> m_config;

    Sample_Accumulator<stream::IFrame, stream::IFrame> m_accumulator;

    math::vec3f compute_feedforward(config::Multi& config, stream::IFrame::Value const& input, stream::IFrame::Value const& target);
    math::vec3f compute_feedback(stream::IFrame::Value const& input, stream::IFrame::Value const& target);

    typedef util::PID<float, float, float> PID;
    PID m_x_pid;
    PID m_y_pid;
    PID m_z_pid;

    typedef Basic_Output_Stream<stream::IAngular_Velocity> Output_Stream;
    mutable std::shared_ptr<Output_Stream> m_output_stream;
};



}
}
