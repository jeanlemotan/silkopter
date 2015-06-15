#include "BrainStdAfx.h"
#include "Velocity_Controller.h"

#include "sz_math.hpp"
#include "sz_Velocity_Controller.hpp"

namespace silk
{
namespace node
{

Velocity_Controller::Velocity_Controller(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::Velocity_Controller::Init_Params())
    , m_config(new sz::Velocity_Controller::Config())
{
}

auto Velocity_Controller::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("velocity_controller::init");

    sz::Velocity_Controller::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Velocity_Controller data: {}", ss.str());
        return false;
    }
    *m_init_params = sz;
    return init();
}
auto Velocity_Controller::init() -> bool
{
    m_output_frame_stream = std::make_shared<Output_Frame_Stream>();
    m_output_force_stream = std::make_shared<Output_Force_Stream>();
    if (m_init_params->rate == 0)
    {
        QLOGE("Bad rate: {}Hz", m_init_params->rate);
        return false;
    }
    m_output_frame_stream->set_rate(m_init_params->rate);
    m_output_frame_stream->set_tp(q::Clock::now());
    m_output_force_stream->set_rate(m_init_params->rate);
    m_output_force_stream->set_tp(q::Clock::now());
    return true;
}

auto Velocity_Controller::get_inputs() const -> std::vector<Input>
{
    std::vector<Input> inputs =
    {{
        { stream::IVelocity::TYPE, m_init_params->rate, "Input", m_accumulator.get_stream_path(0) },
        { stream::IVelocity::TYPE, m_init_params->rate, "Target", m_accumulator.get_stream_path(1) }
    }};
    return inputs;
}
auto Velocity_Controller::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(2);
    outputs[0].type = stream::IFrame::TYPE;
    outputs[0].name = "Frame";
    outputs[0].stream = m_output_frame_stream;
    outputs[1].type = stream::IForce::TYPE;
    outputs[1].name = "Collective Frame";
    outputs[1].stream = m_output_force_stream;
    return outputs;
}

void Velocity_Controller::process()
{
    QLOG_TOPIC("velocity_controller::process");

    m_output_frame_stream->clear();
    m_output_force_stream->clear();

    m_accumulator.process([this](
                          size_t idx,
                          stream::IVelocity::Sample const& i_sample,
                          stream::IVelocity::Sample const& t_sample)
    {
//        math::vec3f ff = compute_feedforward(*multi_config, i_sample.value, t_sample.value);
//        math::vec3f fb = compute_feedback(i_sample.value, t_sample.value);

//        Output_Stream::Value value(ff * m_config->feedforward.weight + fb * m_config->feedback.weight);

//        m_output_stream->push_sample(value, i_sample.is_healthy & t_sample.is_healthy);
    });
}

void Velocity_Controller::set_input_stream_path(size_t idx, q::Path const& path)
{
    m_accumulator.set_stream_path(idx, path, m_init_params->rate, m_hal);
}

auto Velocity_Controller::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("velocity_controller::set_config");

    sz::Velocity_Controller::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize Velocity_Controller config data: {}", ss.str());
        return false;
    }

    *m_config = sz;

    return true;
}
auto Velocity_Controller::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto Velocity_Controller::get_init_params() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_init_params, json);
    return std::move(json);
}
auto Velocity_Controller::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
{
    return rapidjson::Document();
}


}
}
